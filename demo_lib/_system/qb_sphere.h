/*************************************************************************************************
   qb_sphere.h
  
   - Bounding Sphere manipulation class
                                                                                 2002/06/24 16:00
/************************************************************************************************/
#ifndef __QB_SPHERE_H__
#define __QB_SPHERE_H__

//________________________________________________________________________________________________

#include "qb_vector.h"

//________________________________________________________________________________________________

struct QB_SPHERE
{
    //________________________________________________________________________________________________

    QB_VECTOR pivot;
    float radius;

    //________________________________________________________________________________________________

    inline QB_SPHERE() {}
    inline QB_SPHERE( const float _x, const float _y, const float _z, const float _r ) { pivot.x = _x; pivot.y = _y; pivot.z = _z; radius = _r; }

    //________________________________________________________________________________________________
};

//________________________________________________________________________________________________

#endif  __QB_SPHERE_H__