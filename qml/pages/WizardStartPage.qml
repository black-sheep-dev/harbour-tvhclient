import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    acceptDestination: Qt.resolvedUrl("WizardConnectionPage.qml")

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            id: header

            title: qsTr("Setup Wizard")

            acceptText: qsTr("Continue")
            cancelText: qsTr("Close")
        }

        Image {
            id: logo
            source: "/usr/share/icons/hicolor/512x512/apps/" + Qt.application.name + ".png"
            smooth: true
            height: parent.width / 2
            width: parent.width / 2
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x

            text: qsTr("Welcome to TVH Client")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x

            color: Theme.highlightColor

            wrapMode: Text.WordWrap

            text: qsTr("This wizard guides you through the necessary steps to establish a connection with a TVHeadend server.")
        }
    }
}
