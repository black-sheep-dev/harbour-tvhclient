#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include <QDateTime>

#include "event.h"

struct ChannelDTO {
    ChannelDTO() {};
    ChannelDTO(const ChannelDTO &other) = default;

    ChannelDTO &operator=(const ChannelDTO &other) = default;

    EventCurrentDTO current;
    QString icon;
    QString name;
    quint16 number;
    QString uuid;
};
Q_DECLARE_METATYPE(ChannelDTO)

class Channel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool favorite READ favorite WRITE setFavorite NOTIFY favoriteChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(quint16 number READ number WRITE setNumber NOTIFY numberChanged)
    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)

    // currently playing
    Q_PROPERTY(quint16 currentDuration READ currentDuration WRITE setCurrentDuration NOTIFY currentDurationChanged)
    Q_PROPERTY(quint8 currentFeatures READ currentFeatures WRITE setCurrentFeatures NOTIFY currentFeaturesChanged)
    Q_PROPERTY(QDateTime currentStart READ currentStart WRITE setCurrentStart NOTIFY currentStartChanged)
    Q_PROPERTY(QDateTime currentStop READ currentStop WRITE setCurrentStop NOTIFY currentStopChanged)
    Q_PROPERTY(QString currentSubtitle READ currentSubtitle WRITE setCurrentSubtitle NOTIFY currentSubtitleChanged)
    Q_PROPERTY(QString currentTitle READ currentTitle WRITE setCurrentTitle NOTIFY currentTitleChanged)
    Q_PROPERTY(QString nextTitle READ nextTitle WRITE setNextTitle NOTIFY nextTitleChanged)
    Q_PROPERTY(bool recording READ recording WRITE setRecording NOTIFY recordingChanged)

public:
    explicit Channel(QObject *parent = nullptr);
    Channel(const ChannelDTO &dto, QObject *parent = nullptr);

    // properties
    bool enabled() const;
    bool favorite() const;
    const QString &icon() const;
    const QString &name() const;
    quint16 number() const;
    const QString &uuid() const;

    quint16 currentDuration() const;
    quint8 currentFeatures() const;
    const QDateTime &currentStart() const;
    const QDateTime &currentStop() const;
    const QString &currentSubtitle() const;
    const QString &currentTitle() const;
    const QString &nextTitle() const;
    bool recording() const;

signals:
    // properties
    void enabledChanged(bool enabled);
    void favoriteChanged(bool favorite);
    void iconChanged(const QString &icon);
    void nameChanged(const QString &name);
    void numberChanged(quint16 number);
    void uuidChanged(const QString &uuid);

    void currentDurationChanged(quint16 duration);
    void currentFeaturesChanged(quint8 currentFeatures);
    void currentStartChanged(const QDateTime &start);
    void currentStopChanged(const QDateTime &stop);
    void currentSubtitleChanged(const QString &subtitle);
    void currentTitleChanged(const QString &title);
    void nextTitleChanged(const QString &title); 
    void recordingChanged(bool recording);

public slots:
    void setData(const ChannelDTO &dto);

    // properties
    void setEnabled(bool enabled);
    void setFavorite(bool favorite);
    void setIcon(const QString &icon);
    void setName(const QString &name);
    void setNumber(quint16 number);
    void setUuid(const QString &uuid);

    void setCurrentDuration(quint16 duration);
    void setCurrentFeatures(quint8 currentFeatures);
    void setCurrentStart(const QDateTime &start);
    void setCurrentStop(const QDateTime &stop);
    void setCurrentSubtitle(const QString &subtitle);
    void setCurrentTitle(const QString &title);
    void setNextTitle(const QString &title);
    void setRecording(bool recording);



private:
    // properties
    bool m_enabled{false};
    bool m_favorite{false};
    QString m_icon;
    QString m_name;
    quint16 m_number{0};
    QString m_uuid;

    quint16 m_currentDuration{0};
    quint8 m_currentFeatures{0};
    QDateTime m_currentStart;
    QDateTime m_currentStop;
    QString m_currentSubtitle;
    QString m_currentTitle;
    QString m_nextTitle;
    bool m_recording{false};
};

#endif // CHANNEL_H
