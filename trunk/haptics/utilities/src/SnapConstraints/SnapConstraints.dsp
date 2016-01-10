# Microsoft Developer Studio Project File - Name="SnapConstraints" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SnapConstraints - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SnapConstraints.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SnapConstraints.mak" CFG="SnapConstraints - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SnapConstraints - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SnapConstraints - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SnapConstraints - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"SnapConstraintsAfx.h" /FD /c
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
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Release\SnapConstraints.lib "$(3DTOUCH_BASE)\utilities\lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SnapConstraints - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"SnapConstraintsAfx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\SnapConstraintsD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Debug\SnapConstraintsD.lib "$(3DTOUCH_BASE)\utilities\lib"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SnapConstraints - Win32 Release"
# Name "SnapConstraints - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\CompositeConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ConstraintHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\src\LineConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlaneConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PointConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SnapConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SnapConstraintsAfx.cpp
# ADD CPP /Yc"SnapConstraintsAfx.h"
# End Source File
# Begin Source File

SOURCE=.\src\SnapConstraintsAPI.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\SnapConstraints\CompositeConstraint.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SnapConstraints\ConstraintHolder.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SnapConstraints\ISnapConstraintsAPI.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SnapConstraints\LineConstraint.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SnapConstraints\PlaneConstraint.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SnapConstraints\PointConstraint.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SnapConstraints\SnapConstraint.h
# End Source File
# Begin Source File

SOURCE=.\src\SnapConstraintsAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SnapConstraints\StickToConstraint.h
# End Source File
# End Group
# End Target
# End Project
