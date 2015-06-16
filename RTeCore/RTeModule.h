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

#ifndef _RTEMODULE_H_
#define _RTEMODULE_H_

#include <qobject.h>
#include <qstring.h>

//  convenience includes

#include "RTeLog.h"
#include "RTeMath.h"
#include "RTeVideoAudio.h"

class RTeModule : public QObject
{
    Q_OBJECT

public:
    RTeModule(QObject *parent = NULL);

    const QString& getModuleName() { return m_moduleName; }
    void setModuleName(const QString& name) { m_moduleName = name; }

    //  resumeThread() is called when init is complete

    virtual void resumeThread() = 0;

    //  exitThread is called to terminate and delete the thread

    virtual void exitThread() = 0;

protected:
    //  overrides that must be provided in subclasses

    virtual void initModule() = 0;                          // performs all module-specific initialization
    virtual void stopModule() = 0;                          // closes down module

    QString m_moduleName;

};

#endif // _RTEMODULE_H_
