#include "recording.h"

Recording::Recording(QObject *parent) :
    QObject(parent)
{

}

Recording::Recording(const RecordingDTO &dto, QObject *parent) :
    QObject(parent)
{
    setData(dto);
}

bool Recording::active() const
{
    return m_active;
}

quint32 Recording::broadcast() const
{
    return m_broadcast;
}

QString Recording::channelName() const
{
    return m_channelName;
}

QString Recording::channelUuid() const
{
    return m_channelUuid;
}

QString Recording::comment() const
{
    return m_comment;
}

QString Recording::description() const
{
    return m_description;
}

quint32 Recording::fileSize() const
{
    return m_fileSize;
}

quint32 Recording::playCount() const
{
    return m_playCount;
}

quint32 Recording::playPosition() const
{
    return m_playPosition;
}

QDateTime Recording::start() const
{
    return m_start;
}

QDateTime Recording::stop() const
{
    return m_stop;
}

quint16 Recording::status() const
{
    return m_status;
}

QString Recording::subtitle() const
{
    return m_subtitle;
}

QString Recording::title() const
{
    return m_title;
}

QString Recording::url() const
{
    return m_url;
}

QString Recording::uuid() const
{
    return m_uuid;
}

void Recording::setData(const RecordingDTO &dto)
{   
    const quint32 current = QDateTime::currentDateTimeUtc().toTime_t();

    setActive(current >= dto.start && current <= dto.stop);
    setBroadcast(dto.broadcast);
    setChannelName(dto.channelName);
    setChannelUuid(dto.channelUuid);
    setComment(dto.comment);
    setDescription(dto.description);
    setFileSize(dto.fileSize);
    setPlayCount(dto.playCount);
    setPlayPosition(dto.playPosition);
    setStart(QDateTime::fromTime_t(dto.start));
    setStop(QDateTime::fromTime_t(dto.stop));
    setStatus(dto.status);
    setTitle(dto.title);
    setUrl(dto.url);
    setUuid(dto.uuid);
}

void Recording::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    emit activeChanged(m_active);
}

void Recording::setBroadcast(quint32 broadcast)
{
    if (m_broadcast == broadcast)
        return;

    m_broadcast = broadcast;
    emit broadcastChanged(m_broadcast);
}

void Recording::setChannelName(const QString &name)
{
    if (m_channelName == name)
        return;

    m_channelName = name;
    emit channelNameChanged(m_channelName);
}

void Recording::setChannelUuid(const QString &uuid)
{
    if (m_channelUuid == uuid)
        return;

    m_channelUuid = uuid;
    emit channelUuidChanged(m_channelUuid);
}

void Recording::setComment(const QString &comment)
{
    if (m_comment == comment)
        return;

    m_comment = comment;
    emit commentChanged(m_comment);
}

void Recording::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

void Recording::setFileSize(quint32 size)
{
    if (m_fileSize == size)
        return;

    m_fileSize = size;
    emit fileSizeChanged(m_fileSize);
}

void Recording::setPlayCount(quint32 count)
{
    if (m_playCount == count)
        return;

    m_playCount = count;
    emit playCountChanged(m_playCount);
}

void Recording::setPlayPosition(quint32 position)
{
    if (m_playPosition == position)
        return;

    m_playPosition = position;
    emit playPositionChanged(m_playPosition);
}

void Recording::setStart(const QDateTime &start)
{
    if (m_start == start)
        return;

    m_start = start;
    emit startChanged(m_start);
}

void Recording::setStop(const QDateTime &stop)
{
    if (m_stop == stop)
        return;

    m_stop = stop;
    emit stopChanged(m_stop);
}

void Recording::setStatus(quint16 status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(m_status);
}

void Recording::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle)
        return;

    m_subtitle = subtitle;
    emit subtitleChanged(m_subtitle);
}

void Recording::setTitle(const QString &title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(m_title);
}

void Recording::setUrl(const QString &url)
{
    if (m_url == url)
        return;

    m_url = url;
    emit urlChanged(m_url);
}

void Recording::setUuid(const QString &uuid)
{
    if (m_uuid == uuid)
        return;

    m_uuid = uuid;
    emit uuidChanged(m_uuid);
}
