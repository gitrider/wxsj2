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
	string strFName;		//�����ļ���
	string strZip;			//����MPQ������·��
	string strMPQName;		//����
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

const unsigned long MAX_PACKAGE_SIZE = 60 * 1024 * 1024; // ����PackageTools.h�д˶���ĸı���ı�

class UpdateSystem
{
public:
	enum UPDATESYS_WNDMSG
	{
		US_BEGIN = WM_USER+0X4000,		// ��ʼ
		US_DELETEFILE,					// ɾ���ļ�
		US_APPENDFILE,					// ����ļ�
		US_BACKUPFILE,					// �����ļ�
		US_DELETEPACKFILE,				// ɾ�������ļ�
		US_APPENDPACKFILE,				// ���������ļ�
		US_ROLLBACK,					// �ع�
		US_UPDATESYSTEM,				// ɾ��һЩ��ʱ�ļ���
		US_CHECK_ALL,					// ȫ�ּ��
		US_END,							// ����
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

	//************************��������

	//����CRC32��MD5���ж��ļ��Ƿ�����
	BOOL			IsAllRightCRC32AndMD5(TKPMFile* pFile,unsigned char * input, int ilen);
	//�����Լ�
	BOOL			UpdateMyself(PackageOper& PackOper);

	//�����ɢ�ĵ����ļ���Ŀǰֻ��(version)
	BOOL			AddDiscreteFiles(PackageOper& PackOper);
	//���������
	BOOL			AddPackage(PackageOper& PackOper);
	//���һ�����ļ���һ������
	BOOL			AddFileToPackage(PackageOper& PackOper);
	
	BOOL			AddFiles( vector<PackageOper>& vecFiles );

	//ɾ��version�ļ���������
	BOOL			DelDiscreteFiles( PackageOper& PackOper);
	//�Ӱ���ɾ��һ���ļ�
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
