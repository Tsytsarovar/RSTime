
#include "signalgenerator.h"

SignalGenerator::SignalGenerator(QSerialPort *port)
{
    this->port = port;
}

void SignalGenerator::generate()
{
    int curSec = QTime::currentTime().second();
    while(isRun)
    {
        int newSec = QTime::currentTime().second();
        if (curSec != newSec)
        {
            statusDTR = !statusDTR;
            port->setDataTerminalReady(statusDTR);
            emit emitSignal(newSec);

            //QThread::msleep(250);

            statusDTR = !statusDTR;
            port->setDataTerminalReady(statusDTR);
            emit emitSignal(-1);

            curSec = newSec;
        }
        //QThread::msleep(250);
    }
}

void SignalGenerator::stopGenerate()
{
    isRun = false;
}
