#ifndef SECRETWALLET_H
#define SECRETWALLET_H

#include <QObject>

#include <Sailfish/Secrets/secretmanager.h>

class SecretWallet : public QObject
{
    Q_OBJECT

public:
    explicit SecretWallet(QObject *parent = nullptr);

    bool isValid() const;
    bool storeData(const QString &key, const QByteArray &data);
    QByteArray data(const QString &key);
    bool removeCollection();

private:
    Sailfish::Secrets::SecretManager *m_manager{new Sailfish::Secrets::SecretManager(this)};
    bool m_valid{false};

};

#endif // SECRETWALLET_H
