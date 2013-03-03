#pragma once

#include "kpmLib.h"
#include "md5.h"
#include "crc32.h"
#include "TestUpdateSystem.h"

/*
#ifdef _DEBUG
#pragma comment(lib,"../../kpmLib/script/Debug/kpmLib.lib")
#else
#pragma comment(lib,"../../kpmLib/script/Release/kpmLib.lib")
#endif
*/


using namespace std;
using namespace KPM;

struct URLInfo
{
	string strInfo;
	string strURL;
	URLInfo() {;}
	URLInfo( string s1, string s2 ) { strInfo = s1; strURL = s2; }
};

struct PatchInfo
{
	string strVerFrom;
	string strVerTo;
	vector< URLInfo > urlInfo;
};

struct PackageOper
{
	string strFName;		//更新文件名
	string strZip;			//更新MPQ包完整路径
	string strMPQName;		//包名
	DWORD dwCRC;
};

struct PackageInfo
{
	string strName;
	string strPath;
	map< string, PackageOper > vDel;
	map< string, PackageOper > vRpl;
	map< string, PackageOper > vNew;
};

struct BackupDelete
{
	string strFile;
	string strBackFile;
};

struct BackupPackageDelete
{
	string strPackName;
	string strBackFile;
	bool bDirty;
};

struct FileInfo
{
	string strName;
	DWORD dwOffset;
	DWORD dwSize;
	DWORD dwCRC;
};

const unsigned long MAX_PACKAGE_SIZE = 60 * 1024 * 1024; // 跟随PackageTools.h中此定义的改变而改变

class UpdateSystem
{
public:
	enum UPDATESYS_WNDMSG
	{
		US_BEGIN = WM_USER+0X4000,		// 开始
		US_DELETEFILE,					// 删除文件
		US_APPENDFILE,					// 添加文件
		US_BACKUPFILE,					// 备份文件
		US_DELETEPACKFILE,				// 删除包中文件
		US_APPENDPACKFILE,				// 向包中添加文件
		US_ROLLBACK,					// 回滚
		US_UPDATESYSTEM,				// 删除一些临时文件等
		US_CHECK_ALL,					// 全局检查
		US_END,							// 结束
	};

public:
	UpdateSystem(void);
	~UpdateSystem(void);

	static UpdateSystem*	GetMe(void) { return s_pMe; }

	BOOL					Init(HWND hWnd, int nOwner, LPCSTR szUpdateIniFile, LPCSTR szResourcesIniFile, CString& strErrInfo );
	BOOL					IfInit( void );
	BOOL					IfUpdateOK( void );
	int						GetUpdateStep( void );
	const PatchInfo*		GetUpdateInfo( int nStep );
	BOOL					PreUpdate( int nStep, LPCSTR szLocalPatchFile, CString& strErrInfo );
	BOOL					DoUpdate( CString& strErrInfo );
	BOOL					UndoUpdate( void );
	DWORD					GetProcess( void );
	LPCSTR					GetCurrentFileName( void );
	
	void					Log( LPCSTR pData, DWORD dwSize );
	BOOL					BackupDeleteFile( string strFileName );

	void					ClearMyself();

private:

	//************************新增函数

	//根据CRC32和MD5码判断文件是否无损
	BOOL			IsAllRightCRC32AndMD5(TKPMFile* pFile,unsigned char * input, int ilen);
	//更新自己
	BOOL			UpdateMyself(PackageOper& PackOper);

	//添加离散的单个文件，目前只有(version)
	BOOL			AddDiscreteFiles(PackageOper& PackOper);
	//添加完整包
	BOOL			AddPackage(PackageOper& PackOper);
	//添加一个新文件到一个包中
	BOOL			AddFileToPackage(PackageOper& PackOper);
	
	BOOL			AddFiles( vector<PackageOper>& vecFiles );

	//删除version文件或完整包
	BOOL			DelDiscreteFiles( PackageOper& PackOper);
	//从包中删除一个文件
	BOOL			DelFileFromPackage(PackageOper& PackOper);

	BOOL			DeleteFiles( vector<PackageOper>& vecFiles );


	BOOL			ReadPatchInfo( LPCSTR szUpdateIniFile, CString& strErrInfo );
	BOOL			ReadPackages(CString& strErrInfo);
	BOOL			RereadPackages( LPSTR lpData, DWORD dwSize );
	BOOL			PushCommand( LPCSTR cmd, LPCSTR name, LPCSTR from, LPCSTR pak, DWORD dwCRC, LPCSTR zip,CString& strErrInfo );
	BOOL			DeleteFilesFromPackage( string strPackage, vector<string>& vecFiles, CString& strErrInfo );
	BOOL			AppendFilesFromPackage( string strPackage, vector<PackageOper>& vecFiles, CString& strErrInfo );
	BOOL			PreparePackage( string strPackage, CString& strErrInfo );
	//BOOL			BackupDeleteFile( string strFileName );
	BOOL			BackupAddFile( string& strFileName );
	void			ClearTempFile(string  &strDir,string Ext);
	BOOL			EndingPackage( string strPackage );
	BOOL			BeginingPackage( string strPackage, CString& strErrInfo );
	INT				FindFile( LPCSTR szFileString, DWORD dwFileStringSize, LPCSTR szFileName, FileInfo& fi );
	INT				CalPackOffset( pair<INT, INT>* nValueSizes, INT dwPackCount, DWORD dwOffset, DWORD& dwPackOffset );
	DWORD			ReduceHead( LPSTR szHead, DWORD dwSize, DWORD dwOffset );
	INT				AppendFile( string strPackage, pair<INT, INT>* nSizes, INT dwPackCount, LPCSTR lpFile, DWORD dwSize );
	INT				AppendHead( pair<INT, INT>* nSizes, INT dwPackCount, INT nPack, LPCSTR szFileName, DWORD dwSize, DWORD dwCRC, LPSTR lpHead, DWORD& dwHeadSize );
	BOOL			CreateDirectorys( LPSTR szPaths );
	string			FindMaxTo( map< string, PatchInfo >& vPatchInfo, string strFrom, string& strTo );
	void			ConvertVer(const string& strStringVer, string& strComVer);
	int				CheckALL(PackageOper& Version, CString& strErrInfo);

	BOOL			MyDeleteFile( string strPackage, DWORD dwValueSize, DWORD dwOffset, DWORD dwSize );
	BOOL			MyCopyFile( string strDest, string strSrc );
	BOOL			MyAppendFile( string strPackage, LPCSTR lpFile, DWORD dwSize, DWORD dwAppend = 0 );
	BOOL			MyCopyFile1M( string strDest, LPCSTR lpData, DWORD dwSize );

private:
	BOOL			MyDeleteFileM( string& strPackage, DWORD dwValueSize, DWORD dwOffset, DWORD dwSize );
	BOOL			MyDeleteFileD( string& strPackage, DWORD dwValueSize, DWORD dwOffset, DWORD dwSize );
	BOOL			MyCopyFileM( string& strDest, string& strSrc );
	BOOL            MyCopyFileD( string& strDest, string& strSrc );
	BOOL			MyAppendFileM( string& strPackage, LPCSTR lpFile, DWORD dwSize, DWORD dwAppend );
	BOOL			MyAppendFileD( string& strPackage, LPCSTR lpFile, DWORD dwSize, DWORD dwAppend );

private:
	HWND				m_hNotifyWnd;
	FILE*				m_LogFile;
	string				m_strLastVersion;
	string				m_strThisVersion;
	string				m_strResourceCfg;
	string				m_strResourceCfgBasePath;
	string				m_strCurDealFile;
	string				m_strThisPack;
	BOOL				m_bInit;
	BOOL				m_bUpdate;
	BOOL				m_bUndo;
	BOOL				m_bFullCheck;
	int					m_nOwner;
	int					m_nStep;
	int					m_nThisPackageNum;
	pair<INT, INT>*		m_lpPackageSize;
	LPSTR				m_lpThisPackageHead;
	DWORD				m_dwHeadSize;
	DWORD				m_dwHeadSizeAfter;
	DWORD				m_dwTotalSize;
	DWORD				m_dwCurrent;
	DWORD				m_dwPoint;
	DWORD				m_dwLastVersionFilesCount;
	vector< PatchInfo >	m_PatchInfo;
	vector< BackupDelete >		m_BackupDelFiles;
	vector< string >			m_BackupAddFiles;
	map<string,FileInfo >		m_PackFileMap;
	map<string,PackageOper >	m_DeleteFile;
	map<string,PackageOper >	m_ReplaceFile;
	map<string,PackageOper >	m_NewFile;
	map<string,PackageInfo >	m_PackageInfo;
	map<string,BackupPackageDelete >	m_PackDeleteFile;
	typedef pair< string, FileInfo >	m_PackFilePair;
	typedef pair< string, PackageInfo > m_PackageInfoPair;
	typedef pair< string, PackageOper >	m_PackageOperPair;
	typedef pair< string, PackageOper >	m_FilePair;
	typedef pair< string, BackupPackageDelete >	m_PackDeleteFilePair;

protected:
	static int s_Temp;
	static UpdateSystem* s_pMe;
};
