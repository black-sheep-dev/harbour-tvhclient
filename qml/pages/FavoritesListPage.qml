import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

import "../components/"
import "../delegates/"

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
                title: qsTr("Favorites")
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
                showFavoritesOnly: true
            }

            delegate: ChannelListDelegate {
                id: delegate

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

                onClicked: pageStack.push(Qt.resolvedUrl("EventsListPage.qml"), {
                                              channelName: model.name,
                                              eventsModel: TVHClient.getEventsForChannel(model.uuid)
                                          })
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                text: qsTr("No favorite channels")
                hintText: qsTr("Add favorite channels first")
            }

            VerticalScrollDecorator {}
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("RecordingsPage.qml"))
        }
    }
    onVisibleChanged: TVHClient.channelsModel().active = visible
}
