#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <vector>
using namespace std;
#include <QMainWindow>
#include "serialconnect.h"
#include "connectwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //断开串口函数
    void closeSerialPort();

    //实现脉冲的计时器
    QTimer* timer1_1 = NULL;
    QTimer* timer1_2 = NULL;
    QTimer* timer2_1 = NULL;
    QTimer* timer2_2 = NULL;
    QTimer* timer3_1 = NULL;
    QTimer* timer3_2 = NULL;
    QTimer* timer4_1 = NULL;
    QTimer* timer4_2 = NULL;
    QTimer* timer5_1 = NULL;
    QTimer* timer5_2 = NULL;

    //管理串口类对象
    SerialConnect* serialConnect=NULL;

    //记录是否已连接设备
    bool m_flag=false;

    //记录led开关
    bool m_channel1Flag=false;
    bool m_channel2Flag=false;
    bool m_channel3Flag=false;
    bool m_channel4Flag=false;
    bool m_channelAllFlag=false;

    //记录led电流百分比，暂时没用到
    vector<int> m_currentPercent;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
