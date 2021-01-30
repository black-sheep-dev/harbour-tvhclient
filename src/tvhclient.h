#ifndef TVHCLIENT_H
#define TVHCLIENT_H

#include <QObject>

#include <QDateTime>
#include <QNetworkAccessManager>
#include <QThread>

#include "entities/serverinfo.h"
#include "models/channelsmodel.h"
#include "models/eventsmodel.h"
#include "models/recordingsmodel.h"
#include "tools/datacache.h"
#include "tools/secretwallet.h"

struct Settings {
    static const QString hostname;
    static const QString favorites;
    static const QString password;
    static const QString port;
    static const QString showFavoritesOnly;
    static const QString username;
};

class TVHClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool showFavoritesOnly READ showFavoritesOnly WRITE setShowFavoritesOnly NOTIFY showFavoritesOnlyChanged)
    Q_PROPERTY(quint16 states READ states WRITE setStates NOTIFY statesChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)

public: 
    enum State {
        StateUninitialized      = 0x00,
        StateChannelsFetched    = 0x01,
        StateEpgFetched         = 0x02,
        StateRecordingsFetched  = 0x04
    };
    Q_ENUM(State)
    Q_DECLARE_FLAGS(States, State)

    explicit TVHClient(QObject *parent = nullptr);
    ~TVHClient() override;

    Q_INVOKABLE QString baseUrl() const; 
    Q_INVOKABLE ChannelsModel *channelsModel();
    Q_INVOKABLE void fetchData();
    Q_INVOKABLE EventsModel *getEventsForChannel(const QString &uuid);
    Q_INVOKABLE void initialize();
    Q_INVOKABLE void saveSettings(); 
    Q_INVOKABLE RecordingsModel *recordingsModel();
    Q_INVOKABLE void resetAuthentication();
    Q_INVOKABLE void resetCache();
    Q_INVOKABLE void resetIconCache();
    Q_INVOKABLE void saveAuthentication();
    Q_INVOKABLE ServerInfo *serverInfo();

    // API CALLS
    Q_INVOKABLE void cancelRecording(const QString &uuid);
    Q_INVOKABLE void getChannels();
    Q_INVOKABLE void getEpg();
    Q_INVOKABLE void getEpgNow();
    Q_INVOKABLE void getRecordings();
    Q_INVOKABLE void getServerInfo();
    Q_INVOKABLE void recordEvent(const QString &uuid, quint32 eventId);
    Q_INVOKABLE void removeRecording(const QString &uuid);
    Q_INVOKABLE void stopRecording(const QString &uuid);

    // properties
    QString hostname() const;
    QString password() const;
    quint16 port() const;
    bool showFavoritesOnly() const;
    quint16 states() const;
    QString username() const;

signals:
    void iconAvailable(const QString &uuid);

    // invokables
    Q_INVOKABLE void eventCanceled();
    Q_INVOKABLE void eventScheduled(const QString &uuid);

    // thread signals
    void requestChannels();
    void requestChannelsUpdate(const QByteArray data);
    void requestEpgUpdate(const QByteArray data, bool now = false);
    void requestEventsForChannel(const QString uuid);
    void requestRecordings();
    void requestRecordingsUpdate(const QByteArray data);
    void showFavoritesOnlyChanged(bool showFavoritesOnly);

    // properties
    void hostnameChanged(const QString &hostname);
    void passwordChanged(const QString &password);
    void portChanged(quint16 port);
    void statesChanged(quint16 states);
    void usernameChanged(const QString &username);

public slots:
    // properties
    void setHostname(const QString &hostname);
    void setPassword(const QString &password);
    void setPort(quint16 port);
    void setShowFavoritesOnly(bool showFavoritesOnly); 
    void setStates(quint16 states);
    void setUsername(const QString &username);

private slots:
    void onIconAvailable(QNetworkReply *reply);
    void parseReply(QNetworkReply *reply);

private:
    enum Request {
        RequestUndefined,
        RequestChannels,
        RequestEpg,
        RequestEpgNow,
        RequestIcon,
        RequestRecordEvent,
        RequestRecordingCancel,
        RequestRecordingRemove,
        RequestRecordingStop,
        RequestRecordings,
        RequestServerInfo
    };

    void cacheIcons(const QList<Channel *> &channels);
    QNetworkRequest getRequest(const QString &endpoint);
    bool hasCachedIcons();
    bool hasCredentials();
    void parseRecordEventResult(const QJsonObject &obj);
    void parseServerInfoResult(const QJsonObject &obj);
    void updateBaseUrl();

    void readSettings();
    void writeSettings();

    QThread *m_backgroundThread{new QThread};
    QString m_baseUrl;
    DataCache *m_cache{new DataCache};
    ChannelsModel *m_channelsModel{new ChannelsModel(this)};
    QString m_iconCachePath;
    bool m_initialized{false};
    QNetworkAccessManager *m_manager{new QNetworkAccessManager(this)};
    RecordingsModel *m_recordingsModel{new RecordingsModel(this)};
    ServerInfo *m_serverInfo{new ServerInfo(this)};
    SecretWallet *m_wallet{new SecretWallet(this)};

    // properties
    QString m_hostname;
    QString m_password;
    quint16 m_port{9981};
    bool m_showFavoritesOnly{false};
    quint16 m_states{StateUninitialized};
    QString m_username;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(TVHClient::States)

#endif // TVHCLIENT_H
