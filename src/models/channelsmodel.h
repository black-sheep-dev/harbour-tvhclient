#ifndef CHANNELSMODEL_H
#define CHANNELSMODEL_H

#include "basemodel.h"

#include "src/entities/channel.h"

class ChannelsModel : public BaseModel
{
    Q_OBJECT

public:
    enum ChannelRoles {
        EnabledRole         = Qt::UserRole + 1,
        FavoriteRole,
        IconRole,
        NameRole,
        NumberRole,
        SortRole,
        UuidRole,
        CurrentFeaturesRole,
        CurrentTitleRole,
        CurrentStartRole,
        CurrentStopRole,
        CurrentSubtitleRole,
        NextTitleRole,
        RecordingRole
    };
    Q_ENUM(ChannelRoles)

    explicit ChannelsModel(QObject *parent = nullptr);

    void clear();
    QList<Channel *> channels() const;
    QStringList favorites() const;
    void setFavorites(const QStringList &favorites);

signals:
    void channelsLoaded(const QList<Channel *> &channels);
    void requestChannelUpdate(const QString uuid);

public slots:
    void onIconAvailable(const QString &uuid);
    void setChannel(const ChannelDTO channel);
    void setChannels(const QList<ChannelDTO> channels);
    void setRecordingState(const QString &channelUuid, bool active);

private:
    QModelIndex indexOf(const QString &uuid) const;

    QList<Channel *> m_channels;
    QStringList m_favorites;
    QTimer *m_timer{new QTimer(this)};

    // BaseModel interface
public slots:
    void refresh() override;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;


};

#endif // CHANNELSMODEL_H
