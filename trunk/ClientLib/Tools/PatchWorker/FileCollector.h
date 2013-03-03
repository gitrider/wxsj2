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

	//设置进度输出窗口
	void			SetProgressWnd(HWND hWnd) { m_hProgressWnd = hWnd; }
	//所包含的文件（路径）数量
	int				GetNodeNumber(void) const { return (int)m_vFileBuf.size(); }
	//得到文件列表
	const std::vector< FileNode >& GetFileNode(void) const { return m_vFileBuf; }
	//得到版本号
	std::string		GetVersion(void) const { return m_strVersion; }
	//得到路径
	std::string		GetPath(void) const { return m_strPath; }
	//终止正在分析的目录分析函数
	void			TerminateCollecting(void) { ::SetEvent(m_hTerimateHandle); }

public:
	//清空
	void			ClearWorlk(void);

	//分析某一个目录
	bool			CollectPath(const char* szPath);
	//通过该目录的版本文件分析目录
	bool			CollectPath(const char* szPath, const char* szVersionFile, CString& strVersion, CAxCryptoMath* pKeyMath);
	//输出版本文件
	void			OutputVersionFile(const char* szFileName, const char* szVersion, CAxCryptoMath* pKeyMath);
	

protected:
	//递归函数，分析目录
	bool			_CollectPath(const char* szPath, const char* szRelPath, std::vector< FileNode >& vFileBuf);

protected:
	//路径
	std::string					m_strPath;
	//文件数据
	std::vector< FileNode >		m_vFileBuf;
	//版本号
	std::string					m_strVersion;
	

	HANDLE	m_hTerimateHandle;	//终止工作信号
	HWND	m_hProgressWnd;		//进度输出窗口
};

class CPatchMaker
{
public:
	CPatchMaker(const CFileCollector& oldVerData, const CFileCollector& newVerData, const char* szPatchPath);
	virtual ~CPatchMaker();

	//比较两个版本，生成比较数据
	void GeneratePatchData(std::string strFilter="");
	//将生成的数据输出
	bool WritePatchFile(CAxCryptoMath* pKeyMath);
	//设置进度输出窗口
	void SetProgressWnd(HWND hWnd) { m_hProgressWnd = hWnd; }
	//终止正在压缩的工程
	void TerminateZipping(void) { ::SetEvent(m_hTerimateHandle); }
	//得到Patch
	std::string GetPatchFile(void) const { return m_strPatchFile; }

protected:
	//比较节点
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

	//需要更新的数据
	std::vector< FileNode >		m_vReplaceFile;
	//需要新加的数据
	std::vector< FileNode >		m_vNewFile;
	//需要删除的数据
	std::vector< FileNode >		m_vDeleteFile;

	HANDLE	m_hTerimateHandle;	//终止工作信号
	HWND	m_hProgressWnd;		//进度输出窗口

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
