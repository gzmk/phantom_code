# Microsoft Developer Studio Project File - Name="HDU" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=HDU - Win32 ReleaseWithSymbols
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HDU.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HDU.mak" CFG="HDU - Win32 ReleaseWithSymbols"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HDU - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "HDU - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "HDU - Win32 ReleaseWithSymbols" (based on "Win32 (x86) Static Library")
!MESSAGE "HDU - Win32 ReleaseAcademicEdition" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HDU - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hduAfx.h" /FD /c
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
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Release\HDU.lib "$(3DTOUCH_BASE)\utilities\lib\hdu.lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HDU - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"hduAfx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\HDUD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy Debug\HDUD.lib "$(3DTOUCH_BASE)\utilities\lib\hdud.lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HDU - Win32 ReleaseWithSymbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HDU___Win32_ReleaseWithSymbols"
# PROP BASE Intermediate_Dir "HDU___Win32_ReleaseWithSymbols"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseWithSymbols"
# PROP Intermediate_Dir "ReleaseWithSymbols"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hduAfx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zd /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hduAfx.h" /FD /c
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
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy ReleaseWithSymbols\HDU.lib "$(3DTOUCH_BASE)\utilities\lib\hdu.lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HDU - Win32 ReleaseAcademicEdition"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HDU___Win32_ReleaseAcademicEdition"
# PROP BASE Intermediate_Dir "HDU___Win32_ReleaseAcademicEdition"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseAcademicEdition"
# PROP Intermediate_Dir "ReleaseAcademicEdition"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hduAfx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"hduAfx.h" /FD /c
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
PostBuild_Cmds=echo on	mkdir "$(3DTOUCH_BASE)\utilities\lib"	copy ReleaseAcademicEdition\HDU.lib "$(3DTOUCH_BASE)\utilities\lib\hdu.lib"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "HDU - Win32 Release"
# Name "HDU - Win32 Debug"
# Name "HDU - Win32 ReleaseWithSymbols"
# Name "HDU - Win32 ReleaseAcademicEdition"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Math Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\hduDecompose.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduDecompose.h
# End Source File
# Begin Source File

SOURCE=.\src\hduLine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduLineSegment.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduQuaternion.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\hdu.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduAfx.cpp
# ADD CPP /Yc"hduAfx.h"
# End Source File
# Begin Source File

SOURCE=.\src\hduError.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduHapticDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hduRecord.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Math Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\HDU\hduBoundBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduGenericMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduLine.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduLineSegment.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduMath.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduQuaternion.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduVector.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduVector.inl
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\include\HDU\hdu.h
# End Source File
# Begin Source File

SOURCE=.\src\hduAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduError.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduHapticDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDU\hduRecord.h
# End Source File
# End Group
# End Target
# End Project
