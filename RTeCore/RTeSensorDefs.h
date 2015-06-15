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

#ifndef _RTESENSORDEFS_H
#define _RTESENSORDEFS_H

#include "RTeMath.h"

//  All timestamps are uS since epoch

//  Pressure sensor data

class RTeSensorPressureData
{
public:
    RTEFLOAT m_pressure;                                    // in hPa
    qint64 m_timestamp;
};


//  Temperature sensor data

class RTeSensorTemperatureData
{
public:
    RTEFLOAT m_temperature;                                 // in degrees C
    qint64 m_timestamp;
};

//  Gyro sensor data

class RTeSensorGyroData
{
public:
    RTeVector3 m_gyro;                                      // in radians/s
    qint64 m_timestamp;
};

//  Accelerometer sensor data

class RTeSensorAccelData
{
public:
    RTeVector3 m_accel;                                     // in g
    qint64 m_timestamp;
};

//  Magnetometer sensor data

class RTeSensorMagData
{
public:
    RTeVector3 m_mag;                                       // in uT
    qint64 m_timestamp;
};

//  ADC sensor data

class RTeSensorADCData
{
public:
    RTEFLOAT m_voltage;                                     // in volts
    int m_channel;                                  
    qint64 m_timestamp;
};


#endif // _RTESENSORDEFS_H

