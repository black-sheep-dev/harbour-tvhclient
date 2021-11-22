#ifndef EVENTSSORTFILTERMODEL_H
#define EVENTSSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

#include "eventsmodel.h"

class EventsSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QString pattern READ pattern WRITE setPattern)

public:
    explicit EventsSortFilterModel(QObject *parent = nullptr);

    Q_INVOKABLE Event *getEvent(int idx);
    const QString &pattern() const;

signals:

public slots:
    void setPattern(const QString &pattern);

private:
    QString m_pattern;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // EVENTSSORTFILTERMODEL_H
