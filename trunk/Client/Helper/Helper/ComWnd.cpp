// ComWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Helper.h"
#include "ComWnd.h"
#include "HelperSystem.h"
#include "HelperItemSet.h"

// CComWnd dialog
extern int g_nCodePage;

IMPLEMENT_DYNAMIC(CComWnd, CDialog)
CComWnd::CComWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CComWnd::IDD, pParent)
{
}

CComWnd::~CComWnd()
{
}

void CComWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CComWnd, CDialog)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CComWnd message handlers
BOOL CComWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	const char *WorldId="WorldID";
	LPHELPER_COMMAND pCmd =(LPHELPER_COMMAND)(pCopyDataStruct->lpData);
	if(!pCmd) return TRUE;
	//tongxi 判断是不是传入worldid;
	if(strcmp(pCmd->pContents,"HelpStart")==0)
	{
		CHelperSystem::GetMe()->GetHelpItemSet()->ShowPaihang(false);
		((CHelperDlg*)AfxGetApp()->GetMainWnd())->Refresh(CHelperDlg::FLAG_SHOW::FLAG_SHOW_HELP);
		return TRUE;
	}
	if(strlen(pCmd->pContents)>7  && g_nCodePage==936)
	{
		char strTmp[MAX_PATH];
		memset(strTmp, 0, MAX_PATH);
		strncpy(strTmp,pCmd->pContents,7);
		if(strcmp(strTmp,WorldId)==0)
		{
			memset(strTmp, 0, MAX_PATH);
			strncpy(strTmp,pCmd->pContents+7,strlen(pCmd->pContents)-7);
			if(atoi(strTmp)==0) 
			{
				//无效的worldid，默认仍然显示帮助
				CHelperSystem::GetMe()->GetHelpItemSet()->ShowPaihang(false);
				((CHelperDlg*)AfxGetApp()->GetMainWnd())->Refresh(CHelperDlg::FLAG_SHOW::FLAG_SHOW_HELP);
			}
			else
			{
				CHelperSystem::GetMe()->GetHelpItemSet()->ShowPaihang(true);
				CHelperSystem::GetMe()->GetHelpItemSet()->InitPaihangTree(strTmp);
				((CHelperDlg*)AfxGetApp()->GetMainWnd())->Refresh(CHelperDlg::FLAG_SHOW::FLAG_SHOW_PAIHANG);
			}
		}
		else
		{

			CHelperSystem::GetMe()->GetHelpItemSet()->ShowPaihang(false);
			((CHelperDlg*)AfxGetApp()->GetMainWnd())->Refresh(CHelperDlg::FLAG_SHOW::FLAG_SHOW_HELP);
			CHelperSystem::GetMe()->GotoAnchor(pCmd->pContents);
			
		}
		return TRUE;
	}
	CHelperSystem::GetMe()->GetHelpItemSet()->ShowPaihang(false);
	((CHelperDlg*)AfxGetApp()->GetMainWnd())->Refresh(CHelperDlg::FLAG_SHOW::FLAG_SHOW_HELP);
	CHelperSystem::GetMe()->GotoAnchor(pCmd->pContents);

	
	return TRUE;
}