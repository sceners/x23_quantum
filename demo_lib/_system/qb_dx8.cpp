/*************************************************************************************************
   Demo Lib / qb_dx8.cpp
  
   - DirectX 8 Main Functions
                                                                                 2002/10/08 14:39
/************************************************************************************************/
#include "stdafx.h"

//________________________________________________________________________________________________

struct MY_MODE
{
    D3DDISPLAYMODE                      mode;
    DWORD                               tnl;
    int                                 index;
    D3DFORMAT                           stencil;

    struct MY_MODE*                     next;
};

//________________________________________________________________________________________________

LPDIRECT3D8                             QB_DirectX = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE8                       QB_DirectX_Device = NULL; // Our rendering device
DWORD                                   QB_DirectX_Tnl;

//________________________________________________________________________________________________

void                                    QB_DirectX_Release()
{
    //------------------------
    // Release DirectX
    if( QB_DirectX_Device!=NULL )
    {
#ifdef USE_DEBUG
        extern QB_DEBUG* QB_Debug;
        if ( QB_Debug )
            delete QB_Debug;
#endif
        QB_DirectX_Device->Release();
    }
    QB_DirectX_Device = NULL;

    if( QB_DirectX!=NULL ) QB_DirectX->Release(); QB_DirectX = NULL;

    //------------------------
}

//________________________________________________________________________________________________

static int                              Check_Stencil( D3DFORMAT TargetFormat,
                                            D3DFORMAT* pDepthStencilFormat )
{
    //------------------------
    // D3DFMT_D32
    if ( QB_DirectX->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
        TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 )==D3D_OK )
    {
        if ( QB_DirectX->CheckDepthStencilMatch( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            TargetFormat, TargetFormat, D3DFMT_D32 )==D3D_OK )
        {
            *pDepthStencilFormat = D3DFMT_D32;
            return 0;
        }
    }

    //------------------------
    // D3DFMT_D16
    if ( QB_DirectX->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
        TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 )==D3D_OK )
    {
        if ( QB_DirectX->CheckDepthStencilMatch( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            TargetFormat, TargetFormat, D3DFMT_D16 )==D3D_OK )
        {
            *pDepthStencilFormat = D3DFMT_D16;
            return 0;
        }
    }
/*
    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( QB_DirectX->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( QB_DirectX->CheckDepthStencilMatch( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return 0;
            }
        }
    }

    //------------------------
    // D3DFMT_D24X8
    if ( QB_DirectX->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
        TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 )==D3D_OK )
    {
        if ( QB_DirectX->CheckDepthStencilMatch( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            TargetFormat, TargetFormat, D3DFMT_D24X8 )==D3D_OK )
        {
            *pDepthStencilFormat = D3DFMT_D24X8;
            return 0;
        }
    }

    //------------------------
    // D3DFMT_D24S8
    if ( QB_DirectX->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
        TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 )==D3D_OK )
    {
        if ( QB_DirectX->CheckDepthStencilMatch( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            TargetFormat, TargetFormat, D3DFMT_D24S8 )==D3D_OK )
        {
            *pDepthStencilFormat = D3DFMT_D24S8;
            return 0;
        }
    }

    //------------------------
    // D3DFMT_D24X4S4
    if ( QB_DirectX->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
        TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 )==D3D_OK )
    {
        if ( QB_DirectX->CheckDepthStencilMatch( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            TargetFormat, TargetFormat, D3DFMT_D24X4S4 )==D3D_OK )
        {
            *pDepthStencilFormat = D3DFMT_D24X4S4;
            return 0;
        }
    }
*/
    //------------------------

    return -1;

    //------------------------
}

//________________________________________________________________________________________________

void                                    QB_DirectX_Init( HWND hwnd )
{
    //------------------------
    // Create the D3D object.
    if( ( QB_DirectX = Direct3DCreate8( D3D_SDK_VERSION ) )==NULL )
    {
        QB_Exit( "DirectX 8 needed" );
    }
#ifndef _DEBUG
//#if 1
    //------------------------
    // List all wanted modes
    MY_MODE* modes = NULL;
    int count = QB_DirectX->GetAdapterModeCount( D3DADAPTER_DEFAULT );

    //------------------------

    D3DCAPS8 d3dCaps;
    QB_DirectX->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );

    //------------------------
    // Check the tl
    DWORD tnl;
    int b_index;
    if( d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
    {
        if ( d3dCaps.MaxPointSize<=1.f )
            tnl = D3DCREATE_MIXED_VERTEXPROCESSING;
        else
            tnl = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        b_index = 0;
//        tnl = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }
    else
    {
        b_index = 0x80000000;
        tnl = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    for ( int i = 0; i<count; i++ )
    {
        //------------------------

        D3DDISPLAYMODE mode;
        QB_DirectX->EnumAdapterModes( D3DADAPTER_DEFAULT, i, &mode );

        //------------------------

        if ( mode.Width==640 && mode.Height==480 && mode.Format>=20 && mode.Format<=26 )
        {
            //------------------------

            if ( QB_DirectX->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, FALSE )!=D3D_OK )
                continue;

            //------------------------
            // Validate the refresh rate
            if ( mode.RefreshRate>255 )
                continue;

            //------------------------
            // Find a stencil
            D3DFORMAT stencil;
            if ( Check_Stencil( mode.Format, &stencil ) )
                continue;

            //------------------------
            // Build the mode priority
            int index = b_index;
            //index |= ( 255 - mode.RefreshRate )<<8;
            index |= mode.RefreshRate<<8;
            index |= stencil;
            index |= mode.Format<<16;

            //------------------------
            // and store the mode in an ordered list
            MY_MODE* my_mode = new MY_MODE;
            my_mode->mode = mode;
            my_mode->tnl = tnl;
            my_mode->index = index;
            my_mode->stencil = stencil;

            MY_MODE* mode_cur = modes;
            MY_MODE* mode_prev = NULL;
            while ( mode_cur!=NULL )
            {
                if ( my_mode->index<=mode_cur->index )
                    break;
                mode_prev = mode_cur;
                mode_cur = mode_cur->next;
            }

            my_mode->next = mode_cur;
            if ( mode_prev==NULL )
                modes = my_mode;
            else
                mode_prev->next = my_mode;

            //------------------------
        }
    }

    //------------------------
    // no mode?
    if ( modes==NULL )
    {
        QB_Exit( "No valid video mode found" );
    }

    //------------------------
    // Init the mode until we found a good one
    MY_MODE* mode = modes;
    while ( mode )
    {
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory( &d3dpp, sizeof( d3dpp ) );
        d3dpp.BackBufferWidth = 640;
        d3dpp.BackBufferHeight = 480;
        d3dpp.BackBufferFormat = mode->mode.Format;
        d3dpp.Windowed = FALSE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = mode->stencil;
        d3dpp.FullScreen_RefreshRateInHz = mode->mode.RefreshRate;
        d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // no vsync

        if ( QB_DirectX->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
            mode->tnl, &d3dpp, &QB_DirectX_Device )==D3D_OK )
        {
            QB_DirectX_Tnl = mode->tnl;

            //------------------------
            // cool it works, so delete our list and leave
            while ( modes )
            {
                mode = modes->next;
                delete modes;
                modes = mode;
            }
            return;

            //------------------------
        }

        mode = mode->next;
    }

    //------------------------
    // i'm not lucky
    while ( modes )
    {
        mode = modes->next;
        delete modes;
        modes = mode;
    }
    QB_Exit( "No valid video mode found" );

    //------------------------
#else
    //------------------------
    // Get the current desktop display mode, so we can set up a back
    // buffer of the same format
    D3DDISPLAYMODE d3ddm;

    if ( QB_DirectX->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm )!=D3D_OK )
        QB_Exit( "Can't init video mode" );

    //------------------------
    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if ( QB_DirectX->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp, &QB_DirectX_Device )!=D3D_OK )
        QB_Exit( "Can't init video mode" );

    //------------------------
#endif
}

//________________________________________________________________________________________________

int __stdcall                           QB_DirectX_Load( unsigned char* bits,
                                            LPDIRECT3DTEXTURE8* texture,
                                            int width,
                                            int height,
                                            D3DFORMAT format,
                                            int mipmap )
{
    //--------------------------

    if ( FAILED( QB_DirectX_Device->CreateTexture( width, height, mipmap, 0, format, D3DPOOL_MANAGED, texture ) ) )
        return -1;

    LPDIRECT3DSURFACE8 psurf;
    if ( FAILED( ( *texture )->GetSurfaceLevel( 0, &psurf ) ) )
        return -1;

    D3DSURFACE_DESC sd;
    psurf->GetDesc( &sd );

    DWORD dwBytesPerRow;
    switch ( sd.Format )
    {
    case D3DFMT_DXT1:
    case D3DFMT_DXT2:
    case D3DFMT_DXT3:
    case D3DFMT_DXT4:
    case D3DFMT_DXT5:
        dwBytesPerRow = 0; // magic value indicates texture's memory is contiguous
        break;
    case D3DFMT_A8R8G8B8:
        dwBytesPerRow = 4*sd.Width;
        break;
    case D3DFMT_R8G8B8:
        dwBytesPerRow = 3*sd.Width;
        break;
    case D3DFMT_A1R5G5B5:
    case D3DFMT_A4R4G4B4:
    case D3DFMT_R5G6B5:
        dwBytesPerRow = 2*sd.Width;
        break;
    default:
        return -1;
    }

    //--------------------------

    int count = ( *texture )->GetLevelCount();
    int i;

    for ( i = 0; i<count; i++ )
    {
        IDirect3DSurface8* dds;
        ( *texture )->GetSurfaceLevel( i, &dds );

        //D3DSURFACE_DESC desc;
        //dds->GetDesc( &desc );

        D3DLOCKED_RECT lr;
        if ( FAILED( dds->LockRect( &lr, NULL, 0 ) ) )
        {
            dds->Release();
            return -1;
        }
        {
            DWORD yp;
            BYTE* pbDst = ( BYTE* )lr.pBits;
            BYTE* pbSrc = ( BYTE* )bits;
            if ( dwBytesPerRow )
            {
                for ( yp = 0; yp<sd.Height>>i; yp++ )
                {
                    __asm
                    {
                        mov     esi, [pbSrc]
                        mov     edi, [pbDst]
                        mov     ecx, [dwBytesPerRow]
                        rep     movsb
                        mov     [pbSrc], esi
                    }
                    pbDst += lr.Pitch;
                }
            }
            else
            {
                __asm
                {
                    mov     esi, [pbSrc]
                    mov     edi, [lr.pBits]
                    mov     ecx, [sd.Size]
                    rep     movsb
                    mov     [pbSrc], esi
                }
                sd.Size >>= 2;
            }
            bits = pbSrc;
            dwBytesPerRow >>= 1;
        }
        if ( FAILED( dds->UnlockRect() ) )
        {
            dds->Release();
            return -1;
        }

        dds->Release();

        //--------------------------

        width>>=1;
        height>>=1;

        //--------------------------
    }

    //--------------------------

    psurf->Release();

    return 0;

    //--------------------------
}

//________________________________________________________________________________________________

void __stdcall                          QB_DirectX_SetRenderStateList( unsigned* statelist,
                                            int count )
{
    //--------------------------

    int i;
    for ( i = 0; i<count; i++ )
    {
        QB_DirectX_Device->SetRenderState( ( D3DRENDERSTATETYPE )statelist[0], statelist[1] );
        statelist += 2;
    }

    //--------------------------
}

//________________________________________________________________________________________________
