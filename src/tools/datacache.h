#ifndef DATACACHE_H
#define DATACACHE_H

#include <QObject>

#include <QJsonObject>
#include <QSqlDatabase>

#include "src/entities/channel.h"
#include "src/entities/event.h"
#include "src/entities/recording.h"

class DataCache : public QObject
{
    Q_OBJECT

public:
    explicit DataCache(QObject *parent = nullptr);

signals:
    void channelAvailable(const ChannelDTO channel);
    void channelsAvailable(const QList<ChannelDTO> channels);
    void eventsAvailable(const QString uuid, const QList<EventDTO> events);
    void epgUpdated();
    void recordingsAvailable(const QList<RecordingDTO> recordings);

public slots:
    // channels
    void getChannel(const QString uuid);
    void getChannels();
    void getEventsForChannel(const QString uuid);
    void resetChannels();
    void updateChannels(const QByteArray data);

    // epg events
    void resetEpg();
    void updateEpg(const QByteArray data, bool now = false);

    // recordings
    void getRecordings();
    void resetRecordings();
    void updateRecordings(const QByteArray &data);

private:
    void cleanupEpg();
    QVariant getPragma(const QString &key);
    ChannelDTO readChannel(const QString &uuid) const;
    QList<ChannelDTO> readChannels() const;
    QList<EventDTO> readEvents(const QString uuid);
    QList<RecordingDTO> readRecordings() const;

    // database
    void initialize();
    bool openDatabase();
    bool resetDatabase();

    QSqlDatabase m_db;
    QString m_dbPath;

};

#endif // DATACACHE_H
