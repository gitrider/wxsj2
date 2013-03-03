/*****************************************************************************
�ļ�:	LeftTreeWindow
����:	��Զ
����:	fangyuanning@126.com
˵��:	��ߵ����ʹ��ڽ���
******************************************************************************/
#pragma once

namespace CEGUI
{
	class Window;
};

// CLeftTreeWindow ��ͼ

class CLeftTreeWindow : public CTreeCtrl
{
	DECLARE_DYNCREATE(CLeftTreeWindow)

public:
	CLeftTreeWindow();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLeftTreeWindow();

public:

	bool InsertItemToTree(const char* pWindowName, const char* pParentName);
	bool InsertItemToTree(const WCHAR* pWindowName, const WCHAR* pParentName);
	HTREEITEM GetParentItemFromString(const char* pParentPath);
	HTREEITEM GetParentItemFromString(const WCHAR* pParentPath);
	bool DeleteItemFromName(const char*pName);
	void OnWindowSelectedChange(CEGUI::Window* pWin);
	void GetWindowParentTreeString(CEGUI::Window* pWin, CString& outString);
	HTREEITEM FindChildItem(HTREEITEM hItem, const char* name);
	VOID onRenameSelectWindow(const CString& szNewName, const CString& szOldName, const CString& szParentPath );
	//��ʧȥ�����ʱ��
	void OnTabFoucsEvent(bool lose);
protected:
	HTREEITEM FindTreeItem(WCHAR** pWho, int which, int maxParent,  HTREEITEM hParent);
	bool _DeleteItem(HTREEITEM item, const char*pName);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMLclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDelWindowCommand(void);
	afx_msg void OnSaveWindowCommand(void);
	
public:
	afx_msg void OnOnSaveSelWindow();
};


