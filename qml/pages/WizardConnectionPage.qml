import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

Dialog {
    id: dialog

    canAccept: hostnameField.acceptableInput && portField.acceptableInput

    acceptDestination: Qt.resolvedUrl("WizardAuthenticationPage.qml")

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            id: header

            title: qsTr("Connection Settings")

            acceptText: qsTr("Continue")
            cancelText: qsTr("Back")
        }

        TextField {
            id: hostnameField
            width: parent.width

            label: qsTr("Hostname")
            placeholderText: qsTr("Enter hostname (e.g. http://server)")

            inputMethodHints: Qt.ImhUrlCharactersOnly
            validator: RegExpValidator {
                regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
            }

            text: TVHClient.hostname

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

    onAccepted: {
        TVHClient.hostname = hostnameField.text
        TVHClient.port = portField.text
    }
}
