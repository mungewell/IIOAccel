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


#ifndef _RTEIIO_H
#define	_RTEIIO_H

#include "RTeThreadedModule.h"
#include "RTeSensorDefs.h"

#include <qfile.h>

class RTeIIO : public RTeThreadedModule
{
public:
    RTeIIO();

    void setDeviceNumber(const QString& number);
    void setUseBuffer(const QString& useBuffer) { m_useBuffer = useBuffer == "true"; }

protected:
    bool setValue(QString file, int value);
    bool setValue(QString file, qreal value);
    bool setValue(QString file, const QString& value);

    qint64 m_timestamp;

    QString m_devicePath;                                   // base path for device

    int m_timer;
    int m_deviceNumber;                                     // the iio device number
    QString m_deviceBuffer;                                 // buffer device

    bool m_useBuffer;
};

#endif // _RTEIIO_H
