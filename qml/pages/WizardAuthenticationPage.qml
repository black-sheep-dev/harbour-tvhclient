import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

Dialog {
    id: dialog

    canAccept: !authenticationSwitch.checked || (usernameField.text.length > 0 && passwordField.text.length > 0)

    acceptDestination: Qt.resolvedUrl("WizardStatusPage.qml")

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            id: header

            title: qsTr("Authentication Settings")

            acceptText: qsTr("Continue")
            cancelText: qsTr("Back")
        }

        TextSwitch {
            id: authenticationSwitch
            text: qsTr("Authentication")
            description: qsTr("Use below credentials to authenticate on TVHeadend server")
        }

        TextField {
            enabled: authenticationSwitch.checked
            id: usernameField
            width: parent.width

            label: qsTr("Username")
            placeholderText: qsTr("Enter username")
        }

        PasswordField {
            enabled: authenticationSwitch.checked
            id: passwordField
            width: parent.width

            label: qsTr("Password")
            placeholderText: qsTr("Enter password")
        }
    }

    onAccepted: {
        if (authenticationSwitch.checked) {
            TVHClient.username = usernameField.text
            TVHClient.password = passwordField.text
        }
    }
}
