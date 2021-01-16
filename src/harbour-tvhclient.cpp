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


    qmlRegisterType<ChannelsModel>("org.nubecula.harbour.tvhclient", 1, 0, "ChannelsModel");
    qmlRegisterType<ChannelsSortFilterModel>("org.nubecula.harbour.tvhclient", 1, 0, "ChannelsSortFilterModel");
    qmlRegisterType<Event>("org.nubecula.harbour.tvhclient", 1, 0, "Event");
    qmlRegisterType<EventsModel>("org.nubecula.harbour.tvhclient", 1, 0, "EventsModel");
    qmlRegisterType<EventsSortFilterModel>("org.nubecula.harbour.tvhclient", 1, 0, "EventsSortFilterModel");
    qmlRegisterType<Recording>("org.nubecula.harbour.tvhclient", 1, 0, "Recording");
    qmlRegisterType<RecordingsModel>("org.nubecula.harbour.tvhclient", 1, 0, "RecordingsModel");
    qmlRegisterType<RecordingsSortFilterModel>("org.nubecula.harbour.tvhclient", 1, 0, "RecordingsSortFilterModel");
    qmlRegisterType<ServerInfo>("org.nubecula.harbour.tvhclient", 1, 0, "ServerInfo");

    qmlRegisterSingletonType<TVHClient>("org.nubecula.harbour.tvhclient",
                                              1,
                                              0,
                                              "TVHClient",
                                              [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        //Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        engine->addImageProvider(QLatin1String("channel"), new IconProvider);

        auto *client = new TVHClient();

        return client;
    });

    return SailfishApp::main(argc, argv);
}
