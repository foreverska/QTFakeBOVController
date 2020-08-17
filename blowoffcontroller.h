#ifndef BLOWOFFCONTROLLER_H
#define BLOWOFFCONTROLLER_H

#include <QObject>
#include <QSerialPort>

class BlowoffController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList portList READ portList
               WRITE setPortList NOTIFY portListChanged)

    QSerialPort m_serialPort;
    QStringList m_portList;

public:
    explicit BlowoffController(QObject *parent = nullptr);
    ~BlowoffController();
    void enumeratePorts();
    QStringList portList() const;

    Q_INVOKABLE void serialConnect(QString port);
    Q_INVOKABLE void serialDisconnect();
    Q_INVOKABLE void blowoffStart();
    Q_INVOKABLE void blowoffEnd();
    Q_INVOKABLE void blowoffFlutter();
    Q_INVOKABLE void redeemBov();
public slots:
    void setPortList(QStringList portList);

signals:

    void portListChanged(QStringList portList);
};

#endif // BLOWOFFCONTROLLER_H
