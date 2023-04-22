
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stopThreadButton->setEnabled(false);

    if (!QSerialPortInfo::availablePorts().isEmpty())
    {
        port = new QSerialPort(QSerialPortInfo::availablePorts().at(0));

        port->setBaudRate(QSerialPort::Baud9600);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        port->setFlowControl(QSerialPort::NoFlowControl);

        port->open(QIODevice::ReadWrite);
        connect(port, SIGNAL(readyRead()), this, SLOT(portReceive()));
        qDebug() << "Port" << port->portName() << "is open";

        port->setDataTerminalReady(statusDTR);

        ///if (port.pinoutSignals() & QSerialPort::DataTerminalReadySignal) // проверка наличия сигнала на конкретной линии порта

    } else {
        ui->pushButton->setEnabled(false);
        ui->textEdit->setEnabled(false);
        ui->startThreadButton->setEnabled(false);
        qDebug() << "No available ports";
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    if (port->isOpen())
        port->close();
    delete port;
}

void MainWindow::on_pushButton_clicked()
{
    if (port->isOpen())
        port->write(ui->textEdit->toPlainText().toLatin1());
}

void MainWindow::portReceive()
{
    QByteArray data;
    data = port->readAll();

    ui->label_2->setText(data);
    qDebug() << data;
}
///////////////////////////////////////////////////////////////////////////
//void MainWindow::updateTextBox(int number)
//{
//    ui->textEdit_2->insertPlainText(QString::number(number));
//}

void MainWindow::setDTR(int signal)
{
    statusDTR = !statusDTR;
    port->setDataTerminalReady(statusDTR);
    ui->textEdit_2->insertPlainText(QString::number(signal));
    ui->textEdit_3->setPlainText(QString::number(port->isDataTerminalReady()));
}

void MainWindow::on_startThreadButton_clicked()
{
    signalsThread = new QThread;
    signalGenerator = new SignalGenerator;
    signalGenerator->moveToThread(signalsThread);

    //connect(signalGenerator, SIGNAL(emitSignal(int)), this, SLOT(updateTextBox(int)));
    connect(signalGenerator, SIGNAL(emitSignal(int)), this, SLOT(setDTR(int)));
    connect(signalsThread, SIGNAL(started()), signalGenerator, SLOT(generate()));

    ui->startThreadButton->setEnabled(false);
    ui->stopThreadButton->setEnabled(true);

    signalsThread->start();
}

void MainWindow::on_stopThreadButton_clicked()
{
    signalGenerator->stopGenerate();
    signalsThread->quit();
    signalsThread->wait();
    delete signalsThread;
    delete signalGenerator;

    ui->stopThreadButton->setEnabled(false);
    ui->startThreadButton->setEnabled(true);
}

void MainWindow::on_clearButton_clicked()
{
    ui->textEdit_2->clear();
}
