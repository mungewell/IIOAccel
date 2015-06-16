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

#include "RTeMath.h"
#include <math.h>
#include <sys/time.h>

//  Strings are put here. So the display functions are no re-entrant!

QString RTeMath::m_string;

uint64_t RTeMath::currentUSecsSinceEpoch()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
}

const QString& RTeMath::displayRadians(const QString& label, RTeVector3& vec)
{
    m_string.sprintf("%s: x:%f, y:%f, z:%f\n", qPrintable(label), vec.x(), vec.y(), vec.z());
    return m_string;
}

const QString& RTeMath::displayDegrees(const QString& label, RTeVector3& vec)
{
    m_string.sprintf("%s: roll:%f, pitch:%f, yaw:%f", qPrintable(label), vec.x() * RTEMATH_RAD_TO_DEGREE,
            vec.y() * RTEMATH_RAD_TO_DEGREE, vec.z() * RTEMATH_RAD_TO_DEGREE);
    return m_string;
}

const QString& RTeMath::display(const QString& label, RTeQuaternion& quat)
{
    m_string.sprintf("%s: scalar: %f, x:%f, y:%f, z:%f\n", qPrintable(label), quat.scalar(), quat.x(), quat.y(), quat.z());
    return m_string;
}

const QString& RTeMath::display(const QString& label, RTeMatrix4x4& mat)
{
    m_string.sprintf("%s(0): %f %f %f %f\n%s(1): %f %f %f %f\n%s(2): %f %f %f %f\n%s(3): %f %f %f %f\n",
            qPrintable(label), mat.val(0,0), mat.val(0,1), mat.val(0,2), mat.val(0,3),
            qPrintable(label), mat.val(1,0), mat.val(1,1), mat.val(1,2), mat.val(1,3),
            qPrintable(label), mat.val(2,0), mat.val(2,1), mat.val(2,2), mat.val(2,3),
            qPrintable(label), mat.val(3,0), mat.val(3,1), mat.val(3,2), mat.val(3,3));
    return m_string;
}

//  convertPressureToHeight() - the conversion uses the formula:
//
//  h = (T0 / L0) * ((p / P0)**(-(R* * L0) / (g0 * M)) - 1)
//
//  where:
//  h  = height above sea level
//  T0 = standard temperature at sea level = 288.15
//  L0 = standard temperatur elapse rate = -0.0065
//  p  = measured pressure
//  P0 = static pressure = 1013.25 (but can be overridden)
//  g0 = gravitational acceleration = 9.80665
//  M  = mloecular mass of earth's air = 0.0289644
//  R* = universal gas constant = 8.31432
//
//  Given the constants, this works out to:
//
//  h = 44330.8 * (1 - (p / P0)**0.190263)

RTEFLOAT RTeMath::convertPressureToHeight(RTEFLOAT pressure, RTEFLOAT staticPressure)
{
    return 44330.8 * (1 - pow(pressure / staticPressure, (RTEFLOAT)0.190263));
}


RTeVector3 RTeMath::poseFromAccelMag(const RTeVector3& accel, const RTeVector3& mag)
{
    RTeVector3 result;
    RTeQuaternion m;
    RTeQuaternion q;

    accel.accelToEuler(result);

//  q.fromEuler(result);
//  since result.z() is always 0, this can be optimized a little

    RTEFLOAT cosX2 = cos(result.x() / 2.0f);
    RTEFLOAT sinX2 = sin(result.x() / 2.0f);
    RTEFLOAT cosY2 = cos(result.y() / 2.0f);
    RTEFLOAT sinY2 = sin(result.y() / 2.0f);

    q.setScalar(cosX2 * cosY2);
    q.setX(sinX2 * cosY2);
    q.setY(cosX2 * sinY2);
    q.setZ(-sinX2 * sinY2);
//    q.normalize();

    m.setScalar(0);
    m.setX(mag.x());
    m.setY(mag.y());
    m.setZ(mag.z());

    m = q * m * q.conjugate();
    result.setZ(-atan2(m.y(), m.x()));
    return result;
}

void RTeMath::convertToVector(unsigned char *rawData, RTeVector3& vec, RTEFLOAT scale, bool bigEndian)
{
    if (bigEndian) {
        vec.setX((RTEFLOAT)((int16_t)(((uint16_t)rawData[0] << 8) | (uint16_t)rawData[1])) * scale);
        vec.setY((RTEFLOAT)((int16_t)(((uint16_t)rawData[2] << 8) | (uint16_t)rawData[3])) * scale);
        vec.setZ((RTEFLOAT)((int16_t)(((uint16_t)rawData[4] << 8) | (uint16_t)rawData[5])) * scale);
    } else {
        vec.setX((RTEFLOAT)((int16_t)(((uint16_t)rawData[1] << 8) | (uint16_t)rawData[0])) * scale);
        vec.setY((RTEFLOAT)((int16_t)(((uint16_t)rawData[3] << 8) | (uint16_t)rawData[2])) * scale);
        vec.setZ((RTEFLOAT)((int16_t)(((uint16_t)rawData[5] << 8) | (uint16_t)rawData[4])) * scale);
     }
}



//----------------------------------------------------------
//
//  The RTeVector3 class

RTeVector3::RTeVector3()
{
    zero();
}

RTeVector3::RTeVector3(RTEFLOAT x, RTEFLOAT y, RTEFLOAT z)
{
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
}

RTeVector3& RTeVector3::operator =(const RTeVector3& vec)
{
    if (this == &vec)
        return *this;

    m_data[0] = vec.m_data[0];
    m_data[1] = vec.m_data[1];
    m_data[2] = vec.m_data[2];

    return *this;
}


const RTeVector3& RTeVector3::operator +=(RTeVector3& vec)
{
    for (int i = 0; i < 3; i++)
        m_data[i] += vec.m_data[i];
    return *this;
}

const RTeVector3& RTeVector3::operator -=(RTeVector3& vec)
{
    for (int i = 0; i < 3; i++)
        m_data[i] -= vec.m_data[i];
    return *this;
}

void RTeVector3::zero()
{
    for (int i = 0; i < 3; i++)
        m_data[i] = 0;
}

bool RTeVector3::isZero()
{
    return (m_data[0] == 0) && (m_data[1] == 0) && (m_data[2] == 0);
}

RTEFLOAT RTeVector3::dotProduct(const RTeVector3& a, const RTeVector3& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

void RTeVector3::crossProduct(const RTeVector3& a, const RTeVector3& b, RTeVector3& d)
{
    d.setX(a.y() * b.z() - a.z() * b.y());
    d.setY(a.z() * b.x() - a.x() * b.z());
    d.setZ(a.x() * b.y() - a.y() * b.x());
}


void RTeVector3::accelToEuler(RTeVector3& rollPitchYaw) const
{
    RTeVector3 normAccel = *this;

    normAccel.normalize();

    rollPitchYaw.setX(atan2(normAccel.y(), normAccel.z()));
    rollPitchYaw.setY(-atan2(normAccel.x(), sqrt(normAccel.y() * normAccel.y() + normAccel.z() * normAccel.z())));
    rollPitchYaw.setZ(0);
}


void RTeVector3::accelToQuaternion(RTeQuaternion& qPose) const
{
    RTeVector3 normAccel = *this;
    RTeVector3 vec;
    RTeVector3 z(0, 0, 1.0);

    normAccel.normalize();

    RTEFLOAT angle = acos(RTeVector3::dotProduct(z, normAccel));
    RTeVector3::crossProduct(normAccel, z, vec);
    vec.normalize();

    qPose.fromAngleVector(angle, vec);
}


void RTeVector3::normalize()
{
    RTEFLOAT length = sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] +
            m_data[2] * m_data[2]);

    if (length == 0)
        return;

    m_data[0] /= length;
    m_data[1] /= length;
    m_data[2] /= length;
}

RTEFLOAT RTeVector3::length()
{
    return sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] +
            m_data[2] * m_data[2]);
}

QString RTeVector3::display(const QString& label)
{
    QString result;

    result.sprintf("%s: x:%f, y:%f, z:%f", qPrintable(label), m_data[0], m_data[1], m_data[2]);
    return result;
}


//----------------------------------------------------------
//
//  The RTeQuaternion class

RTeQuaternion::RTeQuaternion()
{
    zero();
}

RTeQuaternion::RTeQuaternion(RTEFLOAT scalar, RTEFLOAT x, RTEFLOAT y, RTEFLOAT z)
{
    m_data[0] = scalar;
    m_data[1] = x;
    m_data[2] = y;
    m_data[3] = z;
}

RTeQuaternion& RTeQuaternion::operator =(const RTeQuaternion& quat)
{
    if (this == &quat)
        return *this;

    m_data[0] = quat.m_data[0];
    m_data[1] = quat.m_data[1];
    m_data[2] = quat.m_data[2];
    m_data[3] = quat.m_data[3];

    return *this;
}



RTeQuaternion& RTeQuaternion::operator +=(const RTeQuaternion& quat)
{
    for (int i = 0; i < 4; i++)
        m_data[i] += quat.m_data[i];
    return *this;
}

RTeQuaternion& RTeQuaternion::operator -=(const RTeQuaternion& quat)
{
    for (int i = 0; i < 4; i++)
        m_data[i] -= quat.m_data[i];
    return *this;
}

RTeQuaternion& RTeQuaternion::operator -=(const RTEFLOAT val)
{
    for (int i = 0; i < 4; i++)
        m_data[i] -= val;
    return *this;
}

RTeQuaternion& RTeQuaternion::operator *=(const RTeQuaternion& qb)
{
    RTeVector3 va;
    RTeVector3 vb;
    RTEFLOAT dotAB;
    RTeVector3 crossAB;

    va.setX(m_data[1]);
    va.setY(m_data[2]);
    va.setZ(m_data[3]);

    vb.setX(qb.x());
    vb.setY(qb.y());
    vb.setZ(qb.z());

    dotAB = RTeVector3::dotProduct(va, vb);
    RTeVector3::crossProduct(va, vb, crossAB);
    RTEFLOAT myScalar = m_data[0];

    m_data[0] = myScalar * qb.scalar() - dotAB;
    m_data[1] = myScalar * vb.x() + qb.scalar() * va.x() + crossAB.x();
    m_data[2] = myScalar * vb.y() + qb.scalar() * va.y() + crossAB.y();
    m_data[3] = myScalar * vb.z() + qb.scalar() * va.z() + crossAB.z();

    return *this;
}


RTeQuaternion& RTeQuaternion::operator *=(const RTEFLOAT val)
{
    m_data[0] *= val;
    m_data[1] *= val;
    m_data[2] *= val;
    m_data[3] *= val;

    return *this;
}


const RTeQuaternion RTeQuaternion::operator *(const RTeQuaternion& qb) const
{
    RTeQuaternion result = *this;
    result *= qb;
    return result;
}

const RTeQuaternion RTeQuaternion::operator *(const RTEFLOAT val) const
{
    RTeQuaternion result = *this;
    result *= val;
    return result;
}


const RTeQuaternion RTeQuaternion::operator -(const RTeQuaternion& qb) const
{
    RTeQuaternion result = *this;
    result -= qb;
    return result;
}

const RTeQuaternion RTeQuaternion::operator -(const RTEFLOAT val) const
{
    RTeQuaternion result = *this;
    result -= val;
    return result;
}


void RTeQuaternion::zero()
{
    for (int i = 0; i < 4; i++)
        m_data[i] = 0;
}

void RTeQuaternion::normalize()
{
    RTEFLOAT length = sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] +
            m_data[2] * m_data[2] + m_data[3] * m_data[3]);

    if ((length == 0) || (length == 1))
        return;

    m_data[0] /= length;
    m_data[1] /= length;
    m_data[2] /= length;
    m_data[3] /= length;
}

void RTeQuaternion::toEuler(RTeVector3& vec)
{
    vec.setX(atan2(2.0 * (m_data[2] * m_data[3] + m_data[0] * m_data[1]),
            1 - 2.0 * (m_data[1] * m_data[1] + m_data[2] * m_data[2])));

    vec.setY(asin(2.0 * (m_data[0] * m_data[2] - m_data[1] * m_data[3])));

    vec.setZ(atan2(2.0 * (m_data[1] * m_data[2] + m_data[0] * m_data[3]),
            1 - 2.0 * (m_data[2] * m_data[2] + m_data[3] * m_data[3])));
}

void RTeQuaternion::fromEuler(RTeVector3& vec)
{
    RTEFLOAT cosX2 = cos(vec.x() / 2.0f);
    RTEFLOAT sinX2 = sin(vec.x() / 2.0f);
    RTEFLOAT cosY2 = cos(vec.y() / 2.0f);
    RTEFLOAT sinY2 = sin(vec.y() / 2.0f);
    RTEFLOAT cosZ2 = cos(vec.z() / 2.0f);
    RTEFLOAT sinZ2 = sin(vec.z() / 2.0f);

    m_data[0] = cosX2 * cosY2 * cosZ2 + sinX2 * sinY2 * sinZ2;
    m_data[1] = sinX2 * cosY2 * cosZ2 - cosX2 * sinY2 * sinZ2;
    m_data[2] = cosX2 * sinY2 * cosZ2 + sinX2 * cosY2 * sinZ2;
    m_data[3] = cosX2 * cosY2 * sinZ2 - sinX2 * sinY2 * cosZ2;
    normalize();
}

RTeQuaternion RTeQuaternion::conjugate() const
{
    RTeQuaternion q;
    q.setScalar(m_data[0]);
    q.setX(-m_data[1]);
    q.setY(-m_data[2]);
    q.setZ(-m_data[3]);
    return q;
}

void RTeQuaternion::toAngleVector(RTEFLOAT& angle, RTeVector3& vec)
{
    RTEFLOAT halfTheta;
    RTEFLOAT sinHalfTheta;

    halfTheta = acos(m_data[0]);
    sinHalfTheta = sin(halfTheta);

    if (sinHalfTheta == 0) {
        vec.setX(1.0);
        vec.setY(0);
        vec.setZ(0);
    } else {
        vec.setX(m_data[1] / sinHalfTheta);
        vec.setY(m_data[1] / sinHalfTheta);
        vec.setZ(m_data[1] / sinHalfTheta);
    }
    angle = 2.0 * halfTheta;
}

void RTeQuaternion::fromAngleVector(const RTEFLOAT& angle, const RTeVector3& vec)
{
    RTEFLOAT sinHalfTheta = sin(angle / 2.0);
    m_data[0] = cos(angle / 2.0);
    m_data[1] = vec.x() * sinHalfTheta;
    m_data[2] = vec.y() * sinHalfTheta;
    m_data[3] = vec.z() * sinHalfTheta;
}



//----------------------------------------------------------
//
//  The RTeMatrix4x4 class

RTeMatrix4x4::RTeMatrix4x4()
{
    fill(0);
}

RTeMatrix4x4& RTeMatrix4x4::operator =(const RTeMatrix4x4& mat)
{
    if (this == &mat)
        return *this;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m_data[row][col] = mat.m_data[row][col];

    return *this;
}


void RTeMatrix4x4::fill(RTEFLOAT val)
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m_data[row][col] = val;
}


RTeMatrix4x4& RTeMatrix4x4::operator +=(const RTeMatrix4x4& mat)
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m_data[row][col] += mat.m_data[row][col];

    return *this;
}

RTeMatrix4x4& RTeMatrix4x4::operator -=(const RTeMatrix4x4& mat)
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m_data[row][col] -= mat.m_data[row][col];

    return *this;
}

RTeMatrix4x4& RTeMatrix4x4::operator *=(const RTEFLOAT val)
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m_data[row][col] *= val;

    return *this;
}

const RTeMatrix4x4 RTeMatrix4x4::operator +(const RTeMatrix4x4& mat) const
{
    RTeMatrix4x4 result = *this;
    result += mat;
    return result;
}

const RTeMatrix4x4 RTeMatrix4x4::operator *(const RTEFLOAT val) const
{
    RTeMatrix4x4 result = *this;
    result *= val;
    return result;
}


const RTeMatrix4x4 RTeMatrix4x4::operator *(const RTeMatrix4x4& mat) const
{
    RTeMatrix4x4 res;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            res.m_data[row][col] =
                    m_data[row][0] * mat.m_data[0][col] +
                    m_data[row][1] * mat.m_data[1][col] +
                    m_data[row][2] * mat.m_data[2][col] +
                    m_data[row][3] * mat.m_data[3][col];

    return res;
}


const RTeQuaternion RTeMatrix4x4::operator *(const RTeQuaternion& q) const
{
    RTeQuaternion res;

    res.setScalar(m_data[0][0] * q.scalar() + m_data[0][1] * q.x() + m_data[0][2] * q.y() + m_data[0][3] * q.z());
    res.setX(m_data[1][0] * q.scalar() + m_data[1][1] * q.x() + m_data[1][2] * q.y() + m_data[1][3] * q.z());
    res.setY(m_data[2][0] * q.scalar() + m_data[2][1] * q.x() + m_data[2][2] * q.y() + m_data[2][3] * q.z());
    res.setZ(m_data[3][0] * q.scalar() + m_data[3][1] * q.x() + m_data[3][2] * q.y() + m_data[3][3] * q.z());

    return res;
}

void RTeMatrix4x4::setToIdentity()
{
    fill(0);
    m_data[0][0] = 1;
    m_data[1][1] = 1;
    m_data[2][2] = 1;
    m_data[3][3] = 1;
}

RTeMatrix4x4 RTeMatrix4x4::transposed()
{
    RTeMatrix4x4 res;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            res.m_data[col][row] = m_data[row][col];
    return res;
}

//  Note:
//  The matrix inversion code here was strongly influenced by some old code I found
//  but I have no idea where it came from. Apologies to whoever wrote it originally!
//  If it's you, please let me know at info@richards-tech.com so I can credit it correctly.

RTeMatrix4x4 RTeMatrix4x4::inverted()
{
    RTeMatrix4x4 res;

    RTEFLOAT det = matDet();

    if (det == 0) {
        res.setToIdentity();
        return res;
    }

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if ((row + col) & 1)
                res.m_data[col][row] = -matMinor(row, col) / det;
            else
                res.m_data[col][row] = matMinor(row, col) / det;
        }
    }

    return res;
}

RTEFLOAT RTeMatrix4x4::matDet()
{
    RTEFLOAT det = 0;

    det += m_data[0][0] * matMinor(0, 0);
    det -= m_data[0][1] * matMinor(0, 1);
    det += m_data[0][2] * matMinor(0, 2);
    det -= m_data[0][3] * matMinor(0, 3);
    return det;
}

RTEFLOAT RTeMatrix4x4::matMinor(const int row, const int col)
{
    static int map[] = {1, 2, 3, 0, 2, 3, 0, 1, 3, 0, 1, 2};

    int *rc;
    int *cc;
    RTEFLOAT res = 0;

    rc = map + row * 3;
    cc = map + col * 3;

    res += m_data[rc[0]][cc[0]] * m_data[rc[1]][cc[1]] * m_data[rc[2]][cc[2]];
    res -= m_data[rc[0]][cc[0]] * m_data[rc[1]][cc[2]] * m_data[rc[2]][cc[1]];
    res -= m_data[rc[0]][cc[1]] * m_data[rc[1]][cc[0]] * m_data[rc[2]][cc[2]];
    res += m_data[rc[0]][cc[1]] * m_data[rc[1]][cc[2]] * m_data[rc[2]][cc[0]];
    res += m_data[rc[0]][cc[2]] * m_data[rc[1]][cc[0]] * m_data[rc[2]][cc[1]];
    res -= m_data[rc[0]][cc[2]] * m_data[rc[1]][cc[1]] * m_data[rc[2]][cc[0]];
    return res;
}

