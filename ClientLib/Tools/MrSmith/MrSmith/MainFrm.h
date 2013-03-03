// MainFrm.h : CMainFrame 类的接口
//


#pragma once

#include "ChildView.h"
class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 操作
public:
	// set the agent num(in status bar)
	void setStatusText_AgentNum(int nNumber);
	void setStatusText_AgentStatus(const char* szStatus);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView    m_wndView;
	HICON m_hIcon;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSystemStart();
	afx_msg void OnSystemPause();
	afx_msg void OnUpdateSystemStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSystemPause(CCmdUI *pCmdUI);
	afx_msg void OnSystemAdd();
	afx_msg void OnClose();
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
};


