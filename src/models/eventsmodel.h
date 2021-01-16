#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include "basemodel.h"

#include "src/entities/event.h"

class EventsModel : public BaseModel
{
    Q_OBJECT

    Q_PROPERTY(QString channelUuid READ channelUuid WRITE setChannelUuid NOTIFY channelUuidChanged)

public:
    enum EventRoles {
        ChannelUuidRole           = Qt::UserRole + 1,
        DescriptionRole,
        EpisodeIdRole,
        EventIdRole,
        FeaturesRole,
        RecordingUuidRole,
        ScheduledRole,
        StartRole,
        StopRole,
        SubtitleRole,
        SummaryRole,
        TitleRole
    };
    Q_ENUM(EventRoles)

    explicit EventsModel(QObject *parent = nullptr);

    void clear();
    Event *getEvent(const QModelIndex &index);
    QList<Event *> events() const;

    // properties
    QString channelUuid() const;

signals:
    // properties
    void channelUuidChanged(const QString &uuid);

public slots:
    void setChannelEvents(const QString uuid, const QList<EventDTO> events);
    void setEvents(const QList<EventDTO> events);

    // properties
    void setChannelUuid(const QString &uuid);

private slots:
    void onScheduledChanged();

private:
    QModelIndex indexOf(quint32 id) const;

    QList<Event *> m_events;

    // properties
    QString m_channelUuid;

    // BaseModel interface
public slots:
    void refresh() override;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // EVENTSMODEL_H
