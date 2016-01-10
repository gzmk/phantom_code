# Microsoft Developer Studio Project File - Name="HapticMouse" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=HapticMouse - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HapticMouse.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HapticMouse.mak" CFG="HapticMouse - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HapticMouse - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "HapticMouse - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HapticMouse - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D _WIN32_WINNT=0x0403 /Yu"HapticMouseAfx.h" /FD /c
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
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Release\HapticMouse.lib "$(3DTOUCH_BASE)\utilities\lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HapticMouse - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D _WIN32_WINNT=0x0403 /FR /Yu"HapticMouseAfx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\HapticMouseD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Debug\HapticMouseD.lib "$(3DTOUCH_BASE)\utilities\lib"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "HapticMouse - Win32 Release"
# Name "HapticMouse - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CriticalSectionLock.cpp
# End Source File
# Begin Source File

SOURCE=.\HapticMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\HapticMouseAfx.cpp
# ADD CPP /Yc"HapticMouseAfx.h"
# End Source File
# Begin Source File

SOURCE=.\HapticMouseManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32MouseEmulation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CriticalSectionLock.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HapticMouse\HapticMouse.h
# End Source File
# Begin Source File

SOURCE=.\HapticMouseAfx.h
# End Source File
# Begin Source File

SOURCE=.\HapticMouseManager.h
# End Source File
# Begin Source File

SOURCE=.\HapticMouseUtil.h
# End Source File
# Begin Source File

SOURCE=.\Win32MouseEmulation.h
# End Source File
# End Group
# End Target
# End Project
