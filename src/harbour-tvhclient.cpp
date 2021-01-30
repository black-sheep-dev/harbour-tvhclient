#include <QtQuick>

#include <sailfishapp.h>

#include "tvhclient.h"
#include "models/channelssortfiltermodel.h"
#include "models/eventssortfiltermodel.h"
#include "models/recordingssortfiltermodel.h"
#include "tools/iconprovider.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));


    auto uri = "org.nubecula.harbour.tvhclient";

    qmlRegisterType<ChannelsModel>(uri, 1, 0, "ChannelsModel");
    qmlRegisterType<ChannelsSortFilterModel>(uri, 1, 0, "ChannelsSortFilterModel");
    qmlRegisterType<Event>(uri, 1, 0, "Event");
    qmlRegisterType<EventsModel>(uri, 1, 0, "EventsModel");
    qmlRegisterType<EventsSortFilterModel>(uri, 1, 0, "EventsSortFilterModel");
    qmlRegisterType<Recording>(uri, 1, 0, "Recording");
    qmlRegisterType<RecordingsModel>(uri, 1, 0, "RecordingsModel");
    qmlRegisterType<RecordingsSortFilterModel>(uri, 1, 0, "RecordingsSortFilterModel");
    qmlRegisterType<ServerInfo>(uri, 1, 0, "ServerInfo");

    qmlRegisterSingletonType<TVHClient>(uri,
                                        1,
                                        0,
                                        "TVHClient",
                                        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        //Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        engine->addImageProvider(QLatin1String("channel"), new IconProvider);

        auto client = new TVHClient();

        return client;
    });

    return SailfishApp::main(argc, argv);
}
