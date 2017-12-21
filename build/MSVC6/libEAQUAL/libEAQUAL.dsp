# Microsoft Developer Studio Project File - Name="libEAQUAL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libEAQUAL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libEAQUAL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libEAQUAL.mak" CFG="libEAQUAL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libEAQUAL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libEAQUAL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "libEAQUAL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "../tmp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /I "../../../src/GlobalHeaders" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libEAQUAL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "../tmp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../src/GlobalHeaders" /I "../../../src/GlobalHeaders" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libEAQUAL - Win32 Release"
# Name "libEAQUAL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\AvgBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\AvgTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\CalcTables.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\FBModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\Fft.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\FFTModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\FileIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\KogModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\libEAQUAL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\MOVBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\PreProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\PsychoModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\RunAnalysis.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\Avg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\AvgBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\AvgTime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\BasicUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\CalcTables.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\EAQUALCore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\FBModel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\FFTModel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\FileIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\KogModel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\libEAQUAL.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\MOVBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\PreProc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\ProcessBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\PsychoModel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\RunAnalysis.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libEAQUAL\version.h
# End Source File
# End Group
# End Target
# End Project
