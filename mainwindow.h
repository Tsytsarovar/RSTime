
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QThread>
#include <signalgenerator.h>

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>



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
    void on_pushButton_clicked();
    void portReceive();

    void on_startThreadButton_clicked();

    void on_clearButton_clicked();

    void on_stopThreadButton_clicked();
    void setDTR(int);

private:
    Ui::MainWindow *ui;
    QSerialPort *port;
    QThread *signalsThread;
    SignalGenerator *signalGenerator;
    bool statusDTR = false;
};

#endif // MAINWINDOW_H
