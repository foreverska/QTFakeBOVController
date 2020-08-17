#ifndef TWITCHCONTROLLER_H
#define TWITCHCONTROLLER_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QThread>

#include <signal.h>

#include "blowoffcontroller.h"

class TwitchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString oathToken READ oathToken
               WRITE setOathToken NOTIFY oathTokenChanged)
    Q_PROPERTY(QString channelId READ channelId
               WRITE setChannelId NOTIFY channelIdChanged)

    BlowoffController *mpBovController;

    QWebSocket mWSocket;
    QString mOathToken;
    QTimer mPingTimer;
    sig_atomic_t mConnected;

    QString mChannelId;

    void sendListenReq();
    void parseMessage(const QString &message);
    void redeemReward(const QString &reward);
public:
    explicit TwitchController(QObject *parent = nullptr);
    ~TwitchController();

    void setBovController(BlowoffController *pBovController);

    QString oathToken() const;
    Q_INVOKABLE void startConnect();
    QString channelId() const;
    Q_INVOKABLE void redeemBov();

public slots:
    void setOathToken(QString oathToken);
    void connected();
    void messageRecieved(const QString &message);
    void sendPing();
    void setChannelId(QString channelId);

signals:

    void oathTokenChanged(QString oathToken);
    void channelIdChanged(QString channelId);
};

#endif // TWITCHCONTROLLER_H
