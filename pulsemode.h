#ifndef PULSEMODE_H
#define PULSEMODE_H

#include <QWidget>

namespace Ui {
class PulseMode;
}

class PulseMode : public QWidget
{
    Q_OBJECT

public:
    explicit PulseMode(QWidget *parent = nullptr);
    ~PulseMode();

    //修改标签名
    void changeLabel(QString str);

    //记录频率
    double m_frequency=1;

    //记录占空比
    double m_percent=50;

signals:

    //开关信号
    void onOff();

private:
    Ui::PulseMode *ui;
};

#endif // PULSEMODE_H
