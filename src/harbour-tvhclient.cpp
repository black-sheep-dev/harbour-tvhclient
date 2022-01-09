#include <QtQuick>

#include <sailfishapp.h>

#include "tvhclient.h"
#include "models/channelssortfiltermodel.h"
#include "models/eventssortfiltermodel.h"
#include "models/recordingssortfiltermodel.h"
#include "tools/iconprovider.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationVersion(APP_VERSION);
    app->setApplicationName("tvhclient");
    app->setOrganizationDomain("org.nubecula");
    app->setOrganizationName("org.nubecula");

    QScopedPointer<QQuickView> v(SailfishApp::createView());
    v->engine()->addImageProvider(QStringLiteral("channel"), new IconProvider);

#ifdef QT_DEBUG
    #define uri "org.nubecula.harbour.tvhclient"
#else
    const auto uri = "org.nubecula.harbour.tvhclient";
#endif

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

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        auto client = new TVHClient(qApp);

        return client;
    });



    v->setSource(SailfishApp::pathTo("qml/harbour-tvhclient.qml"));
    v->show();

    return app->exec();
}
