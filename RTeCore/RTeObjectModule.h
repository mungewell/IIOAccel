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

#ifndef _RTEOBJECTMODULE_H_
#define _RTEOBJECTMODULE_H_

#include "RTeModule.h"

class RTeObjectModule : public RTeModule
{
    Q_OBJECT

public:
    RTeObjectModule(QObject *parent = NULL);

    //  resumeThread() is called when init is complete

    virtual void resumeThread() { initModule(); }

    //  exitThread is called to terminate and delete the thread

    virtual void exitThread() { stopModule(); }

protected:

    //  overrides that must be provided by the subclass

    virtual void initModule() = 0;                          // performs all module-specific initialization
    virtual void stopModule() = 0;                          // closes down module
};

#endif // _RTEOBJECTMODULE_H_
