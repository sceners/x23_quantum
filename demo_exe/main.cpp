/*************************************************************************************************
   Demo Exe / main.cpp
  
   - The main file... :)
                                                                                 2003/01/16 18:01
/************************************************************************************************/
#if _DEBUG
#define USE_DEBUG
#endif
#include "../demo_lib/_system/qb_system.h"
#include "../demo_lib/_system/qb_debug.h"
#include "resource.h"
//________________________________________________________________________________________________

// Windows stuff
HINSTANCE                               hInst;
char*                                   szTitle = "Razor 1911 Cracktro";
char*                                   szWindowClass = "Razor1911";
HWND                                    hWnd = NULL;
BOOL                                    g_bActive = FALSE;
bool                                    Ready = false;
int                                     Esc_Pressed = 0;

//_SOUND__________________________________________________________________________________________

void                                    Sound_Init();
void                                    Sound_Release();
void                                    Sound_Play();
void                                    Sound_Volume( int volume );

//_DEMO___________________________________________________________________________________________

int                                     Demo_Init();
void                                    Demo_Release();
int                                     Demo_Update();

//________________________________________________________________________________________________

void                                    QB_Exit( char* message )
{
    //------------------------

    DestroyWindow( hWnd );

    //------------------------
    // Release my datas
    Demo_Release();

    //------------------------
    // Release FMOD
    Sound_Release();

    //------------------------
    // Release DirectX
    QB_DirectX_Release();

    //------------------------
    // And release Windows
    CoUninitialize();
    ShowCursor( TRUE );

    if ( message )
    {
        MessageBox( NULL , message, szTitle, MB_OK );
    }

    //------------------------
    // To kill all thread
    ExitProcess(0);

    //------------------------
}

//________________________________________________________________________________________________

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    //------------------------

    switch( msg )
    {
    case WM_ACTIVATEAPP:
        g_bActive = wParam;
        if ( g_bActive==FALSE ) DestroyWindow( hWnd );
        break;

    case WM_PAINT:
        ValidateRect( hWnd, 0 );
        return TRUE;

    case WM_KEYDOWN:
        if ( wParam!=VK_ESCAPE ) break;
        Esc_Pressed++;
        break;
    case WM_CLOSE:
    case WM_DESTROY:
    case WM_QUIT:
        // Time to leave
        g_bActive = FALSE;
    }

    //------------------------

    return DefWindowProc( hWnd, msg, wParam, lParam );

    //------------------------
}

//________________________________________________________________________________________________

#if _DEBUG
int APIENTRY                            WinMain( HINSTANCE hInstance,
                                            HINSTANCE hPrevInstance,
                                            LPSTR lpCmdLine,
                                            int nCmdShow )
#else
int WINAPI                              WinMainCRTStartup()
#endif
{
    //------------------------

#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//    _CrtSetBreakAlloc( 348 );
#endif

    //------------------------

    hInst = GetModuleHandle( 0 ); 

    //------------------------
    // Prepare some windows stuff
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ( WNDPROC )MsgProc;
    wc.cbClsExtra = 0; wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL; wc.hCursor = 0; wc.hbrBackground = 0;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szWindowClass;
    RegisterClass( &wc );

    hWnd = CreateWindow( szWindowClass, szTitle,
        WS_POPUP | WS_VISIBLE,
        0, 0, 640, 480,
        NULL, NULL, hInst, NULL );

    ShowCursor( FALSE ); ShowWindow( hWnd, SW_SHOW ); UpdateWindow( hWnd );

    CoInitialize( 0 );

    //------------------------
    // Video init
    QB_DirectX_Init( hWnd );

    //------------------------
    // Demo init
    Demo_Init();

    //------------------------
    // Load the music
    Sound_Init();

    //------------------------
    // Show the window
    ShowWindow( hWnd, SW_SHOWDEFAULT ); UpdateWindow( hWnd );

    //------------------------
    // Enter the message loop
    MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    Sound_Play();
    //Sound_Volume( 0 );
    QB_Timer_Reset();
    while ( g_bActive!=FALSE )
    {
        if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            if ( Demo_Update() )
            {
                g_bActive = FALSE;
                DestroyWindow( hWnd );
            }
        }
    }

    //------------------------

    QB_Exit( NULL ); return 0;

    //------------------------
}

//________________________________________________________________________________________________
