#pragma once


typedef std::map< std::string, std::string > CONFIG_BUF;

class CGameConfig
{
public:
	CGameConfig(const char* szFileName);
	virtual ~CGameConfig(void);

	//文件是否被成功加载
	bool IsInvalid(void) const { return m_bInvalid; }
	//文件更新时间
	ULONGLONG GetLastUpdate(void) const { return m_tLastUpdate; }
	//取得某值
	const char* GetValue(const char* szValueName) const;

    void SetValue(const char* szValueName, int nValue);

	//保存更改
	void SaveModifyed(const CONFIG_BUF& toSave);

protected:
	//从文件中加载
	bool LoadFromFile(const char* szFileName);

protected:
	bool		m_bInvalid;		//文件是否被成功加载
	CONFIG_BUF	m_theConfigBuf;	//所有的配置选项
	std::string m_strFileName;	//配置文件
	ULONGLONG	m_tLastUpdate;	//最后一次更新时间
};
