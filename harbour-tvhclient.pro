# VERSION
VERSION = 0.2.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# The name of your application
TARGET = harbour-tvhclient
DEFINES += APP_TARGET=\\\"$$TARGET\\\"

QT += multimedia sql

CONFIG += link_pkgconfig sailfishapp
PKGCONFIG += keepalive

include(secret.pri)
include(extern/sailfishos-utils/compressor/compressor.pri)
include(extern/sailfishos-utils/crypto/crypto.pri)

SOURCES += src/harbour-tvhclient.cpp \
    src/api/api_keys.cpp \
    src/entities/channel.cpp \
    src/entities/event.cpp \
    src/entities/recording.cpp \
    src/entities/serverinfo.cpp \
    src/models/basemodel.cpp \
    src/models/channelsmodel.cpp \
    src/models/channelssortfiltermodel.cpp \
    src/models/eventsmodel.cpp \
    src/models/eventssortfiltermodel.cpp \
    src/models/recordingsmodel.cpp \
    src/models/recordingssortfiltermodel.cpp \
    src/tools/datacache.cpp \
    src/tools/iconprovider.cpp \
    src/tools/utils.cpp \
    src/tvhclient.cpp

DISTFILES += qml/harbour-tvhclient.qml \
    qml/components/CurrentProgressItem.qml \
    qml/components/TimeInfoItem.qml \
    qml/cover/CoverPage.qml \
    qml/delegates/ChannelListDelegate.qml \
    qml/pages/ChannelsListPage.qml \
    qml/pages/EventPage.qml \
    qml/pages/EventsListPage.qml \
    qml/pages/FavoritesListPage.qml \
    qml/pages/RecordingsListPage.qml \
    qml/pages/RecordingsPage.qml \
    qml/pages/SettingsAuthenticationPage.qml \
    qml/pages/SettingsCachesPage.qml \
    qml/pages/SettingsConnectionPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/StreamPlayerPage.qml \
    qml/pages/WizardAuthenticationPage.qml \
    qml/pages/WizardConnectionPage.qml \
    qml/pages/WizardStartPage.qml \
    qml/pages/WizardStatusPage.qml \
    rpm/harbour-tvhclient.changes \
    rpm/harbour-tvhclient.changes.run.in \
    rpm/harbour-tvhclient.spec \
    rpm/harbour-tvhclient.yaml \
    translations/*.ts \
    harbour-tvhclient.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 512x512

CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-tvhclient-de.ts \
    translations/harbour-tvhclient-pl.ts \
    translations/harbour-tvhclient-si.ts \
    translations/harbour-tvhclient-sv.ts \
    translations/harbour-tvhclient-sk.ts \
    translations/harbour-tvhclient-fr.ts

RESOURCES += \
    ressources.qrc

HEADERS += \
    src/api/api_keys.h \
    src/entities/channel.h \
    src/entities/event.h \
    src/entities/recording.h \
    src/entities/serverinfo.h \
    src/models/basemodel.h \
    src/models/channelsmodel.h \
    src/models/channelssortfiltermodel.h \
    src/models/eventsmodel.h \
    src/models/eventssortfiltermodel.h \
    src/models/recordingsmodel.h \
    src/models/recordingssortfiltermodel.h \
    src/tools/datacache.h \
    src/tools/iconprovider.h \ 
    src/tools/utils.h \
    src/tvhclient.h
