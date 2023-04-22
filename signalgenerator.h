
#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QObject>
#include <QTime>
#include <QThread>

#include <QDebug>


class SignalGenerator : public QObject
{
    Q_OBJECT
    bool isRun = true;

public:
    SignalGenerator();

public slots:
    void generate();
    void stopGenerate();

signals:
    void emitSignal(int);
    //void check();
};

#endif // SIGNALGENERATOR_H
