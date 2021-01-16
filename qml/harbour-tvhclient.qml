import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

import org.nubecula.harbour.tvhclient 1.0

ApplicationWindow
{
    initialPage: Component { ChannelsListPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: TVHClient.initialize()
}
