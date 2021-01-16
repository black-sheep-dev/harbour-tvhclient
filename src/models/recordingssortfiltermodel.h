#ifndef RECORDINGSSORTFILTERMODEL_H
#define RECORDINGSSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class RecordingsSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QString pattern READ pattern WRITE setPattern)
    Q_PROPERTY(quint8 status READ status WRITE setStatus)

public:
    explicit RecordingsSortFilterModel(QObject *parent = nullptr);

    QString pattern() const;
    quint8 status() const;

public slots:
    void setPattern(const QString &pattern);
    void setStatus(quint8 status);

private:
    QString m_pattern;
    quint8 m_status{0};

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // RECORDINGSSORTFILTERMODEL_H
