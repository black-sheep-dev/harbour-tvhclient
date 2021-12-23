import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Image {
        width: parent.width * 0.8
        height: width
        smooth: true
        source: "/usr/share/icons/hicolor/512x512/apps/harbour-tvhclient.png"
        anchors.centerIn: parent
        opacity: 0.1
    }

    Label {
        id: headerLabel
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: Theme.paddingLarge
        }
        horizontalAlignment: Text.AlignHCenter
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeLarge
        text: "TVH Client"
    }
}
