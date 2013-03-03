/*****************************************************************************
文件:	LeftTreeImage
作者:	方远
邮箱:	fangyuanning@126.com
说明:	左边的图像集合界面
******************************************************************************/
#pragma once


namespace CEGUI
{
	class Window;
}


// CLeftTreeImage

class CLeftTreeImage : public CTreeCtrl
{
	DECLARE_DYNAMIC(CLeftTreeImage)

public:
	CLeftTreeImage();
	virtual ~CLeftTreeImage();

public:
	//初始化添加所有的图像集合到树中
	void Initialize(void);

	//当失去焦点的时候
	void OnTabFoucsEvent(bool lose);
protected:
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	CEGUI::Window* m_pBackWindow;
	CEGUI::Window* m_pImageWindow;
	CString m_szCurrentSetName;
public:
	afx_msg void OnCopySetName();
};


