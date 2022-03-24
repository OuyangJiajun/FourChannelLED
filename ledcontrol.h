#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <QWidget>

namespace Ui {
class LedControl;
}

class LedControl : public QWidget
{
    Q_OBJECT

public:
    explicit LedControl(QWidget *parent = nullptr);
    ~LedControl();

    //修改标签名
    void changeLabel(QString str);

    //修改电流百分比(读到电流初始化表盘用)
    void changeCurrent(int current);

    //用于获取当前表盘电流百分比
    int getCurrentPercent();

    //同步电流标志
    bool flag;

signals:
    //改变电流信号
    void currentChange();
    //开关信号
    void onOff();

private:
    Ui::LedControl *ui;
};

#endif // LEDCONTROL_H
