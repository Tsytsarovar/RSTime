#include "connectmenu.h"
#include "ui_connectmenu.h"

ConnectMenu::ConnectMenu(QWidget *parent, QSerialPort *port) :
    QMainWindow(parent),
    ui(new Ui::ConnectMenu)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    m = parent;
    this->port = port;
    connect(this->port, SIGNAL(readyRead()), this, SLOT(portReceive()));
}

ConnectMenu::~ConnectMenu()
{
    delete ui;
}

void ConnectMenu::on_pushButton_clicked()
{
    if (port->isOpen())
        port->write(ui->textEdit->toPlainText().toLatin1());
}

void ConnectMenu::portReceive()
{
    QByteArray data;
    data = port->readAll();

    ui->label_2->setText(data);
    qDebug() << data;
}

void ConnectMenu::closeEvent(QCloseEvent *event)
{
    event->ignore();
    m->show();
    event->accept();
}
