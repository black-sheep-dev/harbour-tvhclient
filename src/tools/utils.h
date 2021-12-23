#ifndef UTILS_H
#define UTILS_H

#include "src/entities/channel.h"

class Utils
{
public:
    static ChannelDTO channelFromJson(const QJsonObject &obj);
    static QJsonObject parseJson(const QByteArray &data);

};

#endif // UTILS_H
