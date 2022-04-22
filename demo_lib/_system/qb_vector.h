/*************************************************************************************************
   qb_vector.h
  
   - Vector manipulation structure
                                                                                 2002/10/28 12:17
/************************************************************************************************/
#ifndef __QB_VECTOR_H__
#define __QB_VECTOR_H__

//________________________________________________________________________________________________

#include "qb_matrix.h"

//________________________________________________________________________________________________

struct QB_MATRIX;
struct QB_VECTOR
{
    //_DATAS__________________________________________________________________________________________

    float x;
    float y;
    float z;

    //_CONSTRUCTORS___________________________________________________________________________________

//    inline QB_VECTOR() {}
//    inline QB_VECTOR( const float _x, const float _y, const float _z ) { x = _x; y = _y; z = _z; }
    inline void Set( float _x, float _y, float _z ) { x = _x; y = _y; z = _z; }

    //_OPERATORS______________________________________________________________________________________

    inline float& operator[]( int i ) { return ( &x )[i]; }

    inline QB_VECTOR operator - () const { QB_VECTOR v; v.x = -x; v.y = -y; v.z = -z; return v; }
    inline void operator = ( const QB_VECTOR& v ) { x = v.x; y = v.y; z = v.z; }

    //------------------------

    inline QB_VECTOR operator + ( const QB_VECTOR& v ) const { QB_VECTOR w; w.x = x + v.x; w.y = y + v.y; w.z = z + v.z; return w; }
    inline QB_VECTOR operator - ( const QB_VECTOR& v ) const { QB_VECTOR w; w.x = x - v.x; w.y = y - v.y; w.z = z - v.z; return w; }
    inline float operator * ( const QB_VECTOR& v) const { return x*v.x + y*v.y + z*v.z; }
    inline QB_VECTOR operator ^ ( const QB_VECTOR& v ) const { QB_VECTOR w; w.x = y*v.z - z*v.y; w.y = z*v.x - x*v.z; w.z = x*v.y - y*v.x; return w; }

    //------------------------

    inline void operator += ( const QB_VECTOR& v ) { x += v.x; y += v.y; z += v.z; }
    inline void operator -= ( const QB_VECTOR& v ) { x -= v.x; y -= v.y; z -= v.z; }

    //------------------------

    inline QB_VECTOR operator + ( const float f ) const { QB_VECTOR v; v.x = x + f; v.y = y + f; v.z = z + f; return v; }
    inline QB_VECTOR operator - ( const float f ) const { QB_VECTOR v; v.x = x - f; v.y = y - f; v.z = z - f; return v; }
    inline QB_VECTOR operator * ( const float f ) const { QB_VECTOR v; v.x = x*f; v.y = y*f; v.z = z*f; return v; }
    inline QB_VECTOR operator / ( float f ) const { QB_VECTOR v; f = 1.f/f; v.x = x*f; v.y = y*f; v.z = z*f; return v; }

    //------------------------

    inline void operator += ( const float f ) { x += f; y += f; z += f; }
    inline void operator -= ( const float f ) { x -= f; y -= f; z -= f; }
    inline void operator *= ( const float f ) { x *= f; y *= f; z *= f; }
    inline void operator /= ( float f ) { f = 1.f/f; x *= f; y *= f; z *= f; }

    //------------------------

    inline QB_VECTOR operator * ( const QB_MATRIX& m ) const
    {
        //------------------------------

        QB_VECTOR r;

        r.x = x*m.m[0][0] + y*m.m[1][0] + z*m.m[2][0] + m.m[3][0];
        r.y = x*m.m[0][1] + y*m.m[1][1] + z*m.m[2][1] + m.m[3][1];
        r.z = x*m.m[0][2] + y*m.m[1][2] + z*m.m[2][2] + m.m[3][2];

        return r;

        //------------------------------
    }
    inline QB_VECTOR operator ^ ( const QB_MATRIX& m ) const
    {
        //------------------------------

        QB_VECTOR r;

        r.x = x*m.m[0][0] + y*m.m[1][0] + z*m.m[2][0];
        r.y = x*m.m[0][1] + y*m.m[1][1] + z*m.m[2][1];
        r.z = x*m.m[0][2] + y*m.m[1][2] + z*m.m[2][2];

        return r;

        //------------------------------
    }

    //_FUNCTIONS______________________________________________________________________________________

    inline float                        Norm2()
    {
        return x*x + y*y + z*z;
    }
    inline float                        Norm()
    {
        return sqrtf( x*x + y*y + z*z );
    }
    inline void                         Normalize()
    {
        float n = 1.f/Norm();
        x *= n;
        y *= n;
        z *= n;
    }
    inline float                        Distance( const QB_VECTOR& B ) const
    {
        float dx = x - B.x, dy = y - B.y, dz = z - B.z;
        return sqrtf( dx*dx + dy*dy + dz*dz );
    }

    //________________________________________________________________________________________________
};

//________________________________________________________________________________________________

inline QB_VECTOR operator * ( const float f, const QB_VECTOR& v ) { QB_VECTOR r; r.x = v.x*f; r.y = v.y*f; r.z = v.z*f; return r; }

//________________________________________________________________________________________________

#endif  __QB_VECTOR_H__