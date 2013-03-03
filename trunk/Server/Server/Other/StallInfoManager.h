/*
̯λ��Ϣ��������ÿ��������һ����
����̯λ��Ϣ�ļ��Ķ�ȡ��
̯λ���õĹ���������һ�ΰ�̯��̯λ�ѣ�ÿ�ν��׵Ľ���˰���˵��Ƿ���԰�̯��
13:20 2006-2-7 sunyu
*/

#ifndef _STALLINFO_MANAGER_H_
#define _STALLINFO_MANAGER_H_
#include "Type.h"

/*
�������ο��̯λ��Ϣ
*/
struct StallInfoData_t
{
	StallInfoData_t()
	{
		memset(this, 0, sizeof(StallInfoData_t));
	}

	BYTE	m_bCanStall;	//�Ƿ���԰�̯
	BYTE	m_iTradeTax;	//����˰
	UINT	m_dwPosTax;		//̯λ��
	BYTE	m_iExtraInfo;	//������Ϣ

	StallInfoData_t& operator=(const StallInfoData_t& stallInfo)
	{
		m_bCanStall		= stallInfo.m_bCanStall;
		m_iTradeTax		= stallInfo.m_iTradeTax;
		m_dwPosTax		= stallInfo.m_dwPosTax;
		m_iExtraInfo	= stallInfo.m_iExtraInfo;
	}
};

/*
StallInfoManager,����̯λ���ݵĶ�ȡ�����ʡ�
*/
class StallInfoManager
{
public:
	StallInfoManager();
	~StallInfoManager();
public:
	//-------------------------------------------------------------------------------------------------
	//Init������ڴ棬��Ӧ��CleanUp���ͷ��ڴ�
	BOOL		Init(UINT uMapWidth = 0, UINT uMapHeight = 0);
	VOID		CleanUp();

	//-------------------------------------------------------------------------------------------------
public:
	BOOL				Load( CHAR* filename ) ;
	BOOL				CanStall(UINT pos_x, UINT pos_y);
	VOID				SetCanStall(UINT pos_x, UINT pos_y, BOOL bIsCanStall);
	BYTE				StallExchangeTax(UINT pos_x, UINT pos_y);
	UINT				StallPosPayment(UINT pos_x, UINT pos_y);
	BYTE				StallExtraInfo(UINT pos_x, UINT pos_y);

protected:
	StallInfoData_t**	m_StallMap;
	UINT				m_nMapHeight;
	UINT				m_nMapWidth;
};

#endif