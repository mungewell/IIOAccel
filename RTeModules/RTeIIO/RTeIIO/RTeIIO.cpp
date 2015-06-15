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

#include "RTeIIO.h"

RTeIIO::RTeIIO() : RTeThreadedModule()
{
    setDeviceNumber("0");
    m_useBuffer = true;
}

void RTeIIO::setDeviceNumber(const QString &number)
{
    m_deviceNumber = number.toInt();
    m_devicePath = QString("/sys/bus/iio/devices/iio:device%1/").arg(m_deviceNumber);
    m_deviceBuffer = QString("/dev/iio:device%1").arg(m_deviceNumber);
}

bool RTeIIO::setValue(QString file, const QString &value)
{
    QFile deviceFile(m_devicePath + file);
    if (!deviceFile.open(QIODevice::WriteOnly))
        return false;
    if (deviceFile.write(qPrintable(value)) < 0) {
        deviceFile.close();
        return false;
    }
    deviceFile.close();
    return true;
}

bool RTeIIO::setValue(QString file, int value)
{
    QFile deviceFile(m_devicePath + file);
    if (!deviceFile.open(QIODevice::WriteOnly))
        return false;
    if (deviceFile.write(qPrintable(QString::number(value))) < 0) {
        deviceFile.close();
        return false;
    }
    deviceFile.close();
    return true;
}

bool RTeIIO::setValue(QString file, qreal value)
{
    QFile deviceFile(m_devicePath + file);
    if (!deviceFile.open(QIODevice::WriteOnly))
        return false;
    if (deviceFile.write(qPrintable(QString::number(value))) < 0) {
        deviceFile.close();
        return false;
    }
    deviceFile.close();
    return true;
}
