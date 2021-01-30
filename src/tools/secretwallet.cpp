#include "secretwallet.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <Sailfish/Secrets/collectionnamesrequest.h>
#include <Sailfish/Secrets/createcollectionrequest.h>
#include <Sailfish/Secrets/request.h>
#include <Sailfish/Secrets/result.h>
#include <Sailfish/Secrets/secret.h>
#include <Sailfish/Secrets/storesecretrequest.h>
#include <Sailfish/Secrets/storedsecretrequest.h>
#include <Sailfish/Secrets/deletecollectionrequest.h>

using namespace Sailfish::Secrets;

static const QString collectionName(QStringLiteral("harbourtvhclient"));

bool checkResult(const Request &req)
{
    auto result = req.result();
    auto success = result.errorCode() == Result::NoError;
    if (!success) {
#ifdef QT_DEBUG
        qDebug() << result.errorMessage();
#endif
    }
    return success;
}

Secret::Identifier makeIdent(const QString &name)
{
    return Secret::Identifier(name, collectionName, SecretManager::DefaultEncryptedStoragePluginName);
}

bool createCollection(SecretManager *manager)
{
    CreateCollectionRequest ccr;
    ccr.setManager(manager);
    ccr.setCollectionName(collectionName);
    ccr.setAccessControlMode(SecretManager::OwnerOnlyMode);
    ccr.setCollectionLockType(CreateCollectionRequest::DeviceLock);
    ccr.setDeviceLockUnlockSemantic(SecretManager::DeviceLockKeepUnlocked);
    ccr.setStoragePluginName(SecretManager::DefaultEncryptedStoragePluginName);
    ccr.setEncryptionPluginName(SecretManager::DefaultEncryptedStoragePluginName);
    ccr.startRequest();
    ccr.waitForFinished();
    return checkResult(ccr);
}

SecretWallet::SecretWallet(QObject *parent) :
    QObject(parent)
{
    CollectionNamesRequest cnr;
    cnr.setManager(m_manager);
    cnr.setStoragePluginName(SecretManager::DefaultEncryptedStoragePluginName);
    cnr.startRequest();
    cnr.waitForFinished();
    m_valid = checkResult(cnr) && cnr.collectionNames().contains(collectionName);
}

bool SecretWallet::isValid() const
{
    return m_valid;
}

bool SecretWallet::storeData(const QString &key, const QByteArray &data)
{
    if (!m_valid) {
        m_valid = createCollection(m_manager);
    }

    Secret secret(makeIdent(key));
    secret.setData(data);

    StoreSecretRequest ssr;
    ssr.setManager(m_manager);
    ssr.setSecretStorageType(StoreSecretRequest::CollectionSecret);
    ssr.setUserInteractionMode(SecretManager::SystemInteraction);
    ssr.setSecret(secret);
    ssr.startRequest();
    ssr.waitForFinished();

    return checkResult(ssr);
}

QByteArray SecretWallet::data(const QString &key)
{
    if (!m_valid) {
        return QByteArray();
    }

    StoredSecretRequest ssr;
    ssr.setManager(m_manager);
    ssr.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    ssr.setIdentifier(makeIdent(key));;
    ssr.startRequest();
    ssr.waitForFinished();

    auto success = checkResult(ssr);
    if (success)
    {
        return ssr.secret().data();
    }

    return QByteArray();
}

bool SecretWallet::removeCollection()
{
    if (!m_valid) {
        return false;
    }

    DeleteCollectionRequest dcr;
    dcr.setManager(m_manager);
    dcr.setCollectionName(collectionName);
    dcr.setStoragePluginName(SecretManager::DefaultEncryptedStoragePluginName);
    dcr.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    dcr.startRequest();
    dcr.waitForFinished();
    return checkResult(dcr);
}
