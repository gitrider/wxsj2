# Microsoft Developer Studio Project File - Name="xrcdemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=xrcdemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xrcdemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xrcdemo.mak" CFG="xrcdemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xrcdemo - Win32 DLL Universal Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 DLL Universal Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 DLL Universal Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 DLL Universal Debug" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 DLL Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 DLL Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 DLL Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 DLL Debug" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Universal Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Universal Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Universal Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Universal Debug" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "xrcdemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xrcdemo - Win32 DLL Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswunivudll"
# PROP BASE Intermediate_Dir "vc_mswunivudll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswunivudll"
# PROP Intermediate_Dir "vc_mswunivudll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswunivudll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswunivudll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26u_xrc.lib wxmswuniv26u_html.lib wxmswuniv26u_adv.lib wxmswuniv26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivudll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv26u_xrc.lib wxmswuniv26u_html.lib wxmswuniv26u_adv.lib wxmswuniv26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivudll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 DLL Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivuddll"
# PROP BASE Intermediate_Dir "vc_mswunivuddll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivuddll"
# PROP Intermediate_Dir "vc_mswunivuddll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivuddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivuddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26ud_xrc.lib wxmswuniv26ud_html.lib wxmswuniv26ud_adv.lib wxmswuniv26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivuddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv26ud_xrc.lib wxmswuniv26ud_html.lib wxmswuniv26ud_adv.lib wxmswuniv26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivuddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 DLL Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswunivdll"
# PROP BASE Intermediate_Dir "vc_mswunivdll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswunivdll"
# PROP Intermediate_Dir "vc_mswunivdll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswunivdll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswunivdll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswuniv" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswuniv" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26_xrc.lib wxmswuniv26_html.lib wxmswuniv26_adv.lib wxmswuniv26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivdll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv26_xrc.lib wxmswuniv26_html.lib wxmswuniv26_adv.lib wxmswuniv26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivdll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 DLL Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivddll"
# PROP BASE Intermediate_Dir "vc_mswunivddll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivddll"
# PROP Intermediate_Dir "vc_mswunivddll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26d_xrc.lib wxmswuniv26d_html.lib wxmswuniv26d_adv.lib wxmswuniv26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv26d_xrc.lib wxmswuniv26d_html.lib wxmswuniv26d_adv.lib wxmswuniv26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 DLL Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswudll"
# PROP BASE Intermediate_Dir "vc_mswudll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswudll"
# PROP Intermediate_Dir "vc_mswudll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswudll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswudll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26u_xrc.lib wxmsw26u_html.lib wxmsw26u_adv.lib wxmsw26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswudll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw26u_xrc.lib wxmsw26u_html.lib wxmsw26u_adv.lib wxmsw26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswudll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswuddll"
# PROP BASE Intermediate_Dir "vc_mswuddll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswuddll"
# PROP Intermediate_Dir "vc_mswuddll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswuddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswuddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26ud_xrc.lib wxmsw26ud_html.lib wxmsw26ud_adv.lib wxmsw26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw26ud_xrc.lib wxmsw26ud_html.lib wxmsw26ud_adv.lib wxmsw26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswdll"
# PROP BASE Intermediate_Dir "vc_mswdll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswdll"
# PROP Intermediate_Dir "vc_mswdll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswdll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswdll\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\msw" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\msw" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26_xrc.lib wxmsw26_html.lib wxmsw26_adv.lib wxmsw26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswdll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw26_xrc.lib wxmsw26_html.lib wxmsw26_adv.lib wxmsw26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswdll\xrcdemo.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswddll"
# PROP BASE Intermediate_Dir "vc_mswddll\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswddll"
# PROP Intermediate_Dir "vc_mswddll\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswddll\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26d_xrc.lib wxmsw26d_html.lib wxmsw26d_adv.lib wxmsw26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw26d_xrc.lib wxmsw26d_html.lib wxmsw26d_adv.lib wxmsw26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswddll\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswunivu"
# PROP BASE Intermediate_Dir "vc_mswunivu\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswunivu"
# PROP Intermediate_Dir "vc_mswunivu\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswunivu\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswunivu\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26u_xrc.lib wxmswuniv26u_html.lib wxmswuniv26u_adv.lib wxmswuniv26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivu\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv26u_xrc.lib wxmswuniv26u_html.lib wxmswuniv26u_adv.lib wxmswuniv26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivu\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivud"
# PROP BASE Intermediate_Dir "vc_mswunivud\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivud"
# PROP Intermediate_Dir "vc_mswunivud\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivud\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivud\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26ud_xrc.lib wxmswuniv26ud_html.lib wxmswuniv26ud_adv.lib wxmswuniv26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivud\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv26ud_xrc.lib wxmswuniv26ud_html.lib wxmswuniv26ud_adv.lib wxmswuniv26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivud\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswuniv"
# PROP BASE Intermediate_Dir "vc_mswuniv\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswuniv"
# PROP Intermediate_Dir "vc_mswuniv\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswuniv\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswuniv\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswuniv" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswuniv" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26_xrc.lib wxmswuniv26_html.lib wxmswuniv26_adv.lib wxmswuniv26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuniv\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv26_xrc.lib wxmswuniv26_html.lib wxmswuniv26_adv.lib wxmswuniv26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuniv\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivd"
# PROP BASE Intermediate_Dir "vc_mswunivd\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivd"
# PROP Intermediate_Dir "vc_mswunivd\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivd\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivd\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv26d_xrc.lib wxmswuniv26d_html.lib wxmswuniv26d_adv.lib wxmswuniv26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivd\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv26d_xrc.lib wxmswuniv26d_html.lib wxmswuniv26d_adv.lib wxmswuniv26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivd\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswu"
# PROP BASE Intermediate_Dir "vc_mswu\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswu"
# PROP Intermediate_Dir "vc_mswu\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswu\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswu\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26u_xrc.lib wxmsw26u_html.lib wxmsw26u_adv.lib wxmsw26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswu\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw26u_xrc.lib wxmsw26u_html.lib wxmsw26u_adv.lib wxmsw26u_core.lib wxbase26u_xml.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswu\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswud"
# PROP BASE Intermediate_Dir "vc_mswud\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswud"
# PROP Intermediate_Dir "vc_mswud\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswud\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswud\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26ud_xrc.lib wxmsw26ud_html.lib wxmsw26ud_adv.lib wxmsw26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswud\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw26ud_xrc.lib wxmsw26ud_html.lib wxmsw26ud_adv.lib wxmsw26ud_core.lib wxbase26ud_xml.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswud\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw"
# PROP BASE Intermediate_Dir "vc_msw\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_msw"
# PROP Intermediate_Dir "vc_msw\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_msw\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_msw\xrcdemo.pdb /O2 /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\msw" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\msw" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26_xrc.lib wxmsw26_html.lib wxmsw26_adv.lib wxmsw26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_msw\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw26_xrc.lib wxmsw26_html.lib wxmsw26_adv.lib wxmsw26_core.lib wxbase26_xml.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_msw\xrcdemo.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "xrcdemo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd"
# PROP BASE Intermediate_Dir "vc_mswd\xrcdemo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswd"
# PROP Intermediate_Dir "vc_mswd\xrcdemo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswd\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswd\xrcdemo.pdb /Od /GR /EHsc /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26d_xrc.lib wxmsw26d_html.lib wxmsw26d_adv.lib wxmsw26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswd\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw26d_xrc.lib wxmsw26d_html.lib wxmsw26d_adv.lib wxmsw26d_core.lib wxbase26d_xml.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswd\xrcdemo.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ENDIF

# Begin Target

# Name "xrcdemo - Win32 DLL Universal Unicode Release"
# Name "xrcdemo - Win32 DLL Universal Unicode Debug"
# Name "xrcdemo - Win32 DLL Universal Release"
# Name "xrcdemo - Win32 DLL Universal Debug"
# Name "xrcdemo - Win32 DLL Unicode Release"
# Name "xrcdemo - Win32 DLL Unicode Debug"
# Name "xrcdemo - Win32 DLL Release"
# Name "xrcdemo - Win32 DLL Debug"
# Name "xrcdemo - Win32 Universal Unicode Release"
# Name "xrcdemo - Win32 Universal Unicode Debug"
# Name "xrcdemo - Win32 Universal Release"
# Name "xrcdemo - Win32 Universal Debug"
# Name "xrcdemo - Win32 Unicode Release"
# Name "xrcdemo - Win32 Unicode Debug"
# Name "xrcdemo - Win32 Release"
# Name "xrcdemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\custclas.cpp
# End Source File
# Begin Source File

SOURCE=.\derivdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\myframe.cpp
# End Source File
# Begin Source File

SOURCE=.\xrcdemo.cpp
# End Source File
# Begin Source File

SOURCE=.\xrcdemo.rc
# End Source File
# End Group
# End Target
# End Project

