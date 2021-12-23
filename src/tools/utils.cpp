#include "utils.h"

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>

#include "src/api/api_keys.h"

#include "compressor.h"

ChannelDTO Utils::channelFromJson(const QJsonObject &obj)
{
    ChannelDTO channel;

    channel.icon = obj.value(ApiKey::iconPublicUrl).toString();
    channel.name = obj.value(ApiKey::name).toString();
    channel.number = obj.value(ApiKey::number).toInt();
    channel.uuid = obj.value(ApiKey::uuid).toString();

    return channel;
}

QJsonObject Utils::parseJson(const QByteArray &data)
{
    const QByteArray uncompressed = Compressor::gunzip(data);
    return QJsonDocument::fromJson(uncompressed).object();
}


