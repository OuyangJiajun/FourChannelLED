#include "serialconnect.h"

SerialConnect::SerialConnect()
{
    updatePorts();

    //实例化串口类一个对象，没有设置parent，要手动delete
    m_serialPort = new QSerialPort();

}

bool SerialConnect::connectSerialPort(QString connectPort)
{
    //如果串口已经打开了 先关闭
    if(m_serialPort->isOpen())
    {
        m_serialPort->clear();
        m_serialPort->close();
    }

    //设置串口名字并尝试连接
    m_serialPort->setPortName(connectPort);
    if(!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite模式
    {
        qDebug()<<connectPort<<"打开失败!";
        return false;
    }

    //打开成功设置串口（默认配置，有需要再改或提供接口）
    m_serialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);//设置波特率和读写方向
    m_serialPort->setDataBits(QSerialPort::Data8);		//数据位为8位
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    m_serialPort->setParity(QSerialPort::NoParity);	//无校验位
    m_serialPort->setStopBits(QSerialPort::OneStop); //一位停止位

    return true;
}

void SerialConnect::updatePorts()
{
    //先清空
    m_serialPortName.clear();

    //获取可用串口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
    }
}

void SerialConnect::disConnectSerialPort()
{
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }
    delete m_serialPort;
}

bool SerialConnect::read(QString &str)
{
    str = m_serialPort->readAll();
    if(str.isEmpty())
        return false;
    else
        return true;
}

bool SerialConnect::write(QString str)
{
    QByteArray ba;
    ba.append(str);
    int err = m_serialPort->write(ba);
    if(err==-1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
