/*************************************************************************************************
   Fractal Landscape Generator / qb_texgen_fractal_landscape.cpp
  
   - Generate a fractal landscape using a FFT (the landscape is wrapping)
                                                                                 2002/08/27 18:21
/************************************************************************************************/
#include <stdafx.h>

//________________________________________________________________________________________________

#define MIN( x, y ) ( (x)<(y)?(x):(y) )

struct COMPLEX
{
    float                               real;
    float                               imag;
};

//________________________________________________________________________________________________

/*
   This computes an in-place complex-to-complex FFT
   x and y are the real and imaginary arrays of 256 points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform

     Formula: forward
                  N-1
                  ---
              1   \          - j k 2 pi n / N
      X(n) = ---   >   x(k) e                    = forward transform
              N   /                                n=0..N-1
                  ---
                  k=0

      Formula: reverse
                  N-1
                  ---
                  \          j k 2 pi n / N
      X(n) =       >   x(k) e                    = forward transform
                  /                                n=0..N-1
                  ---
                  k=0
*/
static void                             FFT( int dir,
                                            float* x,
                                            float* y )
{
    int i, i1, j, k, l, l1, l2;
    float c1, c2, tx, ty, t1, t2, u1, u2, z;

    // Do the bit reversal
    j = 0;
    for ( i = 0; i<255; i++ )
    {
        if ( i<j )
        {
            tx = x[i];
            ty = y[i];
            x[i] = x[j];
            y[i] = y[j];
            x[j] = tx;
            y[j] = ty;
        }
        k = 128;
        while ( k<=j )
        {
            j -= k;
            k >>= 1;
        }
        j += k;
    }

    // Compute the FFT
    c1 = -1.f;
    c2 = 0.f;
    l2 = 1;
    for ( l = 0; l<8; l++ )
    {
        l1 = l2;
        l2 <<= 1;
        u1 = 1.f;
        u2 = 0.f;
        for ( j = 0; j<l1; j++ )
        {
            for ( i = j; i<256; i += l2 )
            {
                i1 = i + l1;
                t1 = u1*x[i1] - u2*y[i1];
                t2 = u1*y[i1] + u2*x[i1];
                x[i1] = x[i] - t1;
                y[i1] = y[i] - t2;
                x[i] += t1;
                y[i] += t2;
            }
            z =  u1*c1 - u2*c2;
            u2 = u1*c2 + u2*c1;
            u1 = z;
        }
        c2 = sqrtf( ( 1.f - c1 )/2.f );
        if ( dir==1 )
            c2 = -c2;
        c1 = sqrtf( ( 1.f + c1 )/2.f );
    }

    // Scaling for forward transform
    if ( dir==1 )
    {
        for ( i = 0; i<256; i++ )
        {
            x[i] /= 256.f;
            y[i] /= 256.f;
        }
    }
}

//________________________________________________________________________________________________

/*
   Perform a 2D FFT inplace given a complex 2D array
   The direction dir, 1 for forward, -1 for reverse
*/
static void                             FFT2D( COMPLEX* c,
                                              int dir )
{
    int i, j;
    float real[256];
    float imag[256];

    // Transform the rows
    for ( j = 0; j<256; j++ )
    {
        for ( i = 0; i<256; i++ )
        {
            real[i] = c[( i<<8 ) + j].real;
            imag[i] = c[( i<<8 ) + j].imag;
        }
        FFT( dir, real, imag );
        for ( i = 0; i<256; i++ )
        {
            c[( i<<8 ) + j].real = real[i];
            c[( i<<8 ) + j].imag = imag[i];
        }
    }

    // Transform the columns
    for ( i = 0; i<256; i++ )
    {
        for ( j = 0; j<256; j++ )
        {
            real[j] = c[( i<<8 ) + j].real;
            imag[j] = c[( i<<8 ) + j].imag;
        }
        FFT( dir, real, imag );
        for ( j = 0; j<256; j++ )
        {
            c[( i<<8 ) + j].real = real[j];
            c[( i<<8 ) + j].imag = imag[j];
        }
    }
}

//________________________________________________________________________________________________

void                                    QB_TexGen_Fractal_Landscape( unsigned seed,
                                            float* target,
                                            float factor )
{
    //------------------------

    QB_Srand( seed );
    COMPLEX* pic = new COMPLEX[256*256];
    int i;
    for ( i = 0; i<256*256; i++ )
    {
        pic[i].real = ( 1.f - 2.f*QB_Rand()/32767.f );
        pic[i].imag = 0;
    }

    FFT2D( pic, 1 );

    int a = 0;
    for ( i = 0; i<256; i++ )
    {
        int j;
        for ( j = 0; j<256; j++ )
        {
            float r;

            r = ( i - 127.5f )*( i - 127.5f ) + ( j - 127.5f )*( j - 127.5f );
            pic[a].real = pic[a].real*cosf( r*3.141592f*2*6/( 127.5f*127.5f*2 ) )/r;
            pic[a].imag = pic[a].imag/r;

            a++;
        }
    }

    FFT2D( pic, -1 );

    float min = pic[0].real*pic[0].real + pic[0].imag*pic[0].imag;
    float max = min;
    for ( i = 0; i<256*256; i++ )
    {
        float v = pic[i].real*pic[i].real + pic[i].imag*pic[i].imag;
        if ( v<min )
            min = v;
        if ( v>max )
            max = v;
    }
    float coef = 2.f/( max - min );

    for ( i = 0; i<256*256; i++ )
    {
        float v = ( ( pic[i].real*pic[i].real + pic[i].imag*pic[i].imag - min )*coef );

        target[i] = ( v - 1.f )*factor;
    }

    delete[] pic;

    //------------------------
}

//________________________________________________________________________________________________
