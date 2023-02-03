import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("About")
            }

            Image {
                id: logo
                source: "/usr/share/harbour-tvhclient/icons/harbour-tvhclient.svg"
                smooth: true
                sourceSize.width: parent.width / 2
                sourceSize.height: parent.width / 2
                height: parent.width / 2
                width: parent.width / 2
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                font.pixelSize: Theme.fontSizeExtraLarge
                color: Theme.secondaryHighlightColor

                text: qsTr("TVH Client")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                text: Qt.application.version
            }

            Item {
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2 * x

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                text: qsTr("TVH Client is a native Sailfish OS Client for TVHeadend server.")
            }

            SectionHeader {
                text: qsTr("Social Media")
            }

            BackgroundItem {
                width: parent.width
                height: Theme.itemSizeMedium
                contentHeight: Theme.itemSizeMedium
                Row{
                    width:parent.width - 2 * x
                    height: parent.height
                    x:Theme.horizontalPageMargin
                    spacing:Theme.paddingLarge

                    Image {
                        anchors.verticalCenter: parent.vertimastodoncalCenter
                        width: parent.height * 0.8
                        height: width
                        source: "/usr/share/harbour-tvhclient/icons/mastodon.svg"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "@tvhclient@social.nubecula.org"
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: {
                    notification.show(qsTr("Copied to clipboard"))
                    Clipboard.text = "@tvhclient@social.nubecula.org"
                }
            }

            BackgroundItem {
                width: parent.width
                height: Theme.itemSizeMedium
                contentHeight: Theme.itemSizeMedium
                Row{
                    width:parent.width - 2 * x
                    height: parent.height
                    x:Theme.horizontalPageMargin
                    spacing:Theme.paddingLarge

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.height * 0.8
                        height: width
                        source: "/usr/share/harbour-tvhclient/icons/mastodon.svg"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "@blacksheep@social.nubecula.org"
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: {
                    notification.show(qsTr("Copied to clipboard"))
                    Clipboard.text = "@blacksheep@social.nubecula.org"
                }
            }

            SectionHeader{
                text: qsTr("Translations")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                text: qsTr("Your language is not available? You are welcome to support this project by translating it on my self hosted Weblate server.")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium
                Row{
                    x : Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height
                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "/usr/share/harbour-tvhclient/icons/weblate.svg"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "https://weblate.nubecula.org/projects/harbour-tvhclient"
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: Qt.openUrlExternally("https://weblate.nubecula.org/projects/harbour-tvhclient")
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                height: sourceSize.height * width / sourceSize.width
                smooth: true
                fillMode: Image.PreserveAspectFit
                source: "http://weblate.nubecula.org/widgets/harbour-tvhclient/-/harbour-tvhclient/multi-auto.svg"
            }

            SectionHeader{
                text: qsTr("Sources")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium
                Row{
                    x : Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height
                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "/usr/share/harbour-tvhclient/icons/github.svg"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "https://github.com/black-sheep-dev/harbour-tvhclient"
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: Qt.openUrlExternally("https://github.com/black-sheep-dev/harbour-tvhclient")
            }

            SectionHeader{
                text: qsTr("Donations")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("If you like my work why not buy me a beer?")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium

                Row{
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height
                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "/usr/share/harbour-tvhclient/icons/paypal.svg"
                    }
                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor
                        text: qsTr("Donate with PayPal")
                    }
                }
                onClicked: Qt.openUrlExternally("https://www.paypal.com/paypalme/nubecula/1")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium

                Row{
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height

                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "/usr/share/harbour-tvhclient/icons/liberpay.svg"
                    }
                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor
                        text: qsTr("Donate with Liberpay")
                    }
                }
                onClicked: Qt.openUrlExternally("https://liberapay.com/black-sheep-dev/donate")
            }

            Item {
                width: 1
                height: Theme.paddingSmall
            }
        }
    }
}
