/*************************************************************************************************
   Demo Lib / qb_filter.cpp
  
   - Filter
                                                                                 2003/01/15 17:55
/************************************************************************************************/
#include <stdafx.h>

#include "qb_filter.h"

//________________________________________________________________________________________________

struct QB_VTX_XYZW_D_UV
{
    float                               x;
    float                               y;
    float                               z;
    float                               w;
    QB_COLOR                            diffuse;
    float                               u;
    float                               v;
};

#define QB_FVF_XYZW_D_UV ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//________________________________________________________________________________________________

#define PLANE 32
static QB_VTX_XYZW_D_UV                 Plane[PLANE*4];
struct QB_FILTER
{
    float                               time;
    float                               speed;
};
static QB_FILTER                        Plane_Info[PLANE];

static unsigned                         statelist1[] =
{
    D3DRS_ZENABLE, FALSE,
    D3DRS_CULLMODE, D3DCULL_NONE,
    D3DRS_LIGHTING, FALSE,
    D3DRS_FOGENABLE, FALSE,
    D3DRS_ALPHABLENDENABLE, TRUE,
    D3DRS_SRCBLEND, D3DBLEND_SRCALPHA,
    D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA,
};

static float                            Fade = 4.f;
extern float                            Leave_Time;

//________________________________________________________________________________________________

int                                     Init_Filter()
{
    //------------------------

    int i;
    for ( i = 0; i<PLANE; i++ )
    {
        Plane[i*4 + 0].x = 0.f;
        Plane[i*4 + 1].x = 640.f;
        Plane[i*4 + 2].x = 0.f;
        Plane[i*4 + 3].x = 640.f;

        Plane[i*4 + 0].z = 0.5f;
        Plane[i*4 + 1].z = 0.5f;
        Plane[i*4 + 2].z = 0.5f;
        Plane[i*4 + 3].z = 0.5f;

        Plane[i*4 + 0].w = 1.f;
        Plane[i*4 + 1].w = 1.f;
        Plane[i*4 + 2].w = 1.f;
        Plane[i*4 + 3].w = 1.f;
/*
        Plane[i*4 + 0].diffuse = 0x28000000;
        Plane[i*4 + 1].diffuse = 0x28000000;
        Plane[i*4 + 2].diffuse = 0x28000000;
        Plane[i*4 + 3].diffuse = 0x28000000;
*/
        Plane_Info[i].time = 3.141592f*2.f*QB_Rand()/32767.f;
        int order = ( QB_Rand()&2 ) - 1;
        Plane_Info[i].speed = order*( 0.25f + 0.75f*QB_Rand()/32767.f )*3.141592f*2.f/4.f;
    }

    return 0;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Release_Filter()
{
    //------------------------


    //------------------------
}

//________________________________________________________________________________________________

void                                    Update_Filter()
{
    //------------------------

    QB_DirectX_Device->SetVertexShader( QB_FVF_XYZW_D_UV );
    QB_DirectX_SetRenderState( statelist1 );

    QB_DirectX_Device->SetTexture( 0, NULL );

    QB_COLOR diffuse;
    float c = 0x20 + ( 0xff - 0x20 )*Fade/4.f;
    c = c + ( 0xff - c )*( 6.f - Leave_Time )/6.f;
    diffuse.Set( 0, 0, 0, ( _uc )c );

    int i;
    for ( i = 0; i<PLANE; i++ )
    {
        float y = 480.f*i/( PLANE - 1.f );
        float size = 2*480.f/( PLANE );
        Plane[i*4 + 0].y = y - size + 0.5f*size*cosf( Plane_Info[i].time );
        Plane[i*4 + 1].y = y - size + 0.5f*size*cosf( Plane_Info[i].time );
        Plane[i*4 + 2].y = y + size + 0.5f*size*cosf( Plane_Info[i].time );
        Plane[i*4 + 3].y = y + size + 0.5f*size*cosf( Plane_Info[i].time );

        Plane[i*4 + 0].diffuse = diffuse;
        Plane[i*4 + 1].diffuse = diffuse;
        Plane[i*4 + 2].diffuse = diffuse;
        Plane[i*4 + 3].diffuse = diffuse;

        QB_DirectX_Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &Plane[i*4], sizeof( QB_VTX_XYZW_D_UV ) );

        Plane_Info[i].time += QB_Timer_Request*Plane_Info[i].speed;
        if ( Plane_Info[i].time<0.f )
            Plane_Info[i].time += 3.141592f*2;
        else if ( Plane_Info[i].time>=3.141592f*2 )
            Plane_Info[i].time -= 3.141592f*2;
    }

    Fade -= QB_Timer_Request;
    if ( Fade<0.f )
        Fade = 0.f;

    //------------------------
}

//________________________________________________________________________________________________
