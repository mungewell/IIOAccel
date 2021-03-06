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

#ifndef _RTEDRIVERDEFS_H
#define _RTEDRIVERDEFS_H

#include "RTeMath.h"

//  All timestamps are uS since epoch

//  PWM driver data

#define RTEDRIVER_PWM_MAX_VALUE         32767
#define RTEDRIVER_PWM_MIN_VALUE         -32767
#define RTEDRIVER_PWM_RANGE             32767

class RTeDriverPWMData
{
public:
    int m_value;                                            // -32767 to 32767
    int m_channel;
    qint64 m_timestamp;
};


#endif // _RTESENSORDEFS_H

