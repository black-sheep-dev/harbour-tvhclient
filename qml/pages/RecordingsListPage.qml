import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

Page {
    property int recordingStatus: Recording.Unknown
    property string title

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            busy: TVHClient.recordingsModel().loading
            MenuItem {
                text: qsTr("Refresh")
                onClicked: TVHClient.getRecordings()
            }

            MenuItem {
                text: listView.showSearch ? qsTr("Hide search") : qsTr("Search")
                onClicked: {
                    listView.showSearch = !listView.showSearch

                    if (!listView.showSearch) {
                        searchField.focus = false
                        searchField.text = ""
                    }
                }
            }
        }

        anchors.fill: parent

        Column {
            id: header
            width: parent.width

            PageHeader {
                title: page.title
            }

            SearchField {
                id: searchField
                width: parent.width
                height: listView.showSearch ? implicitHeight : 0
                opacity: listView.showSearch ? 1 : 0
                onTextChanged: TVHClient.recordingsModel().pattern = text


                EnterKey.onClicked: searchField.focus = false

                Connections {
                    target: listView
                    onShowSearchChanged: {
                        searchField.forceActiveFocus()
                    }
                }

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }
                Behavior on opacity {
                    NumberAnimation { duration: 300 }
                }
            }
        }

        SilicaListView {
            property bool showSearch: false

            id: listView

            width: parent.width
            anchors.top: header.bottom
            anchors.bottom: parent.bottom

            clip: true

            model: RecordingsSortFilterModel {
                id: recordingsSortFilterModel
                sourceModel: TVHClient.recordingsModel()
                status: recordingStatus
            }

            delegate: ListItem {
                id: delegate

                contentHeight: contentColumn.height + separatorTop.height + Theme.paddingSmall

                menu: ContextMenu {
                    MenuItem {
                        visible: model.status & Recording.StatusCompleted
                        text: qsTr("Delete")
                        onClicked: delegate.remorseAction(qsTr("Delete recording"), function() { TVHClient.removeRecording(model.uuid)})
                    }
                    MenuItem {
                        visible: model.status & Recording.StatusScheduled
                        text: qsTr("Cancel")
                        onClicked: delegate.remorseAction(qsTr("Cancel recording"), function() { TVHClient.cancelRecording(model.uuid)})
                    }
                    MenuItem {
                        visible: model.status & Recording.StatusRecording
                        text: qsTr("Stop")
                        onClicked: delegate.remorseAction(qsTr("Stop recording"), function() { TVHClient.stopRecording(model.uuid)})
                    }
                    MenuItem {
                        visible: model.status & Recording.StatusCompleted
                        text: qsTr("Play")
                        onClicked: pageStack.push(Qt.resolvedUrl("StreamPlayerPage.qml"), {
                                                      url: "/" + model.url
                                                  })
                    }
                }

                Separator {
                    id: separatorTop
                    anchors.top: parent.top
                    visible: index > 0
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    color: Theme.primaryColor
                }

                Column {
                    id: contentColumn
                    anchors.top: separatorTop.bottom
                    width: parent.width
                    spacing: Theme.paddingSmall

                    Item {
                        visible: index > 0
                        width: 1
                        height: 0
                    }

                    Row {
                        id: contentRow
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x
                        spacing: Theme.paddingSmall

                        Image {
                            id: channelIcon
                            source: "image://channel/" + model.channelUuid

                            cache: true
                            smooth: true

                            width: Theme.itemSizeMedium
                            height: Theme.itemSizeMedium
                            fillMode: Image.PreserveAspectFit

                            Image {
                                id: statusIcon

                                anchors.left: parent.left
                                anchors.bottom: parent.bottom

                                source: {
                                    switch (model.status){
                                    case Recording.StatusCompleted:
                                        return "image://theme/icon-s-checkmark"
                                    case Recording.StatusCompletedError:
                                        return "image://theme/icon-s-warning"
                                    case Recording.StatusRecording:
                                        return "image://theme/icon-s-update"
                                    case Recording.StatusScheduled:
                                        return "image://theme/icon-s-timer"
                                    default:
                                        return "image://theme/icon-s-maybe"
                                    }
                                }
                            }
                        }

                        Column {
                            width: parent.width - channelIcon.width - parent.spacing
                            spacing: Theme.paddingSmall
                            anchors.verticalCenter: parent.verticalCenter

                            Label {
                                width: parent.width
                                font.bold: true
                                font.pixelSize: Theme.fontSizeMedium
                                wrapMode: Text.WordWrap

                                text: model.title
                            }
                            Item {
                                height: dateLabel.height
                                width: parent.width

                                Label {
                                    id: dateLabel
                                    anchors.left: parent.left
                                    font.pixelSize: Theme.fontSizeTiny
                                    text: model.start.toLocaleDateString(Qt.locale())
                                }

                                Label {
                                    anchors.right: parent.right
                                    font.pixelSize: Theme.fontSizeTiny
                                    text: model.start.toLocaleTimeString(Qt.locale(), "hh:mm") + " - " + model.stop.toLocaleTimeString(Qt.locale(), "hh:mm")
                                }
                            }
                        }

                    }
                }
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                text: qsTr("No recordings available")
            }

            VerticalScrollDecorator {}
        }
    }

    onVisibleChanged: TVHClient.recordingsModel().active = visible
}
