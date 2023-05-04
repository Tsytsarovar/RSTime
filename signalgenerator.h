
#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QObject>
#include <QTime>
#include <QThread>

#include <QDebug>

#include <QSerialPort>
#include <QSerialPortInfo>


class SignalGenerator : public QObject
{
    Q_OBJECT
    bool isRun = true;

public:
    QSerialPort *port;
    bool statusDTR = false;
    SignalGenerator(QSerialPort*);

public slots:
    void generate();
    void stopGenerate();

signals:
    void emitSignal(int);
};

#endif // SIGNALGENERATOR_H
