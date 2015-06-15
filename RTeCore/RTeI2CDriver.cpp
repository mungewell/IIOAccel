////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTembedded
//
//  Copyright (c) 2015, richards-tech
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

#include "RTeI2CDriver.h"
#include "RTeLog.h"

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_WRITE_LEN                   255
#define MAX_READ_LEN                    255


RTEI2CDRIVER_CHANNEL RTeI2CDriver::m_channels[RTEI2CDRIVER_MAX_BUS];
QMutex RTeI2CDriver::m_lock;
QString RTeI2CDriver::m_logTag;

//  This is to force initialization

static RTeI2CDriver theRTeI2CDriver;

RTeI2CDriver::RTeI2CDriver()
{

    for (int i = 0; i <  RTEI2CDRIVER_MAX_BUS; i++) {
        m_channels[i].m_open = false;
        m_channels[i].m_currentSlave = -1;
    }
    m_logTag = "RTeI2CDriver";
}

RTeI2CDriver::~RTeI2CDriver()
{
    for (int i = 0; i < RTEI2CDRIVER_MAX_BUS; i++) {
        if (m_channels[i].m_open)
            I2CClose(i);
    }
}

bool RTeI2CDriver::deviceWrite(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr,
                   unsigned char const data, const char *errorMsg)
{
    return deviceWrite(bus, slaveAddr, regAddr, 1, &data, errorMsg);
}

bool RTeI2CDriver::deviceWrite(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr,
                   unsigned char length, unsigned char const *data, const char *errorMsg)
{
    QMutexLocker lock(&m_lock);
    return internalDeviceWrite(bus, slaveAddr, regAddr, length, data, errorMsg);
}

bool RTeI2CDriver::deviceRead(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr, unsigned char length,
                    unsigned char *data, const char *errorMsg)
{
    int tries, result, total;

    QMutexLocker lock(&m_lock);

    if (!internalDeviceWrite(bus, slaveAddr, regAddr, 0, NULL, errorMsg))
        return false;

    total = 0;
    tries = 0;

    while ((total < length) && (tries < 5)) {
        result = read(m_channels[bus].m_fd, data + total, length - total);

        if (result < 0) {
            if (strlen(errorMsg) > 0)
                RTeError(NULL, QString("I2C read error from %1, %2 - %3")
                         .arg(slaveAddr).arg(regAddr).arg(errorMsg));
            return false;
        }

        total += result;

        if (total == length)
            break;

        usleep(10000);
        tries++;
    }

    if (total < length) {
        if (strlen(errorMsg) > 0)
            RTeError(m_logTag, QString("I2C read from %1, %2 failed - %3")
                     .arg(slaveAddr).arg(regAddr).arg(errorMsg));
        return false;
    }
    return true;
}


bool RTeI2CDriver::I2CSelectSlave(unsigned char bus, unsigned char slaveAddr, const char *errorMsg)
{
    if (m_channels[bus].m_currentSlave == slaveAddr)
        return true;

    if (!I2COpen(bus)) {
        RTeError(m_logTag, QString("Failed to open I2C port - %1").arg(errorMsg));
        return false;
    }

    if (ioctl(m_channels[bus].m_fd, I2C_SLAVE, slaveAddr) < 0) {
        RTeError(m_logTag, QString("I2C slave select %1 failed - %2").arg(slaveAddr).arg(errorMsg));
        return false;
    }

    m_channels[bus].m_currentSlave = slaveAddr;

    return true;
}


bool RTeI2CDriver::I2COpen(unsigned char bus)
{
    char buf[32];

    if (m_channels[bus].m_open)
        return true;

    sprintf(buf, "/dev/i2c-%d", bus);

    if ((m_channels[bus].m_fd = open(buf, O_RDWR)) < 0) {
        RTeError(m_logTag, QString("Failed to open I2C bus %1").arg(bus));
        return false;
    }
    m_channels[bus].m_open = true;
    m_channels[bus].m_currentSlave = -1;
    return true;
}

void RTeI2CDriver::I2CClose(unsigned char bus)
{
    if (m_channels[bus].m_open) {
        close(m_channels[bus].m_fd);
        m_channels[bus].m_open = false;
        m_channels[bus].m_currentSlave = -1;
    }
}

bool RTeI2CDriver::internalDeviceWrite(unsigned char bus, unsigned char slaveAddr, unsigned char regAddr,
                   unsigned char length, unsigned char const *data, const char *errorMsg)
{
    int result;
    unsigned char txBuff[MAX_WRITE_LEN + 1];

    if (!I2CSelectSlave(bus, slaveAddr, errorMsg))
            return false;

    if (length == 0) {
        result = write(m_channels[bus].m_fd, &regAddr, 1);
        if (result < 0) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("I2C write of regAddr failed - %1").arg(errorMsg));
            return false;
        } else if (result != 1) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("I2C write of regAddr failed (nothing written) - %1").arg(errorMsg));
            return false;
        }
    } else {
        txBuff[0] = regAddr;
        memcpy(txBuff + 1, data, length);

        result = write(m_channels[bus].m_fd, txBuff, length + 1);

        if (result < 0) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("I2C data write of %1 bytes failed - %2").arg(length).arg(errorMsg));
            return false;
        } else if (result < (int)length) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("I2C data write of %1 bytes failed, only %2 written - %3")
                         .arg(length).arg(result).arg(errorMsg));
            return false;
        }
    }

    return true;
}


