/*****************************************************************************
文件:	UIEditorView
作者:	方远
邮箱:	fangyuanning@126.com
说明:	UI编辑器的主要操作界面类,中间的视图
******************************************************************************/


#pragma once
#include "UIEditorDoc.h"
#include <CEGUIEventArgs.h>
#include <CEGUIString.h>


enum DRAW_STEP
{
	DST_DRAW_PREPARE,
	DST_DRAW_BEGIN,
	DST_DRAW_END,
	DST_DRAW_NONE,
};

enum LAYOUT_SIZE
{
	LS_INVALID,
	LS_1024X768,
	LS_800X600,
	LS_SHOW_ALL,
	LS_NUM,
};

class CUIEditorView : public CView
{
protected: // 仅从序列化创建
	CUIEditorView();
	DECLARE_DYNCREATE(CUIEditorView)

public:
	enum QuadCount
	{
		QC_LAYOUT_TYPE = 0,
		QC_SELECT_WINDOW,
		QC_PREVIEW_WINDOW,
		QC_CREATE_WINDOW,

		QC_TYPE_SPLIT,

		QC_CURRENT_IMAGE ,
		QC_NUM,
	};
	struct QuadInfos
	{
		RECT quadRect;
		COLORREF quadColor;
	};

	enum BorderResponsePos
	{
		BRP_INVALID = -1,

		BRP_SELECT_WINDOW_LEFT_TOP_QUAD,
		BRP_SELECT_WINDOW_MIDDLE_TOP_QUAD,
		BRP_SELECT_WINDOW_RIGHT_TOP_QUAD,
		BRP_SELECT_WINDOW_LEFT_MIDDLE_QUAD,
		BRP_SELECT_WINDOW_RIGHT_MIDDLE_QUAD,
		BRP_SELECT_WINDOW_LEFT_BOTTOM_QUAD,
		BRP_SELECT_WINDOW_MIDDLE_BOTTOM_QUAD,
		BRP_SELECT_WINDOW_RIGHT_BOTTOM_QUAD,
		BRP_SELECT_WINDOW_MIDDLE_MIDDLE_QUAD,

		BRP_CREATE_NEW_WINDOW_QUAD,

		BRP_NUM,
	};
	struct BorderResponseArea
	{
		CRect hoverRect;
		HCURSOR cursor;
	};

// 属性
public:
	CUIEditorDoc* GetDocument() const;

// 操作
public:
	void initialize(void);
	void setLayoutSize(LAYOUT_SIZE type);
	void setWindowSelected(const CEGUI::String& name,bool addEvent = true);
	void deleteSelectedWindow(void);
	static void renderPause(BOOL setting) { s_renderPause = setting;}
	void setCreateWindowFlag(bool setting) { m_bCreateWindow = setting; }
	bool getCreateWindowFlag(void) { return m_bCreateWindow; }
	void setCreateWindow(CEGUI::Window* pWin) { m_pCreatedWindow = pWin; }
	CEGUI::Window* getCreateWindow(void) { return m_pCreatedWindow; }
	void onRenameSelectedWindow(const CString& name, const CEGUI::Window* pWho) ;
	void setShowMode(bool isShowWindow) { m_bIsShowWindow = isShowWindow; }
	bool getShowMode(void) { return m_bIsShowWindow; }
	void setImageQuad(const CRect & rect);
	CString getSelectedWindowName(void) { return m_nSelectWindowName.c_str(); }
protected:
	void drawAllQuad(void);
	void setSelectWindowPos(CRect rect, BOOL clear = FALSE);
	void setCreateWindowQuad();
	bool handleSelectedWindowMoved(const CEGUI::EventArgs &e);
	void processMoveAndSizing(CPoint point);
	void setMoveSpeed(FLOAT sp) { m_fMoveStep = sp; }
	void setProcessing(bool setting) { m_bProcessing = setting; }
	void processArrowMove(UINT nChar, UINT cnt);
	void moveWindow(CPoint point,CPoint step = CPoint(0,0),CEGUI::Window* pWin = NULL);
	void sizingWindow(CPoint size,INT type,CEGUI::Window* pWin = NULL);//0向下,1向右,2右下角
	void moveAndSizingWindow(CPoint size, INT type);	//0左上,1上,2左,右上3

	void updateCurrentWindowStatusText(void);
	void updateCurrentOperationStatusText(void);

	void showRenameDlg();

	bool isControlDown(void);
	bool isAltlDown(void);
	bool isShiftDown(void);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// 实现
public:
	virtual ~CUIEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_canSize;
	BOOL m_lButtonDown;
	BOOL m_rButtonDown;
	BOOL m_prevDrag;
	static BOOL s_renderPause;
	DRAW_STEP m_beginDraw;
	CPoint ptOld ;
	CPoint m_ptMouseMovePos;
	CMenu contexMenu;
	HCURSOR m_hCursor;
	bool m_binitialize;
	bool m_bProcessing;
	bool m_bCreateWindow;
	bool m_bIsShowWindow;
	QuadInfos m_nQuadInfo[QC_NUM];
	BorderResponseArea m_BorderResponse[BRP_NUM];
	BorderResponsePos m_nResponseType;
	CEGUI::String m_nSelectWindowName;
	FLOAT m_fMoveStep;
	CEGUI::Window* m_pSelectedWindow;
	CEGUI::Window* m_pCreatedWindow;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

private:
	void GetKeyStates(UINT nFlags, UINT & key);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	static VOID CALLBACK TimerProc(  HWND hwnd,	UINT uMsg,	UINT_PTR idEvent,	DWORD dwTime);
	static void SetRenderFlag(BOOL flag);
 
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void SetUserCursor(HCURSOR cur);
};

#ifndef _DEBUG  // UIEditorView.cpp 中的调试版本
inline CUIEditorDoc* CUIEditorView::GetDocument() const
   { return reinterpret_cast<CUIEditorDoc*>(m_pDocument); }
#endif

