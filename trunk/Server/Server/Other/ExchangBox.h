/*
�������ν��׺�
*/

#ifndef _EXCHANGE_BOX_H_
#define _EXCHANGE_BOX_H_

#include "Type.h"
#include "DB_Struct.h"
#include "HumanPetContainer.h"

class Obj_Human;

/*
��֤��ǰ����Ľ���״̬�Ƿ��Ѿ�����.
*/
#define	EXCHANGE_CERTIFY_ISLOCK(pHuman)\
		if(pHuman->m_ExchangBox.m_IsLocked == TRUE)\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_ALREADY_LOCKED);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			Log::SaveLog( SERVER_LOGFILE, "<����> [%s] ���������״��ڣ�������Ч ", pHuman->GetName() ) ;\
			return PACKET_EXE_CONTINUE;\
		}\

/*
��֤����˫���Ƿ���ߺϷ�
*/
#define EXCHANGE_CERTIFY_EACH_OTHER(pHuman)\
		ObjID_t	TargetID = pHuman->m_ExchangBox.m_ObjID;\
		Obj_Human* pTargetHuman = pScene->GetHumanManager()->GetHuman( TargetID );\
		if( pTargetHuman == NULL )\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_DROP);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			pHuman->m_ExchangBox.CleanUp();\
			Log::SaveLog( SERVER_LOGFILE, "<����> [%s] �ѵ��ߣ�������Ч ", pHuman->GetName() ) ;\
			return PACKET_EXE_CONTINUE;\
		}\
		ObjID_t	SourceID = pTargetHuman->m_ExchangBox.m_ObjID;\
		if( SourceID != pHuman->GetID())\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			pHuman->m_ExchangBox.CleanUp();\
			Log::SaveLog( SERVER_LOGFILE, "<����> [%s] ���������˽��ף�������Ч ", pTargetHuman->GetName() ) ;\
			return PACKET_EXE_CONTINUE;\
		}\

/*
��֤��ǰ����Ľ���״̬�Ƿ���ȷ.
*/
#define	EXCHANGE_CERTIFY_STATUS(pHuman, status)\
		if(pHuman->m_ExchangBox.m_Status != ServerExchangeBox::status)\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			ObjID_t	DestID = pHuman->m_ExchangBox.m_ObjID;\
			Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( DestID );\
			pHuman->m_ExchangBox.CleanUp();\
			pDestHuman->m_ExchangBox.CleanUp();\
			Log::SaveLog( SERVER_LOGFILE, "<����> [%s] ����״̬���� status = %d ", pHuman->GetName(), pHuman->m_ExchangBox.m_Status );\
			return PACKET_EXE_CONTINUE;\
		}\

/*
�������˽��׺нṹ.
*/
class ServerExchangeBox
{
public:
	enum ExchangeStatus
	{
		EXCHANGE_NONE = 0,										//û�н��׷���
		EXCHANGE_SYNCH_DATA,									//�Լ�����ͬ�����ݽ׶Ρ�
		EXCHANGE_WAIT_FOR_CONFIRM,								//�Լ����ڵȴ��Լ����ȷ�Ͻ׶Ρ�
		EXCHANGE_CONFIRM_READY,									//�Լ����ڵȴ��Է�ȷ�Ͻ׶�
	};	
public:
	ServerExchangeBox()
	{
		m_pMySelf = NULL;
	}
	VOID Init(Obj_Human* pHuman);
	VOID CleanUp();
	VOID UnLockAllItem();
public:
	ExchangeStatus		m_Status;								//����״̬
	ObjID_t				m_ObjID;								//��ǰ���׶���
	Obj_Human* 			m_pMySelf;								//�Լ�
	BOOL				m_IsLocked;								//��ʾ������lockѡ���Ƿ���
	BOOL				m_CanConform;							//��ʾ�Ƿ���ʾȷ����ť
	UINT				m_Money;								//��ʾ���׵Ľ�Ǯ
	ItemContainer		m_Container;							//���׺��е���Ʒ
	_ITEM				m_ItemList[EXCHANGE_BOX_SIZE];			//��Ʒ����ʵ����
	ItemContainer		m_PetContainer;							//���׺��еĳ�������
	_PET_DB_LOAD		m_PetItemList[EXCHANGE_PET_BOX_SIZE];	//�������ʵ����

};
#endif