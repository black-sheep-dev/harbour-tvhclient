#include "eventsmodel.h"

#include <QTimer>

EventsModel::EventsModel(QObject *parent) :
    BaseModel(parent)
{

}

void EventsModel::clear()
{
    beginResetModel();
    qDeleteAll(m_events.begin(), m_events.end());
    m_events.clear();
    endResetModel();
}

Event *EventsModel::getEvent(const QModelIndex &index)
{
    if (!index.isValid())
        return nullptr;

    return m_events.at(index.row());
}

QList<Event *> EventsModel::events() const
{
    return m_events;
}

QString EventsModel::channelUuid() const
{
    return m_channelUuid;
}

void EventsModel::setChannelEvents(const QString uuid, const QList<EventDTO> events)
{
    if (m_channelUuid != uuid)
        return;

    setEvents(events);
}

void EventsModel::setEvents(const QList<EventDTO> events)
{
    beginResetModel();
    if (!m_events.isEmpty()) {
        qDeleteAll(m_events.begin(), m_events.end());
        m_events.clear();
    }

    for (const auto &dto : events) {
        Event *event = new Event(dto, this);

        connect(event, &Event::scheduledChanged, this, &EventsModel::onScheduledChanged);

        m_events.append(event);
    }
    endResetModel();

    setLoading(false);
}

void EventsModel::setChannelUuid(const QString &uuid)
{
    if (m_channelUuid == uuid)
        return;

    m_channelUuid = uuid;
    emit channelUuidChanged(m_channelUuid);
}

void EventsModel::onScheduledChanged()
{
    auto *event = qobject_cast<Event *>(sender());

    if (event == nullptr)
        return;

    const QModelIndex index = indexOf(event->eventId());
    emit dataChanged(index, index);
}

QModelIndex EventsModel::indexOf(quint32 id) const
{
    int row = 0;

    for (int i = 0; i < m_events.count(); ++i) {
        if (m_events.at(i)->eventId() != id)
            continue;

        row = i;
        break;
    }

    return index(row);
}

void EventsModel::refresh()
{
    int count{0};

    for (const auto *event : m_events) {
        if (event->stop() < QDateTime::currentDateTimeUtc())
            count++;
        else
            break;
    }

    if (count == 0 )
        return;

    beginRemoveRows(QModelIndex(), 0, count);
    for (int i = 0; i < count; ++i) {
        m_events.takeAt(i)->deleteLater();
    }
    endRemoveRows();
}

int EventsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_events.count();
}

QVariant EventsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto *event = m_events.at(index.row());

    switch (role) {
    case ChannelUuidRole:
        return event->channelUuid();

    case DescriptionRole:
        return event->description();

    case EpisodeIdRole:
        return event->episodeId();

    case EventIdRole:
        return event->eventId();

    case FeaturesRole:
        return event->features();

    case RecordingUuidRole:
        return event->recordingUuid();

    case ScheduledRole:
        return event->scheduled();

    case StartRole:
        return event->start();

    case StopRole:
        return event->stop();

    case SubtitleRole:
        return event->subtitle();

    case SummaryRole:
        return event->summary();

    case TitleRole:
        return event->title();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> EventsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ChannelUuidRole]      = "channelUuid";
    roles[DescriptionRole]      = "description";
    roles[EpisodeIdRole]        = "episodeId";
    roles[EventIdRole]          = "eventId";
    roles[FeaturesRole]         = "features";
    roles[RecordingUuidRole]    = "recordingUuid";
    roles[ScheduledRole]        = "scheduled";
    roles[StartRole]            = "start";
    roles[StopRole]             = "stop";
    roles[SubtitleRole]         = "subtitle";
    roles[SummaryRole]          = "summary";
    roles[TitleRole]            = "title";

    return roles;
}
