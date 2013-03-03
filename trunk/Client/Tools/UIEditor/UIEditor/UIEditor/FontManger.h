#pragma once

class CFontManger
{
public:
	CFontManger(void);
	~CFontManger(void);
	static void Initialize();
	static CFont m_font12;
	static CFont m_font16;
	static CFont m_font14;
	static CFont m_font15;
};
