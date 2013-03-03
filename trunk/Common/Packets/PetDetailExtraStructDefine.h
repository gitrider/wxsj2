/************************************************************************/
/*
��������:	2006��3��2��
����ʱ��:	11:54
�ļ�����:	PetDetailExtraStructDefine.h
������:		sunyu

�ļ����ܣ�	������Ϣ�ĸ�����Ϣ������������
*/
/************************************************************************/

#ifndef __PETDETAILEXTRASTRUCTDEFINE_H__
#define __PETDETAILEXTRASTRUCTDEFINE_H__

#include "Type.h"
namespace PET_EXTRA_STRUCT
{
	struct GCExchangePetView_t
	{
		BYTE				m_bFlag;							//��־����Ϣ����;

		GCExchangePetView_t()
		{
			m_bFlag			=	0;							//��־����Ϣ����;
		}
		static UINT GetSize()
		{
			return	sizeof(GCExchangePetView_t);
		}
	};


	struct GCPlayerShopPetView_t
	{
		BYTE				m_bFlag;							//��־����Ϣ����;
		INT					m_nPrice;							//����۸�
		INT					m_nSerial;							//�������к�
		BYTE				m_bIsMine;							//�Ƿ����Լ��쿴
		BYTE				m_nStallIndex;						//���ڹ�̨��
		BYTE				m_bIsOnSale;						//�Ƿ��ڼ���

		GCPlayerShopPetView_t()
		{
			m_bFlag			=	0;							//��־����Ϣ����;
			m_nPrice		=	0;							//����۸�
			m_nSerial		=	0;							//�������к�
			m_bIsMine		=	0;							//�Ƿ����Լ��쿴
			m_nStallIndex	=	0;							//���ڹ�̨��
			m_bIsOnSale		=	0;							//�Ƿ��ڼ���

		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopPetView_t);
		}
	};

	struct GCStallPetView_t
	{
		BYTE				m_bFlag;							//��־����Ϣ����;
		INT					m_nPrice;							//����۸�
		INT					m_nSerial;							//�������к�

		GCStallPetView_t()
		{
			m_bFlag			=	0;							//��־����Ϣ����;
			m_nPrice		=	0;							//����۸�
			m_nSerial		=	0;							//�������к�
		}
		static UINT GetSize()
		{
			return	sizeof(GCStallPetView_t);
		}
	};
}

#endif
