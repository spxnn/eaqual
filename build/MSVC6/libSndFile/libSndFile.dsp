# Microsoft Developer Studio Project File - Name="libSndFile" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libSndFile - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libSndFile.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libSndFile.mak" CFG="libSndFile - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libSndFile - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libSndFile - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "libSndFile - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../src/libSndFile/Win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libSndFile - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\src\libSndFile\Win32" /I "../../src/libSndFile/Win32" /I "../../../src/libSndFile/Win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
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

# Name "libSndFile - Win32 Release"
# Name "libSndFile - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\add.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\aiff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\alaw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\au.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\au_g72x.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\code.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\common.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\decode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\float32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\G72x\g721.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\G72x\g723_16.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\G72x\g723_24.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\G72x\g723_40.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\G72x\g72x.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\gsm_create.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\gsm_decode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\gsm_destroy.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\gsm_encode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\gsm_option.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\ircam.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\long_term.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\lpc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\nist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\paf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\pcm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\preprocess.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\raw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\rpe.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\samplitude.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\short_term.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\sndfile.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\svx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\table.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\ulaw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\voc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\wav.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\wav_gsm610.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\wav_ima_adpcm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\wav_ms_adpcm.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\au.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\common.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\floatcast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\G72x\g72x.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\gsm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\G72x\private.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\private.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\sfendian.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\sndfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\GSM610\unproto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libSndFile\src\wav.h
# End Source File
# End Group
# End Target
# End Project
