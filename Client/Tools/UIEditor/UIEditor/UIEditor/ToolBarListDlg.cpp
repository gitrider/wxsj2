// ToolBarListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIEditor.h"
#include "ToolBarListDlg.h"
#include "DataPool.h"
#include "MainFrm.h"
#include <CEGUISchemeManager.h>
#include <CEGUIScheme.h>
#include "Global.h"


using namespace CEGUI;

// CToolBarListDlg dialog

IMPLEMENT_DYNAMIC(CToolBarListDlg, CDialog)

CToolBarListDlg::CToolBarListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarListDlg::IDD, pParent)
{

}

CToolBarListDlg::~CToolBarListDlg()
{
}

void CToolBarListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CToolBarListDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CToolBarListDlg::OnLvnItemchangedList2)
END_MESSAGE_MAP()


// CToolBarListDlg message handlers

BOOL CToolBarListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listCtrl.InsertColumn(0,"窗口控件", LVCFMT_CENTER,180);

	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	m_listCtrl.ModifyStyle(0,LVS_SINGLESEL,0);

	g_toolBarDlg = this;

	return TRUE;
}

VOID CToolBarListDlg::InsertToolWindowToList()
{
	INT cnt = 0;
	SchemeManager::SchemeIterator it = SchemeManager::getSingleton().getIterator();
	while( !it.isAtEnd() )
	{
		Scheme* scheme = it.getCurrentValue();
		if (scheme)
		{
			size_t size  = scheme->d_falagardMappings.size();
			for (size_t i=0; i<size; i++)
			{
				CString typeName = g_DataPool.m_scriptModule.DoFunctionAndRetString("getToolBarNameByType",scheme->d_falagardMappings[i].windowName.c_str());
				//然后添加内容到列表中
				if(typeName.GetLength() > 0)
				{
					m_listCtrl.InsertItem(0,typeName);
					m_toolTypeMap.insert(std::make_pair(typeName, scheme->d_falagardMappings[i].windowName.c_str()));
					++cnt;
				}
			}
		}
		++it;
	}
	char msg[100];
	sprintf(msg, "工具栏(%d)",cnt);
	SetWindowText(msg);
}  

void CToolBarListDlg::OnSelChanged()
{
	char szWindowName[MAX_PATH] = {0};
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	int nItem = m_listCtrl.GetNextSelectedItem(pos);
	if( nItem >= 0)
	{
		m_listCtrl.GetItemText(nItem, 0, szWindowName, MAX_PATH-1);
		if( m_szLastTypeName != szWindowName )
		{
			m_szLastTypeName = szWindowName;
			if(m_szLastTypeName.GetLength() > 0)
			{
				CString szType = GetInnerTypeName(m_szLastTypeName);
				if (szType.GetLength()>0)
				{
					g_DataPool.OnToolBarListWindowTypeChange(szType);

					//刷新状态栏
					char szMsg[100] = {0};
					sprintf(szMsg, "当前选择类型:%s,名字:%s", szType,m_szLastTypeName);
					((CMainFrame*)AfxGetMainWnd())->SetStatusText(3,szMsg);
				}
			}
		}
	}
}

void CToolBarListDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	OnSelChanged();

	*pResult = 0;
}


CString CToolBarListDlg::GetInnerTypeName(const CString& szName)
{
	ToolTypeDefine::iterator it = m_toolTypeMap.find(szName);
	if ( it!=m_toolTypeMap.end())
	{
		return it->second;
	}
	return "";
}