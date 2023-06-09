
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
            //statusDTR = !statusDTR;
            port->setRequestToSend(true);
            emit emitSignal(newSec);

            QThread::msleep(impulseTime);

            if (!isRun)
                break;

            //statusDTR = !statusDTR;
            port->setRequestToSend(false);
            emit emitSignal(-1);

            curSec = newSec;
        }
        QThread::msleep(0);
    }
}

void SignalGenerator::stopGenerate()
{
    isRun = false;
}
