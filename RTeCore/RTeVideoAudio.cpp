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

#include "RTeVideoAudio.h"

#ifdef RTEMBEDDED_LANGUAGE_PYTHON
#include <Python.h>
#endif

#include <qimage.h>
#include <qbuffer.h>

RTeVideoFrame::RTeVideoFrame(){
    m_param = RTE_VIDEO_PARAM_NORMAL;
    m_width = m_height = m_framerate = 0;
    m_format = RTE_VIDEO_FRAME_RGB;
}

#ifdef RTEMBEDDED_LANGUAGE_PYTHON
void *RTeVideoFrame::getPython(bool valid)
{
    return Py_BuildValue("is#iiii", valid, m_frame.data(), m_frame.length(),
                                          m_format, m_width, m_height, m_framerate);
}
#endif

void RTeVideoFrame::setFrameAsRGB(QByteArray jpeg)
{
    QImage img;

    img.loadFromData(jpeg, "JPEG");
    img = img.convertToFormat(QImage::Format_RGB888);
    m_frame = QByteArray((const char *)img.bits(), img.byteCount());
}

void RTeVideoFrame::setFrameAsJpeg(QByteArray rgb)
{
    QImage img;

    img = QImage((const uchar *)rgb.data(), m_width, m_height, QImage::Format_RGB888);
    QByteArray jpegArray;
    QBuffer buffer(&jpegArray);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "JPEG", 70);
    m_frame = jpegArray;
}

QByteArray RTeVideoFrame::getFrameAs(int format)
{
    QImage img;
    QByteArray jpegArray;
    QBuffer buffer(&jpegArray);

    if (m_format == format)
        return m_frame;

    switch (format) {
    case RTE_VIDEO_FRAME_JPEG:
        img = QImage((const uchar *)m_frame.data(), m_width, m_height, QImage::Format_RGB888);

        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "JPEG", 70);
        return jpegArray;

    case RTE_VIDEO_FRAME_RGB:
        img.loadFromData(m_frame, "JPEG");
        img = img.convertToFormat(QImage::Format_RGB888);
        return QByteArray((const char *)img.bits(), img.byteCount());
    }
    return QByteArray();
}
