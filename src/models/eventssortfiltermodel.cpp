#include "eventssortfiltermodel.h"

EventsSortFilterModel::EventsSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

Event *EventsSortFilterModel::getEvent(int idx)
{
    EventsModel *model = qobject_cast<EventsModel *>(sourceModel());

    if (model == nullptr)
        return nullptr;

    return model->getEvent(mapToSource(index(idx, 0)));
}

QString EventsSortFilterModel::pattern() const
{
    return m_pattern;
}

void EventsSortFilterModel::setPattern(const QString &pattern)
{
    m_pattern = pattern;
    invalidateFilter();
}

bool EventsSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    return sourceModel()->data(index, EventsModel::TitleRole).toString().contains(m_pattern)
            || sourceModel()->data(index, EventsModel::SubtitleRole).toString().contains(m_pattern);
}
