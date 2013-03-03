/*****************************************************************************
文件:	RightPropList
作者:	方远
邮箱:	fangyuanning@126.com
说明:	右边的属性窗口
******************************************************************************/
#pragma once

#include "PropertyListBase.h"

#define MAX_MAP_SIZE 50
// CRightPropList
struct MAP_STRUCT
{
	short nImageID;
	short nItemID;
	MAP_STRUCT()
	{
		nItemID = -1;
		nItemID = -1;
	}
	bool DataValid()
	{
		return nItemID > -1 && nImageID > -1;
	}
	void Clear()
	{
		nImageID = -1;
		nItemID = -1;
	}
	void Set(short img, short item)
	{
		nItemID = item;
		nImageID = img;
	}
	bool Find(short item)
	{
		return nItemID == item;
	}
};

class CRightPropList : public CPropertyListBase
{
	DECLARE_DYNAMIC(CRightPropList)

public:
	CRightPropList();
	virtual ~CRightPropList();
	void InsertItemS(const CString& szName, const CString &szType);
private:

	MAP_STRUCT m_mapImage[MAX_MAP_SIZE];
	INT m_currMapImage;
	BYTE m_imageBuf[16*16*16+50];
	CString m_szWindowName;

public:
	void OnTabLoseFocus();
	void UpdateBaseProperty(CEGUI::Window* pWin);
	void UpdateSpecialProperty(CEGUI::Window* pWin);
	void UpdateBasePosAndSizeProperty(CEGUI::Window* pWin);
	void ResetBaseProperty();

protected:
	void AddMapImage(int nImage , int nItem);
	int    FindMapImage(int nItem);
	void ClearMapImage();
	BYTE* CreateImageDataFromColor(COLORREF color);
	virtual		void OnOptionTreeNotify(NMOPTIONTREE* pTreeItem);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


