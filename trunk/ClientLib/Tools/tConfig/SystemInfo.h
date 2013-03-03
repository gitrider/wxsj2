#pragma once


class CSystemInfo
{
public:
	//��ȡdx�汾��
	const char*			GetDirectXVersion(void);
	//��ȡϵͳ����
	const char*			GetSystemInfo(void);

protected:
	//dx�汾��
	std::string 		m_strDirectXVersion;
	//ϵͳ����
	std::string			m_strSystemInfo;

public:
	CSystemInfo();
	virtual ~CSystemInfo();
};