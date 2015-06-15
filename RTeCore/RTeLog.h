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

#ifndef RTELOG_H
#define RTELOG_H

#include <qdebug.h>
#include <qmutex.h>
#include <qstring.h>

#ifdef QT_NO_DEBUG
#define RTeDebug(m, x) 
#else
#define RTeDebug(m, x)                  RTeLog::addMessage(m, "Debug", x)
#endif
#define RTeInfo(m, x)                   RTeLog::addMessage(m, "Info", x)
#define RTeWarning(m, x)                RTeLog::addMessage(m, "Warn", x)
#define RTeError(m, x)                  RTeLog::addMessage(m, "Error", x)
#define RTeFatal(m, x)                  RTeLog::addMessage(m, "Fatal", x)

class RTeLog
{
public:
    static void addMessage(const QString& tag, const QString& type, const QString& desc);

private:
    static QMutex m_lock;
};

#endif // RTELOG_H
