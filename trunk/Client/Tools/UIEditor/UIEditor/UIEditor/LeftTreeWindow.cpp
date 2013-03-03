// LeftTreeWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "LeftTreeWindow.h"
#include "DataPool.h"
#include <ceguiwindow.h>
#include <CEGUISystem.h>

#define TREE_STYLE TVIF_TEXT
// CLeftTreeWindow

IMPLEMENT_DYNCREATE(CLeftTreeWindow, CTreeCtrl)

CLeftTreeWindow::CLeftTreeWindow()
{
	g_leftTreeWindow = this;
}

CLeftTreeWindow::~CLeftTreeWindow()
{
}

BEGIN_MESSAGE_MAP(CLeftTreeWindow, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(TVN_GETINFOTIP, &CLeftTreeWindow::OnTvnGetInfoTip)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CLeftTreeWindow::OnTvnItemexpanded)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CLeftTreeWindow::OnNMRclick)
	ON_NOTIFY_REFLECT(NM_CLICK, &CLeftTreeWindow::OnNMLclick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftTreeWindow::OnSelChanged)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_DEL_C_WINDOW, OnDelWindowCommand)
	ON_COMMAND(ID_SAVE_C_WINDOW, OnSaveWindowCommand)
	ON_COMMAND(ID_ON_SAVE_SEL_WINDOW, &CLeftTreeWindow::OnOnSaveSelWindow)
END_MESSAGE_MAP()


// CLeftTreeWindow 诊断




// CLeftTreeWindow 消息处理程序

int CLeftTreeWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CLeftTreeWindow::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return  FALSE; CTreeCtrl::OnEraseBkgnd(pDC);
}

void CLeftTreeWindow::OnTvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	
	*pResult = 0;
}

void CLeftTreeWindow::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	*pResult = 0;
}

bool CLeftTreeWindow::InsertItemToTree(const char* pWindowName, const char* pParentName)
{
	if (pWindowName == NULL)
	{
		return false;
	}
	//根节点
	if (pParentName == NULL)
	{
		InsertItem(/*GetUnicode*/(pWindowName), NULL, NULL);
		return true;
	}
	HTREEITEM pParentItem = GetParentItemFromString(pParentName);
	if (pParentItem)
	{
		InsertItem(/*GetUnicode*/(pWindowName), pParentItem, NULL);
	}
	else
	{
		return false;
	}
	return true;
}
bool CLeftTreeWindow::InsertItemToTree(const WCHAR* pWindowName, const WCHAR* pParentName)
{
	if (pWindowName == NULL)
	{
		return false;
	}
	//根节点
	if (pParentName == NULL)
	{
		InsertItem(GetMBCS(pWindowName), NULL, NULL);
		return true;
	}
	HTREEITEM pParentItem = GetParentItemFromString(pParentName);
	if (pParentItem)
	{
		InsertItem(GetMBCS(pWindowName), pParentItem, NULL);
	}
	else
	{
		return false;
	}
	return true;
}

HTREEITEM CLeftTreeWindow::GetParentItemFromString(const char* pParentPath)
{
	if (pParentPath == NULL)
	{
		return NULL;
	}
	return GetParentItemFromString(GetUnicode(pParentPath));
}
HTREEITEM CLeftTreeWindow::GetParentItemFromString(const WCHAR* pParentPath)
{
	if (pParentPath == NULL)
	{
		return NULL;
	}
	WCHAR pTemp[1024] = {0};
	int len = (int)wcslen(pParentPath);
	wcscpy(pTemp, pParentPath);
	WCHAR* pChar[50] = {0};
	pChar[0] = pTemp;
	int i=0, j=1;
	for ( i=1; i<len && j<50; i++)
	{
		if (pTemp[i] == '/')
		{
			pChar[j++] = &(pTemp[i+1]);
			pTemp[i] = '\0';
		}
	}
	return FindTreeItem(pChar, 0, j, NULL);
}

HTREEITEM CLeftTreeWindow::FindTreeItem(WCHAR** pWho, int which, int maxParent, HTREEITEM hParent)
{
	//递归结束标志
	if (which >= maxParent)
	{
		return hParent;
	}
	CString str;
	if (hParent == NULL)
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetRootItem();
		while (hChildItem != NULL)
		{
			CString itemText = GetItemText(hChildItem);
			if (itemText == pWho[which])
			{
				return FindTreeItem(pWho++, which+1, maxParent, hChildItem);
				break;
			}
			hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
		return NULL;
	}
	else if(hParent != (HTREEITEM)-1)
	{
		if (ItemHasChildren(hParent))
		{
			HTREEITEM hNextItem;
			HTREEITEM hChildItem = GetChildItem(hParent);
			while (hChildItem != NULL)
			{
				CString itemText = GetItemText(hChildItem);
				if (itemText == pWho[which])
				{
					return FindTreeItem(pWho++, which+1, maxParent, hChildItem);
					break;
				}
				hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
				hChildItem = hNextItem;
			}
		}
	}
	else
	{
		return FindTreeItem(NULL, maxParent, maxParent, NULL);
	}
	//error pos
	printf("FindTreeItem Error Cann't Find Item\n");
	return HTREEITEM(-1);
}
bool CLeftTreeWindow::DeleteItemFromName(const char*pName)
{
	if (pName == NULL)
	{
		return false;
	}

	//遍历所有的根目录
	HTREEITEM hItem = GetRootItem();
	do 
	{
		bool bRemoved = _DeleteItem(hItem, pName);
		if (bRemoved)
		{
			return bRemoved;
		}
		else
		{
			hItem = GetNextItem(hItem, TVGN_NEXT);
		}
	} while(hItem);

	return true;
}
bool CLeftTreeWindow::_DeleteItem(HTREEITEM item, const char*pName)
{
	if (pName == NULL)
	{
		return false;
	}
	CString str = GetItemText(item);
	if (str == pName)
	{
		this->DeleteItem(item);
		return true;
	}
	if (ItemHasChildren(item))
	{
		HTREEITEM hChildItem = GetChildItem(item);
		while (hChildItem != NULL)
		{
			if (_DeleteItem(hChildItem, pName))
			{
				return true;
			}
			hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
	return false;
}
void CLeftTreeWindow::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	UINT uFlags;
	CPoint myPoint;
	CPoint oriPoint;
	GetCursorPos(&myPoint);
	oriPoint = myPoint;
	ScreenToClient(&myPoint);
	HTREEITEM hItem = HitTest(myPoint, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		SelectItem(hItem);
	}
	CMenu menu;
	menu.LoadMenu(IDR_LEFT_TREE_MENU);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, oriPoint.x, oriPoint.y, this);
	*pResult = 0;
}

void CLeftTreeWindow::OnNMLclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	//HTREEITEM hItem = GetSelectedItem();
	//CString szSelectWindow = GetItemText(hItem);
	//g_pEditorView->setWindowSelected((CEGUI::utf8*)szSelectWindow.GetString());
}

void CLeftTreeWindow::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = GetSelectedItem();

	if (hItem == NULL) return;
	
	CString szSelectWindow = GetItemText(hItem);
	g_pEditorView->setWindowSelected((CEGUI::utf8*)szSelectWindow.GetString());
}

void CLeftTreeWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_DELETE && g_pEditorView)
	{
		g_pEditorView->deleteSelectedWindow();
	}
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLeftTreeWindow::OnDelWindowCommand(void)
{
	g_pEditorView->deleteSelectedWindow();
}
void CLeftTreeWindow::OnSaveWindowCommand(void)
{
	CString szText = GetItemText(GetSelectedItem());
	g_DataPool.OnSaveCommand(szText,TRUE);
}

void CLeftTreeWindow::OnWindowSelectedChange(CEGUI::Window* pWin)
{
	if( !pWin ) 
	{
		SelectItem(NULL);
		return;
	}
	CString szTreeString;
	GetWindowParentTreeString(pWin, szTreeString);
	//先找到父结点,然后在找某个子节点
	HTREEITEM hItemParent = GetParentItemFromString(pWin->getName().c_str());
	//寻找这个子节点
	HTREEITEM hItem = FindChildItem(hItemParent, pWin->getName().c_str());
	//选择这个项目
	SelectItem(hItem);
}

void CLeftTreeWindow::GetWindowParentTreeString(CEGUI::Window* pWin, CString& outString)
{
	if(pWin)
	{
		CString szName = pWin->getName().c_str();
		szName += "/";
		szName += outString;
		outString = szName;
	}
	size_t cnt = pWin->getChildCount();
	for (size_t i=0; i<cnt; i++)
	{
		GetWindowParentTreeString(pWin->getChildAtIdx(i), outString);
	}
}

HTREEITEM CLeftTreeWindow::FindChildItem(HTREEITEM hParent, const char* name)
{
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = GetChildItem(hParent);
	while (hChildItem != NULL)
	{
		HTREEITEM hItemFind = FindChildItem(hChildItem,name);
		if(hItemFind != NULL) return hItemFind;
		CString itemText = GetItemText(hChildItem);
		if (itemText == name)
		{
			return hChildItem;
		}
		hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		hChildItem = hNextItem;
	}
	return NULL;
}

//窗口名字发生改变了,需要修改树目录里面的名字
VOID CLeftTreeWindow::onRenameSelectWindow(const CString& szNewName, const CString& szOldName, const CString& szParentPath )
{
	HTREEITEM hParent = GetParentItemFromString(szParentPath);
	if (hParent)
	{
		HTREEITEM hItem = FindChildItem(hParent, szOldName);
		if(hItem)
		{
			SetItemText(hItem, szNewName);
		}
		//如果就一个父窗口,那么找到他,并修改
		else if(szOldName == szParentPath)
		{
			SetItemText(hParent, szNewName);
		}
	}
}
void CLeftTreeWindow::OnOnSaveSelWindow()
{
	CString szText = GetItemText(GetSelectedItem());
	g_DataPool.OnSaveCommand(szText,FALSE);
}

//当失去焦点的时候
void CLeftTreeWindow::OnTabFoucsEvent(bool lose)
{
	//if(lose)
	//{
	//	CEGUI::System::getSingleton().getGUISheet()->hide();
	//}
	//else
	//{
	//	CEGUI::System::getSingleton().getGUISheet()->show();
	//}
}