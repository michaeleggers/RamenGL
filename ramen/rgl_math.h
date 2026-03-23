#ifndef RGL_MATH_H
#define RGL_MATH_H

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "rgl_defines.h"

struct Vec4f;

struct Vec3f
{
    float x;
    float y;
    float z;

    Vec3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vec3f(float s)
    {
        x = s;
        y = s;
        z = s;
    }

    Vec3f()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    Vec3f(const Vec4f& v4);

    const float operator[](const int index) const
    {
        assert(index < 3);
        return (&x)[ index ];
    }

    float& operator[](const int index)
    {
        assert(index < 3);
        return (&x)[ index ];
    }

    Vec3f operator-(const Vec3f& right) const
    {
        return Vec3f{ x - right.x, y - right.y, z - right.z };
    }

    const Vec3f operator-() const
    {
        return Vec3f{ -x, -y, -z };
    }

    Vec3f operator+(const Vec3f& right) const
    {
        return Vec3f{ x + right.x, y + right.y, z + right.z };
    }

    Vec3f operator/(float s) const
    {
        return Vec3f{ x / s, y / s, z / s };
    }

    Vec3f operator*(const Vec3f& right) const
    {
        return Vec3f{ x * right.x, y * right.y, z * right.z };
    }

    /* Vector * Scalar */
    Vec3f operator*(const float& s) const
    {
        return Vec3f{ s * x, s * y, s * z };
    }

    Vec3f& operator*=(const float& s)
    {
        x *= s;
        y *= s;
        z *= s;

        return *this;
    }
};

struct Vec4f
{
    float x;
    float y;
    float z;
    float w;

    Vec4f(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vec4f(float s)
    {
        x = s;
        y = s;
        z = s;
        w = s;
    }

    Vec4f()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    Vec4f(const Vec3f& v3)
    {
        x = v3.x;
        y = v3.y;
        z = v3.z;
        w = 1.0f;
    }

    Vec4f(const Vec3f& v3, const float& s)
    {
        x = v3.x;
        y = v3.y;
        z = v3.z;
        w = s;
    }

    const float& operator[](const int index) const
    {
        assert(index < 4);
        return (&x)[ index ];
    }

    float& operator[](const int index)
    {
        assert(index < 4);
        return (&x)[ index ];
    }

    Vec4f operator-(const Vec4f& right) const
    {
        return Vec4f{ x - right.x, y - right.y, z - right.z, w - right.w };
    }

    const Vec4f operator-() const
    {
        return Vec4f{ -x, -y, -z, -w };
    }

    Vec4f operator+(const Vec4f& right) const
    {
        return Vec4f{ x + right.x, y + right.y, z + right.z, w + right.w };
    }

    Vec4f operator/(float s) const
    {
        return Vec4f{ x / s, y / s, z / s, w / s };
    }

    Vec4f operator*(const Vec4f& right) const
    {
        return Vec4f{ x * right.x, y * right.y, z * right.z, w * right.w };
    }

    /* Vector * Scalar */
    Vec4f operator*(const float& s) const
    {
        return Vec4f{ s * x, s * y, s * z, s * w };
    }

    const char* ToString() const
    {
        static char buffer[ 128 ];
        sprintf(buffer, "[ %.2f, %.2f, %.2f, %.2f ]\n", x, y, z, w);
        return buffer;
    }
};

/* 4x4 Matrix with floats.
 * Stores elements in column major order.
 * Constructors also expect data in columns!
 */
struct Mat4f
{
    float e[ 4 ][ 4 ];

    Mat4f() = default;

    Mat4f(
        /* 1st col */
        const float& d00,
        const float& d01,
        const float& d02,
        const float& d03,
        /* 2nd col */
        const float& d10,
        const float& d11,
        const float& d12,
        const float& d13,
        /* 3rd col */
        const float& d20,
        const float& d21,
        const float& d22,
        const float& d23,
        /* 4th col */
        const float& d30,
        const float& d31,
        const float& d32,
        const float& d33)
    {
        e[ 0 ][ 0 ] = d00;
        e[ 0 ][ 1 ] = d01;
        e[ 0 ][ 2 ] = d02;
        e[ 0 ][ 3 ] = d03;

        e[ 1 ][ 0 ] = d10;
        e[ 1 ][ 1 ] = d11;
        e[ 1 ][ 2 ] = d12;
        e[ 1 ][ 3 ] = d13;

        e[ 2 ][ 0 ] = d20;
        e[ 2 ][ 1 ] = d21;
        e[ 2 ][ 2 ] = d22;
        e[ 2 ][ 3 ] = d23;

        e[ 3 ][ 0 ] = d30;
        e[ 3 ][ 1 ] = d31;
        e[ 3 ][ 2 ] = d32;
        e[ 3 ][ 3 ] = d33;
    }

    Mat4f(const Vec4f& col1, const Vec4f& col2, const Vec4f& col3, const Vec4f& col4)
    {
        e[ 0 ][ 0 ] = col1.x;
        e[ 0 ][ 1 ] = col1.y;
        e[ 0 ][ 2 ] = col1.z;
        e[ 0 ][ 3 ] = col1.w;
        e[ 1 ][ 0 ] = col2.x;
        e[ 1 ][ 1 ] = col2.y;
        e[ 1 ][ 2 ] = col2.z;
        e[ 1 ][ 3 ] = col2.w;
        e[ 2 ][ 0 ] = col3.x;
        e[ 2 ][ 1 ] = col3.y;
        e[ 2 ][ 2 ] = col3.z;
        e[ 2 ][ 3 ] = col3.w;
        e[ 3 ][ 0 ] = col4.x;
        e[ 3 ][ 1 ] = col4.y;
        e[ 3 ][ 2 ] = col4.z;
        e[ 3 ][ 3 ] = col4.w;
    }

    /* Follows mathematical notation:
     * i = row, j = column.
     */
    const float& operator()(int i, int j) const
    {
        return e[ j ][ i ];
    }

    float& operator()(int i, int j)
    {
        return e[ j ][ i ];
    }

    const Vec4f& operator[](int col) const
    {
        return reinterpret_cast<const Vec4f&>(e[ col ]);
    }

    Vec4f& operator[](int col)
    {
        return reinterpret_cast<Vec4f&>(e[ col ]);
    }

    void Transpose()
    {
        Mat4f result{};
        for ( int i = 0; i < 4; i++ )
        {
            const Vec4f& col = this->operator[](i);
            for ( int j = 0; j < 4; j++ )
            {
                result[ j ][ i ] = col[ j ];
            }
        }
        *this = result;
    }

    Mat4f operator*(const Mat4f& rhs)
    {
        Mat4f result{};
        for ( int row = 0; row < 4; row++ )
        {
            for ( int col = 0; col < 4; col++ )
            {
                float sum = 0.0f;
                for ( int k = 0; k < 4; k++ )
                {
                    sum += (*this)(row, k) * rhs(k, col);
                }

                result(row, col) = sum;
            }
        }

        return result;
    }

    Vec4f operator*(const Vec4f& v)
    {
        Vec4f result{};
        for ( int row = 0; row < 4; row++ )
        {
            float value = 0.0f;
            for ( int k = 0; k < 4; k++ )
            {
                value += (*this)(row, k) * v[ k ];
            }
            result[ row ] = value;
        }

        return result;
    }

    static Mat4f Identity()
    {
        return Mat4f{ Vec4f{ 1.0f, 0.0f, 0.0f, 0.0f },
                      Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
                      Vec4f{ 0.0f, 0.0f, 1.0f, 0.0f },
                      Vec4f{ 0.0f, 0.0f, 0.0f, 1.0f } };
    }

    const float* Data()
    {
        return (float*)(e[ 0 ]);
    }

    const char* ToString() const
    {
        static char buffer[ 256 ];
        sprintf(buffer,
                "%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n",
                e[ 0 ][ 0 ],
                e[ 1 ][ 0 ],
                e[ 2 ][ 0 ],
                e[ 3 ][ 0 ],
                e[ 0 ][ 1 ],
                e[ 1 ][ 1 ],
                e[ 2 ][ 1 ],
                e[ 3 ][ 1 ],
                e[ 0 ][ 2 ],
                e[ 1 ][ 2 ],
                e[ 2 ][ 2 ],
                e[ 3 ][ 2 ],
                e[ 0 ][ 3 ],
                e[ 1 ][ 3 ],
                e[ 2 ][ 3 ],
                e[ 3 ][ 3 ]);
        return buffer;
    }
};

struct Quat
{
    float x, y, z, w;

    const Quat operator*(const Quat& rhs) const
    {
        return Quat{ w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
                     w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                     w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                     w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w };
    }

    Quat& operator/=(const float s)
    {
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }
};

Vec3f Cross(const Vec3f& a, const Vec3f& b);
float Length(const Vec3f& v);
float Length(const Vec4f& v);
Vec3f Normalize(const Vec3f& v);
float Dot(const Vec3f& a, const Vec3f& b);
Vec4f Normalize(const Vec4f& v);
float Dot(const Vec4f& a, const Vec4f& b);

/* Create Inverse of 4x4 Matrix.
 * Code from 'Foundations of Game Engine programming Vol.1'
 * by Eric Lengyel.
 */
Mat4f Inverse(const Mat4f& m);

/* Builds a versor from given axis and rotation angle (in degrees), given in degrees.
 * A versor is the expression of a rotation in a '4D rotational vector'.
 * Note that this computes a unit-quaternion.
 * Also note that mathematicians don't like it when we call Quaternions Vectors!
 */
Quat AngleAxis(const float& x, const float& y, const float& z, const float& angleDgr);
Quat AngleAxis(const Vec3f& v, const float& angleDgr);

Mat4f AsMat4f(const Quat& qIn);

Mat4f LookAt(const Vec3f& position, const Vec3f& target, const Vec3f& up);

/* Creates a right-handed, y-up, perspective projection matrix. */
Mat4f PerspectiveProjection(const float& fovy, const float& aspect, const float& near, const float& far);

Mat4f Translate(const Vec3f& v);

Mat4f Scale(const Vec3f& v);

void Translate(Mat4f& m, const Vec3f& v);

void Scale(Mat4f& m, const Vec3f& v);

#endif
