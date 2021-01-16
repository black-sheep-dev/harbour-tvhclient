import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

import "../components/"
Page {
    property string channelName
    property EventsModel eventsModel

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            busy: eventsModel.loading
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
                title: channelName
            }

            SearchField {
                id: searchField
                width: parent.width
                height: listView.showSearch ? implicitHeight : 0
                opacity: listView.showSearch ? 1 : 0
                onTextChanged: eventsSortFilterModel.pattern = text


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

            model: EventsSortFilterModel {
                id: eventsSortFilterModel
                sourceModel: eventsModel
            }

            BusyIndicator {
                id: busyIndicator
                visible: running
                size: BusyIndicatorSize.Large
                anchors.centerIn: parent
                running: eventsModel.loading
            }

            delegate: ListItem {
                id: delegate

                contentHeight: contentColumn.height

                RemorseItem { id: remorse }

                Column {
                    id: contentColumn
                    width: parent.width
                    spacing: Theme.paddingSmall

                    Item {
                        visible: index > 0
                        width: 1
                        height: 1
                    }

                    Label {
                        visible: index === 0 || start.getDate() !== stop.getDate()

                        width: parent.width
                        height: Theme.itemSizeMedium
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        color: Theme.highlightColor

                        text: {
                            if (index === 0)
                                return model.start.toLocaleDateString(Qt.locale());
                            else
                                model.stop.toLocaleDateString(Qt.locale())
                        }
                    }

                    Separator {
                        visible: index > 0 && start.getDate() === stop.getDate()
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x
                        color: Theme.primaryColor
                    }

                    Label {
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x
                        wrapMode: Text.Wrap
                        font.bold: true

                        text: model.title
                    }

                    Label {
                        visible: model.subtitle.length > 0
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x
                        wrapMode: Text.Wrap
                        font.pixelSize: Theme.fontSizeTiny

                        text: model.subtitle
                    }

                    TimeInfoItem {
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x

                        start: model.start
                        stop: model.stop
                    }

                    Row {
                        visible: model.scheduled
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

                    Item {
                        width: 1
                        height: Theme.paddingSmall
                    }

                    Label {
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x
                        wrapMode: Text.Wrap
                        font.pixelSize: Theme.fontSizeSmall

                        text: {
                            var sub = model.description.substr(0, 200)
                            if (sub.length > 200)
                                sub = sub + " ..."

                            return sub
                        }
                    }
                }
                onClicked: pageStack.push(Qt.resolvedUrl("EventPage.qml"), { event: eventsSortFilterModel.getEvent(index) })
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                text: qsTr("No events available")
                hintText: qsTr("Check your internet connection")
            }

            VerticalScrollDecorator {}
        }
    }

    onVisibleChanged: eventsModel.active = visible
}
