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
    QSerialPort *porttwin;
    explicit ConnectMenu(QWidget *parent = nullptr, QSerialPort *port = nullptr);

    void closeEvent(QCloseEvent *event);

    ~ConnectMenu();

private slots:
    void on_send1Button_clicked();
    void port1Receive();

    void on_send2Button_clicked();
    void port2Receive();

private:
    Ui::ConnectMenu *ui;
    QWidget *m;
};

#endif // CONNECTMENU_H
