// LuaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIEditor.h"
#include "LuaDlg.h"
#include "DataPool.h"
#include <CEGUIWindow.h>

// CLuaDlg dialog

IMPLEMENT_DYNAMIC(CLuaDlg, CDialog)

CLuaDlg::CLuaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLuaDlg::IDD, pParent)
{

}

CLuaDlg::~CLuaDlg()
{
}

void CLuaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_richEditCtrl);
}


BEGIN_MESSAGE_MAP(CLuaDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPEN_OR_CLOSE_LUA, &CLuaDlg::OnOpenOrCloseLua)
	ON_COMMAND(ID_SAVE_LUA, &CLuaDlg::OnSaveLua)
	ON_COMMAND(ID_CLOSE, &CLuaDlg::OnClose)
	ON_COMMAND(ID_ABOUT_LUA, &CLuaDlg::OnAboutLua)
	ON_COMMAND(ID_REFRESH_LUA, &CLuaDlg::OnRefreshLua)
END_MESSAGE_MAP()


// CLuaDlg message handlers

void CLuaDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_richEditCtrl.MoveWindow(&CRect(0,0,cx,cy));
}

void	CLuaDlg::SetLuaText(const CString& text)
{
	m_richEditCtrl.SetWindowText(text);
}

void CLuaDlg::OnOpenOrCloseLua()
{
	if( IsWindowVisible() )
	{
		ShowWindow(SW_HIDE);
		g_DataPool.OnShowLuaWindow(FALSE);
	}
	else
	{
		ShowWindow(SW_SHOW);
		g_DataPool.OnShowLuaWindow(TRUE);
	}
}

VOID CLuaDlg::Initailzie(VOID)
{
	m_richEditCtrl.Initialize();
	m_richEditCtrl.SetCaseSensitive(FALSE);
	m_richEditCtrl.SetStringQuotes(L"\"");
	m_richEditCtrl.SetSLComment(L'\--');
	//m_richEditCtrl.SetSLComment(_T('\--[['));
	//m_richEditCtrl.SetSLComment(_T('\]]'));

	g_DataPool.m_scriptModule.DoString("getLuaKeywordAndConst()");

}

void CLuaDlg::AddKeywords(const char* szKeyword)
{
	m_richEditCtrl.AddKeywords(GetUnicode(szKeyword));
}
void CLuaDlg::AddConstants(const char* szKeyword)
{
	m_richEditCtrl.AddConstants(GetUnicode(szKeyword));
}
void CLuaDlg::OnCancel()
{
	g_DataPool.OnShowLuaWindow(FALSE);
	CDialog::OnCancel();
}

void CLuaDlg::OnSaveLua()
{
	if( g_DataPool.m_pCurrentSelectWindow == NULL ) return;

	CString szLuaName = g_DataPool.GetRootWindowName(g_DataPool.m_pCurrentSelectWindow->getName().c_str());

	CFileDialog dlg(FALSE, "", szLuaName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"luaÎÄ¼þ(*.lua)|*.lua||", AfxGetMainWnd());

	if ( dlg.DoModal() == IDOK )
	{
		CString szPathName = dlg.GetPathName();
		
		CString szSave;

		m_richEditCtrl.GetWindowText(szSave);

		FILE* pfile = fopen(szPathName, "w");

		if (pfile)
		{
			fwrite(szSave.GetString(), szSave.GetLength(), 1, pfile);

			fclose(pfile);
		}
	}
}

void CLuaDlg::OnClose()
{
	OnCancel();
}

void CLuaDlg::OnAboutLua()
{
	// TODO: Add your command handler code here
}

void CLuaDlg::OnRefreshLua()
{
	g_DataPool.OnShowLuaWindow(TRUE);
}
