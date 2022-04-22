/*************************************************************************************************
   qb_camera.cpp
  
   - Camera manipulation functions
                                                                                 2002/09/19 12:03
/************************************************************************************************/
#include "stdafx.h"

//________________________________________________________________________________________________

void                                    QB_CAMERA::Init()
{
    //------------------------

    Roll = 0.f;

    World.Identity();

    Clip_Near = 1.f;
    Clip_Far = 10000.f;
    Aspect_Ratio = 4.f/3.f;
    Fov = 3.141592f/4.f;

    //------------------------
}

//________________________________________________________________________________________________

void                                    QB_CAMERA::Convert_To_Matrix()
{
    //------------------------

    QB_VECTOR dir = Source - Target;
    dir.Normalize();

    World.k() = dir;
    QB_VECTOR Z; Z.Set( 0.f, 0.f, 1.f );
    World.i() = Z^World.k();
    World.i().Normalize();
    World.j() = World.k()^World.i();

    World.Pre_Rotate_Z( Roll );

    World.O() = Source;

    //------------------------
}

//________________________________________________________________________________________________

void                                    QB_CAMERA::Set_View()
{
    //------------------------

    View = World;

    View.Inverse();

    View.m[0][2] = -View.m[0][2];
    View.m[1][2] = -View.m[1][2];
    View.m[2][2] = -View.m[2][2];
    View.m[3][2] = -View.m[3][2];

    QB_DirectX_Device->SetTransform( D3DTS_VIEW, ( D3DMATRIX* )View.m );

    //------------------------

    Make_Frustum();

    //------------------------
}

//________________________________________________________________________________________________

void                                    QB_CAMERA::Set_Projection()
{
    //------------------------

    float width = 1.f/tanf( Fov/2.f );
    float height = width*Aspect_Ratio;
    float Q = Clip_Far/( Clip_Far - Clip_Near );

    //------------------------

    Projection.Clear();

    Projection.m[0][0] = width;
    Projection.m[1][1] = height;
    Projection.m[2][2] = Q;
    Projection.m[3][2] = -Q*Clip_Near;
    Projection.m[2][3] = 1.0f;

    QB_DirectX_Device->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)Projection.m );

    //------------------------
}

//________________________________________________________________________________________________

void                                    QB_CAMERA::Create_Frustum_Plane( QB_VECTOR* n,
                                            QB_PLANE* plane )
{
    //-------------------------

    plane->x = n->x*World.m[0][0] + n->y*World.m[1][0] + n->z*World.m[2][0];
    plane->y = n->x*World.m[0][1] + n->y*World.m[1][1] + n->z*World.m[2][1];
    plane->z = n->x*World.m[0][2] + n->y*World.m[1][2] + n->z*World.m[2][2];

    //------------------------

    plane->d = -World.m[3][0]*plane->x - World.m[3][1]*plane->y - World.m[3][2]*plane->z;

    //-------------------------
}

//________________________________________________________________________________________________

void                                    QB_CAMERA::Make_Frustum()
{
    float angle, c, s;
    QB_VECTOR n;

    //-------------------------

    angle = Fov/2.f;
    c = cosf( angle );
    s = sinf( angle );
    n.x = -c;
    n.y = 0.f;
    n.z = s;
    Create_Frustum_Plane( &n, &Frustum[2] ); //left-hand plane

    //-------------------------

    n.x = c;
    Create_Frustum_Plane( &n, &Frustum[3] ); //right-hand plane

    //-------------------------

    angle = atan2f( tanf( angle ), Aspect_Ratio );
    c = cosf( angle );
    s = sinf( angle );
    n.x = 0.f;
    n.y = c;
    n.z = s;
    Create_Frustum_Plane( &n, &Frustum[4] ); //upper plane

    //-------------------------

    n.y = -c;
    Create_Frustum_Plane( &n, &Frustum[5] ); //lower plane

    //-------------------------

    n.y = 0.f, n.z = 1.f;
    Create_Frustum_Plane( &n, &Frustum[0] ); //near plane
    Frustum[0].d += Clip_Near;

    //-------------------------

    n.z = -1.f;
    Create_Frustum_Plane( &n, &Frustum[1] ); //far plane
    Frustum[1].d -= Clip_Far;

    //-------------------------
}

//________________________________________________________________________________________________

unsigned                                QB_CAMERA::Is_Visible( QB_SPHERE& b )
{
    //-----------------------

    int i;
    for ( i = 0; i<6; i++ )
    {
        float d = Frustum[i].x*b.pivot.x + Frustum[i].y*b.pivot.y + Frustum[i].z*b.pivot.z + Frustum[i].d;
        if ( d>=b.radius )
            return 0;
    }

    return 1;

    //-----------------------
}

//________________________________________________________________________________________________
