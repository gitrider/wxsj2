#include "StdAfx.h"
#include "Util.h"
#include <string>
#include <vector>

#include <zzip/zzip.h>
#include <zzip/lib.h>

#include "AXP.h"

void CenterDlg(HWND hDlg)
{
	int x,y;
	int w,h;
	
	RECT rect;

	GetWindowRect(hDlg,&rect);

	w = rect.right - rect.left;
	h = rect.bottom - rect.top;

	x = ( GetSystemMetrics(SM_CXMAXIMIZED) - w ) /2;
	y = ( GetSystemMetrics(SM_CYMAXIMIZED) - h ) /2;

	MoveWindow(hDlg,x,y,w,h,TRUE);

//	::SetWindowPos(hDlg, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
}

bool ExtractFileFromZip(AXP::IUpdater* pUpdater, const char* szZipFile, const char* szFileInZip, const char* szFileInDisk)
{
	//打开zip文件
	zzip_error_t zzipError;
	ZZIP_DIR* mZzipDir = zzip_dir_open_ext_io(szZipFile, &zzipError, 0, 
		(zzip_plugin_io_t)(pUpdater->getEncryptZipPluginIO(szZipFile)));
	if (zzipError != ZZIP_NO_ERROR) 
	{
		return false;
	}

	//得到文件信息
	ZZIP_STAT zstat;
	memset(&zstat, 0, sizeof(ZZIP_STAT));

	//打开文件,如果打不开，是空文件
	ZZIP_FILE* zzipFile = zzip_file_open(mZzipDir, szFileInZip, ZZIP_ONLYZIP | ZZIP_CASELESS);
	if(!zzipFile)
	{
		zzip_dir_close(mZzipDir);
		return false;
	}

	//获得文件信息
	int zip_err = zzip_dir_stat(mZzipDir, szFileInZip, &zstat, ZZIP_CASEINSENSITIVE);
	if(zip_err!=0)
	{
		zzip_file_close(zzipFile);
		zzip_dir_close(mZzipDir);
		return false;
	}

	//创建该文件
	HANDLE hDiskFile = ::CreateFile(szFileInDisk, 
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		0,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE,
		0);

	if(hDiskFile == INVALID_HANDLE_VALUE)
	{
		zzip_file_close(zzipFile);
		zzip_dir_close(mZzipDir);
		return false;
	}

	if(zstat.st_size > 0)
	{
		const int MAX_BUFFER_SIZE = 4096;
		char buffer[MAX_BUFFER_SIZE] = {0};

		zzip_seek(zzipFile, 0, SEEK_SET);
		zzip_ssize_t zReadSize = zzip_file_read(zzipFile, buffer, sizeof(buffer));

		//实际已经写入的尺寸
		unsigned int nActWriteSize = 0;

		//分块读写文件内容
		do
		{
			//文件结束
			if(zReadSize==0) break;

			//写入磁盘文件
			DWORD dwBytesWrite;
			if(!WriteFile(hDiskFile, buffer, (DWORD)zReadSize, &dwBytesWrite, 0) || 
				dwBytesWrite != (DWORD)zReadSize)
			{
				zzip_file_close(zzipFile);
				zzip_dir_close(mZzipDir);
				CloseHandle(hDiskFile);
				return false;
			}

			//文件结束
			if(zzip_tell(zzipFile) >=zstat.st_size) break;

			zReadSize = zzip_file_read(zzipFile, buffer, sizeof(buffer));
		}while(true);
	}
	//关闭句柄
	CloseHandle(hDiskFile); hDiskFile=0;
	zzip_file_close(zzipFile);	zzipFile=0;
	zzip_dir_close(mZzipDir); mZzipDir=0;

	return true;
}

bool checkIfGameRun(HWND hWnd, bool bUserConfirm)
{
	//游戏窗口wndclass
	const char MAINWINDOW_CLASS[] = "TianLongBaBu WndClass";

	do
	{
		HWND hTLBBWnd = ::FindWindow(MAINWINDOW_CLASS, NULL);
		if(hTLBBWnd==0) return false;
		if(!bUserConfirm) break;

		if(IDCANCEL == ::MessageBox(hWnd, 
			"游戏客户端正在运行,在进行下面的操作前请关闭客户端!", 
			"《天龙八部》", MB_RETRYCANCEL|MB_ICONSTOP))
		{
			break;
		}

		//再次确认
		Sleep(10);
	}while(true);

	return true;
}

//分割字符串
int convertStringToVector(const char* strResource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty)
{
	vRet.clear();
	
	if(0==strResource || strResource[0]==0)
	{	//字符串为空
		return 0;
	}

	std::string strSrc = strResource;
	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}

	while(true)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));
		
		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (int)vRet.size();
}


void normalizeVersion(const char* szVersion, char* szNormalizeBuf, int bufSize)
{
	std::vector< std::string > vTemp;

	//按照'.'分解
	convertStringToVector(szVersion, vTemp, ".", true, true);

	int nSecNum = min((int)vTemp.size(), 4);
	while(nSecNum < 4)
	{
		vTemp.insert(vTemp.begin(), "0");
		nSecNum++;
	}

	_snprintf(szNormalizeBuf, bufSize, "%05d.%05d.%05d.%05d", 
		atoi(vTemp[0].c_str()), atoi(vTemp[1].c_str()), atoi(vTemp[2].c_str()), atoi(vTemp[3].c_str()));
}
