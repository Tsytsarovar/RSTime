
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stopThreadButton->setEnabled(false);
    ui->widget->setVisible(false);

    X = 0;
    step = 0.001;

    QTime a = QTime::currentTime();
    qDebug() << QTime::currentTime().msecsTo(a);

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
void MainWindow::setDTR(int signal)
{
    statusDTR = !statusDTR;
    port->setDataTerminalReady(statusDTR);
    ui->textEdit_2->insertPlainText(QString::number(signal));
    ui->textEdit_3->setPlainText(QString::number(port->isDataTerminalReady()));

    x.push_back(startThread.msecsTo(QTime::currentTime()) / 1000.0);
    y.push_back((int)port->isDataTerminalReady());

    //qDebug() << x[x.length() - 1];

    if (x.length() == 2)
    {
        ui->widget->setVisible(true);

        ui->widget->xAxis->setRange(0, 1); // TODO right border
        ui->widget->yAxis->setRange(0, 2);

        ui->widget->addGraph();
        ui->widget->graph(0)->addData(x,y);
        ui->widget->replot();

    }

}

//void MainWindow::plotGraf()
//{
//    if (X < 1)
//    {
//        x.push_back(X);
//        y.push_back((int)port->isDataTerminalReady());

//        X += step;

//    } else if (!isPlotGraf){
//        ui->widget->setVisible(true);

//        ui->widget->xAxis->setRange(0, X);
//        ui->widget->yAxis->setRange(0, 3);

//        ui->widget->addGraph();
//        ui->widget->graph(0)->addData(x,y);
//        ui->widget->replot();

//        isPlotGraf = true;
//    }
//}

void MainWindow::on_startThreadButton_clicked()
{
    signalsThread = new QThread;
    signalGenerator = new SignalGenerator;
    signalGenerator->moveToThread(signalsThread);

    connect(signalGenerator, SIGNAL(emitSignal(int)), this, SLOT(setDTR(int)));
    connect(signalsThread, SIGNAL(started()), signalGenerator, SLOT(generate()));
    //connect(signalGenerator, SIGNAL(check()), this, SLOT(plotGraf()));

    ui->startThreadButton->setEnabled(false);
    ui->stopThreadButton->setEnabled(true);

    signalsThread->start();
    startThread = QTime::currentTime();
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
