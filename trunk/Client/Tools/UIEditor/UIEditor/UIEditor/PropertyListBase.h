/*****************************************************************************
文件:	PropertyListBase
作者:	方远
邮箱:	fangyuanning@126.com
说明:	属性页的基类
******************************************************************************/
#pragma once

#include "OptionTree.h"
#include <vector>
#include <map>
// CPropertyListBase

class COptionTreeItemComboBox;

enum OptionTreeItemEditStyle
{
	OTES_MUTILELINE = 1,
	OTES_READONLY = 2,
	OTES_PASSWORD =4,
	OTES_NUMBERONLY =8,
	OTES_NORMAL = 16,
};

enum OptionTreeItemComboStyle
{
	OTCS_READONLY = 1,
	OTCS_SORTED = 2,
	OTCS_NORMAL = 4,
};

enum OptionTreeItemCheckBoxStyle
{
	OTCBS_READONLY = 1,
	OTCBS_HIDECHECKBOX = 2,
	OTCBS_HIDETEXT = 4,
	OTCBS_NORMAL = 8,
};

enum OptionTreeItemRadioStyle
{
	OTRS_READONLY = 1,
	OTRS_NORMAL = 2,
};

enum OptionTreeItemSpineStyle
{
	OTSS_READONLY = 1,
	OTSS_NORMAL = 2,
	OTSS_FORBIDEN_EDIT = 4,
	OTSS_SET_RANGE = 8,
};

enum OptionTreeItemColorStyle
{
	OTCOLS_READONLY = 1,
	OTCOLS_NORMAL = 2,
	OTCOLS_NOTUPDATE = 4,	//不随着用户选择更新
};

enum OptionTreeItemDataTimeStyle
{
	OTDTS_READONLY = 1,
	OTDTS_DATE = 2,
	OTDTS_TIME = 4,
	OTDTS_NORMAL = 8,
};

enum OptionTreeItemImageStyle
{
	OTIS_READONLY = 1,
	OTIS_NORMAL = 8,
};


typedef std::vector<CString>	StringVector;

struct RadioPropertyValue 
{
	CString RadioText;
	BOOL	bCheck;
};

typedef std::vector<RadioPropertyValue> RadioPropertyValueVector;

class CPropertyListBase : public CWnd
{
	DECLARE_DYNAMIC(CPropertyListBase)

public:
	CPropertyListBase();
	virtual ~CPropertyListBase();
	VOID							SetDelayUpdate(BOOL delay) { m_otTree.SetDelayUpdate(delay);}
	VOID							ForceUpdate(VOID) { m_otTree.ForceUpdate(); }
	VOID							RemoveAllItem();
	VOID							RemoveItem(const CHAR* szName, BOOL clear = FALSE);
	void							CreateRootItem(const CHAR* szRootItem, const CHAR* szLabelText, const CHAR* szInfoText);
	VOID							CopyToBaseAndClear(VOID);
	VOID							ExpandAllItems() { m_otTree.ExpandAllItems(); }
	BOOL							Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	COptionTreeItem*				GetItemTreeByName(const CString& itemName);
	COptionTreeItem*				RemoveItemTreeByName(const CString& itemName);
	COptionTreeItemStatic*			InsertStaticTextItem(const CString& PropName, const CString& PropInfoName, const CString& PropValue, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));
	COptionTreeItemEdit*			InsertEditItem(const CString& PropName, const CString& PropInfoName, const CString& PropValue, OptionTreeItemEditStyle nStyle =OTES_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));
	COptionTreeItemComboBox*		InsertComboItem(const CString& PropName, const CString& PropInfoName, const StringVector& PropValue, OptionTreeItemComboStyle nStyle =OTCS_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));
	COptionTreeItemCheckBox*		InsertCheckBoxItem(const CString& PropName, const CString& PropInfoName, const StringVector& PropValue, OptionTreeItemCheckBoxStyle nStyle =OTCBS_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""), BOOL setChecks = FALSE);
	COptionTreeItemRadio*			InsertRadioItem(const CString& PropName, const CString& PropInfoName, const RadioPropertyValueVector& PropValue, OptionTreeItemRadioStyle nStyle =OTRS_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));
	COptionTreeItemSpinner*			InsertSpinerItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue, FLOAT* fRange, OptionTreeItemSpineStyle nStyle =OTSS_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));
	COptionTreeItemColor*			InsertColorItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue, OptionTreeItemColorStyle nStyle =OTCOLS_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));
	COptionTreeItemDate*			InsertDataTimeItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue,  OptionTreeItemDataTimeStyle nStyle =OTDTS_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));
	COptionTreeItemImage*			InsertImageItem(const CString& PropName, const CString& PropInfoName, const const CString& PropValue,  INT imageSize, INT coloumNum, OptionTreeItemImageStyle nStyle =OTIS_NORMAL, const CString& userStringOrName = CString(""), const CString& parentName =CString(""));

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void		AddItemTree(const CString& itemName, COptionTreeItem* pItem);
	virtual		void OnOptionTreeNotify(NMOPTIONTREE* pTreeItem) { }

protected:
	COptionTree m_otTree;
	typedef std::map<CString, COptionTreeItem*>	OptionTreeItemMap;
	OptionTreeItemMap d_OptionTreeMap;
	OptionTreeItemMap d_OptionTreeMapBase;
};


