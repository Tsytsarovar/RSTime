#include "connectmenu.h"
#include "ui_connectmenu.h"

ConnectMenu::ConnectMenu(QWidget *parent, QSerialPort *port) :
    QMainWindow(nullptr),    // !!!parent!!!
    ui(new Ui::ConnectMenu)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    m = parent;
    this->port = port;
    connect(this->port, SIGNAL(readyRead()), this, SLOT(port1Receive()));



    /////////////////////////////////////
    porttwin = new QSerialPort(QSerialPortInfo::availablePorts().at(1));

    porttwin->setBaudRate(QSerialPort::Baud9600);
    porttwin->setDataBits(QSerialPort::Data8);
    porttwin->setParity(QSerialPort::NoParity);
    porttwin->setStopBits(QSerialPort::OneStop);
    porttwin->setFlowControl(QSerialPort::NoFlowControl);

    porttwin->open(QIODevice::ReadWrite);
    connect(this->porttwin, SIGNAL(readyRead()), this, SLOT(port2Receive()));
    /////////////////////////////////////
}

ConnectMenu::~ConnectMenu()
{
    /////////////////////////////////////
    if (porttwin->isOpen())
    {
        qDebug() << "Port" << porttwin->portName() << "is closed";
        porttwin->close();
    }

    delete porttwin;
    /////////////////////////////////////
    delete ui;
}

void ConnectMenu::on_send1Button_clicked()
{
    if (port->isOpen())
        port->write(ui->send1Text->toPlainText().toLatin1());
}

void ConnectMenu::port1Receive()
{
    QByteArray data;
    data = port->readAll();

    ui->receive1Text->setText(data);
    qDebug() << data;
}

/////////////////////////////////////
void ConnectMenu::on_send2Button_clicked()
{
    if (porttwin->isOpen())
        porttwin->write(ui->send2Text->toPlainText().toLatin1());
}

void ConnectMenu::port2Receive()
{
    QByteArray data;
    data = porttwin->readAll();

    ui->receive2Text->setText(data);
    qDebug() << data;
}
/////////////////////////////////////

void ConnectMenu::closeEvent(QCloseEvent *event)
{
    event->ignore();
    m->show();
    event->accept();
}
