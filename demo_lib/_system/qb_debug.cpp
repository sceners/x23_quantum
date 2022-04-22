/*************************************************************************************************
   Demo Lib / qb_debug.cpp
  
   - Debug functions
                                                                                 2002/09/19 11:44
/************************************************************************************************/
#include "stdafx.h"

#ifdef USE_DEBUG
#include <stdio.h>
#include <stdarg.h>

//_DATAS__________________________________________________________________________________________

int                                     Current_Frame = 0;
float                                   Average_Framerate = 0.f;

QB_DEBUG*                               QB_Debug = NULL;
static int                              QB_Debug_Ready = -1;

//________________________________________________________________________________________________

struct QB_UV
{
    float                               u[4];
    float                               v[4];
    int                                 size;
};

struct QB_VERTEX2D
{
    float                               x;
    float                               y;
    float                               z;
    float                               w;
    float                               u;
    float                               v;
};

#define QB_VERTEX2D_FVF ( D3DFVF_XYZRHW | D3DFVF_TEX1 )

//________________________________________________________________________________________________

QB_DEBUG::QB_DEBUG()
{
    //------------------------

    Texture = NULL;
    UV = NULL;

    //------------------------

    Pos = 0.f;
    Frame = Current_Frame;

    //------------------------
}

//________________________________________________________________________________________________

QB_DEBUG::~QB_DEBUG( )
{
    //------------------------

    if ( Texture )
        Texture->Release();
    if ( UV )
        delete[] UV;

    //------------------------
}

//________________________________________________________________________________________________

int                                     QB_DEBUG::Init()
{
    //------------------------

    if ( Prepare_Font() )
        return -1;

    //------------------------

    return 0;
}

//________________________________________________________________________________________________

int                                     QB_DEBUG::Prepare_Font()
{
    DWORD m_dwTexWidth = 1024;
    DWORD m_dwTexHeight = 1024;
    float m_fTextScale = 1.f;

    UV = new QB_UV[256];

    // If requested texture is too big, use a smaller texture and smaller font,
    // and scale up when rendering.
    D3DCAPS8 d3dCaps;
    QB_DirectX_Device->GetDeviceCaps( &d3dCaps );

    if( m_dwTexWidth>d3dCaps.MaxTextureWidth )
    {
        m_fTextScale = ( FLOAT )d3dCaps.MaxTextureWidth/( FLOAT )m_dwTexWidth;
        m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
    }

    // Create a new texture for the font
    if ( QB_DirectX_Device->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1, 0, D3DFMT_R5G6B5, D3DPOOL_MANAGED, &Texture )!=D3D_OK )
        return -1;

    // Prepare to create a bitmap
    DWORD* pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader, sizeof( BITMAPINFOHEADER ) );
    bmi.bmiHeader.biSize        = sizeof( BITMAPINFOHEADER );
    bmi.bmiHeader.biWidth       = ( int )m_dwTexWidth;
    bmi.bmiHeader.biHeight      = -( int )m_dwTexHeight;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    // Create a DC and a bitmap for the font
    HDC     hDC       = CreateCompatibleDC( NULL );
    HBITMAP hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS, ( VOID** )&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT nHeight    = -MulDiv( 16, ( INT )(GetDeviceCaps( hDC, LOGPIXELSY )*m_fTextScale ), 72 );
    DWORD dwBold   = FW_BOLD /*FW_NORMAL*/;
    DWORD dwItalic = FALSE;
    HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, "Times New Roman" );
    if ( NULL==hFont )
        return -1;

    SelectObject( hDC, hbmBitmap );
    SelectObject( hDC, hFont );

    // Set text properties
    SetTextColor( hDC, RGB( 255, 255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );

    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.
    DWORD x = 0;
    DWORD y = 0;
    char str[2] = "x";
    SIZE size;

    for( int c = 0; c<=255; c++ )
    {
        str[0] = c;
        GetTextExtentPoint32( hDC, str, 1, &size );

        if ( ( DWORD )( x +size.cx + 1 )>m_dwTexWidth )
        {
            x  = 0;
            y += size.cy+1;
        }

        ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL );

        UV[c].u[ 0 ] = UV[c].u[ 2 ] = ((FLOAT)(x+0))/m_dwTexWidth;
        UV[c].v[ 0 ] = UV[c].v[ 1 ] = ((FLOAT)(y+0))/m_dwTexHeight;
        UV[c].u[ 1 ] = UV[c].u[ 3 ] = ((FLOAT)(x+0+size.cx))/m_dwTexWidth;
        UV[c].v[ 2 ] = UV[c].v[ 3 ] = ((FLOAT)(y+0+size.cy))/m_dwTexHeight;

        UV[c].size = size.cx;
        x += size.cx + 1;
    }

    // Lock the surface and write the alpha values for the set pixels
    D3DLOCKED_RECT d3dlr;
    Texture->LockRect( 0, &d3dlr, 0, 0 );
    WORD* pDst16 = ( WORD* )d3dlr.pBits;
    unsigned char bAlpha; // 4-bit measure of pixel intensity

    for ( y = 0; y<m_dwTexHeight; y++ )
    {
        for ( x = 0; x<m_dwTexWidth; x++ )
        {
            bAlpha = ( unsigned char )( pBitmapBits[m_dwTexWidth*y + x]&0xff );
            *pDst16++ = ( ( bAlpha&~7)<<8 ) | ( ( bAlpha&~3 )<<3 ) | ( bAlpha>>3 );
        }
    }

    // Done updating texture, so clean up used objects
    Texture->UnlockRect( 0 );
    DeleteObject( hbmBitmap );
    DeleteDC( hDC );
    DeleteObject( hFont );

    return 0;
}

//________________________________________________________________________________________________

static char                             Message[16384];
void                                    QB_DEBUG::QPrintf( const char *fmt, ... )
{
    //------------------------

    if ( QB_Debug_Ready && QB_Debug==NULL )
    {
        QB_Debug = new QB_DEBUG;
        QB_Debug_Ready = QB_Debug->Init();
    }

    if ( QB_Debug_Ready )
        return;

    if ( Current_Frame!=QB_Debug->Frame )
    {
        QB_Debug->Frame = Current_Frame;
        QB_Debug->Pos = 0.f;
    }

    //------------------------

    va_list	ap;
    char* message = Message;

    //-----------------------

    if ( fmt==NULL )
        return;

    //-----------------------

    va_start( ap, fmt );
    vsprintf( message, fmt, ap );
    va_end( ap );

    //------------------------

    QB_DirectX_Device->SetTexture( 0, QB_Debug->Texture );
/*
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
*/
    //------------------------

    QB_DirectX_Device->SetRenderState( D3DRS_LIGHTING, FALSE );
    QB_DirectX_Device->SetRenderState( D3DRS_FOGENABLE, FALSE );

    //------------------------

    QB_DirectX_Device->SetRenderState( D3DRS_ZENABLE, TRUE );
    QB_DirectX_Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    //------------------------

    QB_DirectX_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    QB_DirectX_Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
    QB_DirectX_Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );

    //------------------------

    QB_DirectX_Device->SetVertexShader( QB_VERTEX2D_FVF );

    float x = 0;
    QB_VERTEX2D vb[4];
    vb[0].w = 1.f;
    vb[0].z = 0.f;
    vb[0].y = QB_Debug->Pos;

    vb[1].w = 1.f;
    vb[1].z = 0.f;
    vb[1].y = QB_Debug->Pos;

    vb[2].w = 1.f;
    vb[2].z = 0.f;
    vb[2].y = QB_Debug->Pos + 16;

    vb[3].w = 1.f;
    vb[3].z = 0.f;
    vb[3].y = QB_Debug->Pos + 16;

    while ( *message )
    {
        vb[0].x = x;
        vb[1].x = x + QB_Debug->UV[*message].size;
        vb[2].x = x;
        vb[3].x = x + QB_Debug->UV[*message].size;

        for ( int i = 0; i<4; i++ )
        {
            vb[ i ].u =  + QB_Debug->UV[*message].u[i];
            vb[ i ].v =  + QB_Debug->UV[*message].v[i];
        }

        QB_DirectX_Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP,
            2,
            vb,
            sizeof( QB_VERTEX2D ) );

        x += QB_Debug->UV[*message].size;
        message++;
    }

    QB_Debug->Pos += 16;

    QB_DirectX_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

    //------------------------
}

//________________________________________________________________________________________________

void                                    QB_DEBUG::QFrame_Rate()
{
    //------------------------

    float rate = QB_Timer_Request;
    if ( rate!=0.f )
    {
        if ( Average_Framerate==0.f )
        {
            Average_Framerate = rate;
            Current_Frame = 1;
        }
        else
        {
            Average_Framerate += rate;
            Current_Frame++;
        }

        QPrintf( "Fps: %04.0f (%04.0f)", Current_Frame/Average_Framerate, 1.f/rate );
        QPrintf( "Time: %06.2f", Time );
    }


    //------------------------
}

//________________________________________________________________________________________________

void                                    QB_DEBUG::QFrame_Rate_Reset()
{
    //------------------------

    Average_Framerate = 0.f;

    //------------------------
}

#endif
//________________________________________________________________________________________________
