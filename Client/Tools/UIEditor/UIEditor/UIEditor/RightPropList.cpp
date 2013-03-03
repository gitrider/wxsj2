// RightPropList.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "RightPropList.h"
#include "FontManger.h"
#include "Global.h"
#include "DataPool.h"
#include <CEGUIWindow.h>
#include <CEGUIPropertyHelper.h>
#include <CEGUIExceptions.h>

// CRightPropList

IMPLEMENT_DYNAMIC(CRightPropList, CPropertyListBase)

CRightPropList::CRightPropList()
{
	m_currMapImage = 0;
	memset(m_imageBuf, 0, sizeof(m_imageBuf));
}

CRightPropList::~CRightPropList()
{
}


BEGIN_MESSAGE_MAP(CRightPropList, CPropertyListBase)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CRightPropList 消息处理程序


void CRightPropList::InsertItemS(const CString& szName, const CString &szType)
{
	
}

void CRightPropList::OnSize(UINT nType, int cx, int cy)
{
	CPropertyListBase::OnSize(nType, cx, cy);
}

void CRightPropList::OnTabLoseFocus()
{

}

void CRightPropList::AddMapImage(int nImage , int nItem)
{
	if (m_currMapImage< MAX_MAP_SIZE && m_currMapImage >= 0)
	{
		if (!m_mapImage[m_currMapImage].DataValid())
		{
			m_mapImage[m_currMapImage++].Set(nImage, nItem);
		}
		else
		{
			for (int i=0; i<MAX_MAP_SIZE; i++)
			{
				if (!m_mapImage[i].DataValid())
				{
					m_mapImage[i].Set(nImage, nItem);
					return ;
				}
			}
		}
	}
}
int    CRightPropList::FindMapImage(int nItem)
{
	for (int i=0; i<MAX_MAP_SIZE; i++)
	{
		if(m_mapImage[i].Find(nItem))
			return m_mapImage[i].nImageID;
	}
	return -1;
}
void CRightPropList::ClearMapImage()
{
	for (int i=0; i<MAX_MAP_SIZE; i++)
	{
		m_mapImage[i].Clear();
	}
}

BYTE* CRightPropList::CreateImageDataFromColor(COLORREF color)
{
	for (register int i=0; i<16*16*4; )
	{
		m_imageBuf[i++] = (BYTE)((color &0x000000FF) );
		m_imageBuf[i++] = (BYTE)((color &0x0000FF00) >>8);
		m_imageBuf[i++] = (BYTE)((color &0x00FF0000) >>16);
		m_imageBuf[i++] = 255;
	}
	return m_imageBuf;
}

void CRightPropList::UpdateBasePosAndSizeProperty(CEGUI::Window* pWindow)
{
	if(pWindow == NULL) return;
	COptionTreeItemEdit* pEdit = NULL;
	CString szContent;
	//PropertyPos
	szContent = pWindow->getProperty("UnifiedXPosition").c_str();
	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedXPosition");
	pEdit->SetWindowText(szContent);
	//pEdit->OnRefresh();

	szContent = pWindow->getProperty("UnifiedYPosition").c_str();
	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedYPosition");
	pEdit->SetWindowText(szContent);
	//pEdit->OnRefresh();

	szContent = pWindow->getProperty("UnifiedWidth").c_str();
	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedWidth");
	pEdit->SetWindowText(szContent);
	//pEdit->OnRefresh();

	szContent = pWindow->getProperty("UnifiedHeight").c_str();
	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedHeight");
	pEdit->SetWindowText(szContent);
	//pEdit->OnRefresh();

	szContent = pWindow->getProperty("UnifiedMaxSize").c_str();
	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedMaxSize");
	pEdit->SetWindowText(szContent);
	//pEdit->OnRefresh();


	szContent = pWindow->getProperty("UnifiedMinSize").c_str();
	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedMinSize");
	pEdit->SetWindowText(szContent);
	//pEdit->OnRefresh();

}

void CRightPropList::UpdateBaseProperty(CEGUI::Window* pWindow)
{
	if(pWindow)
	{
		//刷新基本属性的显示
		CString szContent;
		COptionTreeItemEdit* pEdit;
		COptionTreeItemComboBox* pCombo;
		m_szWindowName = pWindow->getName().c_str();

		//PropertyPos
		szContent = pWindow->getProperty("UnifiedXPosition").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedXPosition");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("UnifiedYPosition").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedYPosition");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("UnifiedWidth").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedWidth");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("UnifiedHeight").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedHeight");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("UnifiedMaxSize").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedMaxSize");
		pEdit->SetWindowText(szContent);


		szContent = pWindow->getProperty("UnifiedMinSize").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedMinSize");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("MetricsMode").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MetricsMode");
		pCombo->SelectString(0,szContent);

		//重要属性

		
		szContent = pWindow->getName().c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Name");
		pEdit->SetWindowText(utf8_to_mbcs(szContent.GetString()));

		szContent = pWindow->getProperty("Text").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Text");
		pEdit->SetWindowText(utf8_to_mbcs(szContent.GetString()));

		szContent = pWindow->getProperty("Tooltip").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Tooltip");
		pEdit->SetWindowText(utf8_to_mbcs(szContent.GetString()));

		szContent = pWindow->getProperty("ID").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("ID");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("Alpha").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Alpha");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("Font").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("Font");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("ClippedByParent").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("ClippedByParent");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("Disabled").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("Disabled");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("Visible").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("Visible");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("RiseOnClick").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("RiseOnClick");
		pCombo->SelectString(0,szContent);

		
		//HollowMode

		szContent = pWindow->getProperty("MouseHollow").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseHollow");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("MouseMoveHollow").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseMoveHollow");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("MouseLButtonHollow").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseLButtonHollow");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("MouseRButtonHollow").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseRButtonHollow");
		pCombo->SelectString(0,szContent);
		
			
		//对齐方式
		szContent = pWindow->getProperty("VerticalAlignment").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("VerticalAlignment");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("HorizontalAlignment").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("HorizontalAlignment");
		pCombo->SelectString(0,szContent);
		
		//Hook模式
			
		szContent = pWindow->getProperty("HookMode").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("HookMode");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("HookPosition").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("HookPosition");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("ZoomMode").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("ZoomMode");
		pCombo->SelectString(0,szContent);
			
		//其他设置
		szContent = pWindow->getProperty("MouseButtonDownAutoRepeat").c_str();
		pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseButtonDownAutoRepeat");
		pCombo->SelectString(0,szContent);

		szContent = pWindow->getProperty("AutoRepeatDelay").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("AutoRepeatDelay");
		pEdit->SetWindowText(szContent);
		
		szContent = pWindow->getProperty("MouseCursorImage").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("MouseCursorImage");
		pEdit->SetWindowText(szContent);

		szContent = pWindow->getProperty("AutoRepeatRate").c_str();
		pEdit = (COptionTreeItemEdit*)GetItemTreeByName("AutoRepeatRate");
		pEdit->SetWindowText(szContent);

	}
	else
	{
		ResetBaseProperty();
	}
}

void CRightPropList::ResetBaseProperty()
{
	//刷新基本属性的显示
	CString szContent;
	COptionTreeItemEdit* pEdit;
	COptionTreeItemComboBox* pCombo;

	//PropertyPos
	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedXPosition");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedYPosition");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedWidth");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedHeight");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedMaxSize");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("UnifiedMinSize");
	pEdit->SetWindowText("");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MetricsMode");
	pCombo->SelectString(0,"Releative");

	//重要属性

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Name");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Text");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Tooltip");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("ID");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("Alpha");
	pEdit->SetWindowText(szContent);

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("Font");
	pCombo->SelectString(0,"");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("ClippedByParent");
	pCombo->SelectString(0,"false");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("Disabled");
	pCombo->SelectString(0,"false");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("Visible");
	pCombo->SelectString(0,"");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("RiseOnClick");
	pCombo->SelectString(0,"");


	//HollowMode

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseHollow");
	pCombo->SelectString(0,"");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseMoveHollow");
	pCombo->SelectString(0,"");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseLButtonHollow");
	pCombo->SelectString(0,"");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseRButtonHollow");
	pCombo->SelectString(0,"");


	//对齐方式
	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("VerticalAlignment");
	pCombo->SelectString(0,"");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("HorizontalAlignment");
	pCombo->SelectString(0,"");

	//Hook模式

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("HookMode");
	pCombo->SelectString(0,"");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("HookPosition");
	pEdit->SetWindowText("");

	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("ZoomMode");
	pCombo->SelectString(0,"");

	//其他设置
	pCombo = (COptionTreeItemComboBox*)GetItemTreeByName("MouseButtonDownAutoRepeat");
	pCombo->SelectString(0,"");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("AutoRepeatDelay");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("MouseCursorImage");
	pEdit->SetWindowText("");

	pEdit = (COptionTreeItemEdit*)GetItemTreeByName("AutoRepeatRate");
	pEdit->SetWindowText("");

}

//刷新特殊属性列表
void CRightPropList::UpdateSpecialProperty(CEGUI::Window* pWin)
{
	try
	{
		if( !pWin ) return;

		CString str = g_DataPool.m_scriptModule.DoFunctionAndRetString("onUpdateSpecialProperty", 
			pWin->getWidgetType().c_str());

		//将字符串转换成属性名字
		StringVector vec;
		char* token = NULL;
		char* ses = " ";
		token = strtok( str.GetBuffer(), ses );
		while( token != NULL )
		{
			/* While there are tokens in "string" */
			vec.push_back(token);
			/* Get next token: */
			token = strtok( NULL, ses );
		}

		//遍历整个数组
		size_t size = vec.size();

		for (size_t i=0; i<size; i++)
		{

			CString szPropValue = pWin->getProperty(vec[i].GetString()).c_str();

			COptionTreeItem* pItem  = GetItemTreeByName(vec[i]);
			if (pItem)
			{
				switch(pItem->GetItemType())
				{
				case OT_ITEM_COLOR:
					{
						CEGUI::colour c = CEGUI::PropertyHelper::stringToColour(szPropValue.GetString());
						((COptionTreeItemColor *)pItem)->SetColor(RGB(c.getRed()*255, c.getBlue()*255, c.getGreen()*255));
					}
					break;
				case OT_ITEM_COMBOBOX:
					{
						((COptionTreeItemComboBox *)pItem)->SelectString(0,szPropValue);
					}
					break;
				case OT_ITEM_EDIT:
					{				
						((COptionTreeItemEdit *)pItem)->SetWindowText(szPropValue);
					}
					break;
				}
			}
		}
	}
	catch (CEGUI::Exception& e)
	{
		MessageBox(e.getMessage().c_str(), "错误提示");
	}

}

void CRightPropList::OnOptionTreeNotify(NMOPTIONTREE* pTreeItem)
{
	if( !pTreeItem ) return;
	if( pTreeItem->hDR.code == OT_NOTIFY_ITEMCHANGED )
	{
		int itemType = pTreeItem->otiItem->GetItemType();
		CString& text = pTreeItem->otiItem->GetLabelText();
		CString szPropValue;
		switch(itemType)
		{
		case OT_ITEM_STATIC:
			break;
		case OT_ITEM_CHECKBOX:
			break;
		case OT_ITEM_COLOR:
			{
				COLORREF c = ((COptionTreeItemColor *)pTreeItem->otiItem)->GetColor();
				szPropValue.Format("0xFF%.2X%.2X%.2X", c&0x000000FF, (c&0x0000FF00)>>8, (c&0x00FF0000)>>16);
			}
			break;
		case OT_ITEM_COMBOBOX:
			{
				((COptionTreeItemComboBox *)pTreeItem->otiItem)->GetWindowText(szPropValue);
			}
			break;
		case OT_ITEM_DATE:
			break;
		case OT_ITEM_EDIT:
			{				
				((COptionTreeItemEdit *)pTreeItem->otiItem)->GetWindowText(szPropValue);
			}
			break;
		case OT_ITEM_IMAGE:
			break;
		case OT_ITEM_RADIO:
			break;
		case OT_ITEM_SPINNER:
			break;
		case OT_ITEM_FONT:
			break;
		case OT_ITEM_FILE:
			break;
		case OT_ITEM_IPADDRESS:
			break;
		case OT_ITEM_LISTBOX:
			break;
		}
		g_DataPool.OnPropertyChange(text, szPropValue);
	}
}