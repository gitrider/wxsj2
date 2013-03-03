/*****************************************************************************
�ļ�:	LeftTreeImage
����:	��Զ
����:	fangyuanning@126.com
˵��:	��ߵ�ͼ�񼯺Ͻ���
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
	//��ʼ��������е�ͼ�񼯺ϵ�����
	void Initialize(void);

	//��ʧȥ�����ʱ��
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


