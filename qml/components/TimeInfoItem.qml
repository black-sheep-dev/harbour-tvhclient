import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    property color color: Theme.primaryColor
    property date start
    property date stop

    id: timeInfoItem

    height: dateLabel.height

    Label {
        id: dateLabel
        anchors.left: parent.left
        font.pixelSize: Theme.fontSizeTiny
        color: timeInfoItem.color
        text: start.toLocaleDateString(Qt.locale())
    }

    Label {
        anchors.right: parent.right
        font.pixelSize: Theme.fontSizeTiny
        color: timeInfoItem.color
        text: start.toLocaleTimeString(Qt.locale(), "hh:mm") + " - " + stop.toLocaleTimeString(Qt.locale(), "hh:mm")
    }
}
