////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTembedded
//
//  Copyright (c) 2015, richards-tech, LLC
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

#ifndef _RTEI2CDRIVER_H
#define	_RTEI2CDRIVER_H

#include <qmutex.h>

#define RTEI2CDRIVER_MAX_BUS 8                              // max 8 busses

typedef struct
{
    bool m_open;
    unsigned char m_currentSlave;                           // current slave address for I2C
    int m_fd;                                               // the file descriptor
} RTEI2CDRIVER_CHANNEL;

class RTeI2CDriver
{
public:
    RTeI2CDriver();
    virtual ~RTeI2CDriver();

    static bool deviceRead(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr, unsigned char length,
                 unsigned char *data, const char *errorMsg);

    static bool deviceWrite(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr,
                  unsigned char length, unsigned char const *data, const char *errorMsg);

    static bool deviceWrite(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr,
                  unsigned char const data, const char *errorMsg);

private:
    static bool internalDeviceWrite(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr,
                  unsigned char length, unsigned char const *data, const char *errorMsg);
    static bool I2COpen(unsigned char bus);
    static void I2CClose(unsigned char bus);
    static bool I2CSelectSlave(unsigned char bus, unsigned char slaveAddr, const char *errorMsg);

    static RTEI2CDRIVER_CHANNEL m_channels[RTEI2CDRIVER_MAX_BUS];

    static QMutex m_lock;
    static QString m_logTag;
};

#endif // _RTEI2CDRIVER_H
