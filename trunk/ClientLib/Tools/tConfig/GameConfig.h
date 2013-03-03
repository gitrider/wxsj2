#pragma once


typedef std::map< std::string, std::string > CONFIG_BUF;

class CGameConfig
{
public:
	CGameConfig(const char* szFileName);
	virtual ~CGameConfig(void);

	//�ļ��Ƿ񱻳ɹ�����
	bool IsInvalid(void) const { return m_bInvalid; }
	//�ļ�����ʱ��
	ULONGLONG GetLastUpdate(void) const { return m_tLastUpdate; }
	//ȡ��ĳֵ
	const char* GetValue(const char* szValueName) const;

    void SetValue(const char* szValueName, int nValue);

	//�������
	void SaveModifyed(const CONFIG_BUF& toSave);

protected:
	//���ļ��м���
	bool LoadFromFile(const char* szFileName);

protected:
	bool		m_bInvalid;		//�ļ��Ƿ񱻳ɹ�����
	CONFIG_BUF	m_theConfigBuf;	//���е�����ѡ��
	std::string m_strFileName;	//�����ļ�
	ULONGLONG	m_tLastUpdate;	//���һ�θ���ʱ��
};
