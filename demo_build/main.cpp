/*************************************************************************************************
   Build / main.cpp
  
   - Main functions
                                                                                 2003/01/16 18:00
/************************************************************************************************/
#include "stdafx.h"

#include "resource.h"

//________________________________________________________________________________________________

HINSTANCE                               Main_Instance;
HWND                                    Main_Window;

char                                    Main_Title[] = "Razor Cracktro Builder v0.1";

BOOL                                    g_bActive = TRUE;

//________________________________________________________________________________________________

extern "C" unsigned char                exefile[];
extern "C" unsigned                     exefile_size;

//________________________________________________________________________________________________

static unsigned char                    Base_Text[] = "/0123456789:;\"()!?ABCDEFGHIJKLMNOPQRSTUVWXYZ[#]><'abcdefghijklmnopqrstuvwxyz.,-*@+\\=";

//________________________________________________________________________________________________

int APIENTRY                            WinMain( HINSTANCE hInstance,
                                            HINSTANCE hPrevInstance,
                                            LPSTR lpCmdLine,
                                            int nCmdShow )
{
    //------------------------
    // Init Windows
    Main_Instance = hInstance;
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc( 230 );
#endif

    //------------------------
    // Get text file
    char **l_argv = ++__argv;
    if ( !*l_argv )
    {
        MessageBox( NULL, "Can't open text file", Main_Title, MB_OK );
        return 0;
    }
    FILE* ftxt = fopen( *l_argv, "r" );
    if ( !ftxt )
    {
        MessageBox( NULL, "Can't open text file", Main_Title, MB_OK );
        return 0;
    }

    //------------------------
    // Find chunk
    unsigned char* buf = exefile;
    unsigned pos = 0;
    while ( ++pos<exefile_size && memcmp( ++buf, "QBCHUNK_TEXT", sizeof( "QBCHUNK_TEXT" ) ) );
    if ( pos==exefile_size )
    {
        MessageBox( NULL, "Can't find text chunk", Main_Title, MB_OK );
        fclose( ftxt );
        return -1;
    }

    //------------------------
    // Build index
    int index[256];
    int i;
    for ( i = 0; i<256; i++ )
    {
        index[i] = -1;

        int c = i;
        if ( i=='`' )
            c = '\'';
        if ( i=='{' )
            c = '(';
        if ( i=='}' )
            c = ')';

        int j;
        for ( j = 0; j<sizeof( Base_Text ) - 1; j++ )
        {
            if ( c==Base_Text[j] )
            {
                index[i] = j;
                break;
            }
        }
    }

    //------------------------
    // Reindex the text and put it in the exe file
    char* dest = ( char* )&exefile[pos + 4];
    int* lines = ( int* )&exefile[pos];
    for ( lines[0] = 0; lines[0]<20; lines[0]++ )
    {
        memset( dest, -1, 40 );
        dest += 40;
    }
    char* message = new char[16*1024];
    while ( !feof( ftxt ) && lines[0]<1638 )
    {
        memset( message, 0, 16*1024 );
        fgets( message, 16*1024, ftxt );

        for ( i = 0; i<40; i++ )
            dest[i] = index[message[i]];
        lines[0]++;
        dest += 40;
    }
    delete[] message;
    fclose( ftxt );

    //------------------------
    // Get program path
    char path_buffer[_MAX_PATH + 3];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath( _pgmptr, drive, dir, fname, ext );

    //------------------------
    // Save unpack quantum.exe
    _makepath( path_buffer, drive, dir, "quantum", "exe" );
    FILE* f = fopen( path_buffer, "wb" );
    if ( !f )
    {
        MessageBox( NULL, "Can't create quantum.exe", Main_Title, MB_OK );
        return -1;
    }

    fwrite( exefile, exefile_size, 1, f );
    fclose( f );

    //------------------------
    // Pack quantum.exe
    char path_buffer2[ _MAX_PATH ];

    _makepath( path_buffer2, drive, dir, NULL, NULL );

    SHELLEXECUTEINFO see;

    memset( &see, 0, sizeof( see ) );
    see.cbSize = sizeof( see );
    see.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS;

    see.lpFile          = ( LPCTSTR )"upx.exe";
    see.lpParameters    = ( LPCTSTR )"--best quantum.exe";
    see.lpDirectory     = ( LPCTSTR )path_buffer2;
    see.hwnd            = NULL;
    see.nShow           = SW_SHOWNORMAL;
    see.hProcess;

    BOOL ok = ShellExecuteEx( &see );

    if ( ok )
    {
        DWORD wait = WaitForSingleObject( see.hProcess, INFINITE );

        if ( wait==WAIT_OBJECT_0 )
        {
            DWORD rc;
            GetExitCodeProcess( see.hProcess, &rc );
        }
    }
    else
    {
        MessageBox( NULL, "Can't pack quantum.exe, deleting", Main_Title, MB_OK );
        _makepath( path_buffer2, drive, dir, "quantum", "exe" );
        remove( path_buffer2 );
    }

    //------------------------

    return 0;

    //------------------------
}

//________________________________________________________________________________________________
