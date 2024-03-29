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
                title: qsTr("Connection Settings")
            }

            TextField {
                id: hostnameField
                width: parent.width

                label: qsTr("Hostname")
                placeholderText: qsTr("Enter hostname")

                text: TVHClient.hostname

                inputMethodHints: Qt.ImhUrlCharactersOnly
                validator: RegExpValidator {
                    regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: portField.focus = true
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                visible: !hostnameField.acceptableInput
                text: qsTr("Valid hostname or IP required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            TextField {
                id: portField
                width: parent.width / 2

                label: qsTr("Port")

                text: TVHClient.port

                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1; top: 65535;}

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: focus = false
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                visible: !portField.acceptableInput
                text: qsTr("Valid port required!") +  " (1-65535)"
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }
    }
    onStatusChanged: {
            if (status != PageStatus.Deactivating) return;

            if (hostnameField.acceptableInput && portField.acceptableInput) {
                TVHClient.hostname = hostnameField.text
                TVHClient.port = portField.text
                TVHClient.saveSettings()
            }
        }
}

