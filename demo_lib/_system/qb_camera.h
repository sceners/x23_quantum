/*************************************************************************************************
   qb_camera.h
  
   - Camera system class
                                                                                 2002/10/03 17:48
/************************************************************************************************/
#ifndef __QB_CAMERA_H__
#define __QB_CAMERA_H__

//________________________________________________________________________________________________

#include "qb_vector.h"
#include "qb_matrix.h"
#include "qb_plane.h"
#include "qb_sphere.h"

//________________________________________________________________________________________________

class QB_CAMERA
{
public:
    //_DATAS__________________________________________________________________________________________

    QB_VECTOR                           Source;
    QB_VECTOR                           Target;
    float                               Roll;

    //------------------------

    float                               Fov;
    float                               Clip_Near;
    float                               Clip_Far;
    float                               Aspect_Ratio;

    //------------------------

    QB_MATRIX                           World;
    QB_MATRIX                           View;
    QB_MATRIX                           Projection;

    QB_PLANE                            Frustum[6];

    //________________________________________________________________________________________________

    void                                Init();

    void                                Convert_To_Matrix();

    void                                Set_Projection();
    void                                Set_View();

    unsigned                            Is_Visible( QB_SPHERE& b );

protected:
    void                                Make_Frustum();
    void                                Create_Frustum_Plane( QB_VECTOR* n,
                                            QB_PLANE* plane );

    //________________________________________________________________________________________________
};

//________________________________________________________________________________________________

#endif  __QB_CAMERA_H__