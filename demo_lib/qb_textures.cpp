/*************************************************************************************************
   Demo Lib / qb_textures.cpp
  
   - Load/Generate the textures
                                                                                 2002/11/27 18:04
/************************************************************************************************/
#include "stdafx.h"

//________________________________________________________________________________________________

extern "C" unsigned char                bitmap_wireframe[];
extern "C" unsigned char                bitmap_sprite[];
extern "C" unsigned char                bitmap_quantum[];
extern "C" unsigned char                bitmap_font[];

//________________________________________________________________________________________________

struct QB_TEXTURE
{
    unsigned char*                      bitmap;
    int                                 size;
    D3DFORMAT                           format;
    int                                 mipmaps;
};
static unsigned                         commands[] =
{
    ( _u )bitmap_wireframe, 256, D3DFMT_R5G6B5, 0,
    ( _u )bitmap_sprite, 32, D3DFMT_R5G6B5, 0,
    ( _u )bitmap_quantum, 256, D3DFMT_R5G6B5, 1,
    ( _u )bitmap_font, 256, D3DFMT_R5G6B5, 1,
};

QB_TEXTURE*                             Commands = ( QB_TEXTURE* )commands;
#define N_TEXTURE ( sizeof( commands )/sizeof( QB_TEXTURE ) )

LPDIRECT3DTEXTURE8                      Textures[N_TEXTURE];

//________________________________________________________________________________________________

int                                     Init_Textures()
{
    //------------------------

    int err = 0;
    int j;
    for ( j = 0; j<N_TEXTURE; j++ )
    {
        _uc* bitmap = Commands[j].bitmap;

        Textures[j] = NULL;
        if ( QB_DirectX_Load( bitmap, &Textures[j], Commands[j].size, Commands[j].size, Commands[j].format, Commands[j].mipmaps )!=D3D_OK )
            err = -1;

        if ( bitmap!=Commands[j].bitmap )
            delete[] bitmap;
    }

    //------------------------

    return err;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Release_Textures()
{
    int j;
    for ( j = 0; j<N_TEXTURE; j++ )
    {
        if ( Textures[j] ) Textures[j]->Release();
    }
}

//________________________________________________________________________________________________
