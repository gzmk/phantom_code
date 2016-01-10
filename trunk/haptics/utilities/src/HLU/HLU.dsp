# Microsoft Developer Studio Project File - Name="HLU" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=HLU - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HLU.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HLU.mak" CFG="HLU - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HLU - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "HLU - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "HLU - Win32 ReleaseWithSymbols" (based on "Win32 (x86) Static Library")
!MESSAGE "HLU - Win32 ReleaseAcademicEdition" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HLU - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hluAfx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Release\HLU.lib "$(3DTOUCH_BASE)\utilities\lib\hlu.lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HLU - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"hluAfx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\HLUD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Debug\HLUD.lib "$(3DTOUCH_BASE)\utilities\lib\hlud.lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HLU - Win32 ReleaseWithSymbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HLU___Win32_ReleaseWithSymbols"
# PROP BASE Intermediate_Dir "HLU___Win32_ReleaseWithSymbols"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseWithSymbols"
# PROP Intermediate_Dir "ReleaseWithSymbols"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hluAfx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zd /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hluAfx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy ReleaseWithSymbols\HLU.lib "$(3DTOUCH_BASE)\utilities\lib\hlu.lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HLU - Win32 ReleaseAcademicEdition"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HLU___Win32_ReleaseAcademicEdition"
# PROP BASE Intermediate_Dir "HLU___Win32_ReleaseAcademicEdition"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseAcademicEdition"
# PROP Intermediate_Dir "ReleaseAcademicEdition"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hluAfx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hluAfx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy ReleaseAcademicEdition\HLU.lib "$(3DTOUCH_BASE)\utilities\lib\hlu.lib"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "HLU - Win32 Release"
# Name "HLU - Win32 Debug"
# Name "HLU - Win32 ReleaseWithSymbols"
# Name "HLU - Win32 ReleaseAcademicEdition"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\hlu.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hluAfx.cpp
# ADD CPP /Yc"hluAfx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\HLU\hlu.h
# End Source File
# Begin Source File

SOURCE=.\src\hluAfx.h
# End Source File
# End Group
# End Target
# End Project
