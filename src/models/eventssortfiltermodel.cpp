#include "eventssortfiltermodel.h"

EventsSortFilterModel::EventsSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

Event *EventsSortFilterModel::getEvent(int idx)
{
    auto *model = qobject_cast<EventsModel *>(sourceModel());

    if (model == nullptr)
        return nullptr;

    return model->getEvent(mapToSource(index(idx, 0)));
}

const QString &EventsSortFilterModel::pattern() const
{
    return m_pattern;
}

void EventsSortFilterModel::setPattern(const QString &pattern)
{
    if (m_pattern == pattern) {
        return;
    }

    m_pattern = pattern;
    emit patternChanged();
    invalidateFilter();
}

bool EventsSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    return sourceModel()->data(index, EventsModel::TitleRole).toString().contains(m_pattern)
            || sourceModel()->data(index, EventsModel::SubtitleRole).toString().contains(m_pattern);
}
