#include <unistd.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>

#include "twitchcontroller.h"

#define TWO_MIN_MS 120000

TwitchController::TwitchController(QObject *parent) :
    QObject(parent)
{
    connect(&mWSocket, SIGNAL(connected()),
            this, SLOT(connected()));
    connect(&mWSocket, SIGNAL(textMessageReceived(const QString &)),
            this, SLOT(messageRecieved(const QString &)));
    connect(&mPingTimer, SIGNAL(timeout()),
            this, SLOT(sendPing()));

    mPingTimer.start(TWO_MIN_MS);
}

TwitchController::~TwitchController()
{

}

void TwitchController::setBovController(BlowoffController *pBovController)
{
    mpBovController = pBovController;
}

QString TwitchController::oathToken() const
{
    return mOathToken;
}

void TwitchController::startConnect()
{
    QUrl url;
    url.setUrl("wss://pubsub-edge.twitch.tv");

    mWSocket.open(url);
}

QString TwitchController::channelId() const
{
    return mChannelId;
}

void TwitchController::setOathToken(QString oathToken)
{
    if (mOathToken == oathToken)
        return;

    mOathToken = oathToken;
    emit oathTokenChanged(mOathToken);
}

void TwitchController::sendListenReq()
{
    QJsonDocument listenDocument;
    QJsonObject listenRequest;
    QJsonObject data;
    QJsonValue value;
    QJsonArray topics;
    QString strRequest;
    QByteArray nonce;

    uint64_t randNumber = QRandomGenerator::global()->generate64();
    nonce = QByteArray::fromRawData((char*) &randNumber, 8);

    value = "LISTEN";
    listenRequest.insert("type", value);
    value = QString(nonce.toBase64().toStdString().c_str());
    listenRequest.insert("nonce", value);

    value = "channel-points-channel-v1." + mChannelId;
    topics.push_back(value);
    value = mOathToken;
    data.insert("topics", topics);
    data.insert("auth_token", value);

    listenRequest.insert("data", data);

    listenDocument.setObject(listenRequest);

    strRequest = listenDocument.toJson(QJsonDocument::Compact);

    mWSocket.sendTextMessage(strRequest);
}

void TwitchController::connected()
{
    mConnected = true;

    qDebug("Connected to twitch sending LISTEN");

    sendListenReq();
}

void TwitchController::redeemBov()
{
    mpBovController->blowoffStart();

    sleep(1);

    mpBovController->blowoffEnd();
}

void TwitchController::redeemReward(const QString &reward)
{
    qDebug("Reward redeemed: %s", reward.toStdString().c_str());

    if (reward == "Blowoff Valve")
    {
        redeemBov();
    }
}

void TwitchController::parseMessage(const QString &message)
{
    QJsonDocument messageJson;

    messageJson = QJsonDocument::fromJson(message.toUtf8());
    if (messageJson.isObject() == false)
    {
        return;
    }

    QJsonObject messsageObj = messageJson.object();
    if (messsageObj.contains("data") == false &&
        messsageObj["data"].isObject() == false)
    {
        return;
    }

    QJsonObject messageData = messsageObj["data"].toObject();
    if (messageData.contains("redemption") == false &&
        messageData["redemption"].isObject() == false)
    {
        return;
    }

    QJsonObject messageRedemption = messageData["redemption"].toObject();
    if (messageRedemption.contains("reward") == false &&
        messageRedemption["reward"].isObject() == false)
    {
        return;
    }

    QJsonObject messageReward = messageRedemption["reward"].toObject();
    if (messageReward.contains("title") == false &&
        messageReward["title"].isString() == false)
    {
        return;
    }

    QString reward = messageReward["title"].toString();
    redeemReward(reward);
}

void TwitchController::messageRecieved(const QString &message)
{
    QJsonDocument messageJson;
    messageJson = QJsonDocument::fromJson(message.toUtf8());

    if (messageJson.isObject())
    {
        QJsonObject messageObj = messageJson.object();
        if (messageObj["type"].toString() != "MESSAGE")
        {
            return;
        }

        if (messageObj.contains("data") &&
            messageObj["data"].isObject() == true)
        {
            QJsonObject messageData = messageObj["data"].toObject();
            if (messageData.contains("message") &&
                messageData["message"].isString() == true)
            {
                parseMessage(messageData["message"].toString());
            }
        }

    }
}

void TwitchController::sendPing()
{
    QJsonDocument pingDocument;
    QJsonObject pingRequest;
    QJsonValue value;
    QString strRequest;

    if (mConnected == false)
    {
        return;
    }

    value = "PING";
    pingRequest.insert("type", value);
    pingDocument.setObject(pingRequest);
    strRequest = pingDocument.toJson(QJsonDocument::Compact);

    mWSocket.sendTextMessage(strRequest);
}

void TwitchController::setChannelId(QString channelId)
{
    if (mChannelId == channelId)
        return;

    mChannelId = channelId;
    emit channelIdChanged(mChannelId);
}
