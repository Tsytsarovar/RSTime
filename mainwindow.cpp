
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setEnabled(false);
    ui->textEdit->setEnabled(false);

    ui->startThreadButton->setEnabled(false);
    ui->stopThreadButton->setEnabled(false);
    ui->textEdit_4->setEnabled(false);
    ui->widget->setVisible(false);
    ui->closePortButton->setEnabled(false);


//    int *lol = new int(1);
//    int *kek = new int(2);

//    lol = kek;

//    qDebug() << lol << *lol;
}

MainWindow::~MainWindow()
{
    delete ui;
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
int timeCount = 0; /// TODO header?
int i = 0;         ///
void MainWindow::setDTR(int signal)
{
//    statusDTR = !statusDTR;
//    port->setDataTerminalReady(statusDTR);
    QTime curTime = QTime::currentTime();

/// НАНОСЕКУНДЫ ???
//    QElapsedTimer timer;
//    timer.start();
//    qDebug() << timer.nsecsElapsed();

    ui->textEdit_2->insertPlainText(QString::number(signal));
    ui->textEdit_3->setPlainText(QString::number(port->isDataTerminalReady()));

    double curTimeX = startThread.msecsTo(curTime) / 1000.0 - timeFrame*i;
    x.push_back(curTimeX);
    x.push_back(curTimeX);

    bool signalDTR = signal == -1 ? 0 : 1;
    y.push_back((int)(!signalDTR));
    y.push_back((int)signalDTR);

    if (signal == -1) // !Привязка ко второму сигналу генератора!
    {
        timeCount++;
    }

    if (timeCount == timeFrame)
    {
        ui->widget->clearPlottables();
        ui->widget->setVisible(true);

        ui->widget->xAxis->setRange(0, timeFrame);
        ui->widget->yAxis->setRange(0, 2);

        QCPCurve *curve = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
        curve->addData(x,y);
        ui->widget->replot();

        timeCount = 0;

        qDebug() << x << y;
        x.clear();
        y.clear();

        ui->textEdit_2->clear();
        ui->textEdit_3->clear();

        i++;
    }

    //qDebug() << timer.nsecsElapsed();
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
    ui->closePortButton->setEnabled(false);

    signalsThread = new QThread;
    signalGenerator = new SignalGenerator(port);
    signalGenerator->moveToThread(signalsThread);

    connect(signalGenerator, SIGNAL(emitSignal(int)), this, SLOT(setDTR(int)));
    connect(signalsThread, SIGNAL(started()), signalGenerator, SLOT(generate()));
    //connect(signalGenerator, SIGNAL(check()), this, SLOT(plotGraf()));

    ui->startThreadButton->setEnabled(false);
    ui->stopThreadButton->setEnabled(true);

    timeFrame = ui->textEdit_4->toPlainText().toInt();
    ui->textEdit_4->setEnabled(false);

    statusDTR = false;
    timeCount = 0;

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

    x.clear();
    y.clear();

    ui->textEdit_2->clear();
    ui->textEdit_3->clear();

    i = 0;

    ui->stopThreadButton->setEnabled(false);
    ui->startThreadButton->setEnabled(true);

    ui->textEdit_4->setEnabled(true);

    ui->closePortButton->setEnabled(true);
}

void MainWindow::on_clearButton_clicked()
{
    ui->textEdit_2->clear();
}

void MainWindow::on_openPortButton_clicked()
{
    if (!QSerialPortInfo::availablePorts().isEmpty())
    {
        ui->pushButton->setEnabled(true);
        ui->textEdit->setEnabled(true);

        ui->openPortButton->setEnabled(false);

        port = new QSerialPort(QSerialPortInfo::availablePorts().at(0));

        port->setBaudRate(QSerialPort::Baud9600);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        port->setFlowControl(QSerialPort::NoFlowControl);

        port->open(QIODevice::ReadWrite);
        connect(port, SIGNAL(readyRead()), this, SLOT(portReceive()));
        qDebug() << "Port" << port->portName() << "is open";

        ui->startThreadButton->setEnabled(true);
        ui->textEdit_4->setEnabled(true);

        ui->openPortText->setPlainText(port->portName());
        ui->closePortButton->setEnabled(true);


        ///if (port.pinoutSignals() & QSerialPort::DataTerminalReadySignal) // проверка наличия сигнала на конкретной линии порта

    } else {
        ui->openPortText->setPlainText("No available ports");
        qDebug() << "No available ports";
    }
}

void MainWindow::on_closePortButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->textEdit->setEnabled(false);

    ui->closePortButton->setEnabled(false);
    ui->startThreadButton->setEnabled(false);
    ui->stopThreadButton->setEnabled(false);
    ui->widget->setVisible(false);
    ui->textEdit_4->setEnabled(false);
    ui->openPortText->clear();

    if (port->isOpen())
    {
        port->close();
    }

    delete port;

    ui->openPortButton->setEnabled(true);
}

