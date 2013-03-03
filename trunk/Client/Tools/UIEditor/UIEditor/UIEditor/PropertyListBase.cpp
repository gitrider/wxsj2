// PropertyListBase.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "PropertyListBase.h"
#include "FontManger.h"


// CPropertyListBase

IMPLEMENT_DYNAMIC(CPropertyListBase, CWnd)

CPropertyListBase::CPropertyListBase()
{

}

CPropertyListBase::~CPropertyListBase()
{
}

BOOL CPropertyListBase::Create(DWORD dwStyle, const RECT& rcClient, CWnd* pParentWnd, UINT nID)
{
	//先创建一个窗口
	BOOL bRet = CWnd::Create(NULL, NULL,dwStyle,rcClient,pParentWnd,nID);
	if (!bRet)
	{
		TRACE0("CPropertyListBase Failed to create window.\r\n");
		return bRet;
	}
	SetFont(&CFontManger::m_font12);
	// Setup the window style
	dwStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	// Setup the tree options 
	// OT_OPTIONS_SHOWINFOWINDOW
	DWORD dwOptions = OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS | OT_OPTIONS_SHOWINFOWINDOW;

	// Create tree options
	if (m_otTree.Create(dwStyle, rcClient, this, dwOptions, nID) == FALSE)
	{
		TRACE0("Failed to create options control.\r\n");
		return FALSE;
	}
	// Want to be notified
	m_otTree.SetNotify(TRUE, this);
	m_otTree.SetFont(&CFontManger::m_font12);
	return TRUE;
}

COptionTreeItem*	CPropertyListBase::GetItemTreeByName(const CString& itemName)
{
	if ( !itemName.IsEmpty() )
	{
		OptionTreeItemMap::iterator it = d_OptionTreeMap.find(itemName);
		if ( it != d_OptionTreeMap.end() )
		{
			return it->second;
		}
		it = d_OptionTreeMapBase.find(itemName);
		if ( it != d_OptionTreeMapBase.end() )
		{
			return it->second;
		}
	}
	return 0;
}

COptionTreeItem*	CPropertyListBase::RemoveItemTreeByName(const CString& itemName)
{
	if ( !itemName.IsEmpty() )
	{
		OptionTreeItemMap::iterator it = d_OptionTreeMap.find(itemName);
		if ( it != d_OptionTreeMap.end() )
		{
			d_OptionTreeMap.erase(it);
		}
	}
	return 0;
}

void		CPropertyListBase::AddItemTree(const CString& itemName, COptionTreeItem* pItem)
{
	if ( pItem && itemName.IsEmpty() == FALSE )
	{
		d_OptionTreeMap.insert(std::make_pair(itemName, pItem));
	}
}

VOID CPropertyListBase::CopyToBaseAndClear(VOID)
{
	d_OptionTreeMapBase = d_OptionTreeMap;
	d_OptionTreeMap.clear();
}


COptionTreeItemStatic*		CPropertyListBase::InsertStaticTextItem(const CString& PropName, const CString& PropInfoName, const CString& PropValue, const CString& userStringOrName , const CString& parentName )
{
	COptionTreeItem *otiItem = GetItemTreeByName(userStringOrName);
	COptionTreeItem*otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL )
	{
		COptionTreeItemStatic *otiItem = 0;
		otiItem = (COptionTreeItemStatic *)m_otTree.InsertItem(new COptionTreeItemStatic(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		otiItem->SetStaticText(PropValue);
		ASSERT( otiItem->CreateStaticItem(0) == TRUE );
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

COptionTreeItemEdit* CPropertyListBase::InsertEditItem(const CString& PropName, const CString& PropInfoName, const CString& PropValue , OptionTreeItemEditStyle nStyle,const CString& userStringOrName , const CString& parentName )
{
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL )
	{
		COptionTreeItemEdit *otiItem = 0;
		otiItem = (COptionTreeItemEdit *)m_otTree.InsertItem(new COptionTreeItemEdit(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		DWORD eStyle = 0;
		DWORD eSubStyle = 0;
		if (nStyle & OTES_MUTILELINE)
		{
			eStyle = OT_EDIT_MULTILINE;
			eSubStyle = ES_WANTRETURN | ES_AUTOVSCROLL;
		}
		if (nStyle & OTES_PASSWORD)
		{
			eStyle = OT_EDIT_PASSWORD;
		}
		if (nStyle & OTES_NUMBERONLY)
		{
			eStyle = OT_EDIT_NUMERICAL;
		}
		if (otiItem->CreateEditItem(eStyle, eSubStyle) == TRUE)
		{
			otiItem->SetWindowText(PropValue);
			if (nStyle & OTES_READONLY)
			{
				otiItem->ReadOnly(TRUE);
			}
		}
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

COptionTreeItemComboBox*			CPropertyListBase::InsertComboItem(const CString& PropName, const CString& PropInfoName, const StringVector& PropValue, OptionTreeItemComboStyle nStyle , const CString& userStringOrName , const CString& parentName )
{
	printf("InsertComboItem Prop = %s\n", PropName.GetString());
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL )
	{
		COptionTreeItemComboBox *otiItem = 0;
		otiItem = (COptionTreeItemComboBox *)m_otTree.InsertItem(new COptionTreeItemComboBox(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		DWORD eStyle = 0;
		if (nStyle & OTCS_SORTED)
		{
			eStyle = CBS_SORT;
		}
		if (otiItem->CreateComboItem(eStyle) == TRUE)
		{
			size_t sz = PropValue.size() ;
			for (size_t i=0; i<sz; i++)
			{
				otiItem->InsertString(-1,PropValue[i]);
			}
			if (sz)
			{
				otiItem->SelectString(0, PropValue[0]);
			}
			if ( nStyle & OTCS_READONLY )
			{
				otiItem->ReadOnly(TRUE);
			}
		}
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	printf("InsertComboItem Prop = %s end\n", PropName.GetString());
	return 0;
}

COptionTreeItemCheckBox*	CPropertyListBase::InsertCheckBoxItem(const CString& PropName, const CString& PropInfoName, const StringVector& PropValue, OptionTreeItemCheckBoxStyle nStyle, const CString& userStringOrName, const CString& parentName, BOOL setChecks )
{
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL )
	{
		COptionTreeItemCheckBox *otiItem = 0;
		otiItem = (COptionTreeItemCheckBox *)m_otTree.InsertItem(new COptionTreeItemCheckBox(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		DWORD eStyle = 0;
		if ( !(nStyle & OTCBS_HIDECHECKBOX) )
		{
			eStyle |= OT_CHECKBOX_SHOWCHECK;
		}
		if ( !(nStyle & OTCBS_HIDETEXT) )
		{
			eStyle |= OT_CHECKBOX_SHOWTEXT;
		}
		if (otiItem->CreateCheckBoxItem(setChecks, eStyle) == TRUE)
		{
			if ( PropValue.size() >= 2 )
			{
				otiItem->SetCheckText(PropValue[0] , PropValue[1]);
			}
			else if ( PropValue.size() == 1 )
			{
				otiItem->SetCheckText(PropValue[0] , PropValue[0]);
			}
			if ( nStyle & OTCBS_READONLY )
			{
				otiItem->ReadOnly(TRUE);
			}
		}
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

COptionTreeItemRadio*	CPropertyListBase::InsertRadioItem(const CString& PropName, const CString& PropInfoName, const RadioPropertyValueVector& PropValue , OptionTreeItemRadioStyle nStyle, const CString& userStringOrName , const CString& parentName )
{
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL )
	{
		COptionTreeItemRadio *otiItem = 0;
		otiItem = (COptionTreeItemRadio *)m_otTree.InsertItem(new COptionTreeItemRadio(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		if (otiItem->CreateRadioItem() == TRUE)
		{
			size_t size = PropValue.size();
			for (size_t i=0; i<size; i++)
			{
				otiItem->InsertNewRadio(PropValue[i].RadioText, PropValue[i].bCheck);
			}
			if ( nStyle & OTRS_READONLY )
			{
				otiItem->ReadOnly(TRUE);
			}
		}
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

COptionTreeItemSpinner*	CPropertyListBase::InsertSpinerItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue, FLOAT* fRange, OptionTreeItemSpineStyle nStyle , const CString& userStringOrName , const CString& parentName )
{
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL && fRange )
	{
		COptionTreeItemSpinner *otiItem = 0;
		otiItem = (COptionTreeItemSpinner *)m_otTree.InsertItem(new COptionTreeItemSpinner(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		DWORD oStyle = 0;
		if ( !(nStyle & OTSS_FORBIDEN_EDIT) )
		{
			oStyle |= OT_EDIT_USEREDIT;
		}
		if ( nStyle & OTSS_SET_RANGE )
		{
			oStyle |= OT_EDIT_WRAPAROUND;
		}
		if (otiItem->CreateSpinnerItem(oStyle, 0, fRange[0], fRange[1]) == TRUE)
		{
			if (nStyle & OTSS_READONLY)
			{
				otiItem->ReadOnly(TRUE);
			}
		}
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

COptionTreeItemColor* CPropertyListBase::InsertColorItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue,  OptionTreeItemColorStyle nStyle, const CString& userStringOrName , const CString& parentName )
{
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL  )
	{
		COptionTreeItemColor *otiItem = 0;
		otiItem = (COptionTreeItemColor *)m_otTree.InsertItem(new COptionTreeItemColor(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		DWORD oStyle = OT_COLOR_SHOWHEX;
		if ( !(nStyle & OTCOLS_NOTUPDATE) )
		{
			oStyle |= OT_COLOR_LIVEUPDATE;
		}
		if (otiItem->CreateColorItem(oStyle, RGB(255, 100, 255), RGB(0, 0, 0)) == TRUE)
		{
			if (nStyle & OTCOLS_READONLY)
			{
				otiItem->ReadOnly(TRUE);
			}
		}
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

COptionTreeItemDate*			CPropertyListBase::InsertDataTimeItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue,  OptionTreeItemDataTimeStyle nStyle, const CString& userStringOrName , const CString& parentName )
{
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL  )
	{
		COptionTreeItemDate *otiItem = 0;
		otiItem = (COptionTreeItemDate *)m_otTree.InsertItem(new COptionTreeItemDate(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		if ( nStyle & OTDTS_DATE )
		{
			if (otiItem->CreateDateItem(_T("ddd, MMM d, yyyy"), DTS_LONGDATEFORMAT) == TRUE)
			{
				if (nStyle & OTCOLS_READONLY)
				{
					otiItem->ReadOnly(TRUE);
				}
			}
		}
		else if ( nStyle & OTDTS_TIME )
		{
			if (otiItem->CreateDateItem(_T("hh:mm tt"), DTS_TIMEFORMAT | DTS_UPDOWN) == TRUE)
			{
				if (nStyle & OTCOLS_READONLY)
				{
					otiItem->ReadOnly(TRUE);
				}
			}
		}
		
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

COptionTreeItemImage*			CPropertyListBase::InsertImageItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue,  INT imageSize, INT coloumNum, OptionTreeItemImageStyle nStyle , const CString& userStringOrName , const CString& parentName )
{
	COptionTreeItem *otiItem = RemoveItemTreeByName(userStringOrName);
	COptionTreeItem *otiItemParent = GetItemTreeByName(parentName);
	if ( otiItem == NULL  )
	{
		COptionTreeItemImage *otiItem = 0;
		otiItem = (COptionTreeItemImage *)m_otTree.InsertItem(new COptionTreeItemImage(), otiItemParent);
		otiItem->SetLabelText(PropName);
		otiItem->SetInfoText(PropInfoName);
		if (otiItem->CreateImageItem(OT_IMAGE_SHOWTEXT, CSize(imageSize, imageSize), coloumNum) == TRUE)
		{
			if (nStyle & OTIS_READONLY)
			{
				otiItem->ReadOnly(TRUE);
			}
		}
		AddItemTree( userStringOrName, otiItem );
		return otiItem;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

VOID CPropertyListBase::RemoveAllItem()
{
	m_otTree.DeleteAllItems();
	d_OptionTreeMap.clear();
}

VOID CPropertyListBase::RemoveItem(const CHAR* szName, BOOL clear)
{
	OptionTreeItemMap::iterator it = d_OptionTreeMap.find(szName);
	if ( it != d_OptionTreeMap.end() )
	{
		m_otTree.DeleteItem(it->second);
		if (clear)
		{
			d_OptionTreeMap.clear();
		}
		else
		{
			d_OptionTreeMap.erase(it);
		}		
	}
}
void CPropertyListBase::CreateRootItem(const CHAR* szRootItem, const CHAR* szLabelText, const CHAR* szInfoText)
{
	COptionTreeItem* pItem = m_otTree.InsertItem(new COptionTreeItem());
	pItem->SetLabelText(szLabelText);
	pItem->SetInfoText(szInfoText);
	AddItemTree(szRootItem, pItem);
}


BEGIN_MESSAGE_MAP(CPropertyListBase, CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPropertyListBase 消息处理程序


void CPropertyListBase::OnSize(UINT nType, int cx, int cy)
{
	m_otTree.MoveWindow(&CRect(0,0,cx,cy));
}

BOOL CPropertyListBase::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if( wParam == OPTION_TREE_NOTIFY_ID )
	{
		OnOptionTreeNotify((NMOPTIONTREE*)lParam);
	}
	return CWnd::OnNotify(wParam,lParam,pResult);
}
