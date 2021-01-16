#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QAbstractListModel>

#include <QTimer>

class BaseModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(quint32 updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
    explicit BaseModel(QObject *parent = nullptr);

    // properties
    bool active() const;
    bool loading() const;
    quint32 updateInterval() const;

signals:
    // properties
    void activeChanged(bool active);
    void loadingChanged(bool loading);
    void updateIntervalChanged(quint32 interval);

public slots:
    // properties
    void setActive(bool active);
    void setLoading(bool loading);
    void setUpdateInterval(quint32 interval);

private:
    QTimer *m_timer{new QTimer(this)};

    // properties
    bool m_active{false};
    bool m_loading{false};
    quint32 m_updateInterval{60000};

    // virtual functions
public slots:
    virtual void refresh();
};

#endif // BASEMODEL_H
