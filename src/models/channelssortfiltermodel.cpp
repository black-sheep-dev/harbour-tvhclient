#include "channelssortfiltermodel.h"

#include "channelsmodel.h"

ChannelsSortFilterModel::ChannelsSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortRole(ChannelsModel::SortRole);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
}

QString ChannelsSortFilterModel::pattern() const
{
    return m_pattern;
}

bool ChannelsSortFilterModel::showFavoritesOnly() const
{
    return m_showFavoritesOnly;
}

void ChannelsSortFilterModel::setPattern(const QString &pattern)
{
    m_pattern = pattern;
    invalidateFilter();
}

void ChannelsSortFilterModel::setShowFavoritesOnly(bool showFavoritesOnly)
{
    if (m_showFavoritesOnly == showFavoritesOnly)
        return;

    m_showFavoritesOnly = showFavoritesOnly;
    emit showFavoritesOnlyChanged(m_showFavoritesOnly);
    invalidateFilter();
}

void ChannelsSortFilterModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);

    sort(0, Qt::AscendingOrder);
}

bool ChannelsSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    if (!index.isValid())
        return false;

    quint8 found{0}; // bit flag

    if (m_showFavoritesOnly) {
        if (sourceModel()->data(index, ChannelsModel::FavoriteRole).toBool())
            found |= 1;
    } else {
        found |= 1;
    }

    if (m_pattern.isEmpty()) {
        found |= 2;
    } else {
        if (sourceModel()->data(index, ChannelsModel::NameRole).toString().contains(m_pattern, Qt::CaseInsensitive))
            found |= 2;
    }

    return (found & 3) == 3;
}

bool ChannelsSortFilterModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    const quint8 numberLeft = sourceModel()->data(source_left, ChannelsModel::NumberRole).toUInt();
    const quint8 numberRight = sourceModel()->data(source_right, ChannelsModel::NumberRole).toUInt();

    if (numberLeft < numberRight)
        return true;
    else if (numberLeft > numberRight)
        return false;

    const QString nameLeft = sourceModel()->data(source_left, ChannelsModel::NameRole).toString();
    const QString nameRight = sourceModel()->data(source_right, ChannelsModel::NameRole).toString();

    return QString::localeAwareCompare(nameLeft, nameRight) < 0;
}
