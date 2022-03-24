#ifndef SERIALCONNECT_H
#define SERIALCONNECT_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QDebug>


class SerialConnect
{
public:
    SerialConnect();

    //更新可用串口
    void updatePorts();

    //连接串口并配置    连接成功返回true否则false    参数：连接串口名
    bool connectSerialPort(QString connectPort);

    //关闭串口
    void disConnectSerialPort();

    //读串口内容到str     成功读到内容true，出错或读到空false
    bool read(QString &str);

    //写str到串口     写成功true，出错false
    bool write(QString str);

    //放可用串口
    QStringList m_serialPortName;

    //连接串口对象
    QSerialPort *m_serialPort;
};

#endif // SERIALCONNECT_H
