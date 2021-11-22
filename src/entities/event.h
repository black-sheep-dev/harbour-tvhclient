#ifndef EVENT_H
#define EVENT_H

#include <QObject>

#include <QDateTime>

struct EventCurrentDTO {
    quint32 eventId{0};
    quint8 features{0};
    QString nextTitle;
    quint32 start{0};
    quint32 stop{0};
    QString title;
    QString subtitle;
};
Q_DECLARE_METATYPE(EventCurrentDTO)

struct EventDTO {
    EventDTO() {};
    EventDTO(const EventDTO &other) = default;

    EventDTO &operator=(const EventDTO &other) = default;

    QString channelUuid;
    QString description;
    quint32 episodeId{0};
    quint32 eventId{0};
    quint8 features{0};
    QString recordingUuid;
    quint32 start{0};
    quint32 stop{0};
    QString subtitle;
    QString summary;
    QString title;
};
Q_DECLARE_METATYPE(EventDTO)

class Event : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString channelUuid READ channelUuid WRITE setChannelUuid NOTIFY channelUuidChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(quint32 episodeId READ episodeId WRITE setEpisodeId NOTIFY episodeIdChanged)
    Q_PROPERTY(quint32 eventId READ eventId WRITE setEventId NOTIFY eventIdChanged)
    Q_PROPERTY(quint8 features READ features WRITE setFeatures NOTIFY featuresChanged)
    Q_PROPERTY(QString recordingUuid READ recordingUuid WRITE setRecordingUuid NOTIFY recordingUuidChanged)
    Q_PROPERTY(bool scheduled READ scheduled WRITE setScheduled NOTIFY scheduledChanged)
    Q_PROPERTY(QDateTime start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QDateTime stop READ stop WRITE setStop NOTIFY stopChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString summary READ summary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    enum Feature {
        FeatureNone         = 0x00,
        FeatureHD           = 0x01,
        FeatureSubtitled    = 0x02,
        FeatureWidescreen   = 0x04

    };
    Q_ENUM(Feature)
    Q_DECLARE_FLAGS(Features, Feature)

    explicit Event(QObject *parent = nullptr);
    Event(const EventDTO &dto, QObject *parent = nullptr);

    // properties
    const QString &channelUuid() const;
    const QString &description() const;
    quint32 episodeId() const;
    quint32 eventId() const;
    quint8 features() const;
    const QString &recordingUuid() const;
    bool scheduled() const;
    const QDateTime &start() const;
    const QDateTime &stop() const;
    const QString &subtitle() const;
    const QString &summary() const;
    const QString &title() const;

signals:
    // properties
    void channelUuidChanged(const QString &uuid);
    void descriptionChanged(const QString &description);
    void episodeIdChanged(quint32 episodeId);
    void eventIdChanged(quint32 eventId);
    void featuresChanged(quint8 features);
    void recordingUuidChanged(const QString &uuid);
    void scheduledChanged(bool scheduled);
    void startChanged(const QDateTime &start);
    void stopChanged(const QDateTime &stop);
    void subtitleChanged(const QString &subtitle);
    void summaryChanged(const QString &summary);
    void titleChanged(const QString &title);

public slots:
    void setData(const EventDTO &dto);

    // properties
    void setChannelUuid(const QString &uuid);
    void setDescription(const QString &description);
    void setEpisodeId(quint32 episodeId);
    void setEventId(quint32 eventId);
    void setFeatures(quint8 features);
    void setRecordingUuid(const QString &uuid);
    void setScheduled(bool scheduled);
    void setStart(const QDateTime &start);
    void setStop(const QDateTime &stop);
    void setSubtitle(const QString &subtitle);
    void setSummary(const QString &summary);
    void setTitle(const QString &title);

private:
    // properties
    QString m_channelUuid;
    QString m_description;
    quint32 m_episodeId{0};
    quint32 m_eventId{0};
    quint8 m_features{FeatureNone};
    QString m_recordingUuid;
    bool m_scheduled{false};
    QDateTime m_start;
    QDateTime m_stop;
    QString m_subtitle;
    QString m_summary;
    QString m_title;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Event::Features)

#endif // EVENT_H
