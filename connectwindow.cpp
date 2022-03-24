#include "connectwindow.h"
#include "ui_connectwindow.h"
#include "serialconnect.h"

ConnectWindow::ConnectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);

    //打开设备按钮
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        emit openDevice();
    });

    //关闭设备按钮
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        emit closeDevice();
    });
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::putInBox(QStringList list)
{
    ui->comboBox->addItems(list);
}

QString ConnectWindow::getComboBoxText()
{
    return ui->comboBox->currentText();
}
