#ifndef CHANNELSSORTFILTERMODEL_H
#define CHANNELSSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class ChannelsSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QString pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(bool showFavoritesOnly READ showFavoritesOnly WRITE setShowFavoritesOnly NOTIFY showFavoritesOnlyChanged)

public:
    explicit ChannelsSortFilterModel(QObject *parent = nullptr);

    const QString &pattern() const;
    bool showFavoritesOnly() const;

signals:
    void patternChanged();
    void showFavoritesOnlyChanged(bool showFavoritesOnly);

public slots:
    void setPattern(const QString &pattern);
    void setShowFavoritesOnly(bool showFavoritesOnly);

private:
    QString m_pattern;
    bool m_showFavoritesOnly{false};

    // QAbstractProxyModel interface
public:
    void setSourceModel(QAbstractItemModel *sourceModel) override;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // CHANNELSSORTFILTERMODEL_H
