#ifndef CONNECTMENU_H
#define CONNECTMENU_H

#include <QMainWindow>

#include <QCloseEvent>

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class ConnectMenu;
}

class ConnectMenu : public QMainWindow
{
    Q_OBJECT

public:
    QSerialPort *port;
    explicit ConnectMenu(QWidget *parent = nullptr, QSerialPort *port = nullptr);

    void closeEvent(QCloseEvent *event);

    ~ConnectMenu();

private slots:
    void on_pushButton_clicked();
    void portReceive();

private:
    Ui::ConnectMenu *ui;
    QWidget *m;
};

#endif // CONNECTMENU_H
