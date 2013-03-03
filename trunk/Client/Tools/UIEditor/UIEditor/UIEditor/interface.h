
#pragma  once

#include "UIStruct.h"


class CUIEditor : public IEditor
{
public:
	virtual bool InsertChildToTree(ENUM_TREE who, const char* childName, const char* parentPath);
	virtual bool DeleteAllTreeItem(ENUM_TREE who);
	virtual bool DeleteTreeItem(ENUM_TREE who, const char* childName);
	virtual bool OnUIWndSelected(const char* Who);
	bool InsertPropety(UINT nCount, PE_List** plist);
	bool InsertEvent(UINT nCount, PE_List** plist);
	virtual bool UpdateListItem(ENUM_LIST who, PE_List* pList, INT nItem) ;
	void GetEditorInfo(EditorInfo* pInfo);
	void LuaDoString(const char* pFunc);
	virtual void OnHideWindow(const char* wndName);
	virtual bool CleanupList(ENUM_LIST who) ;
	virtual void WaitForPluginWork(BOOL wait) ;
	//save
	void PrepareSave();
};
