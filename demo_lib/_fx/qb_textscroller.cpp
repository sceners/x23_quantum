/*************************************************************************************************
   Demo Lib / qb_textscroller.cpp
  
   - Text Scroller
                                                                                 2003/01/15 21:58
/************************************************************************************************/
#include "stdafx.h"

#include "qb_textscroller.h"

#include "../datas/qb_text.h"

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

static LPDIRECT3DVERTEXBUFFER8          VB = NULL;
static LPDIRECT3DINDEXBUFFER8           IB = NULL;

static unsigned                         statelist1[] =
{
    D3DRS_ZENABLE, FALSE,
    D3DRS_CULLMODE, D3DCULL_NONE,
    D3DRS_LIGHTING, FALSE,
    D3DRS_ALPHABLENDENABLE, TRUE,
    D3DRS_SRCBLEND, D3DBLEND_ONE,
    D3DRS_DESTBLEND, D3DBLEND_ONE,

    D3DRS_FOGENABLE, FALSE
};

static float                            Current_Line = 0.f;
static int                              Sprite_Count = 0;

//________________________________________________________________________________________________

int                                     Init_Textscroller()
{
    //------------------------

    if ( QB_DirectX_Device->CreateVertexBuffer( 40*20*4*sizeof( QB_VTX_XYZW_D_UV ),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        QB_FVF_XYZW_D_UV,
        D3DPOOL_DEFAULT,//D3DPOOL_SYSTEMMEM
        &VB )!=D3D_OK )
        return -1;

    //------------------------

    {
        QB_VTX_XYZW_D_UV* vtx;
        VB->Lock( 0, 0, ( BYTE** )&vtx, D3DLOCK_DISCARD );

        int i;
        for ( i = 0; i<40*20*4; i++ )
        {
            vtx[0].z = 0.5f;//32767/32768.f;
            vtx[0].w = 1.f;
            vtx[0].diffuse = 0xffffffff;

            vtx++;
        }

        VB->Unlock();
    }

    //------------------------

    {
        if ( QB_DirectX_Device->CreateIndexBuffer( 40*20*6*sizeof( WORD ),
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_MANAGED,
            &IB )!=D3D_OK ) return -1;

        WORD* index;
        IB->Lock( 0 , 0, ( BYTE** )&index, 0 );

        int i;
        for ( i = 0; i<40*20; i++ )
        {
            index[0] = i*4 + 0;
            index[1] = i*4 + 1;
            index[2] = i*4 + 2;
            index[3] = i*4 + 2;
            index[4] = i*4 + 1;
            index[5] = i*4 + 3;

            index += 6;
        }

        IB->Unlock();
    }

    //------------------------

    return 0;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Release_Textscroller()
{
    //------------------------

    if ( VB ) VB->Release(); //VB = NULL;
    if ( IB ) IB->Release(); //IB = NULL;

    //------------------------
}

//________________________________________________________________________________________________

static void                             Build_Text()
{
    //------------------------

    Sprite_Count = 0;

    //------------------------

    int line = ( int )floorf( Current_Line/28 );
    float y = ( line*28 - floorf( Current_Line ) );
    QB_VTX_XYZW_D_UV* vtx;
    VB->Lock( 0, 0, ( BYTE** )&vtx, 0 );

    //------------------------

    int i;
    for ( i = 0; i<20; i++ )
    {
        int j;
        for ( j = 0; j<40; j++ )
        {
            if ( Text[line*40 + j]!=0xff )
            {
                vtx[0].x = 19.5f + j*12;
                vtx[1].x = 30.5f + j*12;
                vtx[2].x = 19.5f + j*12;
                vtx[3].x = 30.5f + j*12;
                vtx[0].y = y;
                vtx[1].y = y;
                vtx[2].y = y + 27.5f;
                vtx[3].y = y + 27.5f;


                float u = ( float )( Text[line*40 + j]%20 );
                u = u*12.f/256.f;
                float v = ( float )( Text[line*40 + j]/20 );
                v = v*28.f/256.f;

                vtx[0].u = u;
                vtx[1].u = u + 11.f/256.f;
                vtx[2].u = u;
                vtx[3].u = u + 11.f/256.f;

                vtx[0].v = v;
                vtx[1].v = v;
                vtx[2].v = v + 28.f/256.f;
                vtx[3].v = v + 28.f/256.f;

                Sprite_Count++;
                vtx += 4;
            }
        }

        y += 28.f;
        line++;
        if ( line==*Lines )
            line = 0;
    }

    //------------------------

    VB->Unlock();

    //------------------------
}

//________________________________________________________________________________________________

void                                    Update_Textscroller()
{
    //------------------------

    Build_Text();

    QB_DirectX_Device->SetTexture( 0, Textures[3] );
    QB_DirectX_Device->SetStreamSource( 0, VB, sizeof( QB_VTX_XYZW_D_UV ) );
    QB_DirectX_Device->SetVertexShader( QB_FVF_XYZW_D_UV );
    QB_DirectX_Device->SetIndices( IB, 0L );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );

    QB_DirectX_SetRenderState( statelist1 );
    if ( Sprite_Count )
        QB_DirectX_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, Sprite_Count*4, 0, Sprite_Count*2 );

    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );

    //------------------------

    Current_Line += QB_Timer_Request*24;
    if ( Current_Line>=28.f**Lines )
        Current_Line -= 28.f**Lines;

    //------------------------
}

//________________________________________________________________________________________________
