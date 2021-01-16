#include "event.h"

Event::Event(QObject *parent) :
    QObject(parent)
{

}

Event::Event(const EventDTO &dto, QObject *parent) :
    QObject(parent)
{
    setData(dto);
}

QString Event::channelUuid() const
{
    return m_channelUuid;
}

QString Event::description() const
{
    return m_description;
}

quint32 Event::episodeId() const
{
    return m_episodeId;
}

quint32 Event::eventId() const
{
    return m_eventId;
}

quint8 Event::features() const
{
    return m_features;
}

QString Event::recordingUuid() const
{
    return m_recordingUuid;
}

bool Event::scheduled() const
{
    return m_scheduled;
}

QDateTime Event::start() const
{
    return m_start;
}

QDateTime Event::stop() const
{
    return m_stop;
}

QString Event::subtitle() const
{
    return m_subtitle;
}

QString Event::summary() const
{
    return m_summary;
}

QString Event::title() const
{
    return m_title;
}

void Event::setData(const EventDTO &dto)
{
    setChannelUuid(dto.channelUuid);
    setDescription(dto.description);
    setEpisodeId(dto.episodeId);
    setEventId(dto.eventId);
    setFeatures(dto.features);
    setRecordingUuid(dto.recordingUuid);
    setStart(QDateTime::fromTime_t(dto.start));
    setStop(QDateTime::fromTime_t(dto.stop));
    setSubtitle(dto.subtitle);
    setSummary(dto.summary);
    setTitle(dto.title);
}

void Event::setChannelUuid(const QString &uuid)
{
    if (m_channelUuid == uuid)
        return;

    m_channelUuid = uuid;
    emit channelUuidChanged(m_channelUuid);
}

void Event::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

void Event::setEpisodeId(quint32 episodeId)
{
    if (m_episodeId == episodeId)
        return;

    m_episodeId = episodeId;
    emit episodeIdChanged(m_episodeId);
}

void Event::setEventId(quint32 eventId)
{
    if (m_eventId == eventId)
        return;

    m_eventId = eventId;
    emit eventIdChanged(m_eventId);
}

void Event::setFeatures(quint8 features)
{
    if (m_features == features)
        return;

    m_features = features;
    emit featuresChanged(m_features);
}

void Event::setRecordingUuid(const QString &uuid)
{
    if (m_recordingUuid == uuid)
        return;

    m_recordingUuid = uuid;
    emit recordingUuidChanged(m_recordingUuid);

    setScheduled(!m_recordingUuid.isEmpty());
}

void Event::setScheduled(bool scheduled)
{
    if (m_scheduled == scheduled)
        return;

    m_scheduled = scheduled;
    emit scheduledChanged(m_scheduled);
}

void Event::setStart(const QDateTime &start)
{
    if (m_start == start)
        return;

    m_start = start;
    emit startChanged(m_start);
}

void Event::setStop(const QDateTime &stop)
{
    if (m_stop == stop)
        return;

    m_stop = stop;
    emit stopChanged(m_stop);
}

void Event::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle)
        return;

    m_subtitle = subtitle;
    emit subtitleChanged(m_subtitle);
}

void Event::setSummary(const QString &summary)
{
    if (m_summary == summary)
        return;

    m_summary = summary;
    emit summaryChanged(m_summary);
}

void Event::setTitle(const QString &title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(m_title);
}
