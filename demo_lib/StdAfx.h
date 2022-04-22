// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7863262D_CC55_45A9_BF1B_D64E127EF9B0__INCLUDED_)
#define AFX_STDAFX_H__7863262D_CC55_45A9_BF1B_D64E127EF9B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _DEBUG
#define USE_DEBUG
#endif
#include "_system/qb_system.h"
#include "_system/qb_debug.h"

#include "qb_textures.h"

#include "_system/qb_vector.h"
#include "_system/qb_plane.h"
#include "_system/qb_sphere.h"
#include "_system/qb_color.h"
#include "_system/qb_matrix.h"
#include "_system/qb_camera.h"
#include "_system/qb_quat.h"


// TODO: reference additional headers your program requires here
extern QB_CAMERA                        Demo_Camera;
extern int                              Mod_Pos;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7863262D_CC55_45A9_BF1B_D64E127EF9B0__INCLUDED_)
