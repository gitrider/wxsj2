#include "stdafx.h" 
#include "IpTable.h"
#include "FileDef.h"
#include "WXSJ_DBC.h"
#include "Socket.h"

IPRegionTable*					g_pIPRegionTable = NULL;

using namespace DBC;

tag_IPRegion::tag_IPRegion()
{
__ENTER_FUNCTION
	CleanUp();
__LEAVE_FUNCTION
}

VOID tag_IPRegion::CleanUp()
{
	m_CountyID = INVALID_ID;
	m_IPBegin = m_IPEnd = 0;
}

//////////////////////////////////////////////////////////////////////////
//IPRegionTable
IPRegionTable::IPRegionTable()
{
__ENTER_FUNCTION
	m_MaxRegionCount = 0;
	m_RegionData = NULL;
__LEAVE_FUNCTION
}

IPRegionTable::~IPRegionTable()
{
__ENTER_FUNCTION
	m_MaxRegionCount = 0;
	SAFE_DELETE_ARRAY(m_RegionData);
__LEAVE_FUNCTION
}

VOID IPRegionTable::Init()
{
//__ENTER_FUNCTION
//	enum
//	{
//		TBIndex		= 0,
//		TBCountyID	= 1,
//		TBIPBegin	= 2,
//		TBIPEnd		= 3,
//	};
//
//	DBCFile ThirdFile(0);
//	BOOL ret = ThirdFile.OpenFromTXT( FILE_IPREGION_TABLE );
//	Assert(ret);
//
//	INT iTableCount		=	ThirdFile.GetRecordsNum();
//	INT	iTableColumn	=	ThirdFile.GetFieldsNum();
//
//	m_MaxRegionCount = iTableCount;
//	m_RegionData = new IPRegion[iTableCount];
//	Assert(m_RegionData);
//	for(INT i = 0; i < m_MaxRegionCount; ++i)
//	{
//		m_RegionData[i].m_CountyID = ThirdFile.Search_Posistion(i,TBCountyID)->iValue;
//		m_RegionData[i].m_IPBegin = (UINT)(ntohl(inet_addr(ThirdFile.Search_Posistion(i,TBIPBegin)->pString)));
//		m_RegionData[i].m_IPEnd = (UINT)(ntohl(inet_addr(ThirdFile.Search_Posistion(i,TBIPEnd)->pString)));
//	}
//__LEAVE_FUNCTION
}

INT IPRegionTable::FindIPRegion(UINT uIP)
{
__ENTER_FUNCTION
	if(!m_RegionData)	return INVALID_ID;
	INT iCountyID = INVALID_ID;
	for(INT i = 0; i < m_MaxRegionCount; ++i)
	{
		if(m_RegionData[i].m_IPBegin <= uIP && m_RegionData[i].m_IPEnd >= uIP)
		{
			iCountyID = m_RegionData[i].m_CountyID;
			break;
		}
	}
	return iCountyID;
__LEAVE_FUNCTION
	return INVALID_ID;
}

INT IPRegionTable::FindIPRegion(CHAR* cIP)
{
__ENTER_FUNCTION
	if(!cIP)	return INVALID_ID;
	UINT uIP = (UINT)(ntohl(inet_addr(cIP)));

	return FindIPRegion(uIP);
__LEAVE_FUNCTION
	return INVALID_ID;
}