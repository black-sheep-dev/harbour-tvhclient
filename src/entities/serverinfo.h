#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <QObject>

class ServerInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint8 apiVersion READ apiVersion WRITE setApiVersion NOTIFY apiVersionChanged)
    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)

public:
    explicit ServerInfo(QObject *parent = nullptr);

    quint8 apiVersion() const;
    bool available() const;
    bool loading() const;
    QString name() const;
    QString version() const;

signals:
    void apiVersionChanged(quint8 version);
    void availableChanged(bool available);
    void loadingChanged(bool loading);
    void nameChanged(QString name);
    void versionChanged(QString version);

public slots:
    void setApiVersion(quint8 version);
    void setAvailable(bool available);
    void setLoading(bool loading);
    void setName(const QString &name);
    void setVersion(const QString &version);

private:
    quint8 m_apiVersion{0};
    bool m_available{false};
    bool m_loading{false};
    QString m_name;
    QString m_version;
};

#endif // SERVERINFO_H
