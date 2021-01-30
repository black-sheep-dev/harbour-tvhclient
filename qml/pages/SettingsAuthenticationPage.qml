import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.tvhclient 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Reset")
                onClicked: TVHClient.resetAuthentication()
            }
        }

        anchors.fill: parent

        contentHeight: column.height

        RemorsePopup { id: remorse }

        Column {
            id: column

            x: Theme.horizontalPageMargin

            width: page.width - 2 * x
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Authentication Settings")
            }

            TextField {
                id: usernameField
                width: parent.width

                label: qsTr("Username")

                text: TVHClient.username
                readOnly: TVHClient.username.length > 0
            }

            PasswordField {
                id: passwordField
                width: parent.width

                label: qsTr("Password")

                readOnly: TVHClient.password.length > 0
            }
        }
    }
    onStatusChanged: {
            if (status != PageStatus.Deactivating) return;

            if ( TVHClient.username == usernameField.text
                    && TVHClient.password == passwordField.text )
                return;

            if (usernameField.text.length === 0 || passwordField.text.length === 0)
                return;

            TVHClient.username = usernameField.text
            TVHClient.password = passwordField.text

            TVHClient.saveAuthentication()
        }
}

