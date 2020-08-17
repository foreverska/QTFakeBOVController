#include <unistd.h>

#include "blowoffcontroller.h"

#include <QSerialPortInfo>

BlowoffController::BlowoffController(QObject *parent) : QObject(parent)
{

}

BlowoffController::~BlowoffController()
{
    serialDisconnect();
}

void BlowoffController::enumeratePorts()
{
    QStringList newPortList;

    for(auto port : QSerialPortInfo::availablePorts())
    {
        newPortList.push_back(port.portName());
    }

    setPortList(newPortList);
}

QStringList BlowoffController::portList() const
{
    return m_portList;
}

void BlowoffController::serialDisconnect()
{
    if (m_serialPort.isOpen() == true)
    {
        m_serialPort.close();
    }
}

void BlowoffController::blowoffStart()
{
    if (m_serialPort.isOpen() == false)
    {
        return;
    }

    m_serialPort.write("BCS;", 4);
    m_serialPort.waitForBytesWritten(-1);
}

void BlowoffController::blowoffEnd()
{
    if (m_serialPort.isOpen() == false)
    {
        return;
    }

    m_serialPort.write("BCE;", 4);
    m_serialPort.waitForBytesWritten(-1);
}

void BlowoffController::blowoffFlutter()
{
    if (m_serialPort.isOpen() == false)
    {
        return;
    }

    m_serialPort.write("BCF;", 4);
    m_serialPort.waitForBytesWritten(-1);
}

void BlowoffController::redeemBov()
{
    blowoffStart();

    sleep(1);

    blowoffEnd();
}

void BlowoffController::serialConnect(QString port)
{
    bool retval;

    serialDisconnect();

    m_serialPort.setPortName(port);
    retval = m_serialPort.open(QIODevice::ReadWrite);
    if (retval == false)
    {
        qDebug("Failed to open serial port");
    }

    m_serialPort.write("BCN;", 4);

    qDebug("Opened Serial port %s", port.toStdString().c_str());
}

void BlowoffController::setPortList(QStringList portList)
{
    if (m_portList == portList)
        return;

    m_portList = portList;
    emit portListChanged(m_portList);
}
