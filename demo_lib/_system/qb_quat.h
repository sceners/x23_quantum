/*************************************************************************************************
   Demo Lib / qb_quat.h
  
   - Quaternion structure
                                                                                 2001/10/11 12:40
/************************************************************************************************/
#ifndef __QB_QUAT_H__
#define __QB_QUAT_H__

//________________________________________________________________________________________________

struct QB_QUAT
{
    //_DATAS__________________________________________________________________________________________

    float                               x;
    float                               y;
    float                               z;
    float                               w;

    //_CONSTRUCTORS___________________________________________________________________________________

    inline void Set( float _x, float _y, float _z, float _w ) { x = _x; y = _y; z = _z; w = _w; }

    //_OPERATORS______________________________________________________________________________________

    inline float& operator [] ( int i ) { return ( &x )[i]; }
    inline QB_QUAT operator - ( void ) { QB_QUAT q; q.x = -x; q.y = -y; q.z = -z; q.w = -w; return q; }

    //------------------------

    inline int operator == ( const QB_QUAT& Q ) { return ( x == Q.x && y == Q.y && z == Q.z && w == Q.w); }

    //------------------------

    inline void operator += ( const QB_QUAT& Q ) { x += Q.x; y += Q.y; z += Q.z; w += Q.w; }
    inline void operator -= ( const QB_QUAT& Q ) { x -= Q.x; y -= Q.y; z -= Q.z; w -= Q.w; }
    inline void operator *= ( const float f ) { x *= f; y *= f; z *= f; w *= f; }
    inline void operator /= ( float f ) { f = 1.f/f; *this *= f; }

    //_FUNCTIONS______________________________________________________________________________________

    // "this" quaternion is set to correspond to the "identity" transform
    inline void                         Set_Identity( void )
    {
        x = y = z = 0.f;
        w = 1.f;
    }

    //________________________________________________________________________________________________
};

//________________________________________________________________________________________________


#endif __QB_QUAT_H__