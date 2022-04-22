/*************************************************************************************************
   Demo Lib / qb_system.cpp
  
   - System functions
                                                                                 2002/10/03 11:57
/************************************************************************************************/
#include "stdafx.h"

//_TIMER_DATAS____________________________________________________________________________________

static LARGE_INTEGER                    Last_Time;
static LARGE_INTEGER                    Frequency;
float                                   Timer_Delta;

//________________________________________________________________________________________________

void __stdcall                          QB_Timer_Reset( )
{
    QueryPerformanceFrequency( &Frequency );

    QueryPerformanceCounter( &Last_Time );

    Timer_Delta = 0.f;
}

//________________________________________________________________________________________________

void __stdcall                          QB_Timer_Update( )
{
    LARGE_INTEGER time;
    QueryPerformanceCounter( &time );
    unsigned delta;

    LARGE_INTEGER old = Last_Time;
    __asm
    {
        mov     eax, dword ptr [ time ]
        sub     eax, dword ptr [ Last_Time ]
        mov     edx, dword ptr [ time + 4 ]
        sbb     edx, dword ptr [ Last_Time + 4 ]
        shld    edx,eax,16
        shl     eax,16
        mov     ebx, dword ptr [ Frequency ]
        div     ebx
        mov     [ delta ], eax
        mul     ebx
        shrd    eax,edx,16
        shr     edx,16
        add     dword ptr [ Last_Time ], eax
        adc     dword ptr [ Last_Time + 4 ], edx
    }

    if ( old.QuadPart>Last_Time.QuadPart )
    {
    //if ( delta&0x80000000 )
        //_asm int 3;
        delta = 0 - delta;
    }
    Timer_Delta = delta/65536.f;
}

//_RANDOM_DATAS___________________________________________________________________________________
// Period parameters
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   // constant vector a
#define UPPER_MASK 0x80000000 // most significant w-r bits
#define LOWER_MASK 0x7fffffff // least significant r bits

// Tempering parameters
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U( y )  ( y>> 11 )
#define TEMPERING_SHIFT_S( y )  ( y<<7 )
#define TEMPERING_SHIFT_T( y )  ( y<<15 )
#define TEMPERING_SHIFT_L( y )  ( y>>18 )

static unsigned long                    mt[N]; // the array for the state vector
static int                              mti = N+1; // mti==N+1 means mt[N] is not initialized

//________________________________________________________________________________________________
// initializing the array with a NONZERO seed
void                                        QB_Srand( unsigned seed )
{
    // setting initial seeds to mt[N] using
    // the generator Line 25 of Table 1 in
    // [KNUTH 1981, The Art of Computer Programming
    //    Vol. 2 (2nd Ed.), pp102]
    mt[0] = ( seed + 1 )&0xffffffff;
    for ( mti = 1; mti<N; mti++ )
        mt[mti] = ( 69069*mt[mti - 1] )&0xffffffff;
}

//________________________________________________________________________________________________

int                                         QB_Rand()
{
    unsigned long y;
    static unsigned long mag01[2] = { 0x0, MATRIX_A };
    // mag01[x] = x*MATRIX_A  for x = 0, 1

    if (mti >= N) { // generate N words at one time
        int kk;

        if ( mti==N + 1 )   // if sgenrand() has not been called,
        {
            //LARGE_INTEGER time;
            //QueryPerformanceCounter( &time );
            //QB_Srand(/*4357*/time.LowPart ); // a default initial seed is used
            QB_Srand( 4357 );
        }

        for ( kk = 0; kk<N - M; kk++ )
        {
            y = ( mt[kk]&UPPER_MASK ) | ( mt[kk + 1]&LOWER_MASK );
            mt[kk] = mt[kk + M]^( y>>1 )^mag01[y&0x1];
        }
        for ( ; kk<N - 1; kk++ )
        {
            y = ( mt[kk]&UPPER_MASK ) | ( mt[kk + 1]&LOWER_MASK );
            mt[kk] = mt[kk + ( M - N )]^( y>>1 )^mag01[y&0x1];
        }
        y = ( mt[N - 1]&UPPER_MASK ) | ( mt[0]&LOWER_MASK );
        mt[N - 1] = mt[M - 1]^( y>>1 )^mag01[y&0x1];

        mti = 0;
    }
  
    y = mt[mti++];
    y ^= TEMPERING_SHIFT_U( y );
    y ^= TEMPERING_SHIFT_S( y )&TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T( y )&TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L( y );

    return y&32767; 
}

//_SYSTEM_DATAS___________________________________________________________________________________

void *__cdecl operator new( unsigned int size ) 
{ 
    return ( char* ) HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, size + 32 ) + 16; 
}

//________________________________________________________________________________________________

void __cdecl operator delete( void* p )
{
    if ( !p )
        return;
    HeapFree( GetProcessHeap(), 0, ( char* ) p - 16 );
}

//________________________________________________________________________________________________
#if _DEBUG
#else
void __cdecl                            free( void* buf )
{
    if ( !buf )
        return;
    char* ptr = ( char* )buf;
    delete[] buf;
}

//________________________________________________________________________________________________

void* __cdecl                           malloc( size_t size )
{
    return calloc( size, 1 );
}

//________________________________________________________________________________________________

void* __cdecl                           calloc( size_t size1,
                                            size_t size2 )
{
    int newsize = size1*size2;
    char* newbuf = new char[newsize];
    __asm
    {
        mov     edi, [newbuf]
        mov     ecx, [newsize]
        xor     eax, eax
        rep     stosb
    }

    return ( void* )newbuf;
}

#endif
//________________________________________________________________________________________________

int __cdecl                             atexit( void ( __cdecl * )() )
{
    return 0;
}

//________________________________________________________________________________________________

#if _DEBUG
#else
extern "C" const int                    _fltused = 0;
#endif

//________________________________________________________________________________________________
