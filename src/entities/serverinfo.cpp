#include "serverinfo.h"

ServerInfo::ServerInfo(QObject *parent) :
    QObject(parent)
{

}

quint8 ServerInfo::apiVersion() const
{
    return m_apiVersion;
}

bool ServerInfo::available() const
{
    return m_available;
}

bool ServerInfo::loading() const
{
    return m_loading;
}

const QString &ServerInfo::name() const
{
    return m_name;
}

const QString &ServerInfo::version() const
{
    return m_version;
}

void ServerInfo::setApiVersion(quint8 version)
{
    if (m_apiVersion == version)
        return;

    m_apiVersion = version;
    emit apiVersionChanged(m_apiVersion);
}

void ServerInfo::setAvailable(bool available)
{
    if (m_available == available)
        return;

    m_available = available;
    emit availableChanged(m_available);
}

void ServerInfo::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

void ServerInfo::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void ServerInfo::setVersion(const QString &version)
{
    if (m_version == version)
        return;

    m_version = version;
    emit versionChanged(m_version);
}
