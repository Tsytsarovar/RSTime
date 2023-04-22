
#include "signalgenerator.h"


SignalGenerator::SignalGenerator()
{

}

void SignalGenerator::generate()
{
    int curSec = QTime::currentTime().second();
    while(isRun)
    {
        int newSec = QTime::currentTime().second();
        if (curSec != newSec)
        {
            emit emitSignal(newSec);
            QThread::msleep(10);
            emit emitSignal(newSec);
            curSec = newSec;
        }
        QThread::msleep(1); //TODO system load
    }
}

void SignalGenerator::stopGenerate()
{
    isRun = false;
}
