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

#ifndef _RTEVIDEOAUDIO_H
#define _RTEVIDEOAUDIO_H

#include <qbytearray.h>

//  video frame formats

#define RTE_VIDEO_FRAME_JPEG            0
#define RTE_VIDEO_FRAME_RGB             1

//  video frame param values

#define RTE_VIDEO_PARAM_NOOP            0                   // indicates a filler record
#define RTE_VIDEO_PARAM_REFRESH         1                   // indicates a refresh MJPEG frame
#define RTE_VIDEO_PARAM_NORMAL          2                   // indicates a normal record
#define RTE_VIDEO_PARAM_PREROLL         3                   // indicates a preroll frame
#define RTE_VIDEO_PARAM_POSTROLL        4                   // indicates a postroll frame

class RTeVideoFrame
{
public:
    RTeVideoFrame();
    QByteArray getFrameAs(int format);
    void setFrameAsRGB(QByteArray jpeg);
    void setFrameAsJpeg(QByteArray rgb);

#ifdef RTEMBEDDED_LANGUAGE_PYTHON
    void *getPython(bool valid);
#endif

    QByteArray m_frame;
    int m_format;
    int m_width;
    int m_height;
    int m_framerate;
    int m_param;
    qint64 m_timestamp;
};

//  Compressed video formats

#define RTE_COMPRESSED_VIDEO_FORMAT_H264    0               // H.264 segment

class RTeCompressedVideo
{
public:
    QByteArray m_video;
    int m_format;
    int m_width;
    int m_height;
    int m_framerate;
    int m_compressedRate;
    qint64 m_timestamp;
};

//  Audio frames are always pcm

//  Audio sample size options

#define RTE_AUDIO_SAMPLE_SIZE_8         0
#define RTE_AUDIO_SAMPLE_SIZE_16        1
#define RTE_AUDIO_SAMPLE_SIZE_24        2
#define RTE_AUDIO_SAMPLE_SIZE_32        3

//  Audio rate options

#define RTE_AUDIO_SAMPLE_RATE_8000      0
#define RTE_AUDIO_SAMPLE_RATE_16000     1
#define RTE_AUDIO_SAMPLE_RATE_44100     2
#define RTE_AUDIO_SAMPLE_RATE_48000     3

//  Audio channel options

#define RTE_AUDIO_CHANNELS_1            0
#define RTE_AUDIO_CHANNELS_2            1

class RTeAudioFrame
{
public:
    QByteArray m_frame;
    int m_sampleSize;
    int m_sampleRate;
    int m_channels;
    qint64 m_timestamp;
};

#endif // _RTEVIDEOAUDIO_H

