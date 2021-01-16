#ifndef RECORDINGSMODEL_H
#define RECORDINGSMODEL_H

#include "basemodel.h"

#include "src/entities/recording.h"

class RecordingsModel : public BaseModel
{
    Q_OBJECT

public:
    enum RecordingRoles {
        ActiveRole          = Qt::UserRole + 1,
        BroadcastRole,
        ChannelNameRole,
        ChannelUuidRole,
        CommentRole,
        DescriptionRole,
        FileSizeRole,
        PlayCountRole,
        PlayPositionRole,
        StartRole,
        StopRole,
        StatusRole,
        SubtitleRole,
        TitleRole,
        UrlRole,
        UuidRole
    };
    Q_ENUM(RecordingRoles)

    explicit RecordingsModel(QObject *parent = nullptr);

    void clear();
    QList<Recording *> recordings() const;

signals:
    void recordingStateChanged(const QString &channelUuid, bool active);

public slots:
    void setRecordings(const QList<RecordingDTO> recordings);

private slots:
    void sendRecordingState(bool active);

private:
    QList<Recording *> m_recordings;

    // BaseModel interface
public slots:
    void refresh() override;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;


};

#endif // RECORDINGSMODEL_H
