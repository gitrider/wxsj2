/********************************************************************
	created:	2007-12-18   10:33
	filename: 	Login\Login\Main\IpTable.h
	author:		YangJun
	
	purpose:	IP地址对应同乡频道的区间表
*********************************************************************/

#ifndef _IPTABLE_H_
#define _IPTABLE_H_

#include "Type.h"

//每个同乡编号的存储单元
typedef struct tag_IPRegion
{
	INT					m_CountyID;			//同乡编号
	UINT				m_IPBegin;			//起始IP地址
	UINT				m_IPEnd;			//截至IP地址

	tag_IPRegion();
	VOID				CleanUp();
}IPRegion;

//IP区间表
class IPRegionTable
{
public:
	IPRegionTable();
	virtual ~IPRegionTable();

	VOID				Init();						//初始化区间表
	INT					FindIPRegion(CHAR* cIP);	//查找对应的区间
	INT					FindIPRegion(UINT uIP);		//查找对应的区间
private:
	INT					m_MaxRegionCount;			//区间的总数量
	IPRegion*			m_RegionData;				//区间数据池
};

extern	IPRegionTable*	g_pIPRegionTable;

#endif