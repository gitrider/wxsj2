/****************************************\
*										*
* 			   HelperItem				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

class CHelperItem;
class CHelperItemSet
{
public:
	//	Get the root item
	//tongxi modify 排行榜
	CHelperItem*	GetRootItem(void) 
	{
		if(!IsPaihangTree)
			return m_rootItem; 
		else
			return m_rootItemPaihang;
	}

	// Load all items from itemset file.
	void			LoadItemSet(const String& strFileName);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	CHelperItemSet();
	virtual ~CHelperItemSet();

	// add a anchor item( userd by CHelpItem::AddChildItem);
	void	InsertAnchorItem(const String& strName, CHelperItem* pItem);
	
	// search a item by anchor name
	CHelperItem* FindItem_Anchor(const String& strAnchor);

	//tongxi 排行榜
	void InitPaihangTree(const char* worldid);
	String  CheckUrl(String url);
	void ShowPaihang(bool show) {IsPaihangTree=show;};

	bool IsShowPaihang() {return IsPaihangTree;};

protected:

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	CHelperItem*	m_rootItem;		// Root item that hold all items.

	// Anchor map
	typedef std::map< String, CHelperItem* > AnchorMap;
	AnchorMap		m_anchorMap;
	//tongxi 是否显示排行榜
	bool			IsPaihangTree;
	CHelperItem*	m_rootItemPaihang;		// 排行榜的root
	typedef std::vector<String>  WorldIdList;
	WorldIdList		m_worldIdList;
	
private:
    // unimplemented constructors / assignment
    CHelperItemSet(const CHelperItemSet& obj);
    CHelperItemSet& operator=(const CHelperItemSet& obj);
};