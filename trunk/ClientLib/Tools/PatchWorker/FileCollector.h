#pragma once


struct FileNode
{
	std::string		strFileName;
	std::string     strPatchName;
	std::string     strPackName;
	bool			bFold;
	unsigned int	nSize;
	unsigned int	nCRC;
};

#define WM_PROGRESS_TITLE		(WM_USER+101)
#define WM_PROGRESS_COLLFILE	(WM_USER+102)
#define WM_PROGRESS_ZIPFILE		(WM_USER+103)

class CAxCryptoMath;
class CFileCollector
{
public:
	CFileCollector(UINT nMeasureSize = 256);
	virtual ~CFileCollector(void);

	//���ý����������
	void			SetProgressWnd(HWND hWnd) { m_hProgressWnd = hWnd; }
	//���������ļ���·��������
	int				GetNodeNumber(void) const { return (int)m_vFileBuf.size(); }
	//�õ��ļ��б�
	const std::vector< FileNode >& GetFileNode(void) const { return m_vFileBuf; }
	//�õ��汾��
	std::string		GetVersion(void) const { return m_strVersion; }
	//�õ�·��
	std::string		GetPath(void) const { return m_strPath; }
	//��ֹ���ڷ�����Ŀ¼��������
	void			TerminateCollecting(void) { ::SetEvent(m_hTerimateHandle); }

public:
	//���
	void			ClearWorlk(void);

	//����ĳһ��Ŀ¼
	bool			CollectPath(const char* szPath);
	//ͨ����Ŀ¼�İ汾�ļ�����Ŀ¼
	bool			CollectPath(const char* szPath, const char* szVersionFile, CString& strVersion, CAxCryptoMath* pKeyMath);
	//����汾�ļ�
	void			OutputVersionFile(const char* szFileName, const char* szVersion, CAxCryptoMath* pKeyMath);
	

protected:
	//�ݹ麯��������Ŀ¼
	bool			_CollectPath(const char* szPath, const char* szRelPath, std::vector< FileNode >& vFileBuf);

protected:
	//·��
	std::string					m_strPath;
	//�ļ�����
	std::vector< FileNode >		m_vFileBuf;
	//�汾��
	std::string					m_strVersion;
	

	HANDLE	m_hTerimateHandle;	//��ֹ�����ź�
	HWND	m_hProgressWnd;		//�����������
};

class CPatchMaker
{
public:
	CPatchMaker(const CFileCollector& oldVerData, const CFileCollector& newVerData, const char* szPatchPath);
	virtual ~CPatchMaker();

	//�Ƚ������汾�����ɱȽ�����
	void GeneratePatchData(std::string strFilter="");
	//�����ɵ��������
	bool WritePatchFile(CAxCryptoMath* pKeyMath);
	//���ý����������
	void SetProgressWnd(HWND hWnd) { m_hProgressWnd = hWnd; }
	//��ֹ����ѹ���Ĺ���
	void TerminateZipping(void) { ::SetEvent(m_hTerimateHandle); }
	//�õ�Patch
	std::string GetPatchFile(void) const { return m_strPatchFile; }

protected:
	//�ȽϽڵ�
	struct CompareNode
	{
		const FileNode*	pFileNode;
		bool			bMasked;
	};
	typedef std::map< std::string, CompareNode > CompareMap;

	const CFileCollector&		m_oldVerData;
	const CFileCollector&		m_newVerData;
	std::string					m_strPatchPath;
	std::string					m_strPatchFile;

	std::vector< std::string >	m_vModel;
	std::vector< std::string >	m_vMaterial;
	std::vector< std::string >	m_vEffect;
	std::vector< std::string >	m_vInterface;
	std::vector< std::string >	m_vSound;
	std::vector< std::string >	m_vScene;
	std::vector< std::string >	m_vBrushes;
	std::vector< std::string >	m_vConfig;
	std::vector< std::string >	m_vNormal;

	//��Ҫ���µ�����
	std::vector< FileNode >		m_vReplaceFile;
	//��Ҫ�¼ӵ�����
	std::vector< FileNode >		m_vNewFile;
	//��Ҫɾ��������
	std::vector< FileNode >		m_vDeleteFile;

	HANDLE	m_hTerimateHandle;	//��ֹ�����ź�
	HWND	m_hProgressWnd;		//�����������

	std::vector< std::string >	m_vFilter;

private:
	void _generateCompareMap(const std::vector< FileNode >& fileData, CompareMap& compareMap);
	void _dispatchFileName( std::vector< FileNode >& fileData );
	void _writeCommandFile( FILE* fp, std::string strCommand );
	void toLowerCase( std::string& str );
	bool StringMatch( std::string str, std::string pattern, bool caseSensitive );
	void GenerateFilter( std::string filter );
	bool FileNameMatch( std::string fname );
};
