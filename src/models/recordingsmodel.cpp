#include "recordingsmodel.h"

RecordingsModel::RecordingsModel(QObject *parent) :
    BaseModel(parent)
{

}

void RecordingsModel::clear()
{
    beginResetModel();
    qDeleteAll(m_recordings.begin(), m_recordings.end());
    m_recordings.clear();
    endResetModel();
}

QList<Recording *> RecordingsModel::recordings() const
{
    return m_recordings;
}

void RecordingsModel::setRecordings(const QList<RecordingDTO> recordings)
{
    beginResetModel();
    if (!m_recordings.isEmpty()) {
        qDeleteAll(m_recordings.begin(), m_recordings.end());
        m_recordings.clear();
    }

    for (const auto &dto : recordings) {
        auto recording = new Recording(dto, this);

        connect(recording, &Recording::activeChanged, this, &RecordingsModel::sendRecordingState);

        m_recordings.append(recording);
    }
    endResetModel();

    setLoading(false);
}

void RecordingsModel::sendRecordingState(bool active)
{
    auto *recording = qobject_cast<Recording *>(sender());

    if (recording == nullptr)
        return;

    emit recordingStateChanged(recording->channelUuid(), active);
}

void RecordingsModel::refresh()
{
    const QDateTime current = QDateTime::currentDateTimeUtc();

    for (auto &recording : m_recordings) {
        recording->setActive(current > recording->start() && current < recording->stop());
    }
}

int RecordingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_recordings.count();
}

QVariant RecordingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto recording = m_recordings.at(index.row());

    switch (role) {
    case ActiveRole:
        return recording->active();

    case BroadcastRole:
        return recording->broadcast();

    case ChannelNameRole:
        return recording->channelName();

    case ChannelUuidRole:
        return recording->channelUuid();

    case CommentRole:
        return recording->comment();

    case DescriptionRole:
        return recording->description();

    case FileSizeRole:
        return recording->fileSize();

    case PlayCountRole:
        return recording->playCount();

    case PlayPositionRole:
        return recording->playPosition();

    case StartRole:
        return recording->start();

    case StopRole:
        return recording->stop();

    case StatusRole:
        return recording->status();

    case SubtitleRole:
        return recording->subtitle();

    case TitleRole:
        return recording->title();

    case UrlRole:
        return recording->url();

    case UuidRole:
        return recording->uuid();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> RecordingsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ActiveRole]           = "active";
    roles[BroadcastRole]        = "broadcast";
    roles[ChannelNameRole]      = "channelName";
    roles[ChannelUuidRole]      = "channelUuid";
    roles[CommentRole]          = "comment";
    roles[DescriptionRole]      = "description";
    roles[FileSizeRole]         = "fileSize";
    roles[PlayCountRole]        = "playCount";
    roles[PlayPositionRole]     = "playPosition";
    roles[StartRole]            = "start";
    roles[StopRole]             = "stop";
    roles[StatusRole]           = "status";
    roles[SubtitleRole]         = "subtitle";
    roles[TitleRole]            = "title";
    roles[UrlRole]              = "url";
    roles[UuidRole]             = "uuid";

    return roles;
}
