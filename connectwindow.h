#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H
#include <QComboBox>
#include <QPushButton>

#include <QMainWindow>

namespace Ui {
class ConnectWindow;
}

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();

    //把串口放入comboBox
    void putInBox(QStringList list);

    //返回当前comboBox内容
    QString getComboBoxText();

signals:
    void openDevice();
    void closeDevice();

private:
    Ui::ConnectWindow *ui;
};

#endif // CONNECTWINDOW_H
