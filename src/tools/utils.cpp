#include "utils.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "src/api/api_keys.h"

#include "zlib.h"

ChannelDTO Utils::channelFromJson(const QJsonObject &obj)
{
    ChannelDTO channel;

    channel.icon = obj.value(ApiKey::iconPublicUrl).toString();
    channel.name = obj.value(ApiKey::name).toString();
    channel.number = obj.value(ApiKey::number).toInt();
    channel.uuid = obj.value(ApiKey::uuid).toString();

    return channel;
}

QByteArray Utils::gunzip(const QByteArray &data)
{
    if (data.size() <= 4) {
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

QJsonObject Utils::parseJson(const QByteArray &data)
{
    QByteArray uncompressed = Utils::gunzip(data);

    if (uncompressed.isEmpty())
        uncompressed = data;

    return QJsonDocument::fromJson(uncompressed).object();
}


