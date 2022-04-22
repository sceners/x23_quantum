# Microsoft Developer Studio Project File - Name="demo_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=demo_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "demo_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "demo_lib.mak" CFG="demo_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "demo_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "demo_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "demo_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /Ox /Og /Oi /Os /Op /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "demo_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "demo_lib - Win32 Release"
# Name "demo_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "fmod source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_fmod\Fmusic.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\_fmod\Fsound.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\_fmod\mixer_clipcopy.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\_fmod\mixer_fpu_ramp.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\_fmod\music_formatxm.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\_fmod\system_file.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "system source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_system\qb_camera.cpp
# End Source File
# Begin Source File

SOURCE=.\_system\qb_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\_system\qb_dx8.cpp
# End Source File
# Begin Source File

SOURCE=.\_system\qb_floor.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\_system\qb_matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\_system\qb_system.cpp
# End Source File
# End Group
# Begin Group "fx source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_fx\qb_filter.cpp
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_logo.cpp
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_sprite_nrj.cpp
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_texgen_fractal_landscape.cpp
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_textscroller.cpp
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_torus_landscape.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\qb_demo.cpp
# End Source File
# Begin Source File

SOURCE=.\_system\qb_sound.cpp
# End Source File
# Begin Source File

SOURCE=.\qb_textures.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "fmod header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_fmod\minifmod.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\Mixer.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\mixer_clipcopy.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\mixer_fpu_ramp.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\Music.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\music_formatxm.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\Sound.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\system_file.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\system_memory.h
# End Source File
# Begin Source File

SOURCE=.\_fmod\xmeffects.h
# End Source File
# End Group
# Begin Group "system header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_system\qb_camera.h
# End Source File
# Begin Source File

SOURCE=.\_system\qb_color.h
# End Source File
# Begin Source File

SOURCE=.\_system\qb_debug.h
# End Source File
# Begin Source File

SOURCE=.\_system\qb_matrix.h
# End Source File
# Begin Source File

SOURCE=.\_system\qb_plane.h
# End Source File
# Begin Source File

SOURCE=.\_system\qb_sphere.h
# End Source File
# Begin Source File

SOURCE=.\_system\qb_system.h
# End Source File
# Begin Source File

SOURCE=.\_system\qb_vector.h
# End Source File
# End Group
# Begin Group "fx header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_fx\qb_filter.h
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_logo.h
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_sprite_nrj.h
# End Source File
# Begin Source File

SOURCE=.\datas\qb_text.h
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_textscroller.h
# End Source File
# Begin Source File

SOURCE=.\_fx\qb_torus_landscape.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\qb_textures.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Assembler Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\qb_datas.asm

!IF  "$(CFG)" == "demo_lib - Win32 Release"

USERDEP__QB_DA="datas\DB_S1_R.XM"	"datas\wireframe.dds"	"datas\sprite.dds"	"datas\quantum.dds"	"datas\font.dds"	
# Begin Custom Build
OutDir=.\Release
InputPath=.\qb_datas.asm
InputName=qb_datas

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "demo_lib - Win32 Debug"

USERDEP__QB_DA="datas\DB_S1_R.XM"	"datas\wireframe.dds"	"datas\sprite.dds"	"datas\quantum.dds"	"datas\font.dds"	
# Begin Custom Build
OutDir=.\Debug
InputPath=.\qb_datas.asm
InputName=qb_datas

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_system\qb_system_asm.asm

!IF  "$(CFG)" == "demo_lib - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\_system\qb_system_asm.asm
InputName=qb_system_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "demo_lib - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\_system\qb_system_asm.asm
InputName=qb_system_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_fmod\Winmm.lib
# End Source File
# End Group
# End Target
# End Project
