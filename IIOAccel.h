//-----------------------------------------------------------
//
//  File IIOAccel.h generated by RTembedded
//  Created at: Mon Jun 15 16:09:31 2015
//  This file will not be regenerated unless deleted and can be modified as required
//
//-----------------------------------------------------------

#ifndef _IIOACCEL_H
#define _IIOACCEL_H

#include "MainClass.h"

class IIOAccel : public MainClass
{
    Q_OBJECT

public:
    IIOAccel(QObject *parent = 0);

public slots:

signals:

protected:
    virtual void setup();
    virtual void loop();

private:
    void registerSigHandler();
    static void sigHandler(int sig);
    static volatile bool sigIntReceived;

    qint64 m_lastTimestamp;
};

#endif // _IIOACCEL_H