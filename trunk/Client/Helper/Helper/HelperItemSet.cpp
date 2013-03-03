#include "StdAfx.h"
#include "HelperItemSet.h"
#include "HelperItem.h"
#include "HelperItemSet_XmlHandler.h"
#include "HelperSystem.h"
#include "HelperXMLParser.h"

//tongxi 排行榜
String CHelperItemSet::CheckUrl(String itemUrl)
{
	if(!itemUrl.empty())
	{
		DWORD dwServiceType;
		CString strServer, strObject;
		INTERNET_PORT nPort;

		String strFullURL;

		wchar_t wItemUrl[MAX_PATH]={0};
		::MultiByteToWideChar(CP_ACP, 0, itemUrl.c_str(), itemUrl.size(), wItemUrl, MAX_PATH);
		AfxParseURL(wItemUrl, dwServiceType, strServer, strObject, nPort);
		if(AFX_INET_SERVICE_UNK == dwServiceType)
		{
			//local file
			itemUrl = CHelperSystem::GetMe()->GetWorkDictory() + itemUrl;
		}
		else
		{
			itemUrl = itemUrl;
		}
		return itemUrl;
	}
	return "";
}
	
void CHelperItemSet::InitPaihangTree(const char* worldid)
{

	if(m_worldIdList.size()>0 )
	{
		//for(int i=0;i<(int)m_worldIdList.size();i++)
		//{
		//	if(strcmp(((String)m_worldIdList[i]).c_str(),worldid)==0)
				return;
		//}
	}

	m_worldIdList.push_back(String(worldid));
	int ArryLenth=3;
	char *Anchors[]={"top50level.html","top50money.html","top50xinfa.html"};
	wchar_t *Names[]={L"高手排行(TOP50)", L"富豪排行(TOP50)", L"心法排行(TOP50)"};
	if(!m_rootItemPaihang) return;
	char *PathStart="http://tl.sohu.com/tlbbrank";
	char szPath[MAX_PATH];
	char szAnchor[MAX_PATH];
	for(int i=0;i<ArryLenth;i++)
	{
		memset(szAnchor, 0, MAX_PATH);
		memset(szPath, 0, MAX_PATH);
		strcpy(szPath, PathStart);
		strcat(szPath, "/");
		strcat(szPath,worldid);
		strcat(szPath,"/");
		strcat(szPath,Anchors[i]);
		WString itemName(Names[i]);
		strcpy(szAnchor,Anchors[i]);
		strcat(szAnchor,worldid);
		String itemAnchor(szAnchor);
		String itemUrl(szPath);
		itemUrl=CheckUrl(itemUrl);
		CHelperItem item(itemName, itemAnchor,itemUrl );
		m_rootItemPaihang->AddChildItem(item);
	}
}

CHelperItemSet::CHelperItemSet() :
	m_rootItem(0)
{
	// create root item.
	m_rootItem = new CHelperItem(L"root");

	//tongxi 排行榜
	m_rootItemPaihang = new CHelperItem(L"rootpaihang");

	IsPaihangTree=false;
}

CHelperItemSet::~CHelperItemSet()
{
	delete m_rootItem; m_rootItem = 0;
}

/*************************************************************************
	Load all items from itemset file.
*************************************************************************/
void CHelperItemSet::LoadItemSet(const String& strFileName)
{
	if (strFileName.empty() || (strFileName == ""))
	{
		throw std::exception("CHelperItemSet::Load - Filename supplied for ItemSet loading must be valid");
	}

    // create handler object
    CHelperItemSet_xmlHandler handler(this);
	
    // do parse (which uses handler to create actual data)
	CHelperSystem::GetMe()->GetXMLParser()->parseXMLFile(handler, strFileName, "");
}

/*************************************************************************
	Add a anchor item( userd by CHelpItem::AddChildItem);
*************************************************************************/
void CHelperItemSet::InsertAnchorItem(const String& strName, CHelperItem* pItem)
{
	if(strName.empty()) return;

	m_anchorMap.insert(std::make_pair(strName, pItem));
}

/*************************************************************************
	Search a item by anchor name
*************************************************************************/
CHelperItem* CHelperItemSet::FindItem_Anchor(const String& strAnchor)
{
	AnchorMap::iterator itFind = m_anchorMap.find(strAnchor);
	if(itFind == m_anchorMap.end()) return 0;

	return itFind->second;
}
