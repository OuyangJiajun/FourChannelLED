#include "pulsemode.h"
#include "ui_pulsemode.h"
#include <QMessageBox>

PulseMode::PulseMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PulseMode)
{
    ui->setupUi(this);

    //频率
    void (QDoubleSpinBox:: *spSignal)(double) = &QDoubleSpinBox::valueChanged;
    connect(ui->doubleSpinBox_frequence,spSignal,[=](){
        double temp = ui->doubleSpinBox_frequence->value();
        if( temp > 0 && temp <1000 )
        {
            m_frequency = temp;
        }
        else
        {
            QMessageBox::critical(this,"警告","0Hz<频率<1000HZ");
        }
    });


    //百分比
    connect(ui->doubleSpinBox_percent,spSignal,[=](){
        double temp = ui->doubleSpinBox_percent->value();
        m_frequency = temp;
    });

    //发开关信号
    connect(ui->toolButton,&QToolButton::clicked,[=](){
        emit onOff();
    });
}

PulseMode::~PulseMode()
{
    delete ui;
}

void PulseMode::changeLabel(QString str)
{
    ui->label_name->setText(str);
}
