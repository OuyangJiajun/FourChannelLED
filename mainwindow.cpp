#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化窗口名字
    setWindowTitle("四通道led控制软件");
    //初始化串口数据
    serialConnect = new SerialConnect;
    //默认连续模式
    ui->stackedWidget->setCurrentIndex(0);
    //初始化连接状态标签
    ui->label_connect->setText("当前状态：设备未连接");
    //初始化模式标签
    ui->label_mode->setText("连续输出");
    //初始化通道标签
    ui->widget_channel1->changeLabel("通道1");
    ui->widget_channel2->changeLabel("通道2");
    ui->widget_channel3->changeLabel("通道3");
    ui->widget_channel4->changeLabel("通道4");


    //连接按钮
    connect(ui->toolButton_connect,&QToolButton::clicked,[=](){

        //关闭设备后再打开连接窗口的情况，需要重新new
        if(serialConnect==NULL)
        {
            serialConnect = new SerialConnect;
        }

        //创建连接窗口
        ConnectWindow* connectWindow = new ConnectWindow;

        //更新可用串口
        serialConnect->updatePorts();

        //可用串口放入选择框
        connectWindow->putInBox(serialConnect->m_serialPortName);

        //打开设备按钮
        connect(connectWindow,&ConnectWindow::openDevice,[=](){

            //连接串口
            bool ret = serialConnect->connectSerialPort(connectWindow->getComboBoxText());

            if(ret==true)//连接成功时
            {
               m_flag=true;
               ui->label_connect->setText("当前状态：设备连接成功");

               //暂用字符串
               QString str;
               //先关闭所有开关
               serialConnect->write("@AEFFF\n");
               while(!serialConnect->m_serialPort->waitForReadyRead());//阻塞到收到反馈数据
               serialConnect->read(str);//读掉这个@ok

               //读电流初始化
               ui->widget_channel1->flag=true;//正在同步电流标志
               ui->widget_channel2->flag=true;
               ui->widget_channel3->flag=true;
               ui->widget_channel4->flag=true;
               //channel1
               serialConnect->write("@AR01\n");
               while(!serialConnect->m_serialPort->waitForReadyRead());
               serialConnect->read(str);
               str=str.mid(1,3);//截取3位数字，如098代表98.0%
               m_currentPercent.push_back(str.toInt());//转为98代表百分比
               ui->widget_channel1->changeCurrent(m_currentPercent[0]);
               //channel2
               serialConnect->write("@AR02\n");
               while(!serialConnect->m_serialPort->waitForReadyRead());
               serialConnect->read(str);
               str=str.mid(1,3);
               m_currentPercent.push_back(str.toInt());
               ui->widget_channel2->changeCurrent(m_currentPercent[1]);
               //channel3
               serialConnect->write("@AR03\n");
               while(!serialConnect->m_serialPort->waitForReadyRead());
               serialConnect->read(str);
               str=str.mid(1,3);
               m_currentPercent.push_back(str.toInt());
               ui->widget_channel3->changeCurrent(m_currentPercent[2]);
               //channel4
               serialConnect->write("@AR04\n");
               while(!serialConnect->m_serialPort->waitForReadyRead());
               serialConnect->read(str);
               str=str.mid(1,3);
               m_currentPercent.push_back(str.toInt());
               ui->widget_channel4->changeCurrent(m_currentPercent[3]);
               ui->widget_channel1->flag=false;//结束同步电流标志
               ui->widget_channel2->flag=false;
               ui->widget_channel3->flag=false;
               ui->widget_channel4->flag=false;

               delete connectWindow;
            }
            else//连接失败时
            {
                QMessageBox::critical(this,"错误","连接失败");
            }
        });

        //关闭设备按钮
        connect(connectWindow,&ConnectWindow::closeDevice,[&](){
           closeSerialPort();
           delete connectWindow;
        });

        connectWindow->show();
    });


    //模式按钮
    connect(ui->toolButton_mode,&QToolButton::clicked,[=](){
        QStringList items;
        items << tr("连续输出") << tr("脉冲输出");

        bool ok;
        QString item = QInputDialog::getItem(this, tr("模式"),
                                             tr("请选择："), items, 0, false, &ok);
        if (ok && !item.isEmpty())
        {
            //切换模式
            ui->label_mode->setText(item);
            if(item=="连续输出")
            {
                ui->stackedWidget->setCurrentIndex(0);
            }
            else
            {
                ui->stackedWidget->setCurrentIndex(1);
            }
        }
    });


    //语言按钮（未完成）
    connect(ui->toolButton_language,&QToolButton::clicked,[=](){
        QStringList items;
        items << tr("中文（中华人民共和国）") << tr("English(United States)");

        bool ok;
        QString item = QInputDialog::getItem(this, tr("语言"),
                                             tr("请选择："), items, 0, false, &ok);
        if (ok && !item.isEmpty())
        {
            if(item=="中文（中华人民共和国）")
            {
                //把文字换成中文

            }
            else
            {
                //把文字换成英文
                setWindowTitle("Four Channel LED Controler");
                ui->toolButton_connect->setText("Connect");
                ui->toolButton_mode->setText("Mode");
                ui->toolButton_language->setText("Language");
                ui->toolButton_help->setText("Help");
                ui->widget_channel1->changeLabel("channel1");
                ui->widget_channel2->changeLabel("channel2");
                ui->widget_channel3->changeLabel("channel3");
                ui->widget_channel4->changeLabel("channel4");
                ui->widget_pulsechannelAll->changeLabel("total");
                ui->widget_pulsechannel1->changeLabel("channel1");
                ui->widget_pulsechannel2->changeLabel("channel2");
                ui->widget_pulsechannel3->changeLabel("channel3");
                ui->widget_pulsechannel4->changeLabel("channel4");
            }
        }
    });


    //帮助按钮
    connect(ui->toolButton_help,&QToolButton::clicked,[=](){
        QMessageBox::information(this,"帮助","menu下载：www.kayjaopt.com\n技术支持：service@kayjaopt.com");
    });


    //改变电流
    connect(ui->widget_channel1,&LedControl::currentChange,[=](){
        if(m_flag==true)//连接上才发指令
        {
            int cur = ui->widget_channel1->getCurrentPercent();//读取电流
            //转换为指令格式
            QString str = QString("@AW01%1").arg(cur,3,10,QLatin1Char('0'));
            str+="0\n";
            serialConnect->write(str);
        }
    });
    connect(ui->widget_channel2,&LedControl::currentChange,[=](){
        if(m_flag==true)//连接上才发指令
        {
            int cur = ui->widget_channel2->getCurrentPercent();//读取电流
            //转换为指令格式
            QString str = QString("@AW02%1").arg(cur,3,10,QLatin1Char('0'));
            str+="0\n";
            serialConnect->write(str);
        }
    });
    connect(ui->widget_channel3,&LedControl::currentChange,[=](){
        if(m_flag==true)//连接上才发指令
        {
            int cur = ui->widget_channel3->getCurrentPercent();//读取电流
            //转换为指令格式
            QString str = QString("@AW03%1").arg(cur,3,10,QLatin1Char('0'));
            str+="0\n";
            serialConnect->write(str);
        }
    });
    connect(ui->widget_channel4,&LedControl::currentChange,[=](){
        if(m_flag==true)//连接上才发指令
        {
            int cur = ui->widget_channel4->getCurrentPercent();//读取电流
            //转换为指令格式
            QString str = QString("@AW04%1").arg(cur,3,10,QLatin1Char('0'));
            str+="0\n";
            serialConnect->write(str);
        }
    });


    //开关
    connect(ui->widget_channel1,&LedControl::onOff,[=](){
        if(m_flag==true)//连接上才发指令
        {
            if(m_channel1Flag==false)
            {
                serialConnect->write("@AE01T\n");
                m_channel1Flag=true;
            }
            else
            {
                serialConnect->write("@AE01F\n");
                m_channel1Flag=false;
            }
        }
    });
    connect(ui->widget_channel2,&LedControl::onOff,[=](){
        if(m_flag==true)//连接上才发指令
        {
            if(m_channel2Flag==false)
            {
                serialConnect->write("@AE02T\n");
                m_channel2Flag=true;
            }
            else
            {
                serialConnect->write("@AE02F\n");
                m_channel2Flag=false;
            }
        }
    });
    connect(ui->widget_channel3,&LedControl::onOff,[=](){
        if(m_flag==true)//连接上才发指令
        {
            if(m_channel3Flag==false)
            {
                serialConnect->write("@AE03T\n");
                m_channel3Flag=true;
            }
            else
            {
                serialConnect->write("@AE03F\n");
                m_channel3Flag=false;
            }
        }
    });
    connect(ui->widget_channel4,&LedControl::onOff,[=](){
        if(m_flag==true)//连接上才发指令
        {
            if(m_channel4Flag==false)
            {
                serialConnect->write("@AE04T\n");
                m_channel4Flag=true;
            }
            else
            {
                serialConnect->write("@AE04F\n");
                m_channel4Flag=false;
            }
        }
    });


    //初始化脉冲模式标签
    ui->widget_pulsechannelAll->changeLabel("总体控制");
    ui->widget_pulsechannel1->changeLabel("通道1");
    ui->widget_pulsechannel2->changeLabel("通道2");
    ui->widget_pulsechannel3->changeLabel("通道3");
    ui->widget_pulsechannel4->changeLabel("通道4");


    //脉冲模式计时器
    timer1_1= new QTimer(this);
    timer1_2= new QTimer(this);
    timer2_1= new QTimer(this);
    timer2_2= new QTimer(this);
    timer3_1= new QTimer(this);
    timer3_2= new QTimer(this);
    timer4_1= new QTimer(this);
    timer4_2= new QTimer(this);
    timer5_1= new QTimer(this);
    timer5_2= new QTimer(this);
    connect(timer1_1,&QTimer::timeout,[=](){
        serialConnect->write("@AE01T\n");
    });
    connect(timer1_2,&QTimer::timeout,[=](){
        serialConnect->write("@AE01F\n");
    });
    connect(timer2_1,&QTimer::timeout,[=](){
        serialConnect->write("@AE02T\n");
    });
    connect(timer2_2,&QTimer::timeout,[=](){
        serialConnect->write("@AE02F\n");
    });
    connect(timer3_1,&QTimer::timeout,[=](){
        serialConnect->write("@AE03T\n");
    });
    connect(timer3_2,&QTimer::timeout,[=](){
        serialConnect->write("@AE03F\n");
    });
    connect(timer4_1,&QTimer::timeout,[=](){
        serialConnect->write("@AE04T\n");
    });
    connect(timer4_2,&QTimer::timeout,[=](){
        serialConnect->write("@AE04F\n");
    });
    connect(timer5_1,&QTimer::timeout,[=](){
        serialConnect->write("@AEFFT\n");
    });
    connect(timer5_2,&QTimer::timeout,[=](){
        serialConnect->write("@AEFFF\n");
    });


    //脉冲模式开关按钮
    connect(ui->widget_pulsechannel1,&PulseMode::onOff,[=](){
        if(m_flag==true)//连接上才发指令
        {
            if(m_channel1Flag==false)
            {
                timer1_1->start(1000/ui->widget_pulsechannel1->m_frequency);
                QTimer::singleShot(1000/ui->widget_pulsechannel1->m_frequency*(ui->widget_pulsechannel1->m_percent/100),this,[=](){//根据占空比来延时
                    timer1_2->start(1000/ui->widget_pulsechannel1->m_frequency);
                });
                m_channel1Flag=true;
            }
            else
            {
                timer1_1->stop();
                timer1_2->stop();
                serialConnect->write("@AE01F\n");
                m_channel1Flag=false;
            }
        }
    });
    connect(ui->widget_pulsechannel2,&PulseMode::onOff,[=](){
        if(m_flag==true)
        {
            if(m_channel2Flag==false)
            {
                timer2_1->start(1000/ui->widget_pulsechannel2->m_frequency);
                QTimer::singleShot(1000/ui->widget_pulsechannel2->m_frequency*(ui->widget_pulsechannel2->m_percent/100),this,[=](){
                    timer2_2->start(1000/ui->widget_pulsechannel2->m_frequency);
                });
                m_channel2Flag=true;
            }
            else
            {
                timer2_1->stop();
                timer2_2->stop();
                serialConnect->write("@AE02F\n");
                m_channel2Flag=false;
            }
        }
    });
    connect(ui->widget_pulsechannel3,&PulseMode::onOff,[=](){
        if(m_flag==true)
        {
            if(m_channel3Flag==false)
            {
                timer3_1->start(1000/ui->widget_pulsechannel3->m_frequency);
                QTimer::singleShot(1000/ui->widget_pulsechannel3->m_frequency*(ui->widget_pulsechannel3->m_percent/100),this,[=](){
                    timer3_2->start(1000/ui->widget_pulsechannel3->m_frequency);
                });
                m_channel3Flag=true;
            }
            else
            {
                timer3_1->stop();
                timer3_2->stop();
                serialConnect->write("@AE03F\n");
                m_channel3Flag=false;
            }
        }
    });
    connect(ui->widget_pulsechannel4,&PulseMode::onOff,[=](){
        if(m_flag==true)
        {
            if(m_channel4Flag==false)
            {
                timer4_1->start(1000/ui->widget_pulsechannel4->m_frequency);
                QTimer::singleShot(1000/ui->widget_pulsechannel4->m_frequency*(ui->widget_pulsechannel4->m_percent/100),this,[=](){
                    timer4_2->start(1000/ui->widget_pulsechannel4->m_frequency);
                });
                m_channel4Flag=true;
            }
            else
            {
                timer4_1->stop();
                timer4_2->stop();
                serialConnect->write("@AE04F\n");
                m_channel4Flag=false;
            }
        }
    });
    connect(ui->widget_pulsechannelAll,&PulseMode::onOff,[=](){
        if(m_flag==true)
        {
            if(m_channelAllFlag==false)
            {
                timer5_1->start(1000/ui->widget_pulsechannelAll->m_frequency);
                QTimer::singleShot(1000/ui->widget_pulsechannelAll->m_frequency*(ui->widget_pulsechannelAll->m_percent/100),this,[=](){
                    timer5_2->start(1000/ui->widget_pulsechannelAll->m_frequency);
                });
                m_channelAllFlag=true;
            }
            else
            {
                timer5_1->stop();
                timer5_2->stop();
                serialConnect->write("@AEFFF\n");
                m_channelAllFlag=false;
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeSerialPort()
{
    //关闭串口
    serialConnect->disConnectSerialPort();
    delete serialConnect;
    serialConnect=NULL;
    m_flag=false;
    ui->label_connect->setText("当前状态：设备未连接");
}
