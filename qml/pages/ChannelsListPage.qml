import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

import "../components/"

Page {    
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            busy: TVHClient.channelsModel().loading
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem {
                text: qsTr("Recordings")
                onClicked: pageStack.push(Qt.resolvedUrl("RecordingsPage.qml"))
            }

            MenuItem {
                text: TVHClient.showFavoritesOnly ? qsTr("Show all") : qsTr("Show favorites")
                onClicked: {
                    TVHClient.showFavoritesOnly = !TVHClient.showFavoritesOnly
                }
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
                title: qsTr("Channels")
            }

            SearchField {
                id: searchField
                width: parent.width
                height: listView.showSearch ? implicitHeight : 0
                opacity: listView.showSearch ? 1 : 0
                onTextChanged: channelsSortFilterModel.pattern = text


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

            model: ChannelsSortFilterModel {
                id: channelsSortFilterModel
                sourceModel: TVHClient.channelsModel()
                showFavoritesOnly: TVHClient.showFavoritesOnly
            }

            delegate: ListItem {
                id: delegate

                contentHeight: contentColumn.height

                menu: ContextMenu {
                    MenuItem {
                        text: model.favorite ? qsTr("Remove from favorites") : qsTr("Add to favorites")
                        onClicked: {
                            model.favorite = !model.favorite
                            TVHClient.saveSettings()
                        }
                    }
                    MenuItem {
                        text: qsTr("Play")
                        onClicked: pageStack.push(Qt.resolvedUrl("StreamPlayerPage.qml"), {
                                                      url: "/stream/channel/" + model.uuid
                                                  })
                    }
                }

                Column {
                    id: contentColumn
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
                            source: "image://channel/" + model.uuid

                            smooth: true

                            width: Theme.itemSizeLarge
                            height: Theme.itemSizeLarge
                            fillMode: Image.PreserveAspectFit

                            Image {
                                id: favoriteIcon

                                visible: model.favorite

                                anchors.right: parent.right
                                anchors.bottom: parent.bottom

                                source: "image://theme/icon-s-favorite"
                            }
                        }

                        Column {
                            width: parent.width - channelIcon.width - parent.spacing
                            spacing: Theme.paddingSmall
                            anchors.verticalCenter: parent.verticalCenter

                            Item {
                                width: parent.width
                                height: channelNameLabel.height

                                Label {
                                    id: channelNameLabel
                                    anchors.left: parent.left
                                    anchors.right: channelNumberLabel.left
                                    font.bold: true

                                    text: model.name
                                }
                                Label {
                                    id: channelNumberLabel
                                    anchors.right: parent.right
                                    anchors.verticalCenter: channelNameLabel.verticalCenter
                                    font.italic: true

                                    text: model.number
                                }
                            }


                            Label {
                                width: parent.width
                                font.pixelSize: Theme.fontSizeSmall
                                wrapMode: Text.Wrap

                                text: model.currentTitle 
                            }
                            Label {
                                width: parent.width
                                font.pixelSize: Theme.fontSizeTiny
                                wrapMode: Text.Wrap

                                text: model.currentSubtitle
                            }

                            CurrentProgressItem {
                                width: parent.width

                                recording: model.recording
                                start: model.currentStart
                                stop: model.currentStop
                            }

                            Row {
                                width: parent.width
                                spacing: Theme.paddingSmall

                                Icon {
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "image://theme/icon-s-timer"
                                }

                                Label {
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pixelSize: Theme.fontSizeTiny

                                    text: model.nextTitle
                                }
                            }
                        }
                    }
                    Separator {
                        visible: index < (listView.count - 1)
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x
                        color: Theme.primaryColor
                    }

                }
                onClicked: pageStack.push(Qt.resolvedUrl("EventsListPage.qml"), {
                                              channelName: model.name,
                                              eventsModel: TVHClient.getEventsForChannel(model.uuid)
                                          })
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                text: TVHClient.showFavoritesOnly ? qsTr("No favorite channels") : qsTr("No channels available")
                hintText: TVHClient.showFavoritesOnly ? qsTr("Add favorite channels first") : qsTr("Check your internet connection")
            }

            VerticalScrollDecorator {}
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (TVHClient.hostname.length === 0) {
                TVHClient.channelsModel().active = false
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("WizardStartPage.qml"))
            } else {
                if (TVHClient.states === TVHClient.StateUninitialized) TVHClient.fetchData()
            }
        }
    }
    onVisibleChanged: TVHClient.channelsModel().active = visible
}
