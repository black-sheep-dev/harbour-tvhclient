#include "basemodel.h"

BaseModel::BaseModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

bool BaseModel::active() const
{
    return m_active;
}

bool BaseModel::loading() const
{
    return m_loading;
}

quint32 BaseModel::updateInterval() const
{
    return m_updateInterval;
}

void BaseModel::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    emit activeChanged(m_active);

    if (m_active) {
        connect(m_timer, &QTimer::timeout, this, &BaseModel::refresh);
        m_timer->setInterval(m_updateInterval);
        m_timer->start();
    } else {
        m_timer->stop();
        disconnect(m_timer, &QTimer::timeout, this, &BaseModel::refresh);
    }
}

void BaseModel::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

void BaseModel::setUpdateInterval(quint32 interval)
{
    if (m_updateInterval == interval)
        return;

    m_updateInterval = interval;
    emit updateIntervalChanged(m_updateInterval);

    m_timer->setInterval(m_updateInterval);
}

void BaseModel::refresh()
{

}
