/*************************************************************************************************
   Demo Lib / qb_logo.cpp
  
   - Quantum logo
                                                                                 2003/01/15 17:55
/************************************************************************************************/
#include <stdafx.h>

#include "qb_logo.h"

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

static unsigned                         plane[] =
{
    0x44000000, 0x00000000, 0x3f000000, 0x3f800000, 0xffffffff, 0x00000000, 0x00000000,
    0x44200000, 0x00000000, 0x3f000000, 0x3f800000, 0xffffffff, 0x3f000000, 0x00000000,
    0x44000000, 0x43800000, 0x3f000000, 0x3f800000, 0xffffffff, 0x00000000, 0x3f800000,
    0x44200000, 0x43800000, 0x3f000000, 0x3f800000, 0xffffffff, 0x3f000000, 0x3f800000,
    0x44000000, 0x43800000, 0x3f000000, 0x3f800000, 0xffffffff, 0x3f000000, 0x00000000,
    0x44200000, 0x43800000, 0x3f000000, 0x3f800000, 0xffffffff, 0x3f800000, 0x00000000,
    0x44000000, 0x44000000, 0x3f000000, 0x3f800000, 0xffffffff, 0x3f000000, 0x3f800000,
    0x44200000, 0x44000000, 0x3f000000, 0x3f800000, 0xffffffff, 0x3f800000, 0x3f800000
};
static QB_VTX_XYZW_D_UV*                Plane = ( QB_VTX_XYZW_D_UV* )plane;

static unsigned                         statelist1[] =
{
    D3DRS_ZENABLE, FALSE,
    D3DRS_CULLMODE, D3DCULL_NONE,
    D3DRS_LIGHTING, FALSE,
    D3DRS_FOGENABLE, FALSE,
    D3DRS_ALPHABLENDENABLE, TRUE,
    D3DRS_SRCBLEND, D3DBLEND_ONE,
    D3DRS_DESTBLEND, D3DBLEND_ONE,
};

//________________________________________________________________________________________________

int                                     Init_Logo()
{
    //------------------------

    Plane[0].u += 0.5f/256.f;
    Plane[2].u += 0.5f/256.f;
    Plane[4].u += 0.5f/256.f;
    Plane[6].u += 0.5f/256.f;
    return 0;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Release_Logo()
{
    //------------------------


    //------------------------
}

//________________________________________________________________________________________________

void                                    Update_Logo()
{
    //------------------------

    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
/*
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT );
*/
    QB_DirectX_Device->SetVertexShader( QB_FVF_XYZW_D_UV );
    QB_DirectX_SetRenderState( statelist1 );

    QB_DirectX_Device->SetTexture( 0, Textures[2] );
    QB_DirectX_Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Plane, sizeof( QB_VTX_XYZW_D_UV ) );
    QB_DirectX_Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &Plane[4], sizeof( QB_VTX_XYZW_D_UV ) );
/*
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
*/
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );

    //------------------------
}

//________________________________________________________________________________________________
