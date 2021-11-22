#include "channel.h"

Channel::Channel(QObject *parent) :
    QObject(parent)
{

}

Channel::Channel(const ChannelDTO &dto, QObject *parent) :
    QObject(parent)
{
    setData(dto);
}

bool Channel::enabled() const
{
    return m_enabled;
}

bool Channel::favorite() const
{
    return m_favorite;
}

const QString &Channel::icon() const
{
    return m_icon;
}

const QString &Channel::name() const
{
    return m_name;
}

quint16 Channel::number() const
{
    return m_number;
}

const QString &Channel::uuid() const
{
    return m_uuid;
}

quint16 Channel::currentDuration() const
{
    return m_currentDuration;
}

quint8 Channel::currentFeatures() const
{
    return m_currentFeatures;
}

const QDateTime &Channel::currentStart() const
{
    return m_currentStart;
}

const QDateTime &Channel::currentStop() const
{
    return m_currentStop;
}

const QString &Channel::currentSubtitle() const
{
    return m_currentSubtitle;
}

const QString &Channel::currentTitle() const
{
    return m_currentTitle;
}

const QString &Channel::nextTitle() const
{
    return m_nextTitle;
}

bool Channel::recording() const
{
    return m_recording;
}

void Channel::setData(const ChannelDTO &dto)
{
    setIcon(dto.icon);
    setName(dto.name);
    setNumber(dto.number);
    setUuid(dto.uuid);

    setCurrentFeatures(dto.current.features);
    setCurrentStart(QDateTime::fromTime_t(dto.current.start));
    setCurrentStop(QDateTime::fromTime_t(dto.current.stop));
    setCurrentSubtitle(dto.current.subtitle.simplified());
    setCurrentTitle(dto.current.title);
    setNextTitle(dto.current.nextTitle);

    setCurrentDuration((dto.current.stop - dto.current.start) / 60);
}

void Channel::setEnabled(bool enabled)
{
    if (m_enabled == enabled) {
        return; }

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

void Channel::setFavorite(bool favorite)
{
    if (m_favorite == favorite) {
        return; }

    m_favorite = favorite;
    emit favoriteChanged(m_favorite);
}

void Channel::setIcon(const QString &icon)
{
    if (m_icon == icon) {
        return; }

    m_icon = icon;
    emit iconChanged(m_icon);
}

void Channel::setName(const QString &name)
{
    if (m_name == name) {
        return; }

    m_name = name;
    emit nameChanged(m_name);
}

void Channel::setNumber(quint16 number)
{
    if (m_number == number) {
        return; }

    m_number = number;
    emit numberChanged(m_number);
}

void Channel::setUuid(const QString &uuid)
{
    if (m_uuid == uuid) {
        return; }

    m_uuid = uuid;
    emit uuidChanged(m_uuid);
}

void Channel::setCurrentDuration(quint16 duration)
{
    if (m_currentDuration == duration) {
        return; }

    m_currentDuration = duration;
    emit currentDurationChanged(m_currentDuration);
}

void Channel::setCurrentFeatures(quint8 currentFeatures)
{
    if (m_currentFeatures == currentFeatures) {
        return; }

    m_currentFeatures = currentFeatures;
    emit currentFeaturesChanged(m_currentFeatures);
}

void Channel::setCurrentStart(const QDateTime &start)
{
    if (m_currentStart == start) {
        return; }

    m_currentStart = start;
    emit currentStartChanged(m_currentStart);
}

void Channel::setCurrentStop(const QDateTime &stop)
{
    if (m_currentStop == stop) {
        return; }

    m_currentStop = stop;
    emit currentStopChanged(m_currentStop);
}

void Channel::setCurrentSubtitle(const QString &subtitle)
{
    if (m_currentSubtitle == subtitle) {
        return; }

    m_currentSubtitle = subtitle;
    emit currentSubtitleChanged(m_currentSubtitle);
}

void Channel::setCurrentTitle(const QString &title)
{
    if (m_currentTitle == title) {
        return; }

    m_currentTitle = title;
    emit currentTitleChanged(m_currentTitle);
}

void Channel::setNextTitle(const QString &title)
{
    if (m_nextTitle == title) {
        return; }

    m_nextTitle = title;
    emit nextTitleChanged(m_nextTitle);
}

void Channel::setRecording(bool recording)
{
    if (m_recording == recording) {
        return; }

    m_recording = recording;
    emit recordingChanged(m_recording);
}
