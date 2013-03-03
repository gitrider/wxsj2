
#include "StdAfx.h"
#include "UIScript.h"
#include "UIGlobal.h"
#include "GIScriptSystem.h"
#include "GIException.h"
#include "UIWindowMng.h"
#include "LuaPlus.h"
#include "UILuaControl.h"
#include "UIIconsMng.h"
#include <CEGUIPropertyHelper.h>



//CEGUI Event ������ѯ�ӿ�
class CEventArg
{
public:

	//��ѯ����ֵ
	int LUA_GetValue(LuaPlus::LuaState* pState)
	{
		LuaStack args(pState);

		if(!(args[2].IsString()) || m_pEventArg == 0)
		{
			pState->PushString("");
			return 1;
		}

		STRING strArgName = args[2].GetString();

		//-------------------------------------------
		// WindowName
		if(strArgName == "WindowName" && m_pEventArg->isKindOf((CEGUI::utf8*)"Window"))
		{
			if(((CEGUI::WindowEventArgs*)m_pEventArg)->window)
			{
				pState->PushString(((CEGUI::WindowEventArgs*)m_pEventArg)->window->getName().c_str());
			}
			else
			{
				pState->PushString("");
			}
		}
		//-------------------------------------------
		// HyperLinkName
		else if(strArgName == "HyperLinkName" && m_pEventArg->isKindOf((CEGUI::utf8*)"HyperLink"))
		{
			pState->PushString(((CEGUI::HyperLinkEventArgs*)m_pEventArg)->hyperLinkName.c_str());
		}
		//-------------------------------------------
		// MousePosition
		else if(strArgName == "MousePosition" && m_pEventArg->isKindOf((CEGUI::utf8*)"Mouse"))
		{
			pState->PushString(CEGUI::PropertyHelper::pointToString(((CEGUI::MouseEventArgs*)m_pEventArg)->position).c_str());
		}
		//-------------------------------------------
		// MouseButton
		else if(strArgName == "MouseButton"  && m_pEventArg->isKindOf((CEGUI::utf8*)"Mouse"))
		{
			switch(((CEGUI::MouseEventArgs*)m_pEventArg)->button)
			{
			case CEGUI::LeftButton:
				pState->PushString("LeftButton");
				break;
			case CEGUI::RightButton:
				pState->PushString("RightButton");
				break;
			case CEGUI::MiddleButton:
				pState->PushString("MiddleButton");
				break;
			case CEGUI::X1Button:
				pState->PushString("X1Button");
				break;
			case CEGUI::X2Button:
				pState->PushString("X2Button");
				break;
			case CEGUI::NoButton:
				pState->PushString("NoButton");
				break;
			default:
				pState->PushString("");
				break;
			}
		}
		else if(strArgName == "MouseClickCount")
		{
			pState->PushString(CEGUI::PropertyHelper::intToString(((CEGUI::MouseEventArgs*)m_pEventArg)->clickCount).c_str());
		}

		return 1;
	}

public:
	CEventArg() : m_pMetaTable(0), m_pEventArg(0){}
	virtual ~CEventArg(){}
	LuaPlus::LuaObject* m_pMetaTable;
	
	//�������
	const CEGUI::EventArgs*	m_pEventArg;
};

//ȫ��Event������ѯ�ӿ�
static CEventArg g_theUIEventArg;


//////////////////////////////////////////////////////////////////

namespace CEGUI
{

CGameUIScript::CGameUIScript()
{
}

CGameUIScript::~CGameUIScript()
{
}

void CGameUIScript::createBindings(void)
{
	LuaPlus::LuaState* pLuaState = g_pScriptSys->GetLuaState();
	KLAssert(pLuaState);

	//----------------------------------------------------------------------------------
	//ȫ�ֺ���
	pLuaState->GetGlobals().Register("GetIconFullName", CUIIconsManager::LUA_GetIconFullName);
	
	//----------------------------------------------------------------------------------
	//Event ����
	{
		g_theUIEventArg.m_pMetaTable = new LuaPlus::LuaObject;
		*(g_theUIEventArg.m_pMetaTable) = pLuaState->GetGlobals().CreateTable("CEGUIEventMetaTable");
		g_theUIEventArg.m_pMetaTable->SetObject("__index", *(g_theUIEventArg.m_pMetaTable));

		g_theUIEventArg.m_pMetaTable->RegisterObjectFunctor("GetValue", &CEventArg::LUA_GetValue);

		LuaObject obj_UIEvent = pLuaState->BoxPointer(&(g_theUIEventArg));
		obj_UIEvent.SetMetaTable(*(g_theUIEventArg.m_pMetaTable));
		pLuaState->GetGlobals().SetObject("CEArg", obj_UIEvent);
	}

	//----------------------------------------------------------------------------------
	//ע�����ö���

	//CUIWindowItem(this)
	{
		LuaObject metaUIWindowItem = pLuaState->GetGlobals().CreateTable("MetaTable_UIWindowItem");
		metaUIWindowItem.SetObject("__index", metaUIWindowItem);
		
		metaUIWindowItem.RegisterObjectFunctor("RegisterEvent"		, &CUIWindowItem::LUA_RegisterEvent);
		metaUIWindowItem.RegisterObjectFunctor("Show"				, &CUIWindowItem::LUA_Show);
		metaUIWindowItem.RegisterObjectFunctor("Hide"				, &CUIWindowItem::LUA_Hide);
		metaUIWindowItem.RegisterObjectFunctor("TogleShow"			, &CUIWindowItem::LUA_TogleShow);
		metaUIWindowItem.RegisterObjectFunctor("IsVisible"			, &CUIWindowItem::LUA_IsVisible);
		metaUIWindowItem.RegisterObjectFunctor("TransAllWindowText"	, &CUIWindowItem::LUA_TransAllWindowText);
		metaUIWindowItem.RegisterObjectFunctor("CareObject"			, &CUIWindowItem::LUA_CareObject);
	}


	//�ؼ�
	{
		LUA_CONTROL::Window::RegisterMetaTable();
	}

}

void CGameUIScript::destroyBindings(void)
{
	//Event ����
	delete g_theUIEventArg.m_pMetaTable;
	g_theUIEventArg.m_pMetaTable = 0;
	
	//�ͷſؼ�ע��
	LUA_CONTROL::Window::DestroyMetaTable();
}

void CGameUIScript::executeScriptFile(const String& filename, const String &resourceGroup)
{
}

INT CGameUIScript::executeScriptGlobal(const String& function_name)
{
	return 0;
}

void CGameUIScript::executeString(const String& str)
{
}

bool CGameUIScript::executeScriptedEventHandler(const String& strHandle, const EventArgs& e)
{
	const WindowEventArgs& eWindow = (const WindowEventArgs&)e;
	Window* pWindow = eWindow.window;
	g_theUIEventArg.m_pEventArg = &e;

	do
	{
		if(!pWindow) break;
		void* pUserData = pWindow->getUserData();
		if(pUserData)
		{
			((CUIWindowItem*)pUserData)->FireUIEvent(strHandle.c_str(), eWindow.window);
			break;
		}

		pWindow = pWindow->getParent();
	}while(pWindow);

	g_theUIEventArg.m_pEventArg = 0;
	return true;
}

}