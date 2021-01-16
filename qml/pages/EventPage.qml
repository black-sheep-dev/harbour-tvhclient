import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

import "../components/"

Page {
    property bool busy: false
    property Event event

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            busy: page.busy
            MenuItem {
                visible: !event.scheduled
                text: qsTr("Schedule Recording")
                onClicked:  remorse.execute(qsTr("Schedule recording event"), function() {
                    page.busy = true
                    TVHClient.recordEvent(event.channelUuid, event.eventId)
                })
            }
            MenuItem {
                visible: event.scheduled
                text: qsTr("Cancel scheduled recording")
                onClicked: remorse.execute(qsTr("Cancel scheduled event"), function() {
                    page.busy = true
                    TVHClient.cancelRecording(event.recordingUuid)
                })
            }
            MenuItem {
                visible: event.scheduled && (new Date().getTime() > event.start && new Date().getTime() < event.stop)
                text: qsTr("Stop recording")
                onClicked: remorse.execute(qsTr("Stop recording event "), function() {
                    page.busy = true
                    TVHClient.stopRecording(event.recordingUuid)
                })
            }
        }

        anchors.fill: parent
        contentHeight: column.height

        RemorsePopup { id: remorse }

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Event")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeLarge
                font.bold: true
                color: Theme.highlightColor

                text: event.title
            }

            Label {
                visible: event.subtitle.length > 0
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeTiny
                color: Theme.highlightColor

                text: event.subtitle
            }

            TimeInfoItem {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                start: event.start
                stop: event.stop
                color: Theme.highlightColor
            }

            Row {
                visible: event.scheduled
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                spacing: Theme.paddingMedium

                Icon {
                    id: scheduledIcon
                    source: "image://theme/icon-s-timer?" + Theme.errorColor
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.errorColor
                    font.pixelSize: Theme.fontSizeSmall

                    text: qsTr("Scheduled for recording")
                }
            }

            Separator {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x
                color: Theme.highlightBackgroundColor
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.Wrap

                font.pixelSize: Theme.fontSizeSmall

                text: event.description
            }

        }
    }

    Connections {
        target: TVHClient
        onEventScheduled: {
            page.busy = false
            event.recordingUuid = uuid
        }
    }

    Connections {
        target: TVHClient
        onEventCanceled: {
            page.busy = false
            event.recordingUuid = ""
        }
    }
}




