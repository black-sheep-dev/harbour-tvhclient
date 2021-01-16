import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    property bool recording: false
    property date start
    property date stop

    height: contentColumn.height

    Column {
        id: contentColumn
        width: parent.width
        spacing: Theme.paddingSmall

        Item {
            width: parent.width
            height: Theme.paddingSmall

            Rectangle {
                id: backgroundRect
                anchors.fill: parent

                color: Theme.primaryColor

                Rectangle {
                    width: parent.width * getPercentage()
                    height: parent.height

                    color: recording ? Theme.errorColor : Theme.highlightColor
                }
            }
        }

        Item {
            height: startTimeLabel.height
            width: parent.width

            Label {
                id: startTimeLabel
                anchors.left: parent.left
                text: start.toLocaleTimeString(Qt.locale(), "hh:mm")
                font.pixelSize: Theme.fontSizeTiny
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("%n min(s) left", "", getTimeLeft() / 60000);
                font.pixelSize: Theme.fontSizeTiny
            }

            Label {
                anchors.right: parent.right
                text: stop.toLocaleTimeString(Qt.locale(), "hh:mm")
                font.pixelSize: Theme.fontSizeTiny
            }
        }
    }

    function getDuration() {
        return stop.getTime() - start.getTime()
    }

    function getTimeLeft() {
        return (stop.getTime() - new Date().getTime())
    }

    function getPercentage() {
        return 1.0 - getTimeLeft() / getDuration()
    }
}
