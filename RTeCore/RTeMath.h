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

#ifndef _RTEMATH_H_
#define _RTEMATH_H_

#include <inttypes.h>
#include <qstring.h>

//  The fundamental float type

#ifdef RTEMATH_USE_DOUBLE
typedef double RTEFLOAT;
#else
typedef float RTEFLOAT;
#endif

//  Useful constants

#define	RTEMATH_PI					3.1415926535
#define	RTEMATH_DEGREE_TO_RAD		(RTEMATH_PI / 180.0)
#define	RTEMATH_RAD_TO_DEGREE		(180.0 / RTEMATH_PI)

#define RTEMATH_CLOCKS_PER_SEC      1000000

class RTeVector3;
class RTeMatrix4x4;
class RTeQuaternion;

class RTeMath
{
public:
    // convenient display routines

    static const QString& displayRadians(const QString& label, RTeVector3& vec);
    static const QString& displayDegrees(const QString& label, RTeVector3& vec);
    static const QString& display(const QString& label, RTeQuaternion& quat);
    static const QString& display(const QString& label, RTeMatrix4x4& mat);

    //  currentUSecsSinceEpoch() is the source of all timestamps and
    //  is the number of uS sonce the standard epoch

    static uint64_t currentUSecsSinceEpoch();

    //  poseFromAccelMag generates pose Euler angles from measured settings

    static RTeVector3 poseFromAccelMag(const RTeVector3& accel, const RTeVector3& mag);

    //  Takes signed 16 bit data from a char array and converts it to a vector of scaled RTEFLOATs

    static void convertToVector(unsigned char *rawData, RTeVector3& vec, RTEFLOAT scale, bool bigEndian);

    //  Takes a pressure in hPa and returns height above sea level in meters

    static RTEFLOAT convertPressureToHeight(RTEFLOAT pressure, RTEFLOAT staticPressure = 1013.25);

private:
    static QString m_string;                                // for the display routines
};


class RTeVector3
{
public:
    RTeVector3();
    RTeVector3(RTEFLOAT x, RTEFLOAT y, RTEFLOAT z);

    const RTeVector3&  operator +=(RTeVector3& vec);
    const RTeVector3&  operator -=(RTeVector3& vec);

    RTeVector3& operator =(const RTeVector3& vec);

    RTEFLOAT length();
    void normalize();
    void zero();
    bool isZero();
    QString display(const QString &label);
 
    static float dotProduct(const RTeVector3& a, const RTeVector3& b);
    static void crossProduct(const RTeVector3& a, const RTeVector3& b, RTeVector3& d);

    void accelToEuler(RTeVector3& rollPitchYaw) const;
    void accelToQuaternion(RTeQuaternion& qPose) const;

    inline RTEFLOAT x() const { return m_data[0]; }
    inline RTEFLOAT y() const { return m_data[1]; }
    inline RTEFLOAT z() const { return m_data[2]; }
    inline RTEFLOAT data(const int i) const { return m_data[i]; }

    inline void setX(const RTEFLOAT val) { m_data[0] = val; }
    inline void setY(const RTEFLOAT val) { m_data[1] = val; }
    inline void setZ(const RTEFLOAT val) { m_data[2] = val; }
    inline void setData(const int i, RTEFLOAT val) { m_data[i] = val; }

private:
    RTEFLOAT m_data[3];
};


class RTeQuaternion
{
public:
    RTeQuaternion();
    RTeQuaternion(RTEFLOAT scalar, RTEFLOAT x, RTEFLOAT y, RTEFLOAT z);

    RTeQuaternion& operator +=(const RTeQuaternion& quat);
    RTeQuaternion& operator -=(const RTeQuaternion& quat);
    RTeQuaternion& operator *=(const RTeQuaternion& qb);
    RTeQuaternion& operator *=(const RTEFLOAT val);
    RTeQuaternion& operator -=(const RTEFLOAT val);

    RTeQuaternion& operator =(const RTeQuaternion& quat);
    const RTeQuaternion operator *(const RTeQuaternion& qb) const;
    const RTeQuaternion operator *(const RTEFLOAT val) const;
    const RTeQuaternion operator -(const RTeQuaternion& qb) const;
    const RTeQuaternion operator -(const RTEFLOAT val) const;

    void normalize();
    void toEuler(RTeVector3& vec);
    void fromEuler(RTeVector3& vec);
    RTeQuaternion conjugate() const;
    void toAngleVector(RTEFLOAT& angle, RTeVector3& vec);
    void fromAngleVector(const RTEFLOAT& angle, const RTeVector3& vec);

    void zero();
    const char *display();

    inline RTEFLOAT scalar() const { return m_data[0]; }
    inline RTEFLOAT x() const { return m_data[1]; }
    inline RTEFLOAT y() const { return m_data[2]; }
    inline RTEFLOAT z() const { return m_data[3]; }
    inline RTEFLOAT data(const int i) const { return m_data[i]; }

    inline void setScalar(const RTEFLOAT val) { m_data[0] = val; }
    inline void setX(const RTEFLOAT val) { m_data[1] = val; }
    inline void setY(const RTEFLOAT val) { m_data[2] = val; }
    inline void setZ(const RTEFLOAT val) { m_data[3] = val; }
    inline void setData(const int i, RTEFLOAT val) { m_data[i] = val; }

private:
    RTEFLOAT m_data[4];
};

class RTeMatrix4x4
{
public:
    RTeMatrix4x4();

    RTeMatrix4x4& operator +=(const RTeMatrix4x4& mat);
    RTeMatrix4x4& operator -=(const RTeMatrix4x4& mat);
    RTeMatrix4x4& operator *=(const RTEFLOAT val);

    RTeMatrix4x4& operator =(const RTeMatrix4x4& vec);
    const RTeQuaternion operator *(const RTeQuaternion& q) const;
    const RTeMatrix4x4 operator *(const RTEFLOAT val) const;
    const RTeMatrix4x4 operator *(const RTeMatrix4x4& mat) const;
    const RTeMatrix4x4 operator +(const RTeMatrix4x4& mat) const;

    inline RTEFLOAT val(int row, int col) const { return m_data[row][col]; }
    inline void setVal(int row, int col, RTEFLOAT val) { m_data[row][col] = val; }
    void fill(RTEFLOAT val);
    void setToIdentity();

    RTeMatrix4x4 inverted();
    RTeMatrix4x4 transposed();

private:
    RTEFLOAT m_data[4][4];                                   // row, column

    RTEFLOAT matDet();
    RTEFLOAT matMinor(const int row, const int col);
};

#endif /* _RTEMATH_H_ */
