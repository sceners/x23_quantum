/*************************************************************************************************
   qb_plane.h
  
   - Plane manipulation class
                                                                                 2002/05/21 14:37
/************************************************************************************************/
#ifndef __QB_PLANE_H__
#define __QB_PLANE_H__

//________________________________________________________________________________________________

#include "qb_vector.h"

//________________________________________________________________________________________________

struct QB_PLANE:public QB_VECTOR
{
    //________________________________________________________________________________________________

    float d;

    //________________________________________________________________________________________________

    inline QB_PLANE() {}
    inline QB_PLANE( const float _x, const float _y, const float _z, const float _d ) { x = _x; y = _y; z = _z; d = _d; }

    //------------------------

    inline void Flip() { x = -x; y = -y; z = -z; d = -d; }

    inline float Distance( const QB_VECTOR& v ) const { return x*v.x + y*v.y + z*v.z + d; }
    inline void Set_Distance( const QB_VECTOR& v) { d = -( x*v.x + y*v.y + z*v.z ); }

    inline void Create( const QB_VECTOR& a, const QB_VECTOR& b, const QB_VECTOR& c )
    {
        QB_VECTOR v = ( b - a )^( c - a );
        v.Normalize();
        x = v.x;  y = v.y;  z = v.z;
        Set_Distance( a );
    }
    inline void Create( const QB_VECTOR& v, const QB_VECTOR& n )
    {
        x = n.x; y = n.y; z = n.z;
        Set_Distance( v );
    }

    //________________________________________________________________________________________________
};

//________________________________________________________________________________________________

#endif __QB_PLANE_H__