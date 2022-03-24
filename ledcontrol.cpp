#include "ledcontrol.h"
#include "ui_ledcontrol.h"
#include <QSpinBox>
#include <QSlider>

LedControl::LedControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedControl)
{
    ui->setupUi(this);

    //百分比动滑条跟着动
    void (QSpinBox:: *spSignal)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBox,spSignal,ui->verticalSlider,&QSlider::setValue);

    //滑条动百分比跟着动
    connect(ui->verticalSlider,&QSlider::valueChanged,ui->spinBox,&QSpinBox::setValue);

    //发改变电流信号（百分比和滑条已经绑定一起动，都触发）
    connect(ui->spinBox,spSignal,[=](){
        //初始化时不要发
        if(!flag)
        {
            emit currentChange();
        }
    });

    //发开关信号
    connect(ui->toolButton,&QToolButton::clicked,[=](){
        emit onOff();
    });
}

LedControl::~LedControl()
{
    delete ui;
}

void LedControl::changeLabel(QString str)
{
    ui->label->setText(str);
}

void LedControl::changeCurrent(int current)
{
    ui->spinBox->setValue(current);
}

int LedControl::getCurrentPercent()
{
    return ui->spinBox->value();
}
