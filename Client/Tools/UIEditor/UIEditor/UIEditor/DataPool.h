/*****************************************************************************
文件:	DataPool
作者:	方远
邮箱:	fangyuanning@126.com
说明:	数据池保存各种数据
******************************************************************************/
#pragma once

#include <vector>
#include <map>
#include "ScriptModule.h"

namespace CEGUI
{
	class Window;
}

struct Event_Define
{
	CString eventName;
	CString funcName;
	BOOL	isReadFromLayout;
	Event_Define()
	{
		isReadFromLayout = FALSE;
	}
};

typedef std::vector<Event_Define> EventArray;

typedef std::map<CString,EventArray> EventMap;

class CDataPool
{
public:
	CDataPool(void);
	~CDataPool(void);
	bool Initialize();

public:
	BOOL OpenLayOut(const CHAR* szName);
	VOID InsertWindowToTree(CEGUI::Window* pParent,const CString& szParentPath);
	VOID OnDeleteWindow(const CString& name);
	void OnSaveCommand(const CString& szName, BOOL bSaveRoot = FALSE);
	void OnShowLuaWindow(BOOL show);
	void OnShowLayouXmlWindow(BOOL show);
	void OnSelectWindowChanged(const CEGUI::Window* pOldWindow, const CEGUI::Window* pNewWindow);
	void OnAddFunctionDlg(const CString& szName, const CString &szLeft, const CString& szRight);
	void OnPropertyChange(const CString &szName, const CString& szValue, CEGUI::Window* pWindwo = NULL);
	void OnToolBarListWindowTypeChange(const CString& szTypeName);
	BOOL OnCreateNewWindow(CEGUI::Window* pWind);
	EventArray* GetWindowEvent(const CString& szName);
	void AddWindowEvent(const CString& szName);
	void DeleteWindowEvent(CEGUI::Window* pWin);
	void AddWindowEventItem(const CString& szWindowName, const CString& szEventName, const CString& szFunction,BOOL isFromLayout  =FALSE);
	void InsertEventToList(CEGUI::Window* pWindow);
	void InsertPropertyToList(CEGUI::Window* pWindow);
	void GenLuaText(CString& textLua,const CString& szWinName,const CString& szSystemName);
	CString GetRootWindowName(const CString& szName);
	CString GetParentTreePath(CEGUI::Window* pWin);
	void	OnReloadScript();
	CString GetCreateWindowDefaultName(CEGUI::Window* pWin);
	
protected:
	VOID WriteXMLHead(CString &text, const CString& startWindowName);
	VOID WriteXMLEnd(CString& text, const CString& startWindowName);
	VOID WriteXMLContent(CString& text,CEGUI::Window* pWindow, const CString& gap);
	VOID WriteWindowEventToText(CString & text,CEGUI::Window* pWindow, const CString& gap);
	VOID WriteWindowPropertyToText(CString & text,CEGUI::Window* pWindow, const CString& gap);
	VOID InsertBaseEventList(CEGUI::Window* pWindow);
	VOID InsertBasePropertyList(CEGUI::Window* pWindow);
	VOID InsertSpecialEventList(CEGUI::Window* pWindow);
	VOID InsertSpecialPropertyList(CEGUI::Window* pWindow);
	VOID WriteWindowLuaFunction(CString& outText , CEGUI::Window* pWindow);
	VOID UpdateEventToPool(CEGUI::Window* pWindow);
protected:
	EventMap m_EventMap;
	CString m_szCreateWindowType;

public:
	CScriptModule m_scriptModule;
	CEGUI::Window* m_pCurrentSelectWindow;
};

extern CDataPool g_DataPool;