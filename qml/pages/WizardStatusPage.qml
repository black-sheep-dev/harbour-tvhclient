import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

Dialog {
    property ServerInfo info: TVHClient.serverInfo()

    id: dialog

    canAccept: info.available

    Column {
        id: contentColumn
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            id: header
            title: qsTr("Server Status")

            acceptText: qsTr("Finish")
            cancelText: qsTr("Back")
        }

        Label {
            visible: info.loading
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            color: Theme.highlightColor

            text: qsTr("Connecting to server...")
        }

        DetailItem {
            visible: info.available
            label: qsTr("Server Name")
            value: info.name
        }

        DetailItem {
            visible: info.available
            label: qsTr("Software Version")
            value: info.version
        }

        DetailItem {
            visible: info.available
            label: qsTr("API Version")
            value: info.apiVersion
        }

        Label {
            visible: !info.available && !info.loading

            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            wrapMode: Text.Wrap

            text: qsTr("Connection to TVHeadend server failed. Please go back and check your connection settings.")

            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeLarge
        }
    }

    onAccepted: {
        pageStack.clear()
        pageStack.push(Qt.resolvedUrl("ChannelsListPage.qml"))
    }

    onStatusChanged: if (status === PageStatus.Activating) TVHClient.getServerInfo()
}

