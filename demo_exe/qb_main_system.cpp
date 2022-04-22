/*************************************************************************************************
   qb_system.cpp
  
   - System functions
                                                                                 2002/06/05 16:50
/************************************************************************************************/

#include "qb_system.h"

//_SYSTEM_DATAS___________________________________________________________________________________

void *__cdecl operator new( unsigned int size ) 
{ 
    return ( char* ) HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, size + 32 ) + 16; 
}

//________________________________________________________________________________________________

void __cdecl operator delete( void* p )
{
    HeapFree( GetProcessHeap(), 0, ( char* ) p - 16 );
}

//________________________________________________________________________________________________
#if _DEBUG
#else
void __cdecl                            free( void* buf )
{
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
