#include "StdAfx.h"
#include "DataPool.h"
#include "CoreSystem.h"
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUIFontManager.h>
#include <CEGUIExceptions.h>
#include "Global.h"
#include "RightEventList.h"
#include "RightPropList.h"
#include "LuaDlg.h"
#include "CreateWindowDlg.h"
#include "LayoutXMLPreviewDlg.h"

CDataPool g_DataPool;

using namespace CEGUI;

CDataPool::CDataPool(void)
{
}

CDataPool::~CDataPool(void)
{
}

//加载layout里面的定义
bool EventCallback(Window* window, String& eventName, String& FunctionName, void* userdata)
{
	if (window)
	{
		g_DataPool.AddWindowEventItem(window->getName().c_str(), eventName.c_str(), FunctionName.c_str(),TRUE);
	}
	return true;
}

bool CDataPool::Initialize()
{
	m_pCurrentSelectWindow = NULL;
	//初始化系统
	m_scriptModule.Initialize("");
	//加载全局函数
	OnReloadScript();

	//初始化界面中不变化的列表项
	InsertBaseEventList(NULL);
	InsertBasePropertyList(NULL);
	return true;
}

void CDataPool::OnReloadScript()
{
	//加载全局函数
	char szExePath[MAX_PATH];
	GetModuleFileName(NULL, szExePath,MAX_PATH);
	std::string ss = szExePath;
	CString szScriptPath = ss.substr(0,ss.find_last_of('\\')).c_str();
	szScriptPath += "/editor_global.lua";
	m_scriptModule.DoFile(szScriptPath.GetString());
}


BOOL CDataPool::OpenLayOut(const CHAR* szName)
{
	Window* pWindow = g_CoreSystem.LoadLayOut(szName,EventCallback);
	if(NULL == pWindow)
	{
		return FALSE;
	}
	WindowManager::getSingleton().getWindow(pWindow->getName().c_str());
	
	//插入到左边的树结构中
	g_leftTreeWindow->InsertItemToTree(pWindow->getName().c_str(), NULL);
	CString pParentName = pWindow->getName().c_str();
	InsertWindowToTree(pWindow,pParentName);
	return TRUE;
}

VOID CDataPool::UpdateEventToPool(CEGUI::Window* pWindow)
{
	if(pWindow)
	{
		EventSet::EventIterator it = pWindow->getIterator();
		while (!it.isAtEnd())
		{
			CEGUI::String eventName = it.getCurrentKey();
			CEGUI::Event* ev = it.getCurrentValue();
			ev->getName();

		}
	}
}

VOID CDataPool::InsertWindowToTree(CEGUI::Window* pParent,const CString& szParentPath)
{
	if (!pParent)
	{
		return;
	}
	for (INT i=0; i<pParent->getChildCount(); i++)
	{
		Window* pChild = pParent->getChildAtIdx(i);
		if (pChild->isAutoWindow())
		{
			continue;
		}
		g_leftTreeWindow->InsertItemToTree(pChild->getName().c_str(),szParentPath);
		CString szPath = szParentPath + "/";
		szPath += pChild->getName().c_str();
		InsertWindowToTree(pChild,szPath);
	}
}

VOID CDataPool::OnDeleteWindow(const CString& name)
{
	//清空窗口对应的数据
	Window* pWin = WindowManager::getSingleton().getWindow(name.GetString());
	DeleteWindowEvent(pWin);
	g_leftTreeWindow->DeleteItemFromName(name);
}

void CDataPool::OnSaveCommand(const CString& szName, BOOL bSaveRoot/* = FALSE*/)
{
	if( szName.GetLength() == 0 ) return;
	
	CString useName = szName;

	if (bSaveRoot)
	{
		useName = GetRootWindowName(szName);
	}
	CString testXML;
	CEGUI::Window* pWindow = WindowManager::getSingleton().getWindow(useName.GetString());

	//先刷新窗口定义
	GetRightEventList()->UpdateWindowEventDefine();

	WriteXMLHead(testXML, useName);
	WriteXMLContent(testXML,pWindow,"\t");
	WriteXMLEnd(testXML, useName);

	CFileDialog dlg(FALSE, useName, useName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Layout文件(*.layout.xml)|*.layout.xml||", AfxGetMainWnd());
	if(IDOK == dlg.DoModal())
	{
		//layout文件
		CString szFileName = dlg.GetFileName();
		CString szFullPath = dlg.GetPathName();
		//CString szLayoutXml = szFullPath + "\\" + useName;
		//szLayoutXml += ".layout.xml";
		FILE* pfileb =fopen(szFullPath.GetString() , "w");
		fwrite(testXML.GetString(), testXML.GetLength(),1, pfileb);
		fclose(pfileb);

		//lua文件
		testXML = "";
		szFullPath.Replace(szFileName,"");
		szFileName.Replace(".layout.xml","");
		CString szLua = szFullPath + "\\" + szFileName;
		szLua += ".lua";
		GenLuaText(testXML,useName,szFileName);

		//判断是否存在文件,提示用户
		pfileb = fopen(szLua, "r");
		if (pfileb)
		{
			CString szMsg = "脚本文件:\n";
			szMsg += szLua;
			szMsg += "\n已经存在,是否覆盖??";
			if( MessageBox(NULL, szMsg, "提示", MB_YESNO) == IDNO )
			{
				fclose(pfileb);
				return;
			}
		}
		pfileb = fopen(szLua, "w");
		fwrite(testXML.GetString(), testXML.GetLength(),1, pfileb);
		fclose(pfileb);
	}
	

}

void CDataPool::GenLuaText(CString& textLua,const CString& szWinName,const CString& szSystemName)
{
	//获取基本函数
	GetRightEventList()->GenSystemFunction(textLua, szSystemName);
	//找到这个窗口,然后遍历所有子窗口函数
	Window* pWindow = WindowManager::getSingleton().getWindow(szWinName.GetString());
	WriteWindowLuaFunction(textLua, pWindow);
}

VOID CDataPool::WriteWindowLuaFunction(CString& outText , CEGUI::Window* pWindow)
{
	if (pWindow)
	{
		EventMap::iterator it = m_EventMap.find(pWindow->getName().c_str());
		if (it != m_EventMap.end())
		{
			EventArray::iterator it2 = it->second.begin();
			EventArray::iterator ed2 = it->second.end();
			for (; it2 != ed2; it2++)
			{
				if(it2->funcName.Find(':') > 0) continue;
				outText += "\nfunction ";
				if (it2->isReadFromLayout)
				{
					//如果存在;那么去除它
					CString szTemp = it2->funcName;
					szTemp.Replace(";","");
					outText += szTemp;
					outText += "\n\nend\n";
				}
				else
				{
					outText += it2->funcName;
					outText += "()\n\nend\n";
				}
			}
		}
		//遍历子窗口
		for (size_t i=0; i<pWindow->getChildCount(); i++)
		{
			WriteWindowLuaFunction(outText, pWindow->getChildAtIdx(i) );
		}
	}
}


//读取用户设置的事件
void CDataPool::OnSelectWindowChanged(const CEGUI::Window* pOldWindow, const CEGUI::Window* pNewWindow)
{
	//如果是同一个窗口,那么返回
	if(pNewWindow)
	{
		CString szName = pNewWindow->getName().c_str();
		if( szName == GetRightEventList()->GetCurrentFunctionPrefix() )
		{
			return;
		}
	}

	//刷新选择窗口
	m_pCurrentSelectWindow = (CEGUI::Window*)pNewWindow;

	//刷新事件数据到DataPool中,保存上一个窗口的数据
	GetRightEventList()->UpdateWindowEventDefine();

	//插入数据到界面中,将新窗口的数据刷新到界面里
	InsertEventToList((CEGUI::Window*)pNewWindow);

	//插入属性数据到界面中
	InsertPropertyToList((CEGUI::Window*)pNewWindow);

	//刷新左边的选择树
	g_leftTreeWindow->OnWindowSelectedChange((CEGUI::Window*)pNewWindow);

	//是否显示xml
	if (g_xmlViewDlg->IsWindowVisible())
	{
		OnShowLayouXmlWindow(TRUE);
	}
}

void CDataPool::OnAddFunctionDlg(const CString& szName, const CString &szLeft, const CString& szRight)
{
	BOOL bDirty = FALSE;
	//添加到数据库里
	if (szLeft.GetLength() >0)
	{
		AddWindowEventItem(szName, "Clicked", szLeft);
		bDirty = TRUE;
	}

	if (szRight.GetLength() >0)
	{
		AddWindowEventItem(szName, "RBClicked", szRight);
		bDirty = TRUE;
	}
	//刷新界面
	if(bDirty && szName == GetRightEventList()->GetCurrentFunctionPrefix())
	{
		GetRightEventList()->SetCurrentFunctionPrefix(szName);
	}
}

void CDataPool::OnPropertyChange(const CString &szName, const CString& szValue, CEGUI::Window* pSpecialWindow/* = NULL*/)
{
	try
	{
		CEGUI::Window* pWin = pSpecialWindow ? pSpecialWindow : m_pCurrentSelectWindow;
		if (pWin)
		{
			//需要转化成utf8
			if(szName == "Text" || szName == "Tooltip" || szName == "Name")
			{
				if (szName == "Name")
				{
					CString szPath = GetParentTreePath(pWin);
					if (szPath.IsEmpty())
					{
						szPath = pWin->getName().c_str();
					}
					g_leftTreeWindow->onRenameSelectWindow(szValue,pWin->getName().c_str(), szPath);
					pWin->setName((CEGUI::utf8*)mbcs_to_utf8(szValue.GetString()));
					g_pEditorView->onRenameSelectedWindow(szValue,pWin);
				}
				else
				{
					pWin->setProperty(szName.GetString(), (CEGUI::utf8*)mbcs_to_utf8(szValue.GetString()));
				}
			}
			else
			{
				pWin->setProperty(szName.GetString(), szValue.GetString());
			}
		}
	}
	catch (CEGUI::Exception& e) 
	{
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), e.what(), "改变属性出错信息", MB_OK);
	}
	
}

void CDataPool::OnToolBarListWindowTypeChange(const CString& szTypeName)
{
	//创建显示这个窗口的样子
	try
	{
		if( szTypeName.GetLength() == 0 )
		{
			m_szCreateWindowType = "";
			g_pEditorView->setCreateWindowFlag(false);
			return;
		}
		if( WindowManager::getSingleton().isWindowPresent("___Shower_Window____") )
		{
			WindowManager::getSingleton().destroyWindow("___Shower_Window____");
		}

		Window* pWin = WindowManager::getSingleton().createWindow(szTypeName.GetString(), "___Shower_Window____");
		if (pWin)
		{
			System::getSingleton().getGUISheet()->addChildWindow(pWin);
			pWin->setPosition(CEGUI::Absolute, Point(1024,768));
			pWin->setWidth(CEGUI::Absolute, 160);
			pWin->setHeight(CEGUI::Absolute, 160);
		}
		m_szCreateWindowType = szTypeName;
		g_pEditorView->setCreateWindowFlag(true);
	}
	catch (CEGUI::UnknownObjectException& e)
	{
		char szMsg[100] = {0};
		sprintf(szMsg, "类型窗口%s定义有问题,请检查", szTypeName);
		MessageBox(NULL, szMsg,"提示",MB_OK);
		m_szCreateWindowType = "";
		g_pEditorView->setCreateWindowFlag(false);
	}

}


VOID CDataPool::WriteXMLContent(CString& text,CEGUI::Window* pWindow, const CString& gap)
{
	if (pWindow && !pWindow->isAutoWindow())
	{
		//写入Window的头
		CString textTemp = gap;
		textTemp += "<Window Type=\"";
		textTemp += pWindow->getType().c_str();
		textTemp += "\" Name=\"";
		textTemp += pWindow->getName().c_str();
		textTemp += "\" > \n";
		text += mbcs_to_utf8(textTemp);
		//写属性
		WriteWindowPropertyToText(text, pWindow,gap + "\t");
		//写事件
		WriteWindowEventToText(text, pWindow,gap + "\t");

		//递归
		Window::ChildList* pList = pWindow->getDrawList();
		if (pList)
		{
			for (INT i=0; i<pList->size(); i++)
			{
				WriteXMLContent(text,(*pList)[i],gap + "\t");
			}
		}
		//写入窗口结束
		text += gap;
		text += "</Window>\n";
	}
}

EventArray* CDataPool::GetWindowEvent(const CString& szName)
{
	EventMap::iterator it = m_EventMap.find(szName);
	if( it!= m_EventMap.end() )
	{
		return &(it->second);
	}
	return NULL;
}

void CDataPool::DeleteWindowEvent(CEGUI::Window* pWin)
{
	if (pWin)
	{
		//删除自己
		EventMap::iterator it = m_EventMap.find(pWin->getName().c_str());
		if( it!= m_EventMap.end() )
		{
			m_EventMap.erase(it);
		}
		//递归子窗口
		for (size_t i=0; i<pWin->getChildCount(); i++)
		{
			DeleteWindowEvent(pWin->getChildAtIdx(i));
		}
	}
}

void CDataPool::AddWindowEvent(const CString& szName)
{
	EventMap::iterator it = m_EventMap.find(szName);
	if( it== m_EventMap.end() )
	{
		m_EventMap.insert(std::make_pair(szName, EventArray()));
	}
}

void CDataPool::AddWindowEventItem(const CString& szWindowName, const CString& szEventName, const CString& szFunction,BOOL isFromLayout /* =FALSE*/)
{
	//如果没有,那么添加
	AddWindowEvent(szWindowName);
	EventArray* pEA = GetWindowEvent(szWindowName);
	if (pEA)
	{
		//更新旧的
		EventArray::iterator it = pEA->begin();
		EventArray::iterator end = pEA->end();
		for (; it != end; it++)
		{
			if(it->eventName == szEventName)
			{
				it->funcName = szFunction;
				return;
			}
		}
		//添加新的
		Event_Define ed;
		ed.eventName = szEventName;
		ed.funcName = szFunction;
		ed.isReadFromLayout = isFromLayout;
		pEA->push_back(ed);
	}
}

VOID CDataPool::WriteWindowEventToText(CString & text,CEGUI::Window* pWindow, const CString& gap)
{
	EventArray* pEventArray = GetWindowEvent(pWindow->getName().c_str());
	if (pEventArray)
	{
		CString textTemp;
		EventArray::iterator it = pEventArray->begin();
		EventArray::iterator end = pEventArray->end();
		for (; it != end; it++)
		{
			textTemp = gap;
			textTemp += "<Event Name=\"";
			textTemp += it->eventName;
			textTemp += "\" Function=\"";
			textTemp += it->funcName;
			if (it->isReadFromLayout)
			{
				textTemp += "\" />\n";
			}
			else
			{
				textTemp += "()\" />\n";
			}
			text += mbcs_to_utf8(textTemp);
		}
	}
}

VOID CDataPool::WriteWindowPropertyToText(CString & text,CEGUI::Window* pWindow, const CString& gap)
{
	if(pWindow)
	{
		////如果模板不再,那么现创建这种类型的窗口模板
		//CString szTemplateName = pWindow->getType().c_str();
		//szTemplateName += "_Template";
		//if (WindowManager::getSingleton().getWindow(szTemplateName.c_str()) == NULL)
		//{
		//	WindowManager::getSingleton().createWindow(pWindow->getType(), szTemplateName);
		//}
		std::ofstream file ;
		file.open ("C:\\TempWindow.bin", std::ios::out) ;
		if (!file.is_open ()) 
		{
			return ;
		}
		pWindow->writePropertiesXML(file,gap.GetString());
		file.close();
		FILE* pfile = fopen("C:\\TempWindow.bin", "rb");
		fseek(pfile, 0L, SEEK_END);
		size_t fsize = ftell(pfile);
		fseek(pfile, 0L, SEEK_SET);
		char* szRead = new char[fsize + 1];
		if (pfile)
		{
			size_t pos = fread(szRead,fsize+1,1,pfile);
			szRead[fsize] = 0;
			text += szRead;
			fclose(pfile);
		}
		delete[] szRead;
	}
}

VOID CDataPool::WriteXMLHead(CString &text, const CString& startWindowName)
{
	text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<GUILayout>\n";
}
VOID CDataPool::WriteXMLEnd(CString& text, const CString& startWindowName)
{
	text += "</GUILayout>";
}

//插入属性数据到界面
void CDataPool::InsertPropertyToList(CEGUI::Window* pWindow)
{
	//if(pWindow == NULL) return;

	//先不刷新
	GetRightPropList()->SetDelayUpdate(TRUE);

	//删除所有内容
	GetRightPropList()->RemoveItem("SpecialPropertyList");

	//插入这类窗口的特殊事件
	InsertSpecialPropertyList(pWindow);

	//刷新基本属性值
	GetRightPropList()->UpdateBaseProperty(pWindow);

	//刷新特殊属性值
	GetRightPropList()->UpdateSpecialProperty(pWindow);

	//展开所有的子项
	GetRightPropList()->ExpandAllItems();

	//然后刷新树
	//GetRightPropList()->SetDelayUpdate(FALSE);
	GetRightPropList()->ForceUpdate();

}

VOID CDataPool::InsertBasePropertyList(CEGUI::Window* pWindow)
{
	CRightPropList* pList = GetRightPropList();
	StringVector vec;

	//位置相关
	pList->CreateRootItem("PropertyPos","窗口位置大小相关","窗口位置大小相关集合");

	pList->InsertEditItem("UnifiedXPosition", "位置X{rx, ax}","",OTES_NORMAL,"UnifiedXPosition","PropertyPos");
	pList->InsertEditItem("UnifiedYPosition", "位置Y{ry, ay}","",OTES_NORMAL,"UnifiedYPosition","PropertyPos");
	pList->InsertEditItem("UnifiedWidth", "宽度{rw, aw}","",OTES_NORMAL,"UnifiedWidth","PropertyPos");
	pList->InsertEditItem("UnifiedHeight", "高度{rh, ah}","",OTES_NORMAL,"UnifiedHeight","PropertyPos");

	pList->InsertEditItem("UnifiedMaxSize", "窗口最大尺寸{{rw,aw},{rh,ah}}","",OTES_NORMAL,"UnifiedMaxSize","PropertyPos");
	pList->InsertEditItem("UnifiedMinSize", "窗口最小尺寸{{rw,aw},{rh,ah}}","",OTES_NORMAL,"UnifiedMinSize","PropertyPos");

	vec.push_back("");
	vec.push_back("Relative");
	vec.push_back("Absolute");
	vec.push_back("Inherited");
	pList->InsertComboItem("MetricsMode", "相对模式(Relative)随窗口变化自动缩放,绝对模式(Absolute),位置不会随父窗口变化而变化",vec, OTCS_NORMAL, "MetricsMode", "PropertyPos");

	//重要的设置
	pList->CreateRootItem("ImportantSet","重要设置","重要设置相关集合");
	pList->InsertEditItem("Name", "窗口Alpha","",OTES_NORMAL,"Name","ImportantSet");
	pList->InsertEditItem("Text", "窗口显示的文本","",OTES_NORMAL,"Text","ImportantSet");
	pList->InsertEditItem("Tooltip", "窗口Tooltip","",OTES_NORMAL,"Tooltip","ImportantSet");
	pList->InsertEditItem("ID", "窗口ID","",OTES_NORMAL,"ID","ImportantSet");
	pList->InsertEditItem("Alpha", "窗口Alpha","",OTES_NORMAL,"Alpha","ImportantSet");
	
	
	FontManager::FontIterator it = FontManager::getSingleton().getIterator();
	vec.clear();
	vec.push_back("");
	while ( !it.isAtEnd() )
	{
		vec.push_back(it.getCurrentKey().c_str());
		++it;
	}
	pList->InsertComboItem("Font", "窗口Font",vec, OTCS_NORMAL, "Font", "ImportantSet");

	vec.clear();
	vec.push_back("");
	vec.push_back("True");
	vec.push_back("False");
	pList->InsertComboItem("ClippedByParent", "是否被父窗口裁剪",vec, OTCS_NORMAL, "ClippedByParent", "ImportantSet");
	pList->InsertComboItem("Disabled", "是否Disable窗口",vec, OTCS_NORMAL, "Disabled", "ImportantSet");
	pList->InsertComboItem("Visible", "是否Disable窗口",vec, OTCS_NORMAL, "Visible", "ImportantSet");
	pList->InsertComboItem("RiseOnClick", "鼠标单击后是否将窗口提升到最高层",vec, OTCS_NORMAL, "RiseOnClick", "ImportantSet");

	
	//鼠标镂空
	pList->CreateRootItem("HollowMode","鼠标镂空相关","鼠标镂空相关集合");

	vec.clear();
	vec.push_back("");
	vec.push_back("True");
	vec.push_back("False");
	pList->InsertComboItem("MouseHollow", "所有鼠标操作都镂空",vec, OTCS_NORMAL, "MouseHollow", "HollowMode");
	pList->InsertComboItem("MouseMoveHollow", "镂空鼠标移动操作",vec, OTCS_NORMAL, "MouseMoveHollow", "HollowMode");
	pList->InsertComboItem("MouseLButtonHollow", "镂空鼠标左键操作",vec, OTCS_NORMAL, "MouseLButtonHollow", "HollowMode");
	pList->InsertComboItem("MouseRButtonHollow", "镂空鼠标右键操作",vec, OTCS_NORMAL, "MouseRButtonHollow", "HollowMode");

	//对齐方式
	pList->CreateRootItem("PropertyAlignmentMode","对齐方式相关","对齐方式相关集合");

	vec.clear();
	vec.push_back("");
	vec.push_back("Centre");
	vec.push_back("Bottom");
	vec.push_back("Top");
	pList->InsertComboItem("VerticalAlignment", "窗口竖直对齐方式",vec, OTCS_NORMAL, "VerticalAlignment", "PropertyAlignmentMode");

	vec.clear();
	vec.push_back("");
	vec.push_back("Centre");
	vec.push_back("Right");
	vec.push_back("Left");
	pList->InsertComboItem("HorizontalAlignment", "窗口水平对齐方式",vec, OTCS_NORMAL, "HorizontalAlignment", "PropertyAlignmentMode");

	//Hook模式
	pList->CreateRootItem("HookingMode","Hook模式","Hook模式集合");
	pList->InsertEditItem("HookPosition", "Hook的位置","",OTES_NORMAL,"HookPosition","HookingMode");

	vec.clear();
	vec.push_back("");
	vec.push_back("None");
	vec.push_back("Left");
	vec.push_back("Right");
	vec.push_back("Top");
	vec.push_back("Bottom");
	vec.push_back("LeftTop");
	vec.push_back("LeftBottom");
	vec.push_back("RightTop");
	vec.push_back("RightBottom");
	vec.push_back("Center");
	pList->InsertComboItem("HookMode", "定位到窗口的某个位置",vec, OTCS_NORMAL, "HookMode", "HookingMode");

	vec.clear();
	vec.push_back("");
	vec.push_back("Relative");
	vec.push_back("Absolute");
	vec.push_back("Inherited");
	pList->InsertComboItem("ZoomMode", "缩放模式Absolute和Relative",vec, OTCS_NORMAL, "ZoomMode", "HookingMode");

	//调用脚本添加基本属性
	//创建父窗口
	pList->CreateRootItem("OtherSets","其他属性","其他属性集合");

	vec.clear();
	vec.push_back("");
	vec.push_back("True");
	vec.push_back("False");
	pList->InsertComboItem("MouseButtonDownAutoRepeat", "鼠标事件是否自动重复",vec, OTCS_NORMAL, "MouseButtonDownAutoRepeat", "OtherSets");

	pList->InsertEditItem("AutoRepeatDelay", "重复延迟时间","",OTES_NORMAL,"AutoRepeatDelay","OtherSets");
	pList->InsertEditItem("AutoRepeatRate", "重复延迟时间","",OTES_NORMAL,"AutoRepeatRate","OtherSets");
	pList->InsertEditItem("MouseCursorImage", "图片作为鼠标(set:setname image:imagename)","",OTES_NORMAL,"MouseCursorImage","OtherSets");
	
		
	//调用脚本处理
	m_scriptModule.DoString("getBasePropList('OtherSets')");
	
	
	pList->ExpandAllItems();
}

VOID CDataPool::InsertSpecialPropertyList(CEGUI::Window* pWindow)
{
	if(!pWindow) return;
	//创建父窗口
	GetRightPropList()->CreateRootItem("SpecialPropertyList","窗口个性属性","窗口个性属性集合");
	//调用脚本处理
	char szFunction[256] = {0};
	sprintf(szFunction, "getSpecialWindowPropertyList(\"%s\",\"SpecialPropertyList\")", pWindow->getWidgetType().c_str());
	m_scriptModule.DoString(szFunction);
}

void CDataPool::InsertEventToList(CEGUI::Window* pWindow)
{
	CString szName;
	if(pWindow != NULL) 
	{
		szName = pWindow->getName().c_str();
	}
	//先不刷新
	GetRightEventList()->SetDelayUpdate(TRUE);

	//删除所有内容
	GetRightEventList()->RemoveItem("SpecialWindowEvent",TRUE);

	//插入这类窗口的特殊事件
	InsertSpecialEventList(pWindow);

	//插入Window的事件
	GetRightEventList()->SetCurrentFunctionPrefix(szName);

	//展开所有的子项
	GetRightEventList()->ExpandAllItems();

	//然后刷新树
	GetRightPropList()->SetDelayUpdate(FALSE);
	GetRightEventList()->ForceUpdate();
}

VOID CDataPool::InsertBaseEventList(CEGUI::Window* pWindow)
{
	CRightEventList* pList = GetRightEventList();
	
	pList->CreateRootItem("WindowBaseEvent","窗口基本事件","窗口基本事件集合");

	//如果需要下面的事件,请打开注释,不过一般来说不需要
	
	//pList->InsertItemS(Window::EventParentSized.c_str(),"WindowBaseEvent");				//!< Parent of this Window has been re-sized.
	//pList->InsertItemS(Window::EventSized.c_str(),"WindowBaseEvent");					//!< Window size has changed
	//pList->InsertItemS(Window::EventMoved.c_str(),"WindowBaseEvent");					//!< Window position has changed
	pList->InsertItemS(Window::EventTextChanged.c_str(),"WindowBaseEvent");				//!< Text string for the Window has changed
	pList->InsertItemS(Window::EventFontChanged.c_str(),"WindowBaseEvent");		//!< Font object for the Window has been changed
	//pList->InsertItemS(Window::EventAlphaChanged.c_str(),"WindowBaseEvent");	//!< Alpha blend value for the Window has changed
	//pList->InsertItemS(Window::EventIDChanged.c_str(),"WindowBaseEvent");	//!< Client assigned ID code for the Window has changed
	//pList->InsertItemS(Window::EventActivated.c_str(),"WindowBaseEvent");		//!< Window has been activated (has input focus)
	//pList->InsertItemS(Window::EventDeactivated.c_str(),"WindowBaseEvent");				//!< Window has been deactivated (loses input focus)
	pList->InsertItemS(Window::EventShown.c_str(),"WindowBaseEvent");					//!< Window has been made visible
	pList->InsertItemS(Window::EventHidden.c_str(),"WindowBaseEvent");			//!< Window has been hidden from view
	pList->InsertItemS(Window::EventParentHidden.c_str(),"WindowBaseEvent");			//!< Window's parent has been hidden from view
	pList->InsertItemS(Window::EventEnabled.c_str(),"WindowBaseEvent");		//!< Window has been enabled (interaction is possible)
	pList->InsertItemS(Window::EventDisabled.c_str(),"WindowBaseEvent");		//!< Window has been disabled (interaction is no longer possible)
	//pList->InsertItemS(Window::EventMetricsModeChanged.c_str(),"WindowBaseEvent");	//!< Active metrics mode has been modified
	//pList->InsertItemS(Window::EventClippedByParentChanged.c_str(),"WindowBaseEvent");	//!< Clipping by parent mode has been modified
	//pList->InsertItemS(Window::EventDestroyedByParentChanged.c_str(),"WindowBaseEvent");//!< Destruction by parent mode has been modified
	//pList->InsertItemS(Window::EventInheritsAlphaChanged.c_str(),"WindowBaseEvent");	//!< Alpha inherited from parent mode has been modified.
	//pList->InsertItemS(Window::EventAlwaysOnTopChanged.c_str(),"WindowBaseEvent");		//!< Always on top mode has been modified
	pList->InsertItemS(Window::EventInputCaptureGained.c_str(),"WindowBaseEvent");		//!< Window has captured all inputs
	pList->InsertItemS(Window::EventInputCaptureLost.c_str(),"WindowBaseEvent");		//!< Window has lost it's capture on inputs
	//pList->InsertItemS(Window::EventRenderingStarted.c_str(),"WindowBaseEvent");		//!< Rendering of the Window has started
	//pList->InsertItemS(Window::EventRenderingEnded.c_str(),"WindowBaseEvent");			//!< Rendering for the Window has finished
	//pList->InsertItemS(Window::EventChildAdded.c_str(),"WindowBaseEvent");				//!< A child Window has been added
	//pList->InsertItemS(Window::EventChildRemoved.c_str(),"WindowBaseEvent");			//!< A child window has been removed
	//pList->InsertItemS(Window::EventDestructionStarted.c_str(),"WindowBaseEvent");		//!< Destruction of the Window is about to begin.
	//pList->InsertItemS(Window::EventZOrderChanged.c_str(),"WindowBaseEvent");			//!< The z-order of the window has changed
	//pList->InsertItemS(Window::EventDragDropItemEnters.c_str(),"WindowBaseEvent");	//!< A DragContainer has been dragged over this window.
	//pList->InsertItemS(Window::EventDragDropItemLeaves.c_str(),"WindowBaseEvent");	//!< A DragContainer has left this window.
	//pList->InsertItemS(Window::EventDragDropItemDropped.c_str(),"WindowBaseEvent");	//!< A DragContainer was dropped on this Window.
	//pList->InsertItemS(Window::EventVerticalAlignmentChanged.c_str(),"WindowBaseEvent");    //!< The vertical alignment of the window has changed.
	//pList->InsertItemS(Window::EventHorizontalAlignmentChanged.c_str(),"WindowBaseEvent");  //!< The vertical alignment of the window has changed.

	// generated externally (inputs)
	pList->InsertItemS(Window::EventMouseEnters.c_str(),"WindowBaseEvent");				//!< Mouse cursor has entered the Window.
	pList->InsertItemS(Window::EventMouseLeaves.c_str(),"WindowBaseEvent");				//!< Mouse cursor has left the Window.
	pList->InsertItemS(Window::EventMouseMove.c_str(),"WindowBaseEvent");				//!< Mouse cursor was moved within the area of the Window.
	//pList->InsertItemS(Window::EventMouseWheel.c_str(),"WindowBaseEvent");				//!< Mouse wheel was scrolled within the Window.
	pList->InsertItemS(Window::EventMouseButtonDown.c_str(),"WindowBaseEvent");			//!< A mouse button was pressed down within the Window.
	pList->InsertItemS(Window::EventMouseButtonUp.c_str(),"WindowBaseEvent");			//!< A mouse button was released within the Window.
	pList->InsertItemS(Window::EventMouseClick.c_str(),"WindowBaseEvent");				//!< A mouse button was clicked (down then up) within the Window.
	pList->InsertItemS(Window::EventMouseDoubleClick.c_str(),"WindowBaseEvent");		//!< A mouse button was double-clicked within the Window.
	//pList->InsertItemS(Window::EventMouseTripleClick.c_str(),"WindowBaseEvent");		//!< A mouse button was triple-clicked within the Window.
	//pList->InsertItemS(Window::EventKeyDown.c_str(),"WindowBaseEvent");					//!< A key on the keyboard was pressed.
	//pList->InsertItemS(Window::EventKeyUp.c_str(),"WindowBaseEvent");					//!< A key on the keyboard was released.
	pList->InsertItemS(Window::EventCharacterKey.c_str(),"WindowBaseEvent");			//!< A text character was typed on the keyboard.
	pList->InsertItemS(Window::EventMouseLClick.c_str(),"WindowBaseEvent");
	pList->InsertItemS(Window::EventMouseRClick.c_str(),"WindowBaseEvent");
	pList->InsertItemS(Window::EventMouseLButtonDown.c_str(),"WindowBaseEvent");			//!< A mouse left button was pressed down within the Window.
	pList->InsertItemS(Window::EventMouseRButtonDown.c_str(),"WindowBaseEvent");			//!< A mouse right button wa

	//拷贝数据到基本表中
	pList->CopyToBaseAndClear();
	//全部展开
	pList->ExpandAllItems();
}

VOID CDataPool::InsertSpecialEventList(CEGUI::Window* pWindow)
{
	if(!pWindow) return;
	//创建父窗口
	GetRightEventList()->CreateRootItem("SpecialWindowEvent","窗口个性事件","窗口个性事件集合");
	//调用脚本处理
	char szFunction[256] = {0};
	sprintf(szFunction, "getSpecialWindowEventList(\"%s\")", pWindow->getWidgetType().c_str());
	m_scriptModule.DoString(szFunction);
}

void CDataPool::OnShowLuaWindow(BOOL show)
{
	//渲染是否暂停
	//g_pEditorView->renderPause(show);
	//设置Lua文字
	CString textLua;
	if(GetRightEventList()->GetCurrentFunctionPrefix().GetLength() > 0)
	{
		CString winName = GetRootWindowName(GetRightEventList()->GetCurrentFunctionPrefix());
		Window* pWin = WindowManager::getSingleton().getWindow(winName.GetString());
		if (pWin)
		{
			GetRightEventList()->GenSystemFunction(textLua, pWin->getName().c_str());
			GetRightEventList()->UpdateWindowEventDefine();
			WriteWindowLuaFunction(textLua, pWin);
			g_luaDlg->SetLuaText(textLua);
		}
	}
}

void CDataPool::OnShowLayouXmlWindow(BOOL show)
{
	if( show && m_pCurrentSelectWindow )
	{
		CString szName = m_pCurrentSelectWindow->getName().c_str();

		CString testXML;
		//先刷新窗口定义
		GetRightEventList()->UpdateWindowEventDefine();

		WriteXMLHead(testXML, szName);
		WriteXMLContent(testXML,m_pCurrentSelectWindow,"\t");
		WriteXMLEnd(testXML, szName);

		g_xmlViewDlg->SetLayoutText(testXML);
	}
}

CString CDataPool::GetRootWindowName(const CString& szName)
{
	Window* pWindow = WindowManager::getSingleton().getWindow(szName.GetString());
	if (pWindow)
	{
		while (pWindow->getParent() && pWindow->getParent() != System::getSingleton().getGUISheet())
		{
			pWindow = pWindow->getParent();
		}
		return pWindow->getName().c_str();
	}
	return "";
}

//通知创建一个新窗口
BOOL CDataPool::OnCreateNewWindow(CEGUI::Window* pWinParent)
{
	CEGUI::Window* pWindow = NULL;
	if (m_szCreateWindowType.IsEmpty())
	{
		g_pEditorView->setCreateWindowFlag(false);
		g_pEditorView->setCreateWindow(NULL);
		return FALSE;
	}

	////显示名字对话框
	//CCreateWindowDlg dlg(pWinParent == NULL);
	//if (dlg.DoModal() == IDCANCEL)
	//{
	//	return FALSE;
	//}


	//CString name ;
	//name = dlg.m_szDefaultName;

	//if (name.IsEmpty())
	//{
	//	g_pEditorView->setCreateWindowFlag(false);
	//	g_pEditorView->setCreateWindow(NULL);
	//	return FALSE;
	//}

	//创建窗口
	try
	{
		pWindow = WindowManager::getSingleton().createWindow(m_szCreateWindowType.GetString()/*, name.GetString()*/);
		pWindow->setClippedByParent(true);
		//pWindow->setPosition(CEGUI::Absolute, Point(0,0));
		pWindow->setSize(CEGUI::Absolute, Size(100,100));
		if (pWinParent == NULL)
		{
			System::getSingleton().getGUISheet()->addChildWindow(pWindow);
			g_leftTreeWindow->InsertItemToTree(pWindow->getName().c_str(), NULL);
		}
		else
		{
			pWinParent->addChildWindow(pWindow);
			CString szParentPath = GetParentTreePath(pWindow);
			g_leftTreeWindow->InsertItemToTree(pWindow->getName().c_str(), szParentPath);
		}
		g_pEditorView->setCreateWindow(pWindow);
		return TRUE;
	}
	catch (...)
	{
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), "创建窗口出现未知错误", "提示", MB_OK);
	}
	return FALSE;
}

CString CDataPool::GetParentTreePath(CEGUI::Window* pWindow )
{
	std::string  outName;
	if(pWindow)
	{
		while (pWindow->getParent() && pWindow->getParent() != System::getSingleton().getGUISheet())
		{
			outName += "/";
			outName += pWindow->getParent()->getName().c_str();
			pWindow = pWindow->getParent();
		}
	}
	//去除多于的一个/
	if(outName.length()> 0)
	{
		return outName.substr(1).c_str();
	}
	return outName.c_str();
}

CString CDataPool::GetCreateWindowDefaultName(CEGUI::Window* pWin)
{
	if( pWin == NULL ) return "";

	CString szRoot = GetRootWindowName(pWin->getName().c_str());

	if ( szRoot.IsEmpty() || szRoot == pWin->getName().c_str())
	{
		return "";
	}

	return szRoot + "_";
}