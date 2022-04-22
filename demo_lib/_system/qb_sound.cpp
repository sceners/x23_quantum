/*************************************************************************************************
   Demo Lib / qb_sound.cpp
  
   - The Fmod functions
                                                                                 2002/10/03 11:55
/************************************************************************************************/
#include "stdafx.h"

#include "../_fmod/minifmod.h"

//________________________________________________________________________________________________
// it's always a pleasure if i don't listen the music always when i work :)
//#define NO_SOUND

//_FMOD___________________________________________________________________________________________

FMUSIC_MODULE*                          Module = NULL;
struct MEMFILE
{
    int                                 length;
    int                                 pos;
    void*                               data;
};

typedef struct FMUSIC_MODULE_EXT
{
    void*                               pattern;                // patterns array for this song
    void*                               instrument;             // instrument array for this song (not used in MOD/S3M)
    int                                 mixer_samplesleft;
    int                                 mixer_samplespertick;

    // ====================================================================================
    // 6 bytes read here in the loader for size, dont alter order or insert!!!
    short                               numorders;              // number of orders (song length)
    short                               restart;                // restart position
    short                               numchannels;            // number of channels
    // ====================================================================================

    int                                 numpatterns;            // number of physical patterns
    int                                 numpatternsmem;         // number of allocated patterns

    // ====================================================================================
    // 256+8 6 bytes read here in the loader for size, dont alter order or insert!!!
    short                               numinsts;               // number of instruments
    unsigned short                      flags;                  // flags such as linear frequency, format specific quirks etc
    short                               defaultspeed;
    short                               defaultbpm;
    unsigned char                       orderlist[256];         // pattern playing order list
    // ====================================================================================

    unsigned char                       defaultglobalvolume;

    int                                 globalvolume;           // global mod volume
    unsigned char                       globalvsl;              // global mod volume
    int                                 tick;                   // current mod tick
    int                                 speed;                  // speed of song in ticks per row
    int                                 bpm;                    // speed of song in bpm
    int                                 row;                    // current row in pattern
    int                                 order;                  // current song order position
    int                                 patterndelay;           // pattern delay counter
    int                                 nextrow;                // current row in pattern
    int                                 nextorder;              // current song order position
    int                                 time_ms;                // time passed in seconds since song started
} FMUSIC_MODULE_EXT;

//________________________________________________________________________________________________

extern "C" unsigned char                module[];
extern "C" unsigned                     module_size;

//________________________________________________________________________________________________

void                                    Sound_Release()
{
    //------------------------
    // Release FMOD
    if ( Module )
    {
        FMUSIC_FreeSong( Module );
    }
    FSOUND_Close( );

    //------------------------
}

//________________________________________________________________________________________________

static unsigned int                     memopen( char* name )
{
    //------------------------

    MEMFILE* memfile = new MEMFILE;

    // hey look some load from resource code!
    memfile->data = module;
    memfile->length = module_size;
    memfile->pos = 0;

    return ( unsigned int )memfile;

    //------------------------
}

//________________________________________________________________________________________________

static void                             memclose( unsigned int handle )
{
    //------------------------

    MEMFILE* memfile = ( MEMFILE* )handle;

    delete memfile;

    //------------------------
}

//________________________________________________________________________________________________

static int                              memread( void* buffer,
                                            int size,
                                            unsigned int handle )
{
    //------------------------

    MEMFILE* memfile = ( MEMFILE* )handle;

    if ( ( memfile->pos + size )>=memfile->length )
        size = memfile->length - memfile->pos;

    memcpy( buffer, ( char* )memfile->data + memfile->pos, size );
    memfile->pos += size;

    return size;

    //------------------------
}

//________________________________________________________________________________________________

static void                             memseek( unsigned int handle,
                                            int pos,
                                            signed char mode )
{
    //------------------------

    MEMFILE* memfile = ( MEMFILE* )handle;

    if ( mode==SEEK_SET )
        memfile->pos = pos;
    else if ( mode==SEEK_CUR )
        memfile->pos += pos;
    else if ( mode==SEEK_END )
        memfile->pos = memfile->length + pos;

    if ( memfile->pos>memfile->length )
        memfile->pos = memfile->length;

    //------------------------
}

//________________________________________________________________________________________________

static int                              memtell( unsigned int handle )
{
    //------------------------

    MEMFILE* memfile = ( MEMFILE* )handle;

    return memfile->pos;

    //------------------------
}

//________________________________________________________________________________________________

void                                    Sound_Init()
{
    //------------------------
    // Load the music
    FSOUND_File_SetCallbacks( memopen, memclose, memread, memseek, memtell );
    if ( !FSOUND_Init( 44100, 0 ) )
    {
        MessageBox( NULL, "Error during initialization of FMOD", "Razor 1911", MB_OK );
    }
    else
        Module = FMUSIC_LoadSong( NULL, NULL );

    //------------------------
}

//________________________________________________________________________________________________

void                                    Sound_Play()
{
    //------------------------

    if ( Module )
    {
        FMUSIC_PlaySong( Module );

#ifdef NO_SOUND
        FMUSIC_MODULE_EXT* module = ( FMUSIC_MODULE_EXT* )Module;
        module->globalvolume = 0;
#endif
    }

    //------------------------
}

//________________________________________________________________________________________________

void                                    Sound_Volume( int volume )
{
    //------------------------

#ifndef NO_SOUND
    if ( Module )
    {
        FMUSIC_MODULE_EXT* module = ( FMUSIC_MODULE_EXT* )Module;
        module->globalvolume = volume;
    }
#endif

    //------------------------
}
//________________________________________________________________________________________________
