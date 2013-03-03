# Microsoft Developer Studio Project File - Name="wxdockit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=wxdockit - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxdockit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxdockit.mak" CFG="wxdockit - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxdockit - Win32 DLL Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxdockit - Win32 DLL Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxdockit - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxdockit - Win32 DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxdockit - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxdockit - Win32 Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "wxdockit - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxdockit - Win32 Default" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxdockit - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "msvc6prjud\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "msvc6prjud\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxdockitud.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxdockitud.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26ud_adv.lib wxbase26ud_xml.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockitud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockitud.lib" /debug
# ADD LINK32 wxmsw26ud_adv.lib wxbase26ud_xml.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockitud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockitud.lib" /debug

!ELSEIF  "$(CFG)" == "wxdockit - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "msvc6prju\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "msvc6prju\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_dll\wxdockitu.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_dll\wxdockitu.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26u_adv.lib wxbase26u_xml.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockitu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockitu.lib"
# ADD LINK32 wxmsw26u_adv.lib wxbase26u_xml.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockitu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockitu.lib"

!ELSEIF  "$(CFG)" == "wxdockit - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "msvc6prjd\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "msvc6prjd\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxdockitd.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxdockitd.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26d_adv.lib wxbase26d_xml.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockitd.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockitd.lib" /debug
# ADD LINK32 wxmsw26d_adv.lib wxbase26d_xml.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockitd.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockitd.lib" /debug

!ELSEIF  "$(CFG)" == "wxdockit - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "msvc6prj\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "msvc6prj\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_dll\wxdockit.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_dll\wxdockit.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_DOCKIT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_DOCKIT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26_adv.lib wxbase26_xml.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockit.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockit.lib"
# ADD LINK32 wxmsw26_adv.lib wxbase26_xml.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\vc_dll\wxdockit.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll" /implib:"..\..\lib\vc_dll\wxdockit.lib"

!ELSEIF  "$(CFG)" == "wxdockit - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "msvc6prjud\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "msvc6prjud\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxdockitud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxdockitud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockitud.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockitud.lib"

!ELSEIF  "$(CFG)" == "wxdockit - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "msvc6prju\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "msvc6prju\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_lib\wxdockitu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_lib\wxdockitu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockitu.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockitu.lib"

!ELSEIF  "$(CFG)" == "wxdockit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "msvc6prjd\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "msvc6prjd\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxdockitd.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxdockitd.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockitd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockitd.lib"

!ELSEIF  "$(CFG)" == "wxdockit - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxdockit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "msvc6prj\wxdockit"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_lib\wxdockit.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\..\include" /w /O2 /Fd..\..\lib\vc_lib\wxdockit.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockit.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxdockit.lib"

!ENDIF

# Begin Target

# Name "wxdockit - Win32 DLL Unicode Debug"
# Name "wxdockit - Win32 DLL Unicode"
# Name "wxdockit - Win32 DLL Debug"
# Name "wxdockit - Win32 DLL"
# Name "wxdockit - Win32 Unicode Debug"
# Name "wxdockit - Win32 Unicode"
# Name "wxdockit - Win32 Debug"
# Name "wxdockit - Win32 Default"
# Begin Group "MSW-specific sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\msw\dockwindow_msw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\msw\gdi_msw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\msw\pane_msw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\msw\toolbutton_msw.cpp
# End Source File
# End Group
# Begin Group "MSW-specific headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\wx\msw\dockwindow.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\msw\gdi.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\msw\pane.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\msw\toolbutton.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\generic\barholder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\dockhost.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\dockpanel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\dockwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\exsplitter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\gdi.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\gripper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\layoutmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\pane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\slidebar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\toolbutton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\generic\util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\wx\barholder.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\dockhost.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\dockpanel.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\dockwindow.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\export.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\exsplitter.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\gdi.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\gripper.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\layoutmanager.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\pane.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\slidebar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\toolbutton.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\util.h
# End Source File
# End Group
# End Target
# End Project

