/*************************************************************************************************
   Demo Lib / qb_torus_landscape.cpp
  
   - Torus Landscape Generation
                                                                                 2003/01/15 16:57
/************************************************************************************************/
#include <stdafx.h>

#include "qb_torus_landscape.h"

//________________________________________________________________________________________________

void                                    QB_TexGen_Fractal_Landscape( unsigned seed,
                                            float* target,
                                            float factor );

//________________________________________________________________________________________________

struct QB_VTX_XYZ_D_UV
{
    float                               x;
    float                               y;
    float                               z;
    QB_COLOR                            diffuse;
    float                               u;
    float                               v;
};

#define QB_FVF_XYZ_D_UV ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//________________________________________________________________________________________________

static LPDIRECT3DVERTEXBUFFER8          VB = NULL;
static LPDIRECT3DINDEXBUFFER8           IB = NULL;

static float                            matrix[] =
{
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
};
static float                            matrix2[] =
{
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
};

static float                            Angle = 0.f;
static float                            Loop = 0.f;
static float                            Speed = 0.f;

static QB_VECTOR                        Light;

static unsigned                         statelist1[] =
{
    D3DRS_ZENABLE, TRUE,
    D3DRS_ZWRITEENABLE, TRUE,
    D3DRS_ALPHABLENDENABLE, FALSE,
    D3DRS_CULLMODE, D3DCULL_CCW,
};

static float                            Camera_Time = 0.f;
static float                            Roll_Time = 0.f;

//________________________________________________________________________________________________

float*                                  Landscape = NULL;

//________________________________________________________________________________________________

static void                             Set_Normal( QB_VECTOR &dest,
                                            QB_VECTOR v0,
                                            QB_VECTOR v1,
                                            QB_VECTOR v2 )
{
    //------------------------

    QB_VECTOR n[3];
    n[0] = v1 - v0;
    n[1] = v2 - v0;

    n[2].x = n[0].y*n[1].z - n[0].z*n[1].y;
    n[2].y = n[0].z*n[1].x - n[0].x*n[1].z;
    n[2].z = n[0].x*n[1].y - n[0].y*n[1].x;

    float norm = 1.f/sqrtf( n[2].x*n[2].x + n[2].y*n[2].y + n[2].z*n[2].z );

    dest.x = n[2].x*norm;
    dest.y = n[2].y*norm;
    dest.z = n[2].z*norm;

    //------------------------
}

//________________________________________________________________________________________________

static float                            Set_Intensity( _uc x,
                                            _uc y )
{
    //------------------------

    QB_VECTOR norm, n;

    x &= 0x7f; y &= 0x7f;
    QB_VECTOR v0, v1, v2;
    v0.Set( 0.f, 0.f, Landscape[x + ( y<<7 )] );

    //------------------------

#define FACTOR ( RADIUS2*0.005f )
    y = ( y + 1 )&0x7f;
    v1.Set( 0.f, FACTOR, Landscape[x + ( y<<7 )] );        // x =  0  y =  1
    y = ( y - 1 )&0x7f; x = ( x + 1 )&0x7f;
    v2.Set( FACTOR, 0.f, Landscape[x + ( y<<7 )] );        // x =  1  y =  0
    Set_Normal( norm, v0, v1, v2 );

    x = ( x - 1 )&0x7f; y = ( y - 1 )&0x7f;
    v1.Set( 0.f, -FACTOR, Landscape[x + ( y<<7 )] );       // x =  0  y = -1
    Set_Normal( n, v0, v2, v1 ); norm += n;

    x = ( x - 1 )&0x7f; y = ( y + 1 )&0x7f;
    v2.Set( -FACTOR, 0.f, Landscape[x + ( y<<7 )] );       // x = -1  y =  0
    Set_Normal( n, v0, v1, v2 ); norm += n;

    x = ( x + 1 )&0x7f; y = ( y + 1 )&0x7f;
    v1.Set( 0.f, FACTOR, Landscape[x + ( y<<7 )] );        // x =  0  y =  1
    Set_Normal( n, v0, v2, v1 ); norm += n;

    //------------------------

    norm.Normalize();
    float d = norm*Light;
    d += 1.f;
    d *= 0.5f*255;

    return d;

    //------------------------
}

//________________________________________________________________________________________________

int                                     Init_Torus_Landscape()
{
    //------------------------

    Light.Set( 1.f, 1.f, -1.f );
    Light.Normalize();

    //------------------------

    {
        float* l = new float[256*256];
        QB_TexGen_Fractal_Landscape( QB_Rand() + ( QB_Rand()<<16 ), l, 1.f );

        Landscape = new float[128*128];
        int i;
        for ( i = 0; i<128; i++ )
        {
            int j;
            for ( j = 0; j<128; j++ )
            {
                Landscape[i*128 + j]  = l[( i*2 + 0 )*256 + j*2];
                Landscape[i*128 + j] += l[( i*2 + 0 )*256 + j*2 + 1];
                Landscape[i*128 + j] += l[( i*2 + 1 )*256 + j*2];
                Landscape[i*128 + j] += l[( i*2 + 1 )*256 + j*2 + 1];
                Landscape[i*128 + j] /= 4.f;
            }
        }
        delete[] l;

        float min = Landscape[0], max = Landscape[0];
        for ( i = 0; i<128*128; i++ )
        {
            if ( Landscape[i]<min )
                min = Landscape[i];
            else if ( Landscape[i]>max )
                max = Landscape[i];
        }
        for ( i = 0; i<128*128; i++ )
        {
            Landscape[i] = ( RADIUS2*0.1f ) + ( RADIUS2*0.5f*( max - Landscape[i] )/( max - min ) );
        }
    }

    //------------------------

    Demo_Camera.Source.z = 0.f;
    Demo_Camera.Target.z = 0.f;

    //------------------------

    {
        if ( QB_DirectX_Device->CreateIndexBuffer( TILE*( TILE + 1 )*2*sizeof( WORD ),
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_MANAGED,
            &IB )!=D3D_OK )
            return -1;

        WORD* index;
        IB->Lock( 0, 0, ( BYTE** )&index, 0 );
        int i, j;
        for ( i = 0; i<TILE; i++ )
        {
            for ( j = 0; j<=TILE; j++ )
            {
                index[0] = i*( TILE + 1 ) + j;
                index[1] = ( i + 1 )*( TILE + 1 ) + j;
                index += 2;
            }
        }

        IB->Unlock();
    }

    //------------------------

    {
        if ( QB_DirectX_Device->CreateVertexBuffer( ( TILE + 1 )*( TILE + 1 )*sizeof( QB_VTX_XYZ_D_UV ),
            D3DUSAGE_WRITEONLY,
            QB_FVF_XYZ_D_UV,
            D3DPOOL_MANAGED,
            &VB )!=D3D_OK )
            return -1;

        QB_VTX_XYZ_D_UV* vtx;
        VB->Lock( 0, 0, ( BYTE** )&vtx, 0 );

        int y;
        for ( y = 0; y<=TILE; y++ )
        {
            int x;
            for ( x = 0; x<=TILE; x++ )
            {
                int ofs = ( ( y&127 )<<7 ) + ( x&127 );
                float l = Landscape[ofs];

                float v[3];

                v[0] = ( RADIUS1 + l*cosf( x*2.f*3.141592f/TILE ) )*cosf( y*2.f*3.141592f/TILE );
                v[1] = ( RADIUS1 + l*cosf( x*2.f*3.141592f/TILE ) )*sinf( y*2.f*3.141592f/TILE );
                v[2] = l*sinf( x*2.f*3.141592f/TILE );

                vtx->x = v[0];
                vtx->y = v[1];
                vtx->z = v[2];

                float intensity = Set_Intensity( x, y );
                vtx->diffuse.Set( ( _uc )intensity, ( _uc )intensity, ( _uc )intensity, 255 );

                vtx->u = ( float )x;
                vtx->v = 4.f*( float )y;

                vtx++;
            }
        }

        VB->Unlock();
    }

    //------------------------

    return 0;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Release_Torus_Landscape()
{
    //------------------------

    if ( VB ) VB->Release();
    if ( IB ) IB->Release();
    delete[] Landscape;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Update_Torus_Landscape_Camera()
{
    float old = Angle;
    Angle = atan2f( Demo_Camera.Source.y, Demo_Camera.Source.x );
    Angle *= TILE/( 2*3.141592f );
    if ( Angle<0.f )
        Angle += TILE;

    if ( old>Angle )
    {
        Loop++;
    }
}

//________________________________________________________________________________________________

void                                    Update_Torus_Landscape()
{
    //------------------------

    Demo_Camera.Source.x = ( RADIUS1 + RADIUS2*0.f )*cosf( Camera_Time );
    Demo_Camera.Source.y = ( RADIUS1 + RADIUS2*0.f )*sinf( Camera_Time );
    Demo_Camera.Target.x = ( RADIUS1 - RADIUS2*0.25f )*cosf( Camera_Time + 0.2f );
    Demo_Camera.Target.y = ( RADIUS1 - RADIUS2*0.25f )*sinf( Camera_Time + 0.2f );
    Demo_Camera.Roll = Roll_Time;

    Update_Torus_Landscape_Camera();

    QB_DirectX_SetRenderState( statelist1 );

    //------------------------

    QB_DirectX_Device->SetTexture( 0, Textures[0] );

    QB_DirectX_Device->SetVertexShader( QB_FVF_XYZ_D_UV );
    QB_DirectX_Device->SetIndices( IB, 0L );

    //------------------------

    Demo_Camera.Convert_To_Matrix();

    Demo_Camera.Set_View();
    Demo_Camera.Set_Projection();

    QB_DirectX_Device->SetStreamSource( 0, VB, sizeof( QB_VTX_XYZ_D_UV ) );
    QB_DirectX_Device->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matrix[0] );

    int i;
    for ( i = ( int )Angle; i<=26 + ( int )Angle; i++ )
    {
        int block = i&( TILE - 1 );
        QB_DirectX_Device->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, block*( TILE + 1 ), ( TILE + 1 )*2, block*( TILE + 1 )*2, TILE*2 );
    }

    //------------------------

    Camera_Time += QB_Timer_Request*0.06f;
    if ( Camera_Time>=3.141592f*2 )
        Camera_Time -= 3.141592f*2;
    Roll_Time -= QB_Timer_Request*0.03f;
    if ( Roll_Time<0.f )
        Roll_Time += 3.141592f*2;

    //------------------------
}

//________________________________________________________________________________________________
