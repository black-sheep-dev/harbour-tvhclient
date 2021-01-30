# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# VERSION
VERSION = 0.1.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# The name of your application
TARGET = harbour-tvhclient
DEFINES += APP_TARGET=\\\"$$TARGET\\\"

QT += multimedia sql

LIBS += -lz

CONFIG += sailfishapp

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
    qml/pages/ChannelsListPage.qml \
    qml/pages/EventPage.qml \
    qml/pages/EventsListPage.qml \
    qml/pages/RecordingsListPage.qml \
    qml/pages/RecordingsPage.qml \
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

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-tvhclient-de.ts

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
