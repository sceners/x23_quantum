/*************************************************************************************************
   qb_matrix.h
  
   - Matrix manipulation class
                                                                                 2002/10/01 12:38
/************************************************************************************************/
#ifndef __QB_MATRIX_H__
#define __QB_MATRIX_H__

//________________________________________________________________________________________________

#include "qb_vector.h"

//________________________________________________________________________________________________

struct QB_VECTOR;
struct QB_QUAT;
struct QB_MATRIX
{
    //_DATAS__________________________________________________________________________________________

    float m[4][4];

    //_OPERATORS______________________________________________________________________________________

    inline QB_VECTOR& operator[]( int row ) const { return ( QB_VECTOR& )( *m[row] ); }
    inline QB_VECTOR& i() const { return ( QB_VECTOR& )( *m[0] ); }
    inline QB_VECTOR& j() const { return ( QB_VECTOR& )( *m[1] ); }
    inline QB_VECTOR& k() const { return ( QB_VECTOR& )( *m[2] ); }
    inline QB_VECTOR& O() const { return ( QB_VECTOR& )( *m[3] ); }

    //------------------------

    QB_MATRIX operator * ( const QB_MATRIX& m2 ) const;

    QB_MATRIX& operator *= ( const QB_MATRIX& m );

    inline void operator *= ( const float f )
    {
        m[0][0] *= f; m[0][1] *= f; m[0][2] *= f;
        m[1][0] *= f; m[1][1] *= f; m[1][2] *= f;
        m[2][0] *= f; m[2][1] *= f; m[2][2] *= f;
    }

    //_FUNCTIONS______________________________________________________________________________________

    void                                Clear();
    void                                Inverse();
    void                                Identity();
    void                                Pre_Rotate_X( const float angle );
    void                                Pre_Rotate_Y( const float angle );
    void                                Pre_Rotate_Z( const float angle );
    void                                Rotation_XYZ( float ax,
                                            float ay,
                                            float az );
    void                                Set_Rotation_YPR( float yaw,
                                            float pitch,
                                            float roll );
    void                                Set_Rotation( const QB_QUAT& q );

    //________________________________________________________________________________________________
};

//________________________________________________________________________________________________

extern QB_MATRIX                        QB_MATRIX_IDENTITY;

//________________________________________________________________________________________________

#endif  __QB_MATRIX_H__