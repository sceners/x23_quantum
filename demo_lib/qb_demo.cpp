/*************************************************************************************************
   Demo Lib / qb_demo.cpp
  
   - The demo management file... :)
                                                                                 2002/10/03 18:12
/************************************************************************************************/
#include "stdafx.h"

#include "_fx/qb_torus_landscape.h"
#include "_fx/qb_sprite_nrj.h"
#include "_fx/qb_logo.h"
#include "_fx/qb_filter.h"
#include "_fx/qb_textscroller.h"

//_GLOBAL_CAMERA__________________________________________________________________________________

QB_CAMERA                               Demo_Camera;

//_GLOBAL_TIME____________________________________________________________________________________

float                                   Time = 0.f;

//_End_System_____________________________________________________________________________________

extern BOOL                             g_bActive;
extern int                              Esc_Pressed;
float                                   Leave_Time = 6.f;
static int                              Slow_Exit = 1;
void                                    Sound_Volume( int volume );

//________________________________________________________________________________________________

#include "_fmod/minifmod.h"
//struct FMUSIC_MODULE;
extern FMUSIC_MODULE*                   Module;
int                                     Mod_Pos = 0;
static int                              old_mod_pos = 0;

static float                            Text_Time = 4.f;

//________________________________________________________________________________________________

void                                    Demo_Release()
{
    //------------------------

    Release_Textures();
    Release_Torus_Landscape();
    Release_Sprite_NRJ();
    Release_Logo();
    Release_Filter();
    Release_Textscroller();

    //------------------------
}

//________________________________________________________________________________________________

int                                     Demo_Update()
{
    //------------------------
    // Clear the backbuffer and the zbuffer
//    QB_DirectX_Device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
//        /*0x5C2B7B*/0, 1.0f, 0 );

    QB_Timer_Update();

    //------------------------
    // Begin the scene
    QB_DirectX_Device->BeginScene();

    Mod_Pos = FMUSIC_GetOrder( Module )<<8;
    Mod_Pos |= FMUSIC_GetRow( Module );
    //Sound_Volume( 0 );

    //------------------------
    // do our stuff
    QB_DirectX_Device->Clear( 0, NULL, /*D3DCLEAR_TARGET | */D3DCLEAR_ZBUFFER,
        0, 1.0f, 0 );

    Update_Torus_Landscape();
    Update_Sprite_NRJ();
    Update_Logo();
    if ( Text_Time==0.f )
        Update_Textscroller();
    Update_Filter();

    if ( Esc_Pressed )
    {
        Leave_Time -= QB_Timer_Request;

        int volume = 64;
        if ( Leave_Time<=5.f )
            volume = ( int )( ( Leave_Time - 2.5f )*64/2.5f );
        if ( Leave_Time<2.5f )
            volume = 0;
        Sound_Volume( volume );

        if ( Leave_Time<0.f || Esc_Pressed>2 )
            Slow_Exit = 0;
    }

    if ( Esc_Pressed && !Slow_Exit )
    {
        g_bActive = FALSE;
    }

    if ( old_mod_pos!=Mod_Pos )
        old_mod_pos = Mod_Pos;


    //------------------------
    // End the scene
    Frame_Rate();
    Printf( "%x / %x", FMUSIC_GetOrder(Module), FMUSIC_GetRow(Module));
    QB_DirectX_Device->EndScene( );

    //------------------------
    // Present the backbuffer contents to the display
    QB_DirectX_Device->Present( NULL, NULL, NULL, NULL );

    Time += QB_Timer_Request;

    Text_Time -= QB_Timer_Request;
    if ( Text_Time<0.f )
        Text_Time = 0.f;

    //------------------------

    return 0;
}

//________________________________________________________________________________________________

int                                     Demo_Init()
{
    //------------------------
    // Create the scene geometry
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    QB_DirectX_Device->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
    QB_DirectX_Device->SetRenderState( D3DRS_LIGHTING, FALSE );
    QB_DirectX_Device->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW( 0.00f ) );
    QB_DirectX_Device->SetRenderState( D3DRS_POINTSCALE_A, FtoDW( 0.00f ) );
    QB_DirectX_Device->SetRenderState( D3DRS_POINTSCALE_B, FtoDW( 0.00f ) );
    QB_DirectX_Device->SetRenderState( D3DRS_POINTSCALE_C, FtoDW( 1.00f ) );

    // reset random at a random pos :)
    LARGE_INTEGER time;
    QueryPerformanceCounter( &time );
    QB_Srand( time.LowPart ); // a default initial seed is used

    //------------------------
    // Init my stuff
    int state = 0;

    state |= Init_Textures();
    state |= Init_Torus_Landscape();
    state |= Init_Sprite_NRJ();
    state |= Init_Logo();
    state |= Init_Filter();
    state |= Init_Textscroller();

    if ( state )
    {
        QB_Exit( "Can't init demo plugins" );
        return -1;
    }

    Demo_Camera.Init();
    Demo_Camera.Clip_Far = 1500.f;
    Demo_Camera.Clip_Near = 1.f;

    //------------------------

    return 0;

    //------------------------
}

//________________________________________________________________________________________________
