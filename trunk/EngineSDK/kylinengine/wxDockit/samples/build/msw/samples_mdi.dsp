# Microsoft Developer Studio Project File - Name="samples_mdi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mdi - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "samples_mdi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "samples_mdi.mak" CFG="mdi - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mdi - Win32 DLL Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "mdi - Win32 DLL Unicode" (based on "Win32 (x86) Application")
!MESSAGE "mdi - Win32 DLL Debug" (based on "Win32 (x86) Application")
!MESSAGE "mdi - Win32 DLL" (based on "Win32 (x86) Application")
!MESSAGE "mdi - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "mdi - Win32 Unicode" (based on "Win32 (x86) Application")
!MESSAGE "mdi - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "mdi - Win32 Default" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mdi - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prjud\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prjud\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockitud.lib wxmsw26ud_adv.lib wxbase26ud_xml.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /debug /subsystem:windows
# ADD LINK32 wxdockitud.lib wxmsw26ud_adv.lib wxbase26ud_xml.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "mdi - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prju\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prju\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\..\..\include" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\..\..\include" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockitu.lib wxmsw26u_adv.lib wxbase26u_xml.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxdockitu.lib wxmsw26u_adv.lib wxbase26u_xml.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "mdi - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prjd\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prjd\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockitd.lib wxmsw26d_adv.lib wxbase26d_xml.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /debug /subsystem:windows
# ADD LINK32 wxdockitd.lib wxmsw26d_adv.lib wxbase26d_xml.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "mdi - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prj\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prj\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\..\..\include" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\..\..\include" /d "WXUSINGDLL" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockit.lib wxmsw26_adv.lib wxbase26_xml.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxdockit.lib wxmsw26_adv.lib wxbase26_xml.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "mdi - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prjud\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prjud\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockitud.lib wxmsw26ud_adv.lib wxbase26ud_xml.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /debug /subsystem:windows
# ADD LINK32 wxdockitud.lib wxmsw26ud_adv.lib wxbase26ud_xml.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "mdi - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prju\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prju\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\..\..\include" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\..\..\include" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockitu.lib wxmsw26u_adv.lib wxbase26u_xml.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxdockitu.lib wxmsw26u_adv.lib wxbase26u_xml.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "mdi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prjd\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prjd\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\..\..\include" /W4 /Od /Zi /Gm /GZ /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\..\..\include" /d "_DEBUG" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockitd.lib wxmsw26d_adv.lib wxbase26d_xml.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /debug /subsystem:windows
# ADD LINK32 wxdockitd.lib wxmsw26d_adv.lib wxbase26d_xml.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "mdi - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\mdi"
# PROP BASE Intermediate_Dir "msvc6prj\mdi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\mdi"
# PROP Intermediate_Dir "msvc6prj\mdi"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\..\..\include" /w /O2 /Fd..\..\mdi\mdi.pdb /I "..\..\mdi\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\..\..\include" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\..\..\include" /i "..\..\mdi\include" /d "_WINDOWS" /i "..\..\mdi" /i ..\..\..\include\msw
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxdockit.lib wxmsw26_adv.lib wxbase26_xml.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxdockit.lib wxmsw26_adv.lib wxbase26_xml.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\..\mdi\mdi.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\..\lib\vc_lib" /subsystem:windows

!ENDIF

# Begin Target

# Name "mdi - Win32 DLL Unicode Debug"
# Name "mdi - Win32 DLL Unicode"
# Name "mdi - Win32 DLL Debug"
# Name "mdi - Win32 DLL"
# Name "mdi - Win32 Unicode Debug"
# Name "mdi - Win32 Unicode"
# Name "mdi - Win32 Debug"
# Name "mdi - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\mdi\src\app.cpp
# End Source File
# Begin Source File

SOURCE=..\..\mdi\mdi.rc
# End Source File
# Begin Source File

SOURCE=..\..\mdi\src\mdi_example.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\mdi\include\mdi_example.h
# End Source File
# End Group
# End Target
# End Project

