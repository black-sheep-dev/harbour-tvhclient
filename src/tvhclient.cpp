#include "tvhclient.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDir>
#include <QImageReader>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QStandardPaths>

#include "api/api_keys.h"
#include "tools/utils.h"

TVHClient::TVHClient(QObject *parent) :
    QObject(parent),
    m_iconCachePath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/icons/"))
{
    // create cache path
    QDir().mkpath(m_iconCachePath);

    // read settings
    readSettings();

    // get server info
    getServerInfo();

    // create connections
    connect(m_manager, &QNetworkAccessManager::finished, this, &TVHClient::parseReply);

    // start background thread and move objects to it
    m_backgroundThread->start();

    connect(this, &TVHClient::requestChannels, m_cache, &DataCache::getChannels, Qt::QueuedConnection);
    connect(this, &TVHClient::requestChannelsUpdate, m_cache, &DataCache::updateChannels, Qt::QueuedConnection);
    connect(this, &TVHClient::requestEpgUpdate, m_cache, &DataCache::updateEpg, Qt::QueuedConnection);
    connect(this, &TVHClient::requestEventsForChannel, m_cache, &DataCache::getEventsForChannel, Qt::QueuedConnection);
    connect(this, &TVHClient::requestRecordings, m_cache, &DataCache::getRecordings, Qt::QueuedConnection);
    connect(this, &TVHClient::requestRecordingsUpdate, m_cache, &DataCache::updateRecordings, Qt::QueuedConnection);

    connect(m_cache, &DataCache::channelsAvailable, m_channelsModel, &ChannelsModel::setChannels, Qt::QueuedConnection);
    connect(m_cache, &DataCache::channelAvailable, m_channelsModel, &ChannelsModel::setChannel, Qt::QueuedConnection);
    connect(m_cache, &DataCache::recordingsAvailable, m_recordingsModel, &RecordingsModel::setRecordings, Qt::QueuedConnection);

    connect(m_channelsModel, &ChannelsModel::requestChannelUpdate, m_cache, &DataCache::getChannel, Qt::QueuedConnection);
    connect(m_channelsModel, &ChannelsModel::channelsLoaded, this, &TVHClient::cacheIcons);

    m_cache->moveToThread(m_backgroundThread);

    // other connections
    connect(m_recordingsModel, &RecordingsModel::recordingStateChanged, m_channelsModel, &ChannelsModel::setRecordingState);

    // get data from cache
    emit requestChannels();
    emit requestRecordings(); 
}

TVHClient::~TVHClient()
{
    writeSettings();

    m_backgroundThread->quit();
    m_backgroundThread->wait();
}

QString TVHClient::baseUrl() const
{
    return QString("http://%1:%2").arg(m_hostname, QString::number(m_port));
}

ChannelsModel *TVHClient::channelsModel()
{
    return m_channelsModel;
}

void TVHClient::fetchData()
{
    getChannels();
    getRecordings();
    getEpgNow();
}

EventsModel *TVHClient::getEventsForChannel(const QString &uuid)
{
    EventsModel *model = new EventsModel;
    model->setChannelUuid(uuid);
    model->setLoading(true);
    connect(m_cache, &DataCache::eventsAvailable, model, &EventsModel::setChannelEvents, Qt::QueuedConnection);

    emit requestEventsForChannel(uuid);

    return model;
}

void TVHClient::initialize()
{

}

void TVHClient::saveSettings()
{
    writeSettings();
}

RecordingsModel *TVHClient::recordingsModel()
{
    return m_recordingsModel;
}

void TVHClient::resetCache()
{
    getChannels();
    getRecordings();
    getEpgNow();
}

void TVHClient::resetIconCache()
{
    QDir dir(m_iconCachePath);

    for (const auto &info : dir.entryInfoList(QDir::Files)) {
        QFile::remove(info.absoluteFilePath());
    }

    cacheIcons(m_channelsModel->channels());
}

ServerInfo *TVHClient::serverInfo()
{
    return m_serverInfo;
}

void TVHClient::cancelRecording(const QString &uuid)
{
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/dvr/entry/cancel?uuid=%1").arg(uuid)));
    reply->setProperty("request", RequestRecordingCancel);
}


void TVHClient::getChannels()
{
    m_channelsModel->setLoading(true);
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/channel/grid?start=0&limit=999999")));
    reply->setProperty("request", RequestChannels);
}

void TVHClient::getEpg()
{
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/epg/events/grid?start=0&limit=999999")));
    reply->setProperty("request", RequestEpg);
}

void TVHClient::getEpgNow()
{
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/epg/events/grid?start=0&limit=999999&mode=now")));
    reply->setProperty("request", RequestEpgNow);
}

void TVHClient::getRecordings()
{
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/dvr/entry/grid?start=0&limit=999999")));
    reply->setProperty("request", RequestRecordings);
}

void TVHClient::getServerInfo()
{
    m_serverInfo->setLoading(true);
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/serverinfo")));
    reply->setProperty("request", RequestServerInfo);
}

void TVHClient::recordEvent(const QString &uuid, quint32 eventId)
{
    QNetworkReply *reply =
            m_manager->get(getRequest(QStringLiteral("/api/dvr/entry/create_by_event"
                                                     "?config_uuid=%1"
                                                     "&event_id=%2"
                                                     "&comment=Added by SailfishOS TVHClient")
                                                    .arg(uuid, QString::number(eventId))));

    reply->setProperty("request", RequestRecordEvent);
}

void TVHClient::removeRecording(const QString &uuid)
{
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/dvr/entry/remove?uuid=%1").arg(uuid)));
    reply->setProperty("request", RequestRecordingRemove);
}

void TVHClient::stopRecording(const QString &uuid)
{
    QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/api/dvr/entry/stop?uuid=%1").arg(uuid)));
    reply->setProperty("request", RequestRecordingStop);
}

QString TVHClient::hostname() const
{
    return m_hostname;
}

quint16 TVHClient::port() const
{
    return m_port;
}

bool TVHClient::showFavoritesOnly() const
{
    return m_showFavoritesOnly;
}

quint16 TVHClient::states() const
{
    return m_states;
}

void TVHClient::setHostname(const QString &hostname)
{
    if (m_hostname == hostname)
        return;

    m_hostname = hostname;
    if (!m_hostname.startsWith(QLatin1String("http")))
        m_hostname.prepend(QStringLiteral("http://"));

    emit hostnameChanged(m_hostname);
}

void TVHClient::setPort(quint16 port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit portChanged(m_port);
}

void TVHClient::setShowFavoritesOnly(bool showFavoritesOnly)
{
    if (m_showFavoritesOnly == showFavoritesOnly)
        return;

    m_showFavoritesOnly = showFavoritesOnly;
    emit showFavoritesOnlyChanged(m_showFavoritesOnly);
}

void TVHClient::setStates(quint16 states)
{
    if (m_states == states)
        return;

    m_states = states;
    emit statesChanged(m_states);
}

void TVHClient::onIconAvailable(QNetworkReply *reply)
{
    const QString uuid = reply->property("channel_uuid").toString();

    QImageReader reader(reply);

    const QImage img = reader.read();

    if (img.isNull())
        return;

    const QString file =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
            + QStringLiteral("/icons/")
            + uuid
            + QStringLiteral(".png");

    img.save(file);

    emit iconAvailable(uuid);
}

void TVHClient::parseReply(QNetworkReply *reply)
{
    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->errorString();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
#endif


        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 0) {
            m_serverInfo->setAvailable(false);
            m_serverInfo->setLoading(false);
        }

        reply->deleteLater();
        return;
    }

    m_serverInfo->setAvailable(true);

    const quint8 request = reply->property("request").toInt();

    // read icon
    if (request == RequestIcon) {
        onIconAvailable(reply);
        return;
    }

    // read json
    const QByteArray data = reply->readAll();

    switch (request) {
    case RequestChannels:
        setStates(m_states |= StateChannelsFetched);
        emit requestChannelsUpdate(data);
        break;

    case RequestEpgNow:
        emit requestEpgUpdate(data, true);
        getEpg();
        break;

    case RequestEpg:
        setStates(m_states |= StateEpgFetched);
        emit requestEpgUpdate(data, false);
        break;

    case RequestRecordings:
        setStates(m_states |= StateRecordingsFetched);
        emit requestRecordingsUpdate(data);
        break;

    case RequestRecordEvent:
        parseRecordEventResult(Utils::parseJson(data));
        getRecordings();
        break;

    case RequestRecordingCancel: 
    case RequestRecordingStop: 
        emit eventCanceled();
        getRecordings();
        break;

    case RequestRecordingRemove:
        getRecordings();
        break;

    case RequestServerInfo:
        parseServerInfoResult(Utils::parseJson(data));
        break;

    default:
        break;
    }

    reply->deleteLater();
}

void TVHClient::cacheIcons(const QList<Channel *> &channels)
{
    if ( hasCachedIcons() )
        return;

    connect(this, &TVHClient::iconAvailable, m_channelsModel, &ChannelsModel::onIconAvailable);

    for (const auto *channel : channels) {
        QNetworkReply *reply = m_manager->get(getRequest(QStringLiteral("/") + channel->icon()));
        reply->setProperty("channel_uuid", channel->uuid());
        reply->setProperty("request", RequestIcon);
    }
}

QNetworkRequest TVHClient::getRequest(const QString &endpoint)
{
    const QString url = QString("%1:%2%3").arg(m_hostname,
                                               QString::number(m_port),
                                               endpoint);

    QNetworkRequest request(url);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Accept-Encoding", "gzip");

    return request;
}

bool TVHClient::hasCachedIcons()
{
    return QDir(m_iconCachePath)
            .entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() != 0;
}

void TVHClient::parseRecordEventResult(const QJsonObject &obj)
{
    const QString uuid = obj.value(TVHCLIENT_API_KEY_UUID)
            .toArray()
            .first()
            .toString();

    emit eventScheduled(uuid);
}

void TVHClient::parseServerInfoResult(const QJsonObject &obj)
{
    m_serverInfo->setVersion(obj.value(TVHCLIENT_API_KEY_SW_VERSION).toString());
    m_serverInfo->setApiVersion(obj.value(TVHCLIENT_API_KEY_API_VERSION).toInt());
    m_serverInfo->setName(obj.value(TVHCLIENT_API_KEY_NAME).toString());
    m_serverInfo->setLoading(false);
}

void TVHClient::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("APP"));
    m_hostname = settings.value(QStringLiteral("hostname"), QString()).toString();
    m_port = settings.value(QStringLiteral("port"), 9981).toInt();
    m_channelsModel->setFavorites(settings.value(QStringLiteral("favorites")).toStringList());
    m_showFavoritesOnly = settings.value(QStringLiteral("showFavoritesOnly"), false).toBool();
    settings.endGroup();
}

void TVHClient::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("APP"));
    settings.setValue(QStringLiteral("hostname"), m_hostname);
    settings.setValue(QStringLiteral("port"), m_port);
    settings.setValue(QStringLiteral("favorites"), m_channelsModel->favorites());
    settings.setValue(QStringLiteral("showFavoritesOnly"), m_showFavoritesOnly);
    settings.endGroup();
}
