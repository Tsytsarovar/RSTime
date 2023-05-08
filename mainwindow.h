
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectmenu.h"

#include <QThread>
#include <signalgenerator.h>

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QVector>
#include <QTime>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_startThreadButton_clicked();

    void on_stopThreadButton_clicked();
    void setDTR(int);
    //void plotGraf();


    void on_openPortButton_clicked();

    void on_closePortButton_clicked();

    void on_portsButton_clicked();

    void on_connectMenuButton_clicked();

private:
    Ui::MainWindow *ui;
    ConnectMenu *cm;

    QSerialPort *port;
    QThread *signalsThread;
    SignalGenerator *signalGenerator;
    bool statusDTR = false;

    //bool isPlotGraf = false;
    int timeFrame;

    QVector<double> x, y;

    QTime startThread;

    int timeCount = 0;
    int i = 0;
};

#endif // MAINWINDOW_H
