/********************************************************************
	created:	2007-12-18   10:33
	filename: 	Login\Login\Main\IpTable.h
	author:		YangJun
	
	purpose:	IP��ַ��Ӧͬ��Ƶ���������
*********************************************************************/

#ifndef _IPTABLE_H_
#define _IPTABLE_H_

#include "Type.h"

//ÿ��ͬ���ŵĴ洢��Ԫ
typedef struct tag_IPRegion
{
	INT					m_CountyID;			//ͬ����
	UINT				m_IPBegin;			//��ʼIP��ַ
	UINT				m_IPEnd;			//����IP��ַ

	tag_IPRegion();
	VOID				CleanUp();
}IPRegion;

//IP�����
class IPRegionTable
{
public:
	IPRegionTable();
	virtual ~IPRegionTable();

	VOID				Init();						//��ʼ�������
	INT					FindIPRegion(CHAR* cIP);	//���Ҷ�Ӧ������
	INT					FindIPRegion(UINT uIP);		//���Ҷ�Ӧ������
private:
	INT					m_MaxRegionCount;			//�����������
	IPRegion*			m_RegionData;				//�������ݳ�
};

extern	IPRegionTable*	g_pIPRegionTable;

#endif