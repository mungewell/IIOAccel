////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTembedded
//
//  Copyright (c) 2015, richards-tech LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in
//  the Software without restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
//  Software, and to permit persons to whom the Software is furnished to do so,
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef _RTEIIOACCEL_H
#define	_RTEIIOACCEL_H

#include "RTeIIO.h"

#define RTEMBEDDED_EXTRADIRECTORIES_IIOACCEL \
    ..:RTeIIO;

#define RTEMBEDDED_SIGNALS_IIOACCEL \
    void newAccelSample(RTeModule *, RTeSensorAccelData *);

typedef struct
{
    qint16 x;
    qint16 y;
    qint16 z;
    qint64 timestamp;
} RTEIIOACCEL_DATA;

class RTeIIOAccel : public RTeIIO
{
    Q_OBJECT

public:
    RTeIIOAccel();

    void setSampleRate(const QString& rate) { m_sampleRate = rate.toInt(); }
    void setFSR(const QString& fsr) { m_fsr = fsr.toInt(); }

signals:
    void newAccelSample(RTeModule *, RTeSensorAccelData *);

protected:
    void initModule();
    void stopModule();
    void timerEvent(QTimerEvent *);

private:
    int m_sampleRate;                                       // the accel sample rate
    int m_fsr;                                              // the accel full scale range

    QString m_dataNames[3];

    qint64 m_startTime;
    int m_count;

    int m_fp;
    int m_bytesGot;
    int m_bytesLeft;
};

#endif // _RTEIIOACCEL_H
