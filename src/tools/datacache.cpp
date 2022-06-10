#include "datacache.h"

constexpr int TVHCLIENT_DATACACHE_DB_VERSION = 1;

#ifdef QT_DEBUG
    #include <QDebug>
#endif

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProcess>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QVariant>

#include "compressor.h"

#include "src/api/api_keys.h"
#include "src/tools/utils.h"

DataCache::DataCache(QObject *parent) :
    QObject(parent),
    m_dbPath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/cache.db"))
{
    // create cache path
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));

    openDatabase();

    initialize();

    cleanupEpg();
}

void DataCache::getChannel(const QString uuid)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Update channel: ") << uuid;
#endif

    const ChannelDTO channel = readChannel(uuid);

    if (!channel.uuid.isEmpty())
        emit channelAvailable(channel);
}

void DataCache::getChannels()
{
    emit channelsAvailable(readChannels());
}

void DataCache::getEventsForChannel(const QString uuid)
{
    emit eventsAvailable(uuid, readEvents(uuid));
}

void DataCache::resetChannels()
{
    QSqlQuery query(m_db);

    query.exec(QStringLiteral("DELETE FROM channels"));
    query.exec(QStringLiteral("VACCUM"));
}

void DataCache::updateChannels(const QByteArray data)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Update channels start: ")  << QDateTime::currentDateTime();;
#endif


    const QJsonObject json = Utils::parseJson(data);

    const QJsonArray channels = json.value(ApiKey::entries).toArray();

    // reset channels
    resetChannels();

    // insert new data
    if (!m_db.transaction())
        return;

    const quint64 timestamp = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

    for (const auto &value : channels) {
        ChannelDTO channel = Utils::channelFromJson(value.toObject());

        if (channel.name == QLatin1String("."))
            continue;


        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("INSERT INTO channels (uuid, name, number, icon, updated_at) "
                                     "VALUES (:uuid, :name, :number, :icon, :updated_at)"));

        query.bindValue(QStringLiteral(":uuid"), channel.uuid);
        query.bindValue(QStringLiteral(":name"), channel.name);
        query.bindValue(QStringLiteral(":number"), channel.number);
        query.bindValue(QStringLiteral(":icon"), channel.icon);
        query.bindValue(QStringLiteral(":updated_at"), timestamp);

        if (!query.exec()) {
#ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to insert channel");
            qDebug() << query.lastError().text();        
#endif
            continue;
        }
    }

    // cleanup old channels
    QSqlQuery query(m_db);
    if (!query.exec(QString("DELETE FROM channels WHERE updated_at < %1").arg(timestamp))) {
        #ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to clean channels");
            qDebug() << query.lastError().text();
        #endif
    }

    // commit or rollback
    if (!m_db.commit())
        m_db.rollback();

    emit channelsAvailable(readChannels());

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Update channels finnished: ") << QDateTime::currentDateTime();
#endif
}

void DataCache::resetEpg()
{
    QSqlQuery query(m_db);

    query.exec(QStringLiteral("DELETE FROM events"));
    query.exec(QStringLiteral("VACCUM"));
}

void DataCache::updateEpg(const QByteArray data, bool now)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Update EPG events start: ")  << QDateTime::currentDateTime();
#endif

    const QByteArray uncompressed = Compressor::gunzip(data);

    QJsonParseError error{};

    const QJsonObject json = QJsonDocument::fromJson(uncompressed, &error).object();

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << error.errorString();
#endif
        return;
    }

    const QJsonArray entries = json.value(ApiKey::entries).toArray();

    // reset EPG
    if (!now)
        resetEpg();

    // insert new data
    if (!m_db.transaction())
        return;

    for (const auto &value : entries) {
        const QJsonObject entry = value.toObject();

        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("INSERT %1 INTO events ("
                                     "event_id,"
                                     "episode_id,"
                                     "channel_uuid,"
                                     "start,"
                                     "stop,"
                                     "next_event_id,"
                                     "features,"
                                     "title,"
                                     "subtitle,"
                                     "summary,"
                                     "description"
                                     ") VALUES ("
                                     ":event_id,"
                                     ":episode_id,"
                                     ":channel_uuid,"
                                     ":start,"
                                     ":stop,"
                                     ":next_event_id,"
                                     ":features,"
                                     ":title,"
                                     ":subtitle,"
                                     ":summary,"
                                     ":description)").arg(now ? QStringLiteral("OR REPLACE") : ""));

        query.bindValue(QStringLiteral(":event_id"), entry.value(ApiKey::eventId).toInt());
        query.bindValue(QStringLiteral(":episode_id"), entry.value(ApiKey::episodeId).toInt());
        query.bindValue(QStringLiteral(":channel_uuid"), entry.value(ApiKey::channelUuid).toString());
        query.bindValue(QStringLiteral(":start"), entry.value(ApiKey::start).toInt());
        query.bindValue(QStringLiteral(":stop"), entry.value(ApiKey::stop).toInt());
        query.bindValue(QStringLiteral(":next_event_id"), entry.value(ApiKey::nextEventId).toInt());
        query.bindValue(QStringLiteral(":title"), entry.value(ApiKey::title).toString());
        query.bindValue(QStringLiteral(":subtitle"), entry.value(ApiKey::subtitle).toString());
        query.bindValue(QStringLiteral(":summary"), entry.value(ApiKey::summary).toString());
        query.bindValue(QStringLiteral(":description"), entry.value(ApiKey::description).toString());

        // features
        quint8 features{0};

        if (entry.value(ApiKey::hd).toInt())
            features |= Event::FeatureHD;

        if (entry.value(ApiKey::subtitled).toInt())
            features |= Event::FeatureSubtitled;

        if (entry.value(ApiKey::widescreen).toInt())
            features |= Event::FeatureWidescreen;

        query.bindValue(QStringLiteral(":features"), features);

        if (!query.exec()) {
#ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to insert epg event");
            qDebug() << query.lastError().text();
#endif
        }
    }

    // cleanup
    cleanupEpg();

    // commit or rollback
    if (!m_db.commit()) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("Failed to update epg events");
#endif
        m_db.rollback();
        return;
    }

    emit epgUpdated();

    // refresh channels
    getChannels();

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Update EPG events finished: ")  << QDateTime::currentDateTime();
#endif
}

void DataCache::getRecordings()
{
    emit recordingsAvailable(readRecordings());
}

void DataCache::resetRecordings()
{
    QSqlQuery query(m_db);

    query.exec(QStringLiteral("DELETE FROM recordings"));
    query.exec(QStringLiteral("VACCUM"));
}

void DataCache::updateRecordings(const QByteArray &data)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Update recordings start: ")  << QDateTime::currentDateTime();
#endif


    const QJsonObject json = Utils::parseJson(data);

    const QJsonArray entries = json.value(ApiKey::entries).toArray();

    // reset recordings
    resetRecordings();

    // insert new data
    if (!m_db.transaction())
        return;

    for (const auto &value : entries) {
        const QJsonObject entry = value.toObject();

        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("INSERT INTO recordings ("
                                     "uuid,"
                                     "channel_uuid,"
                                     "broadcast,"
                                     "status,"
                                     "file_size,"
                                     "play_count,"
                                     "play_position,"
                                     "start,"
                                     "stop,"
                                     "url,"
                                     "comment,"
                                     "title,"
                                     "subtitle,"
                                     "description"
                                     ") VALUES ("
                                     ":uuid,"
                                     ":channel_uuid,"
                                     ":broadcast,"
                                     ":status,"
                                     ":file_size,"
                                     ":play_count,"
                                     ":play_position,"
                                     ":start,"
                                     ":stop,"
                                     ":url,"
                                     ":comment,"
                                     ":title,"
                                     ":subtitle,"
                                     ":description"
                                     ")"));

        query.bindValue(QStringLiteral(":uuid"), entry.value(ApiKey::uuid).toString());
        query.bindValue(QStringLiteral(":channel_uuid"), entry.value(ApiKey::channel).toString());
        query.bindValue(QStringLiteral(":broadcast"), entry.value(ApiKey::broadcast).toInt());

        const QString statusStr = entry.value(ApiKey::status).toString();
        quint8 status{0};

        if (statusStr == QLatin1String("completed")) {
            status = Recording::StatusCompleted;
        } else if (statusStr == QLatin1String("completedError")) {
            status = Recording::StatusCompletedError;
        } else if (statusStr == QLatin1String("recording")) {
            status = Recording::StatusRecording;
        } else if (statusStr == QLatin1String("scheduled")) {
            status = Recording::StatusScheduled;
        }
        query.bindValue(QStringLiteral(":status"), status);

        query.bindValue(QStringLiteral(":file_size"), entry.value(ApiKey::fileSize).toInt());
        query.bindValue(QStringLiteral(":play_count"), entry.value(ApiKey::playCount).toInt());
        query.bindValue(QStringLiteral(":play_position"), entry.value(ApiKey::playPosition).toInt());
        query.bindValue(QStringLiteral(":start"), entry.value(ApiKey::start).toInt());
        query.bindValue(QStringLiteral(":stop"), entry.value(ApiKey::stop).toInt());
        query.bindValue(QStringLiteral(":url"), entry.value(ApiKey::url).toString());
        query.bindValue(QStringLiteral(":comment"), entry.value(ApiKey::comment).toString());
        query.bindValue(QStringLiteral(":title"), entry.value(ApiKey::displayTitle).toString());
        query.bindValue(QStringLiteral(":subtitle"), entry.value(ApiKey::displaySubtitle).toString());
        query.bindValue(QStringLiteral(":description"), entry.value(ApiKey::displayDescription).toString());


        if (!query.exec()) {
#ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to insert recording");
            qDebug() << query.lastError().text();
#endif
        }
    }

    // commit or rollback
    if (!m_db.commit()) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("Failed to update recordings");
#endif
        m_db.rollback();
        return;
    }

    emit recordingsAvailable(readRecordings());

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Update recordings finished: ")  << QDateTime::currentDateTime();
#endif
}

void DataCache::cleanupEpg()
{
    QSqlQuery query(m_db);
    if (!query.exec(QStringLiteral("DELETE FROM events WHERE stop < %1").arg(QDateTime::currentDateTimeUtc().toTime_t()))) {
        #ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to clean channels");
            qDebug() << query.lastError().text();
        #endif
    }

}

QVariant DataCache::getPragma(const QString &key)
{
    QSqlQuery query(m_db);
    query.exec(QString("PRAGMA %1").arg(key));
    query.next();

    return query.value(0);
}

ChannelDTO DataCache::readChannel(const QString &uuid) const
{
    QSqlQuery query(m_db);

    ChannelDTO channel;

    const QString sql = QStringLiteral("SELECT "
                                       "ch.uuid,"
                                       "ch.name,"
                                       "ch.number,"
                                       "ch.icon,"
                                       "ev.event_id,"
                                       "ev.title,"
                                       "ev.subtitle,"
                                       "ev.start,"
                                       "ev.stop,"
                                       "ev.features,"
                                       "ne.title "
                                       "FROM channels AS ch "
                                       "LEFT JOIN events AS ev "
                                       "ON ch.uuid = ev.channel_uuid "
                                       "LEFT OUTER JOIN events AS ne "
                                       "ON ev.next_event_id = ne.event_id "
                                       "WHERE ev.start < %1 AND %1 < ev.stop AND ch.uuid = '%2'")
            .arg(QString::number(QDateTime::currentDateTimeUtc().toTime_t()),
                 uuid);

    if (!query.exec(sql)) {
#ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to read channel: ") << uuid;
            qDebug() << query.lastError().text();
#endif
            return channel;
    }

    if (!query.next())
        return channel;

    channel.uuid = query.value(0).toString();
    channel.name = query.value(1).toString();
    channel.number = query.value(2).toUInt();
    channel.icon = query.value(3).toString();

    EventCurrentDTO event;
    event.eventId = query.value(4).toUInt();
    event.title = query.value(5).toString();
    event.subtitle = query.value(6).toString();
    event.start = query.value(7).toUInt();
    event.stop = query.value(8).toUInt();
    event.features = query.value(9).toUInt();
    event.nextTitle = query.value(10).toString();

    channel.current = event;

    return channel;
}

QList<ChannelDTO> DataCache::readChannels() const
{
    QSqlQuery query(m_db);

    QList<ChannelDTO> channels;

    const QString sql = QStringLiteral("SELECT "
                                       "ch.uuid,"
                                       "ch.name,"
                                       "ch.number,"
                                       "ch.icon,"
                                       "ev.event_id,"
                                       "ev.title,"
                                       "ev.subtitle,"
                                       "ev.start,"
                                       "ev.stop,"
                                       "ev.features,"
                                       "ne.title "
                                       "FROM channels AS ch "
                                       "LEFT JOIN events AS ev "
                                       "ON ch.uuid = ev.channel_uuid "
                                       "LEFT OUTER JOIN events AS ne "
                                       "ON ev.next_event_id = ne.event_id "
                                       "WHERE ev.start < %1 AND %1 < ev.stop")
                                .arg(QDateTime::currentDateTimeUtc().toTime_t());


    if (!query.exec(sql)) {
#ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to read channels");
            qDebug() << query.lastError().text();
#endif
            return channels;
    }

    while (query.next()) {
        ChannelDTO channel;
        channel.uuid = query.value(0).toString();
        channel.name = query.value(1).toString();
        channel.number = query.value(2).toUInt();
        channel.icon = query.value(3).toString();

        EventCurrentDTO event;
        event.eventId = query.value(4).toUInt();
        event.title = query.value(5).toString();
        event.subtitle = query.value(6).toString();
        event.start = query.value(7).toUInt();
        event.stop = query.value(8).toUInt();
        event.features = query.value(9).toUInt();
        event.nextTitle = query.value(10).toString();

        channel.current = event;

        channels.append(channel);
    }

    return channels;
}

QList<EventDTO> DataCache::readEvents(const QString uuid)
{
    QSqlQuery query(m_db);

    QList<EventDTO> events;

    const QString sql = QStringLiteral("SELECT "
                                       "ev.event_id,"
                                       "ev.episode_id,"
                                       "ev.start,"
                                       "ev.stop,"
                                       "ev.features,"
                                       "ev.title,"
                                       "ev.subtitle,"
                                       "ev.summary,"
                                       "ev.description, "
                                       "rec.uuid "
                                       "FROM events AS ev "
                                       "LEFT OUTER JOIN recordings AS rec "
                                       "ON ev.event_id = rec.broadcast "
                                       "WHERE ev.channel_uuid = '%1' AND ev.stop > %2 "
                                       "ORDER BY ev.start ASC")
                                .arg(uuid, QString::number(QDateTime::currentDateTimeUtc().toTime_t()));


    if (!query.exec(sql)) {
#ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to read events for channel: ") << uuid;
            qDebug() << query.lastError().text();
#endif
            return events;
    }

    while (query.next()) {
        EventDTO event;

        event.eventId = query.value(0).toUInt();
        event.episodeId = query.value(1).toUInt();
        event.start = query.value(2).toUInt();
        event.stop = query.value(3).toUInt();
        event.features = query.value(4).toUInt();
        event.title = query.value(5).toString();
        event.subtitle = query.value(6).toString();
        event.summary = query.value(7).toString();
        event.description = query.value(8).toString();
        event.recordingUuid = query.value(9).toString();
        event.channelUuid = uuid;

        events.append(event);
    }

    return events;
}


QList<RecordingDTO> DataCache::readRecordings() const
{
    QSqlQuery query(m_db);

    QList<RecordingDTO> recordings;

    if (!query.exec(QStringLiteral("SELECT "
                                   "rec.uuid,"
                                   "ch.name,"
                                   "rec.channel_uuid,"
                                   "rec.broadcast,"
                                   "rec.status,"
                                   "rec.file_size,"
                                   "rec.play_count,"
                                   "rec.play_position,"
                                   "rec.start,"
                                   "rec.stop,"
                                   "rec.url,"
                                   "rec.comment,"
                                   "rec.title,"
                                   "rec.subtitle,"
                                   "rec.description "
                                   "FROM recordings AS rec "
                                   "LEFT JOIN channels AS ch "
                                   "ON ch.uuid = rec.channel_uuid "
                                   "ORDER BY rec.start DESC"))) {
#ifdef QT_DEBUG
            qDebug() << QStringLiteral("Failed to read recordings");
            qDebug() << query.lastError().text();
#endif
            return recordings;
    }

    while (query.next()) {
        RecordingDTO recording;

        recording.uuid = query.value(0).toString();
        recording.channelName = query.value(1).toString();
        recording.channelUuid = query.value(2).toString();
        recording.broadcast = query.value(3).toUInt();
        recording.status = query.value(4).toUInt();
        recording.fileSize = query.value(5).toUInt();
        recording.playCount = query.value(6).toUInt();
        recording.playPosition = query.value(7).toUInt();
        recording.start = query.value(8).toUInt();
        recording.stop = query.value(9).toUInt();
        recording.url = query.value(10).toString();
        recording.comment = query.value(11).toString();
        recording.title = query.value(12).toString();
        recording.subtitle = query.value(13).toString();
        recording.description = query.value(14).toString();

        recordings.append(recording);
    }

    return recordings;
}

void DataCache::initialize()
{
    const quint8 version = getPragma(QStringLiteral("user_version")).toUInt();

    if (version != TVHCLIENT_DATACACHE_DB_VERSION)
        resetDatabase();


    QSqlQuery query(m_db);

    bool ok = false;

    // pragmas
    query.exec(QString("PRAGMA user_version = %1").arg(TVHCLIENT_DATACACHE_DB_VERSION));
    query.exec(QStringLiteral("PRAGMA auto_vacuum = FULL"));


    // create table channels
    ok = query.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS channels("
                                   "uuid TEXT PRIMARY KEY UNIQUE NOT NULL,"
                                   "name TEXT DEFAULT NULL,"
                                   "number INTEGER DEFAULT NULL,"
                                   "icon TEXT DEFAULT NULL,"
                                   "updated_at INTEGER DEFAULT NULL)"));

#ifdef QT_DEBUG
    if (!ok) {
        qDebug() << QStringLiteral("Could not create table channels");
        qDebug() << query.lastError().text();
    }
#endif

    // create table epg events
    ok = query.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS events("
                                   "event_id INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                                   "episode_id INTEGER DEFAULT NULL,"
                                   "channel_uuid TEXT NOT NULL,"
                                   "start INTEGER DEFAULT NULL,"
                                   "stop INTEGER_DEFAULT NULL,"
                                   "next_event_id INTEGER DEFAULT NULL,"
                                   "features INTEGER DEFAULT NULL,"
                                   "title TEXT,"
                                   "subtitle TEXT,"
                                   "summary TEXT,"
                                   "description TEXT)"));

#ifdef QT_DEBUG
    if (!ok) {
        qDebug() << QStringLiteral("Could not create table events");
        qDebug() << query.lastError().text();
    }
#endif

//    // create index in table epg events
//    ok = query.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_events_channel_uuid ON events(channel_uuid)"));

//#ifdef QT_DEBUG
//    if (!ok) {
//        qDebug() << QStringLiteral("Could not create index for idx_events_channel_uuid");
//        qDebug() << query.lastError().text();
//    }
//#endif

//    // create index for next event
//    ok = query.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_events_next ON events(next_event_id)"));

//#ifdef QT_DEBUG
//    if (!ok) {
//        qDebug() << QStringLiteral("Could not create index for idx_events_next");
//        qDebug() << query.lastError().text();
//    }
//#endif

    // create table records
    ok = query.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS recordings("
                                   "uuid TEXT PRIMARY KEY UNIQUE NOT NULL,"
                                   "channel_uuid TEXT NOT NULL,"
                                   "broadcast INTEGER DEFAULT NULL,"
                                   "status INTEGER DEFAULT NULL,"
                                   "file_size INTEGER DEFAULT NULL,"
                                   "play_count INTEGER DEFAULT NULL,"
                                   "play_position INTEGER DEFAULT NULL,"
                                   "start INTEGER DEFAULT NULL,"
                                   "stop INTEGER DEFAULT NULL,"
                                   "url TEXT,"
                                   "comment TEXT,"
                                   "title TEXT,"
                                   "subtitle TEXT,"
                                   "description TEXT)"));

#ifdef QT_DEBUG
    if (!ok) {
        qDebug() << QStringLiteral("Could not create table recordings");
        qDebug() << query.lastError().text();
    }
#endif

//    // create index for recording channel
//    ok = query.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_recording_channel ON recordings(channel_uuid)"));

//#ifdef QT_DEBUG
//    if (!ok) {
//        qDebug() << QStringLiteral("Could not create index for idx_recording_channel");
//        qDebug() << query.lastError().text();
//    }
//#endif

//    // create index for recording broadcast
//    ok = query.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_recording_broadcast ON recordings(broadcast)"));

//#ifdef QT_DEBUG
//    if (!ok) {
//        qDebug() << QStringLiteral("Could not create index for idx_recording_broadcast");
//        qDebug() << query.lastError().text();
//    }
//#endif

}

bool DataCache::openDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Could not open database");
#endif
        return false;
    }

    return true;
}

bool DataCache::resetDatabase()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Reset Database");
#endif

    m_db.close();

    if (!QFile(m_dbPath).remove()) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("Could not delete old database");
#endif
        return false;
    }

    return openDatabase();

}
