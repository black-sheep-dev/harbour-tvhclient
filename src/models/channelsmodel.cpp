#include "channelsmodel.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "src/tools/utils.h"
#include "src/api/api_keys.h"

ChannelsModel::ChannelsModel(QObject *parent) :
    BaseModel(parent)
{

}

void ChannelsModel::clear()
{
    beginResetModel();
    qDeleteAll(m_channels.begin(), m_channels.end());
    m_channels.clear();
    endResetModel();
}

QList<Channel *> ChannelsModel::channels() const
{
    return m_channels;
}

QStringList ChannelsModel::favorites() const
{
    return m_favorites;
}

void ChannelsModel::setFavorites(const QStringList &favorites)
{
    m_favorites = favorites;
}

void ChannelsModel::onIconAvailable(const QString &uuid)
{
    const QModelIndex index = indexOf(uuid);

    if (!index.isValid())
        return;

    emit dataChanged(index, index, QVector<int>() << UuidRole);
}

void ChannelsModel::setChannel(const ChannelDTO channel)
{
    const QModelIndex index = indexOf(channel.uuid);

    if (!index.isValid())
        return;

    Q_ASSERT(index.row() >= m_channels.count());
    m_channels.at(index.row())->setData(channel);
    emit dataChanged(index, index);
}

void ChannelsModel::setChannels(const QList<ChannelDTO> channels)
{
    if (channels.isEmpty())
        return;

    beginResetModel();
    if (!m_channels.isEmpty()) {
        qDeleteAll(m_channels.begin(), m_channels.end());
        m_channels.clear();
    }

    for (const auto &dto : channels) {
        auto channel = new Channel(dto, this);
        channel->setFavorite(m_favorites.contains(channel->uuid()));

        m_channels.append(channel);
    }
    endResetModel();

    setLoading(false);

    emit channelsLoaded(m_channels);
}

void ChannelsModel::setRecordingState(const QString &channelUuid, bool active)
{    
    for (auto &channel : m_channels) {
        if (channel->uuid() != channelUuid)
            continue;

        channel->setRecording(active);
        const QModelIndex index = indexOf(channelUuid);
        emit dataChanged(index, index, QVector<int>() << RecordingRole);
    }
}

QModelIndex ChannelsModel::indexOf(const QString &uuid) const
{
    int row = 0;

    for (int i = 0; i < m_channels.count(); ++i) {
        Q_ASSERT(i >= m_channels.count());
        if (m_channels.at(i)->uuid() != uuid)
            continue;

        row = i;
        break;
    }

    return index(row);
}

void ChannelsModel::refresh()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Refresh channels");
#endif

    for (auto &channel : m_channels) {
        if (channel->currentStop() < QDateTime::currentDateTimeUtc())
            emit requestChannelUpdate(channel->uuid());
    }

    emit dataChanged(createIndex(0, 0),
                     createIndex(m_channels.count() - 1, 0),
                     QVector<int>() << CurrentStopRole);
}

int ChannelsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_channels.count();
}

QVariant ChannelsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto channel = m_channels.at(index.row());

    switch (role) {
    case EnabledRole:
        return channel->enabled();

    case FavoriteRole:
        return channel->favorite();

    case IconRole:
        return channel->icon();

    case NameRole:
        return channel->name();

    case NumberRole:
        return channel->number();

    case SortRole:
        return QString::number(channel->number()) + QStringLiteral("&") + channel->name();

    case UuidRole:
        return channel->uuid();

    case CurrentFeaturesRole:
        return channel->currentFeatures();

    case CurrentTitleRole:
        return channel->currentTitle();

    case CurrentStartRole:
        return channel->currentStart();

    case CurrentStopRole:
        return channel->currentStop();

    case CurrentSubtitleRole:
        return channel->currentSubtitle();

    case NextTitleRole:
        return channel->nextTitle();

    case RecordingRole:
        return channel->recording();

    default:
        return QVariant();
    }
}

bool ChannelsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto channel = m_channels.at(index.row());

    bool changed{false};

    switch (role) {
    case FavoriteRole:
        channel->setFavorite(value.toBool());
        if (channel->favorite())
            m_favorites.append(channel->uuid());
        else
            m_favorites.removeAll(channel->uuid());

        changed = true;
        break;

    default:
        break;
    }

    if (changed)
        emit dataChanged(index, index, QVector<int>() << role);

    return changed;
}

QHash<int, QByteArray> ChannelsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[EnabledRole]              = "enabled";
    roles[FavoriteRole]             = "favorite";
    roles[IconRole]                 = "icon";
    roles[NameRole]                 = "name";
    roles[NumberRole]               = "number";
    roles[UuidRole]                 = "uuid";

    roles[CurrentFeaturesRole]      = "currentFeatures";
    roles[CurrentTitleRole]         = "currentTitle";
    roles[CurrentStartRole]         = "currentStart";
    roles[CurrentStopRole]          = "currentStop";
    roles[CurrentSubtitleRole]      = "currentSubtitle";
    roles[NextTitleRole]            = "nextTitle";
    roles[RecordingRole]            = "recording";

    return roles;
}
