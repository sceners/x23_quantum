/*************************************************************************************************
   Demo Lib / qb_sprite_nrj.cpp
  
   - Sprite NRJ management
                                                                                 2003/01/15 16:53
/************************************************************************************************/
#include <stdafx.h>

#include "qb_torus_landscape.h"

//________________________________________________________________________________________________

struct QB_VTX_XYZ_D
{
    float                               x;
    float                               y;
    float                               z;
    QB_COLOR                            color;
};

#define QB_FVF_XYZ_D ( D3DFVF_XYZ | D3DFVF_DIFFUSE )

//________________________________________________________________________________________________

#define SPRITE_COUNT 128
#define SPRITE_DECAL 4.f
#define NRJ_COUNT 16

struct QB_SPRITE_NRJ
{
    float                               time;

    union
    {
        struct
        {
            float                       yaw, pitch;
            float                       d;
        };
        struct
        {
            float                       x, y, z;
        };
    };
};

struct EMITTER
{
    float                               dist;
    float                               angle;
};
struct QB_NRJ
{
    int                                 mode;
    float                               time;

    EMITTER                             emitter[4];
    float                               length;

    QB_SPRITE_NRJ                       sprite[SPRITE_COUNT];
};

static EMITTER                          disp_v[4] = {
    {  0.125f,  0.f },
    { -0.125f,  0.f },
    {  0.f,  0.5f },
    {  0.f, -0.5f } };

static QB_NRJ                           Nrj[NRJ_COUNT];
static float                            Time_Next_Overflow = 0.f;

//________________________________________________________________________________________________

static LPDIRECT3DVERTEXBUFFER8          VB = NULL;

static unsigned                         statelist1[] =
{
    D3DRS_POINTSPRITEENABLE, TRUE,
    D3DRS_POINTSCALEENABLE, TRUE,
    D3DRS_POINTSIZE, 0x41200000, //10.f
    D3DRS_ALPHABLENDENABLE, TRUE,
    D3DRS_SRCBLEND, D3DBLEND_ONE,
    D3DRS_DESTBLEND, D3DBLEND_ONE,
    D3DRS_ZENABLE, TRUE,
    D3DRS_ZWRITEENABLE, FALSE,
    D3DRS_CULLMODE, D3DCULL_NONE,
    D3DRS_FOGENABLE, FALSE,
};

static unsigned                         statelist2[] =
{
    D3DRS_POINTSPRITEENABLE, FALSE,
    D3DRS_POINTSCALEENABLE, FALSE,
    D3DRS_ZWRITEENABLE, TRUE,
};

//________________________________________________________________________________________________

int                                     Init_Sprite_NRJ()
{
    //------------------------

    if ( QB_DirectX_Device->CreateVertexBuffer( SPRITE_COUNT*sizeof( QB_VTX_XYZ_D ),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
        QB_FVF_XYZ_D, D3DPOOL_DEFAULT, &VB )!=D3D_OK ) return -1;

    int i;
    for ( i = 0; i<NRJ_COUNT; i++ )
        Nrj[i].mode = -1;

    return 0;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Release_Sprite_NRJ()
{
    //------------------------

    if ( VB ) VB->Release();

    //------------------------
}

//________________________________________________________________________________________________

void                                    Update_Sprite_NRJ()
{
    if ( Time_Next_Overflow<=0.f )
    {
        int i;
        for ( i = 0; i<NRJ_COUNT && Nrj[i].mode!=-1; i++ );
        if ( i!=NRJ_COUNT )
        {
            //------------------------

            Time_Next_Overflow = 1.f*QB_Rand()/32767.f;

            //------------------------

            float a = atan2f( Demo_Camera.Source.y, Demo_Camera.Source.x );
            a *= TILE/( 2*3.141592f );
            if ( a<0.f )
                a += TILE;

            //------------------------

            int d = 6 + ( QB_Rand()&0xf ) + ( int )a;
            int angle = QB_Rand()&0x7f;
            
            Nrj[i].mode = 0;

            Nrj[i].emitter[0].angle = angle*2.f*3.141592f/TILE;
            Nrj[i].emitter[0].dist = d*2.f*3.141592f/TILE;
            Nrj[i].emitter[1] = Nrj[i].emitter[2] = Nrj[i].emitter[3] = Nrj[i].emitter[0];

            int ofs = angle + ( d&0x7f )*128;
            Nrj[i].length = Landscape[ofs] - SPRITE_DECAL;

            int j;
            for ( j = 0; j<128; j++ )
            {
                Nrj[i].sprite[j].d = 0.05f + ( QB_Rand()/32767.f );
                Nrj[i].sprite[j].yaw = 3.141593f*2*QB_Rand()/32767.f;
                Nrj[i].sprite[j].pitch = 5.f*( 0.5f - QB_Rand()/32767.f );
            }
            Nrj[i].time = 0.f;
        }
    }

    //------------------------

    QB_MATRIX m;
    m.Identity();
    QB_DirectX_Device->SetTransform( D3DTS_WORLD, ( D3DMATRIX* )&m.m[0][0] );

    //------------------------

    if ( QB_DirectX_Tnl==D3DCREATE_MIXED_VERTEXPROCESSING )
        QB_DirectX_Device->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, TRUE );
    QB_DirectX_SetRenderState( statelist1 );

    //------------------------

    QB_DirectX_Device->SetStreamSource( 0, VB, sizeof( QB_VTX_XYZ_D ) );
    QB_DirectX_Device->SetVertexShader( QB_FVF_XYZ_D );

    QB_DirectX_Device->SetTexture( 0, Textures[1] );

    int i;
    for ( i = 0; i<NRJ_COUNT; i++ )
    {
        switch( Nrj[i].mode )
        {
        case 0:
            {
                QB_VTX_XYZ_D* vtx;
                VB->Lock( 0, 0, ( BYTE** )&vtx, D3DLOCK_DISCARD );

                float ox = 10.f;
                if ( i&1 )
                    ox = -10.f;

                int j;
                for ( j = 0; j<128; j++ )
                {
                    float angle = Nrj[i].emitter[0].angle + ( ( 1.01f - Nrj[i].time )*Nrj[i].sprite[j].d )*cosf( Nrj[i].time*ox + Nrj[i].sprite[j].yaw );
                    float dist = Nrj[i].emitter[0].dist + ( ( 1.01f - Nrj[i].time )*Nrj[i].sprite[j].d )*0.1f*sinf( Nrj[i].time*ox + Nrj[i].sprite[j].yaw );
                    float length = Nrj[i].length + Nrj[i].sprite[j].pitch*( ( 1.01f - Nrj[i].time )*Nrj[i].sprite[j].d );

                    vtx[j].x = ( RADIUS1 + length*cosf( angle ) )*cosf( dist );
                    vtx[j].y = ( RADIUS1 + length*cosf( angle ) )*sinf( dist );
                    vtx[j].z = length*sinf( angle );

                    unsigned char c = ( unsigned char )( Nrj[i].time*Nrj[i].time*Nrj[i].time*127 );
                    vtx[j].color = 0xff000000 + c + ( c<<8 ) + ( c<<16 );
                }
                Nrj[i].time += QB_Timer_Request*0.5f;

                if ( Nrj[i].time>1.f )
                {
                    float time = Nrj[i].time;
                    Nrj[i].mode = 1;
                    for ( j = 0; j<128; j++ )
                    {
                        float angle = Nrj[i].emitter[0].angle + ( ( 1.01f - Nrj[i].time )*Nrj[i].sprite[j].d )*cosf( Nrj[i].time*ox + Nrj[i].sprite[j].yaw );
                        float dist = Nrj[i].emitter[0].dist + ( ( 1.01f - Nrj[i].time )*Nrj[i].sprite[j].d )*0.1f*sinf( Nrj[i].time*ox + Nrj[i].sprite[j].yaw );
                        float length = Nrj[i].length + Nrj[i].sprite[j].pitch*( ( 1.01f - Nrj[i].time )*Nrj[i].sprite[j].d );

                        Nrj[i].sprite[j].x = ( RADIUS1 + length*cosf( angle ) )*cosf( dist );
                        Nrj[i].sprite[j].y = ( RADIUS1 + length*cosf( angle ) )*sinf( dist );
                        Nrj[i].sprite[j].z = length*sinf( angle );

                        Nrj[i].sprite[j].time = ( j&31 )/32.f;
                    }

                    Nrj[i].time = 3.f;
                }

                VB->Unlock();

                QB_DirectX_Device->DrawPrimitive( D3DPT_POINTLIST, 0, 128 );
            }
            break;
        case 1:
            {
                QB_VTX_XYZ_D* vtx;
                VB->Lock( 0, 0, ( BYTE** )&vtx, D3DLOCK_DISCARD );

                int j;
                for ( j = 0; j<128; j++ )
                {
                    vtx[j].x = Nrj[i].sprite[j].x;
                    vtx[j].y = Nrj[i].sprite[j].y;
                    vtx[j].z = Nrj[i].sprite[j].z;

                    unsigned char c = ( unsigned char )( Nrj[i].sprite[j].time*( Nrj[i].time/3.f )*255 );
                    vtx[j].color = 0xff000000 + c + ( c<<8 ) + ( c<<16 );

                    Nrj[i].sprite[j].time -= QB_Timer_Request*2;
                    if ( Nrj[i].sprite[j].time<0 )
                    {
                        float angle = Nrj[i].emitter[( j>>5 )].angle + 0.01f*( 0.5f - QB_Rand()/32767.f );
                        float dist = Nrj[i].emitter[( j>>5 )].dist + 0.005f*( 0.5f - QB_Rand()/32767.f );

                        float a = angle*TILE/( 2.f*3.141592f );
                        float d = dist*TILE/( 2.f*3.141592f );

                        int dx = ( int )floor( a );
                        int dy = ( int )floor( d );
                        float rx = a - dx;
                        float ry = d - dy;

                        float d0 = 1.f - rx - ry + rx*ry;
                        float d1 = rx - rx*ry;
                        float d2 = ry - rx*ry;
                        float d3 = rx*ry;
                        int ofs0 = ( dx&0x7f ) + ( dy&0x7f )*128;
                        int ofs1 = ( ( dx + 1 )&0x7f ) + ( dy&0x7f )*128;
                        int ofs2 = ( dx&0x7f ) + ( ( dy + 1 )&0x7f )*128;
                        int ofs3 = ( ( dx + 1 )&0x7f ) + ( ( dy + 1 )&0x7f )*128;
                        float z = d0*Landscape[ofs0];
                        z += d1*Landscape[ofs1];
                        z += d2*Landscape[ofs2];
                        z += d3*Landscape[ofs3];

                        float length = z - SPRITE_DECAL;

                        Nrj[i].sprite[j].x = ( RADIUS1 + length*cosf( angle ) )*cosf( dist );
                        Nrj[i].sprite[j].y = ( RADIUS1 + length*cosf( angle ) )*sinf( dist );
                        Nrj[i].sprite[j].z = length*sinf( angle );

                        Nrj[i].sprite[j].time += 1.f;
                    }
                }

                for ( j = 0; j<4; j++ )
                {
                    Nrj[i].emitter[j].angle += disp_v[j].angle*QB_Timer_Request;
                    Nrj[i].emitter[j].dist += disp_v[j].dist*QB_Timer_Request;
                }

                Nrj[i].time -= QB_Timer_Request;
                if ( Nrj[i].time<0.f )
                {
                    Nrj[i].mode = -1;
                }

                VB->Unlock();
                QB_DirectX_Device->DrawPrimitive( D3DPT_POINTLIST, 0, 128 );
            }
        break;
        }
    }

    //------------------------

    if ( QB_DirectX_Tnl==D3DCREATE_MIXED_VERTEXPROCESSING )
        QB_DirectX_Device->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, FALSE );
    QB_DirectX_SetRenderState( statelist2 );

    Time_Next_Overflow -= QB_Timer_Request;

    //------------------------
}

//________________________________________________________________________________________________
