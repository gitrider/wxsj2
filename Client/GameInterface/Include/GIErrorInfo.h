
/** 程序崩溃时保存信息并启动 ErrorReport
 */

#pragma optimize("y", off)		// generate stack frame poINTers for all functions - same as /Oy- in the project


BOOL CreateSmallDumpInfo(PEXCEPTION_POINTERS pException, PTCHAR szSmallFile, UINT dwLastError);
BOOL CreateBigInfoFile(PEXCEPTION_POINTERS pException, PTCHAR szBigFile, UINT dwLastError);

BOOL CreateDumpHelpFile(PEXCEPTION_POINTERS pException, PTCHAR szDumpFile);
