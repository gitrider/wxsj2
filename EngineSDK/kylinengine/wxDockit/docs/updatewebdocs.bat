@echo off

if not exist ..\..\wxDockitWebsite\docs goto end

echo.
echo Copying the HTML documentation to the wxDockitWebsite module
@copy html\* ..\..\wxDockitWebsite\docs
echo.
echo.

:end