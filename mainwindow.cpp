
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->connectMenuButton->setEnabled(false);

    ui->startThreadButton->setEnabled(false);
    ui->stopThreadButton->setEnabled(false);
    ui->textEdit_4->setEnabled(false);
    ui->impulseTimeText->setEnabled(false);
    ui->widget->setVisible(false);
    ui->closePortButton->setEnabled(false);

    ui->portFrame->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDTR(int signal)
{
//    statusDTR = !statusDTR;
//    port->setDataTerminalReady(statusDTR);
    QTime curTime = QTime::currentTime();

/// НАНОСЕКУНДЫ ???
//    QElapsedTimer timer;
//    timer.start();
//    qDebug() << timer.nsecsElapsed();


    if (signal >= 0)
        ui->textEdit_2->setPlainText(QString::number(signal));

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
    if (ui->impulseTimeText->toPlainText().toInt() >= 0 && ui->impulseTimeText->toPlainText().toInt() <= 750)
    {
        ui->closePortButton->setEnabled(false);

        ui->connectMenuButton->setEnabled(false);

        signalsThread = new QThread;
        signalGenerator = new SignalGenerator(port);
        signalGenerator->impulseTime = ui->impulseTimeText->toPlainText().toInt();
        signalGenerator->moveToThread(signalsThread);

        connect(signalGenerator, SIGNAL(emitSignal(int)), this, SLOT(setDTR(int)));
        connect(signalsThread, SIGNAL(started()), signalGenerator, SLOT(generate()));
        //connect(signalGenerator, SIGNAL(check()), this, SLOT(plotGraf()));

        ui->startThreadButton->setEnabled(false);
        ui->stopThreadButton->setEnabled(true);

        timeFrame = ui->textEdit_4->toPlainText().toInt();
        ui->textEdit_4->setEnabled(false);
        ui->impulseTimeText->setEnabled(false);

        statusDTR = false;

        signalsThread->start();
        startThread = QTime::currentTime();
    }
    else qDebug() << "Impulse Time is wrong";
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

    timeCount = 0;
    i = 0;

    ui->stopThreadButton->setEnabled(false);
    ui->startThreadButton->setEnabled(true);

    ui->textEdit_4->setEnabled(true);
    ui->impulseTimeText->setEnabled(true);

    ui->closePortButton->setEnabled(true);

    ui->connectMenuButton->setEnabled(true);
}

void MainWindow::on_openPortButton_clicked()
{
    int portIndex = -1;
    for (int i = 0; i < QSerialPortInfo::availablePorts().length(); i++)
    {
        if (QSerialPortInfo::availablePorts().at(i).portName() == ui->portsComboBox->currentText())
        {
            portIndex = i;
            break;
        }
    }

    if (portIndex != -1)
    {
        ui->connectMenuButton->setEnabled(true);

        ui->openPortButton->setEnabled(false);

        port = new QSerialPort(QSerialPortInfo::availablePorts().at(portIndex));

        port->setBaudRate(QSerialPort::Baud9600);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        port->setFlowControl(QSerialPort::NoFlowControl);

        port->open(QIODevice::ReadWrite);

        qDebug() << "Port" << port->portName() << "is open";

        ui->startThreadButton->setEnabled(true);
        ui->textEdit_4->setEnabled(true);
        ui->textEdit_4->setPlainText("1");
        ui->impulseTimeText->setEnabled(true);
        ui->impulseTimeText->setPlainText("500");

        ui->closePortButton->setEnabled(true);
        ui->portsButton->setEnabled(false);
        ui->portsComboBox->setEnabled(false);


        ///if (port.pinoutSignals() & QSerialPort::DataTerminalReadySignal) // проверка наличия сигнала на конкретной линии порта

    } else {
        qDebug() << "No available ports";
    }
}

void MainWindow::on_closePortButton_clicked()
{
    ui->connectMenuButton->setEnabled(false);

    ui->closePortButton->setEnabled(false);
    ui->startThreadButton->setEnabled(false);
    ui->stopThreadButton->setEnabled(false);
    ui->widget->setVisible(false);
    ui->textEdit_4->setEnabled(false);
    ui->impulseTimeText->setEnabled(false);

    if (port->isOpen())
    {
        qDebug() << "Port" << port->portName() << "is closed";
        port->close();
    }

    delete port;

    ui->openPortButton->setEnabled(true);
    ui->portsButton->setEnabled(true);

    ui->portFrame->setVisible(false);
    ui->portsButton->setVisible(true);

    ui->textEdit_4->setPlainText("1");
    ui->impulseTimeText->setPlainText("500");
}

void MainWindow::on_portsButton_clicked()
{
    ui->portsComboBox->clear();

    foreach (const QSerialPortInfo &curPort, QSerialPortInfo::availablePorts())
    {
        if (!curPort.isBusy())
            ui->portsComboBox->addItem(curPort.portName());
    }

    ui->portFrame->setVisible(true);
    ui->portsComboBox->setEnabled(true);
}

void MainWindow::on_connectMenuButton_clicked()
{
    this->hide();
    cm = new ConnectMenu(this, port);
    cm->show();
}

