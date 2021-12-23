import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

import "../components/"

ListItem {
    id: delegate

    contentHeight: contentColumn.height + separatorTop.height + Theme.paddingSmall

    Separator {
        id: separatorTop
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

        Row {
            id: contentRow
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            spacing: Theme.paddingSmall

            Image {
                id: channelIcon
                source: "image://channel/" + model.uuid

                smooth: true
                cache: true

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
    }
}
