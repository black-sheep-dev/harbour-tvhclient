#include "iconprovider.h"

#include <QStandardPaths>

IconProvider::IconProvider() :
    QQuickImageProvider(QQuickImageProvider(QQuickImageProvider::Image))
{

}

QImage IconProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)

    const QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/icons/");

    QImage img(path + id + ".png");

    if (img.isNull())
        return QImage(":/icons/channel");

    return img;
}
