#ifndef RECORDING_H
#define RECORDING_H

#include <QObject>

#include <QDateTime>

struct RecordingDTO {
    RecordingDTO() {};
    RecordingDTO(const RecordingDTO &other) = default;
    RecordingDTO &operator=(const RecordingDTO &other) = default;

    quint32 broadcast{0};
    QString channelName;
    QString channelUuid;
    QString comment;
    QString description;
    quint32 fileSize{0};
    quint32 playCount{0};
    quint32 playPosition{0};
    quint32 start{0};
    quint32 stop{0};
    quint16 status{0};
    QString subtitle;
    QString title;
    QString url;
    QString uuid;
};
Q_DECLARE_METATYPE(RecordingDTO)

class Recording : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(quint32 broadcast READ broadcast WRITE setBroadcast NOTIFY broadcastChanged)
    Q_PROPERTY(QString channelName READ channelName WRITE setChannelName NOTIFY channelNameChanged)
    Q_PROPERTY(QString channelUuid READ channelUuid WRITE setChannelUuid NOTIFY channelUuidChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(quint32 fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged)
    Q_PROPERTY(quint32 playCount READ playCount WRITE setPlayCount NOTIFY playCountChanged)
    Q_PROPERTY(quint32 playPosition READ playPosition WRITE setPlayPosition NOTIFY playPositionChanged)
    Q_PROPERTY(QDateTime start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QDateTime stop READ stop WRITE setStop NOTIFY stopChanged)
    Q_PROPERTY(quint32 status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)

public:
    enum Status {
        StatusUnknown           = 0x00,
        StatusCompleted         = 0x01,
        StatusCompletedError    = 0x02,
        StatusRecording         = 0x04,
        StatusScheduled         = 0x08
    };
    Q_ENUM(Status)
    Q_DECLARE_FLAGS(Statuses, Status)

    explicit Recording(QObject *parent = nullptr);
    Recording(const RecordingDTO &dto, QObject *parent = nullptr);

    // properties
    bool active() const;
    quint32 broadcast() const;
    const QString &channelName() const;
    const QString &channelUuid() const;
    const QString &comment() const;
    const QString &description() const;
    quint32 fileSize() const;
    quint32 playCount() const;
    quint32 playPosition() const;
    const QDateTime &start() const;
    const QDateTime &stop() const;
    quint16 status() const;
    const QString &subtitle() const;
    const QString &title() const;
    const QString &url() const;
    const QString &uuid() const;

signals:
    // properties
    void activeChanged(bool active);
    void broadcastChanged(quint32 broadcast);
    void channelNameChanged(const QString &name);
    void channelUuidChanged(const QString &uuid);
    void commentChanged(const QString &comment);
    void descriptionChanged(const QString &description);
    void fileSizeChanged(quint32 size);
    void playCountChanged(quint32 count);
    void playPositionChanged(quint32 position);
    void startChanged(const QDateTime &start);
    void stopChanged(const QDateTime &stop);
    void statusChanged(quint16 status);
    void subtitleChanged(const QString &subtitle);
    void titleChanged(const QString &title);
    void urlChanged(const QString &url);
    void uuidChanged(const QString &uuid);

public slots:
    void setData(const RecordingDTO &dto);

    // properties
    void setActive(bool active);
    void setBroadcast(quint32 broadcast);
    void setChannelName(const QString &name);
    void setChannelUuid(const QString &uuid);
    void setComment(const QString &comment);
    void setDescription(const QString &description);
    void setFileSize(quint32 size);
    void setPlayCount(quint32 count);
    void setPlayPosition(quint32 position);
    void setStart(const QDateTime &start);
    void setStop(const QDateTime &stop);
    void setStatus(quint16 status);
    void setSubtitle(const QString &subtitle);
    void setTitle(const QString &title);
    void setUrl(const QString &url);
    void setUuid(const QString &uuid);


private:
    // properties
    bool m_active{false};
    quint32 m_broadcast{0};
    QString m_channelUuid;
    QString m_channelName;
    QString m_comment;
    QString m_description;
    quint32 m_fileSize{0};
    quint32 m_playCount{0};
    quint32 m_playPosition{0};
    QDateTime m_start;
    QDateTime m_stop;
    quint16 m_status{StatusUnknown};
    QString m_subtitle;
    QString m_title;
    QString m_url;
    QString m_uuid;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Recording::Statuses)

#endif // RECORDING_H
