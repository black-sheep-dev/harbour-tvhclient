import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        RemorsePopup { id: remorse }

        Column {
            id: column

            x: Theme.horizontalPageMargin

            width: page.width - 2 * x
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Cache Settings")
            }

            SectionHeader {
                text: qsTr("Local Cache")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.Wrap

                color: Theme.highlightColor

                text: qsTr("By pressing the reset button the local cache is cleared. All channels, events and recordings will be fetched from the server again.")
            }

            ButtonLayout {
                width: parent.width

                Button {
                    id: refreshChannelsButton
                    text: qsTr("Reset")

                    onClicked: remorse.execute(qsTr("Resetting local cache"), function() { TVHClient.resetCache() } )
                }
            }


            SectionHeader {
                text: qsTr("Icon Cache")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.Wrap

                color: Theme.highlightColor

                text: qsTr("By pressing the reset button the icon cache is cleared. All icons will be fetched from the server again.")
            }

            ButtonLayout {
                width: parent.width

                Button {
                    id: refreshChannelIconsButton
                    text: qsTr("Reset")

                    onClicked: remorse.execute(qsTr("Resetting icons"), function() { TVHClient.resetIconCache() } )
                }
            }
        }
    }
}

