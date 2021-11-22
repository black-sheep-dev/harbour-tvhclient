#include "recordingssortfiltermodel.h"

#include "recordingsmodel.h"

RecordingsSortFilterModel::RecordingsSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

const QString &RecordingsSortFilterModel::pattern() const
{
    return m_pattern;
}

quint8 RecordingsSortFilterModel::status() const
{
    return m_status;
}

void RecordingsSortFilterModel::setPattern(const QString &pattern)
{
    m_pattern = pattern;
    invalidateFilter();
}

void RecordingsSortFilterModel::setStatus(quint8 status)
{
    m_status = status;
    invalidateFilter();
}

bool RecordingsSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    quint8 found{0}; // bit flag

    if (m_status == Recording::StatusUnknown) {
        found |= 1;
    } else if (m_status & sourceModel()->data(index, RecordingsModel::StatusRole).toUInt()) {
        found |= 1;
    }

    if (m_pattern.isEmpty()) {
        found |= 2;
    } else {
        if (sourceModel()->data(index, RecordingsModel::TitleRole).toString().contains(m_pattern, Qt::CaseInsensitive))
            found |= 2;
    }

    return (found & 3) == 3;
}
