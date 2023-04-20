
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

        ///port->isDataTerminalReady(); // проверка состояния линии DTR
        ///port.setDataTerminalReady(true); // установка значения на линии DTR
        ///if (port.pinoutSignals() & QSerialPort::DataTerminalReadySignal) // проверка наличия сигнала на конкретной линии порта


    } else {
        ui->pushButton->setEnabled(false);
        ui->textEdit->setEnabled(false);
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

bool isButtonSwitch = false; // просто реакция на нажатие кнопки

void MainWindow::on_pushButton_clicked()
{
    isButtonSwitch = !isButtonSwitch;
    ui->label->setText(QString::number(isButtonSwitch));

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
