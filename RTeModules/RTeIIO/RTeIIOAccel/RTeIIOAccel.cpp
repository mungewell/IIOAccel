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


#include "RTeIIOAccel.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

RTeIIOAccel::RTeIIOAccel() : RTeIIO()
{
    m_dataNames[0] = "in_accel_x_raw";
    m_dataNames[1] = "in_accel_y_raw";
    m_dataNames[2] = "in_accel_z_raw";
    m_count = 0;
    m_fp = -1;
    m_timer = -1;
    m_sampleRate = 2;
}


void RTeIIOAccel::initModule()
{
    int rate;

    if (!setValue("buffer/enable", 0)) {
        RTeError(getModuleName(), "Failed to disable buffer");
    }

    switch (m_sampleRate) {
    case 0: rate = 1; break;
    case 1: rate = 10; break;
    case 2: rate = 25; break;
    case 3: rate = 50; break;
    case 4: rate = 100; break;
    case 5: rate = 200; break;
    case 6: rate = 400; break;
    case 7: rate = 1600; break;
    default: rate = 25; break;
    }

    setValue("sampling_frequency", rate);

    if (m_useBuffer) {
        if (!setValue("scan_elements/in_accel_x_en", 1)) {
            RTeError(getModuleName(), "Failed enable x axis");
        }
        if (!setValue("scan_elements/in_accel_y_en", 1)) {
            RTeError(getModuleName(), "Failed enable y axis");
        }
        if (!setValue("scan_elements/in_accel_z_en", 1)) {
            RTeError(getModuleName(), "Failed enable z axis");
        }
        if (!setValue("scan_elements/in_timestamp_en", 1)) {
            RTeError(getModuleName(), "Failed enable timestamp");
        }
        if (!setValue("buffer/length", 128)) {
            RTeError(getModuleName(), "Failed to set buffer length");
        }
        if (!setValue("buffer/enable", 1)) {
            RTeError(getModuleName(), "Failed to enable buffer");
        }

        m_fp = open(qPrintable(m_deviceBuffer), O_RDONLY | O_NONBLOCK);

        if (m_fp == -1) {
            RTeError(getModuleName(), QString("Failed to open iio device ") + m_deviceBuffer);
            return;
        }
        m_bytesLeft = sizeof(RTEIIOACCEL_DATA);
        m_bytesGot = 0;
    }

    m_timer = startTimer(2);
    m_startTime = RTeMath::currentUSecsSinceEpoch();
}

void RTeIIOAccel::stopModule()
{
    if (m_timer != -1)
        killTimer(m_timer);
    if (m_useBuffer) {
        if (m_fp != -1)
            close(m_fp);
        if (!setValue("buffer/enable", 0)) {
            RTeError(getModuleName(), "Failed to disable buffer");
        }
    }
}

void RTeIIOAccel::timerEvent(QTimerEvent *)
{
    RTEIIOACCEL_DATA rawData;
    RTeSensorAccelData accelData;

    if (!m_useBuffer) {
        QFile dataFile;
        for (int i = 0; i < 3; i++) {
            dataFile.setFileName(m_devicePath + m_dataNames[i]);
            dataFile.open(QIODevice::ReadOnly);

            QString value = dataFile.readAll();
            accelData.m_accel.setData(i, value.toDouble() / 1000.0);

            dataFile.close();
        }
        m_count++;

        if ((RTeMath::currentUSecsSinceEpoch() - m_startTime) >= 1000000) {
            qDebug() << accelData.m_accel.display("Accel: ");
            qDebug() << "Sample rate: " << m_count;
            m_count = 0;
            m_startTime = RTeMath::currentUSecsSinceEpoch();
        }
    } else {
        while (1) {
            int count = read(m_fp, (char *)(&rawData) + m_bytesGot, m_bytesLeft);

            if (count <= 0) {
                if (errno != EAGAIN)
                    RTeError(getModuleName(), QString("Read failed %1").arg(errno));
                return;
            }

            m_bytesGot += count;
            m_bytesLeft -=count;

            if (m_bytesLeft <= 0) {
                m_count++;
                m_bytesGot = 0;
                m_bytesLeft = sizeof(RTEIIOACCEL_DATA);
                accelData.m_accel.setX((RTEFLOAT)rawData.x / (RTEFLOAT)16384.0);
                accelData.m_accel.setY((RTEFLOAT)rawData.y / (RTEFLOAT)16384.0);
                accelData.m_accel.setZ((RTEFLOAT)rawData.z / (RTEFLOAT)16384.0);
                accelData.m_timestamp = rawData.timestamp / 1000;
                emit newAccelSample(this, &accelData);

                if ((RTeMath::currentUSecsSinceEpoch() - m_startTime) >= 1000000) {
                    RTeDebug(getModuleName(), QString("Accel sample rate: %1").arg(m_count));
                    m_count = 0;
                    m_startTime = RTeMath::currentUSecsSinceEpoch();
                }
            }
        }
    }
}

