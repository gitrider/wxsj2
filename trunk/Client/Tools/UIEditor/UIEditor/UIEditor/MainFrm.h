// MainFrm.h : CMainFrame 类的接口
//


#pragma once

#include "SplitterWndEx.h"
#include "LeftView.h"
#include "RightView.h"
#include "LuaDlg.h"
#include "ToolBarListDlg.h"
#include "LayoutXMLPreviewDlg.h"


class CMainFrame : public CFrameWnd
{
	
public: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:
	static DESING_SIZE m_desingSize;
// 操作
public:
	void SetSkinIndex(INT index)
	{
		if (index > 9 || index < 0)
		{
			m_nIndex = 0;
		}
		else
		{
			m_nIndex = index;
		}
	};
//重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CRightView* GetRightView()
	{
		return m_pRightView;
	}
	CLeftView* GetLeftView()
	{
		return m_pLeftView;
	}

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	//CToolBar    m_wndToolBar;
	CSplitterWnd m_wndSpliter1;
	CSplitterWnd m_wndSpliter2;
	CRightView* m_pRightView;
	CLeftView*   m_pLeftView;
	CToolTipCtrl m_toolTips;
	CLuaDlg*	 m_luaDlg;
	CToolBarListDlg m_toolDlg;
	CLayoutXMLPreviewDlg m_xmlDlg;

private:
	INT m_nIndex;
	bool m_checkLHide;
	bool m_checkRHide;
	bool m_isInit;
	TOOLBAR m_barInfo;
	HCURSOR m_hCurrentCursor;
	
	//bool m_checkView;

// 生成的消息映射函数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType,int cx,	int cy 	);
	afx_msg void OnFaceChange(UINT nID);
	afx_msg void OnToolCommand(UINT nID);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) ;
	virtual BOOL PreTranslateMessage(MSG* pMsg) ;
	void OnViewTool();
	void OnHideTool();
	void InitializeToolBar();
	void InitializeMenu();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHideLTool();
	afx_msg void OnUpdateHideLTool(CCmdUI *pCmdUI);
	afx_msg void OnHideRTool();
	afx_msg void OnUpdateHideRTool(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHideTool(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewTool(CCmdUI *pCmdUI);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	afx_msg void OnShowToolbar();
	afx_msg void OnFloatBar();
	afx_msg void OnLimitSize_1024_768();
	afx_msg void OnLimitSize_800_600();
	afx_msg void OnLimitSize_ShowAll();
	afx_msg void OnUpdate800600(CCmdUI *pCmdUI);
	afx_msg void OnUpdate1024768(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowAll(CCmdUI *pCmdUI);
	
public:
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void	SetStatusText(INT id, LPCTSTR szStr);
	VOID	SetLuaText(const CString& text);
	VOID	ShowLuaDlg(BOOL bShow);
public:
	afx_msg void OnOpenPrj();
public:
	afx_msg void OnOpenOrCloseLua();
public:
	afx_msg void OnReloadScript();
public:
	afx_msg void OnXmlView();
public:
	afx_msg void OnFileSaveSelf();
public:
	afx_msg void OnFileSaveAsSelf();
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
};


