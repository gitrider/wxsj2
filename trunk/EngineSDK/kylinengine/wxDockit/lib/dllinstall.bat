echo
echo DLL installation script
echo
echo Usage:
echo         dllinstall vc/mingw/bcc
echo to install the vc/mingw/bcc-compiled DLLs...


@echo off
echo DLL installation batch file
copy %1\*.dll %SystemRoot%\system32
echo DLL installation completed
