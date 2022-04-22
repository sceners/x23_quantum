/*************************************************************************************************
   qb_color.h
  
   - color manipulation structure
                                                                                 2002/11/14 16:52
/************************************************************************************************/
#ifndef __QB_COLOR_H__
#define __QB_COLOR_H__

//________________________________________________________________________________________________

struct QB_COLOR
{
    //_DATAS__________________________________________________________________________________________

    unsigned char                       b;
    unsigned char                       g;
    unsigned char                       r;
    unsigned char                       a;

    //_CONSTRUCTORS___________________________________________________________________________________

    inline void Set( unsigned char c ) { b = c; g = c; r = c; a = c; }
    inline void Set( unsigned char _b, unsigned char _g, unsigned char _r, unsigned char _a )
    { b = _b; g = _g; r = _r; a = _a; }

    //_OPERATORS______________________________________________________________________________________

    inline unsigned char& operator [] ( int i ) { return ( &a )[i]; }

    inline QB_COLOR operator - () const { QB_COLOR v; v.a = -a; v.r = -r; v.g = -g; v.b = -b; return v; }
    inline QB_COLOR operator - ( const QB_COLOR& v ) const { QB_COLOR w; w.a = a - v.a; w.r = r - v.r; w.g = g - v.g; w.b = b - v.b; return w; }
//    inline void operator = ( const QB_COLOR& v ) { a = v.a; r = v.r; g = v.g; b = v.b; }

    inline void operator = ( const int color ) { *( int* )&b = color; }
    inline void operator = ( const unsigned int color ) { *( unsigned int* )&b = color; }
    inline operator unsigned () const { return *( unsigned* )this; }

    //________________________________________________________________________________________________
};

//________________________________________________________________________________________________

#endif  __QB_COLOR_H__