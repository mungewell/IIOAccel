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

#ifndef _RTESYNTRONETROBOTDEFS_H
#define _RTESYNTRONETROBOTDEFS_H

#include "SyntroUtils.h"

//  SyntroNet Robotics defs

#define RTESYNTRONET_DRIVE_THROTTLE_RANGE        100.0f
#define RTESYNTRONET_DRIVE_STEER_RANGE           90.0f

typedef struct
{
    SYNTRO_UC8 timestamp;                                   // this is the timestamp of the sample with microsecond resolution
    float steer;                                            // steering angle (-90.0 to 90.0)
    float throttle;                                         // throttle position (0.0 to 100.0)
    float brake;                                            // brake position (0 to 100.0)
    float clutch;                                           // clutch position (0 to 100.0)
    unsigned char active;                                   // if false, data should be ignored
    unsigned char gear;                                     // gear number (0 = neutral, 0xff = reverse)
    unsigned char spare0;
    unsigned char spare1;
} RTEROBOT_SYNTRONET_DRIVE_DATA;

typedef struct
{
    SYNTRO_UC8 timestamp;                                   // this is the timestamp of the sample with microsecond resolution
    float fusionQPose[4];                                   // the IMU data
    unsigned char active;                                   // if drive is active
    unsigned char spare0;
    unsigned char spare1;
    unsigned char spare2;
} RTEROBOT_SYNTRONET_HEAD_TRACK_DATA;

typedef struct
{
    SYNTRO_UC8 timestamp;                                   // this is the timestamp of the data with microsecond resolution

    // GPS section

    unsigned char gpsValid;								    // true if valid GPS data

    unsigned char gpsDateYear;								// cracked date fields
    unsigned char gpsDateMonth;
    unsigned char gpsDateDay;

    unsigned char gpsTimeHours;                             // cracked time fields (UTC)
    unsigned char gpsTimeMinutes;
    unsigned char gpsTimeSeconds;
    unsigned char gpsTimeDecimalSeconds;

    float gpsLatitude;
    float gpsLongitude;

    float gpsSpeed;                                         // speed in MPH
    float gpsHeading;                                       // direction of speed

    //  Encoder section

    float encoderSpeed;

    //  Battery section

    float batteryVoltage;

    //  IMU section

    float fusionQPose[4];
    float gyro[3];
    float accel[3];
    float mag[3];

} RTEROBOT_SYNTRONET_TELEMETRY_DATA;

#endif // _RTESYNTRONETROBOTDEFS_H

