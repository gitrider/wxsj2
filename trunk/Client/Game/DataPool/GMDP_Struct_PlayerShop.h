
#pragma once

#include "Type.h"

struct PLAYERSHOP_APPLY_INFO
{
	FLOAT							m_ComFactor;					//��ǰ��ҵָ��
	UINT							m_Cost;							//������
	BYTE							m_Type;							//�ɿ�������
	PLAYERSHOP_APPLY_INFO()
	{
		memset(this, 0, sizeof(PLAYERSHOP_APPLY_INFO));
	}
};

struct PLAYERSHOP_INFO
{
	STRING							m_szShopName;					//�̵���
	STRING							m_szShopDesc;					//�̵�����
	STRING							m_szOwnerName;					//̯����
	BYTE							m_StallNumOpened;				//���ŵĹ�̨��
	BYTE							m_StallNumOnSale;				//���ŵĹ�̨��
	BYTE							m_Type;							//���̵�����
	_PLAYERSHOP_GUID				m_ID;							//���̵�ΨһID
	UINT							m_OwnerGuid;					//̯��GUID
	STRING							m_szFoundedTime;				//����ʱ��
	BYTE							m_Serial;						//�̵����к�
	BYTE							m_bIsFavor;						//�Ƿ����ղؼ���

	PLAYERSHOP_INFO()
	{
		m_szShopName	=	"";				//�̵���
		m_szShopDesc	=	"";				//�̵�����
		m_szOwnerName	=	"";				//̯����
		m_StallNumOpened=	0;				//���ŵĹ�̨��
		m_StallNumOnSale=	0;				//���ŵĹ�̨��
		m_Type			=	0;				//���̵�����
		m_ID.Reset();						//���̵�ΨһID
		m_OwnerGuid		=	0;				//̯��GUID
		m_szFoundedTime	=	"";				//����ʱ��
		m_Serial		=	0;
		m_bIsFavor		=	0;				//�Ƿ����ղؼ���
	}
};
//------------
// ����̵�ϻ���
//------------

struct PlayerShopPartners_t
{
	GUID_t		m_Guid;
	STRING		m_szName;
	VOID		CleanUp()
	{
		m_Guid = INVALID_GUID;
		m_szName = "";
	}
};