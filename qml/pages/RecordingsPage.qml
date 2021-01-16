import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: ListModel {
            ListElement {
                title: qsTr("Upcoming / Ongoing");
                description: qsTr("Show upcoming and ongoing recordings")
                icon: "image://theme/icon-m-backup"
                recordingStatus: 12 // Recording.StatusScheduled | Recording.StatusRecording
            }
            ListElement {
                title: qsTr("Finished");
                description: qsTr("Show finished recordings")
                icon: "image://theme/icon-m-acknowledge"
                recordingStatus: Recording.StatusCompleted
            }
            ListElement {
                title: qsTr("Failed");
                description: qsTr("Show failed recordings")
                icon: "image://theme/icon-m-clear"
                recordingStatus: Recording.StatusCompletedError
            }
        }

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Recordings")
        }

        delegate: ListItem {
            id: delegate
            width: parent.width
            contentHeight: Theme.itemSizeLarge

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIcon
                    source: icon
                    anchors.verticalCenter: parent.verticalCenter
                }

                Item {
                    width:Theme.paddingMedium
                    height:1
                }

                Column {
                    id: data
                    width: parent.width - itemIcon.width
                    anchors.verticalCenter: itemIcon.verticalCenter
                    spacing: Theme.paddingSmall

                    Label {
                        id: text
                        width: parent.width
                        text: model.title
                        color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                    Label {
                        text: model.description
                        color: pressed ? Theme.secondaryColor:Theme.primaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl("RecordingsListPage.qml"), {
                                          recordingStatus: model.recordingStatus,
                                          title: model.title
                                      })
        }

        VerticalScrollDecorator {}
    }
}
