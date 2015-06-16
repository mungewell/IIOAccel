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

#include "RTeSPIDriver.h"
#include "RTeLog.h"

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_WRITE_LEN                   255
#define MAX_READ_LEN                    255


RTESPIDRIVER_CHANNEL RTeSPIDriver::m_channels[RTESPIDRIVER_MAX_BUS][RTESPIDRIVER_MAX_SELECT];
QMutex RTeSPIDriver::m_lock;
QString RTeSPIDriver::m_logTag;

//  This is to force initialization

static RTeSPIDriver theRTeSPIDriver;

RTeSPIDriver::RTeSPIDriver()
{

    for (int bus = 0; bus <  RTESPIDRIVER_MAX_BUS; bus++) {
        for (int select = 0; select < RTESPIDRIVER_MAX_SELECT; select++) {
            m_channels[bus][select].m_open = false;
            m_channels[bus][select].m_speed = 500000;
            m_channels[bus][select].m_bus = bus;
            m_channels[bus][select].m_select = select;
        }
    }
    m_logTag = "RTeSPIDriver";
}

RTeSPIDriver::~RTeSPIDriver()
{
    for (int bus = 0; bus <  RTESPIDRIVER_MAX_BUS; bus++) {
        for (int select = 0; select < RTESPIDRIVER_MAX_SELECT; select++) {
            if (m_channels[bus][select].m_open)
                SPIClose(bus, select);
        }
    }
}

bool RTeSPIDriver::deviceWrite(unsigned char bus, unsigned char select, unsigned char regAddr,
                   unsigned char const data, const char *errorMsg)
{
    return deviceWrite(bus, select, regAddr, 1, &data, errorMsg);
}

bool RTeSPIDriver::deviceWrite(unsigned char bus, unsigned char select, unsigned char regAddr,
                   unsigned char length, unsigned char const *data, const char *errorMsg)
{
    QMutexLocker lock(&m_lock);

    RTESPIDRIVER_CHANNEL *channel = getChannel(bus, select);

    if (channel == 0)
        return false;

    return internalDeviceWrite(channel, regAddr, length, data, errorMsg);
}

bool RTeSPIDriver::deviceRead(unsigned char bus, unsigned char select, unsigned char regAddr, unsigned char length,
                    unsigned char *data, const char *errorMsg)
{
    unsigned char rxBuff[MAX_READ_LEN + 1];
    struct spi_ioc_transfer rdIOC;

    QMutexLocker lock(&m_lock);

    RTESPIDRIVER_CHANNEL *channel = getChannel(bus, select);
    if (channel == 0)
        return false;

    if (!SPIOpen(channel))
        return false;

    rxBuff[0] = regAddr | 0x80;
    memcpy(rxBuff + 1, data, length);
    memset(&rdIOC, 0, sizeof(rdIOC));
    rdIOC.tx_buf = (unsigned long) rxBuff;
    rdIOC.rx_buf = (unsigned long) rxBuff;
    rdIOC.len = length + 1;

    if (ioctl(channel->m_fd, SPI_IOC_MESSAGE(1), &rdIOC) < 0) {
        if (strlen(errorMsg) > 0)
            RTeError(m_logTag, QString("SPI read error from %1 - %2").arg(regAddr).arg(errorMsg));
        return false;
    }
    memcpy(data, rxBuff + 1, length);

    return true;
}


bool RTeSPIDriver::SPIOpen(RTESPIDRIVER_CHANNEL *channel)
{
    unsigned char SPIMode = SPI_MODE_0;
    unsigned char SPIBits = 8;
    char buf[32];

    if (channel->m_open)
        return true;

    sprintf(buf, "/dev/spidev%d.%d", channel->m_bus, channel->m_select);
    if ((channel->m_fd = open(buf, O_RDWR)) < 0) {
        RTeError(m_logTag, QString("Failed to open SPI bus %1, select %2").arg(channel->m_bus).arg(channel->m_select));
        return false;
    }

    if (ioctl(channel->m_fd, SPI_IOC_WR_MODE, &SPIMode) < 0) {
        RTeError(m_logTag, QString("Failed to set WR SPI_MODE0 on bus %1").arg(channel->m_bus));
        close(channel->m_fd);
        return false;
    }

    if (ioctl(channel->m_fd, SPI_IOC_RD_MODE, &SPIMode) < 0) {
        RTeError(m_logTag, QString("Failed to set RD SPI_MODE0 on bus %1").arg(channel->m_bus));
        close(channel->m_fd);
        return false;
    }

    if (ioctl(channel->m_fd, SPI_IOC_WR_BITS_PER_WORD, &SPIBits) < 0) {
        RTeError(m_logTag, QString("Failed to set WR 8 bit mode on bus %1").arg(channel->m_bus));
        close(channel->m_fd);
        return false;
    }

    if (ioctl(channel->m_fd, SPI_IOC_RD_BITS_PER_WORD, &SPIBits) < 0) {
        RTeError(m_logTag, QString("Failed to set RD 8 bit mode on bus %1").arg(channel->m_bus));
        close(channel->m_fd);
        return false;
    }

    if (ioctl(channel->m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &(channel->m_speed)) < 0) {
         RTeError(m_logTag, QString("Failed to set WR %1Hz on bus %2").arg(channel->m_speed).arg(channel->m_bus));
         close(channel->m_fd);
         return false;
    }

    if (ioctl(channel->m_fd, SPI_IOC_RD_MAX_SPEED_HZ, &(channel->m_speed)) < 0) {
        RTeError(m_logTag, QString("Failed to set RD %1Hz on bus %2").arg(channel->m_speed).arg(channel->m_bus));
        close(channel->m_fd);
        return false;
    }
    channel->m_open = true;
    return true;
}

void RTeSPIDriver::SPIClose(unsigned char bus, unsigned char select)
{
    RTESPIDRIVER_CHANNEL *channel = &(m_channels[bus][select]);

    if (channel->m_open) {
        close(channel->m_fd);
        channel->m_open = false;
    }
}

bool RTeSPIDriver::internalDeviceWrite(RTESPIDRIVER_CHANNEL *channel, unsigned char regAddr,
                   unsigned char length, unsigned char const *data, const char *errorMsg)
{
    int result;
    unsigned char txBuff[MAX_WRITE_LEN + 1];

    if (!SPIOpen(channel))
        return false;

    if (length == 0) {
        result = ifWrite(channel, &regAddr, 1);
        if (result < 0) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("SPI write of regAddr failed - %1").arg(errorMsg));
            return false;
        } else if (result != 1) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("SPI write of regAddr failed (nothing written) - %1").arg(errorMsg));
            return false;
        }
    } else {
        txBuff[0] = regAddr;
        memcpy(txBuff + 1, data, length);

        result = ifWrite(channel, txBuff, length + 1);

        if (result < 0) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("SPI data write of %1 bytes failed - %2").arg(length).arg(errorMsg));
            return false;
        } else if (result < (int)length) {
            if (strlen(errorMsg) > 0)
                RTeError(m_logTag, QString("SPI data write of %1 bytes failed, only %2 written - %3")
                         .arg(length).arg(result).arg(errorMsg));
            return false;
        }
    }

    return true;
}

bool RTeSPIDriver::ifWrite(RTESPIDRIVER_CHANNEL *channel, unsigned char *data, unsigned char length)
{
    struct spi_ioc_transfer wrIOC;

    memset(&wrIOC, 0, sizeof(wrIOC));
    wrIOC.tx_buf = (unsigned long) data;
    wrIOC.rx_buf = 0;
    wrIOC.len = length;
    return ioctl(channel->m_fd, SPI_IOC_MESSAGE(1), &wrIOC);
}

RTESPIDRIVER_CHANNEL *RTeSPIDriver::getChannel(unsigned char bus, unsigned char select)
{
    if (bus >= RTESPIDRIVER_MAX_BUS) {
        RTeError(m_logTag, QString("SPI bus out of range %1.%2").arg(bus).arg(select));
        return 0;
    }
    if (select >= RTESPIDRIVER_MAX_SELECT) {
        RTeError(m_logTag, QString("SPI select out of range %1.%2").arg(bus).arg(select));
        return 0;
    }
    return &(m_channels[bus][select]);
}
