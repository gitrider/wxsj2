#pragma once


class CSystemInfo
{
public:
	//获取dx版本号
	const char*			GetDirectXVersion(void);
	//获取系统设置
	const char*			GetSystemInfo(void);

protected:
	//dx版本号
	std::string 		m_strDirectXVersion;
	//系统设置
	std::string			m_strSystemInfo;

public:
	CSystemInfo();
	virtual ~CSystemInfo();
};