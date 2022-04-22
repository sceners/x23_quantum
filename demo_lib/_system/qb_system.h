/*************************************************************************************************
   Demo Lib / qb_system.h
  
   - All external function, parameters... definitions
                                                                                 2002/11/14 16:50
/************************************************************************************************/
#ifndef __QB_SYSTEM_H__
#define __QB_SYSTEM_H__

//________________________________________________________________________________________________

#include <d3d8.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WIN32_WINNT 0x0400

//_CASTS__________________________________________________________________________________________

#define _uc unsigned char
#define _c char
#define _u unsigned
#define _i int
#define _s short
#define _us unsigned short

//_SYSTEM_________________________________________________________________________________________

void                                    QB_Exit( char* message );

//_TIMER__________________________________________________________________________________________

void __stdcall                          QB_Timer_Reset();
void __stdcall                          QB_Timer_Update();
extern float                            Timer_Delta;
#define QB_Timer_Request Timer_Delta
extern float                            Time; // defined in main.cpp

//_MATHS__________________________________________________________________________________________
#ifndef _INC_MATH
extern "C" double                       fabs( double a );
#pragma intrinsic ( fabs )
#define fabsf( a ) ( ( float )fabs( ( float )a ) )

extern "C"
{
    double __stdcall                    sqrt( double f );
    double __stdcall                    cos( double d );
    double __stdcall                    sin( double d );
    double __stdcall                    tan( double a );
    double __stdcall                    asin( double d );
    double __stdcall                    atan2( double d, double b );
    double __stdcall                    pow( double d, double e );
    long _declspec ()                   _ftol();
}
#define cosf( a ) ( ( float )cos( ( double )( a ) ) )
#define sinf( a ) ( ( float )sin( ( double )( a ) ) )
#define tanf( a ) ( ( float )tan( ( double )( a ) ) )
#define sqrtf( a ) ( ( float )sqrt( ( double )( a ) ) )
#define atan2f( a, b ) ( ( float )atan2( ( double )( a ), ( double )( b ) ) )

extern "C" double                       floor( double x );
#define floorf( a ) ( ( float )floor( ( double )( a ) ) )
#endif
inline DWORD FtoDW( FLOAT f ) { return *( ( DWORD* )&f ); }

//_RANDOM_________________________________________________________________________________________

extern "C"
{
    void                                QB_Srand( unsigned seed );
    int                                 QB_Rand();
}
#define rand() QB_Rand()

//_MEMORY_________________________________________________________________________________________

void *__cdecl operator new( unsigned int size );
void __cdecl operator delete( void* p );
#if _DEBUG
#else
void __cdecl                            free( void* buf );
void* __cdecl                           malloc( size_t size );
void* __cdecl                           calloc( size_t size1,
                                            size_t size2 );
#endif

int __cdecl                             atexit( void ( __cdecl* ) () );

//_DIRECTX________________________________________________________________________________________

extern DWORD                            QB_DirectX_Tnl;
extern LPDIRECT3D8                      QB_DirectX; // Used to create the D3DDevice
extern LPDIRECT3DDEVICE8                QB_DirectX_Device; // Our rendering device

void                                    QB_DirectX_Release();
void                                    QB_DirectX_Init( HWND hwnd );
int __stdcall                           QB_DirectX_Load( unsigned char* bits,
                                            LPDIRECT3DTEXTURE8* texture,
                                            int width,
                                            int height,
                                            D3DFORMAT format,
                                            int mipmap );
void __stdcall                          QB_DirectX_SetRenderStateList( unsigned* statelist,
                                            int count );
#define QB_DirectX_SetRenderState( a ) QB_DirectX_SetRenderStateList( a, sizeof( a )>>3 )

//________________________________________________________________________________________________

#endif __QB_SYSTEM_H__