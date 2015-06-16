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

#ifndef _RTESPIDRIVER_H
#define	_RTESPIDRIVER_H

#include <qmutex.h>
#include <stdint.h>

#define RTESPIDRIVER_MAX_BUS 8                              // max 8 busses
#define RTESPIDRIVER_MAX_SELECT 8                           // max 8 selects per bus

typedef struct
{
    bool m_open;
    int m_fd;                                               // the file descriptor
    uint32_t m_speed;                                       // speed of interface
    unsigned char m_bus;
    unsigned char m_select;
} RTESPIDRIVER_CHANNEL;

class RTeSPIDriver
{
public:
    RTeSPIDriver();
    virtual ~RTeSPIDriver();

    static bool setSpeed(unsigned char bus, unsigned select, int speed);

    static bool deviceRead(unsigned char bus, unsigned char select, unsigned char regAddr, unsigned char length,
                 unsigned char *data, const char *errorMsg);

    static bool deviceWrite(unsigned char bus, unsigned char select, unsigned char regAddr,
                  unsigned char length, unsigned char const *data, const char *errorMsg);

    static bool deviceWrite(unsigned char bus, unsigned char select, unsigned char regAddr,
                  unsigned char const data, const char *errorMsg);

private:
    static bool internalDeviceWrite(RTESPIDRIVER_CHANNEL *channel, unsigned char regAddr,
                  unsigned char length, unsigned char const *data, const char *errorMsg);
    static bool SPIOpen(RTESPIDRIVER_CHANNEL *channel);
    static void SPIClose(unsigned char bus, unsigned char select);

    static bool ifWrite(RTESPIDRIVER_CHANNEL *channel, unsigned char *data, unsigned char length);
    static RTESPIDRIVER_CHANNEL *getChannel(unsigned char bus, unsigned char select);

    static RTESPIDRIVER_CHANNEL m_channels[RTESPIDRIVER_MAX_BUS][RTESPIDRIVER_MAX_SELECT];

    static QMutex m_lock;
    static QString m_logTag;
};

#endif // _RTEI2CDRIVER_H
