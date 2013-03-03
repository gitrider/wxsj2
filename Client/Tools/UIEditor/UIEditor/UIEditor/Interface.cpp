
#include "stdafx.h"
#include "interface.h"
#include "RightEventList.h"
#include "RightPropList.h"

bool CUIEditor::InsertChildToTree(ENUM_TREE who, const char* childName, const char* parentPath)
{
	//if (who == ET_IMAGE_TREE)
	//{
	//	return g_leftTreeImage->InsertItemToTree(childName, parentPath);
	//}
	//else if(who == ET_WINDOW_TREE)
	//{
	//	return g_leftTreeWindow->InsertItemToTree(childName, parentPath);
	//}
	return false;
}
bool CUIEditor::DeleteAllTreeItem(ENUM_TREE who)
{
	//if (who == ET_IMAGE_TREE)
	//{
	//	return g_leftTreeImage->DeleteAllItems() == TRUE;
	//}
	//else if(who == ET_WINDOW_TREE)
	//{
	//	return g_leftTreeWindow->DeleteAllItems() == TRUE;
	//}
	return true;
}
//删除指定的节点
bool CUIEditor::DeleteTreeItem(ENUM_TREE who, const char* childName)
{
	//if (who == ET_IMAGE_TREE)
	//{
	//	return g_leftTreeImage->DeleteItemFromName(childName);
	//}
	//else if(who == ET_WINDOW_TREE)
	//{
	//	return g_leftTreeWindow->DeleteItemFromName(childName);
	//}
	return true;
}
bool CUIEditor::OnUIWndSelected(const char* Who)
{
	return true;
}
bool CUIEditor::InsertPropety(UINT nCount, PE_List** plist)
{
	//if (GetRightPropList())
	//{
	//	GetRightPropList()->InsertItemS(nCount, plist);
	//}
	return true;
}
bool CUIEditor::InsertEvent(UINT nCount, PE_List** plist)
{
	//if (GetRightEventList())
	//{
	//	GetRightEventList()->InsertItemS(nCount, plist);
	//}
	return true;
}
bool CUIEditor::CleanupList(ENUM_LIST who)
{
	//if (who == ECL_PROP_LIST)
	//{
	//	GetRightPropList()->Cleanup();
	//}
	//else if(who == ECL_EVENT_LIST)
	//{
	//	GetRightEventList()->Cleanup();
	//}
	return true;
}
bool CUIEditor::UpdateListItem(ENUM_LIST who, PE_List* pList, INT nItem)
{
	//if (who == ECL_PROP_LIST)
	//{
	//	GetRightPropList()->UpdateListItem(who, pList, nItem);
	//}
	//else if(who == ECL_EVENT_LIST)
	//{
	//	GetRightEventList()->UpdateListItem(who , pList, nItem);
	//}
	return true;
}
void CUIEditor::GetEditorInfo(EditorInfo* pInfo)
{

}
void CUIEditor::LuaDoString(const char* pFunc)
{

}
void CUIEditor::OnHideWindow(const char* wndName)
{

}

void CUIEditor::WaitForPluginWork(BOOL wait)
{
	if (wait)
	{
		AfxGetMainWnd()->BeginWaitCursor();
	}
	else
	{
		AfxGetMainWnd()->EndWaitCursor();
	}
}
void CUIEditor::PrepareSave()
{

}
