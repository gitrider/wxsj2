#include "StdAfx.h"
#include "GCPlayerShopRecordList.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Action\GMActionSystem.h"
#include "GIException.h"

UINT GCPlayerShopRecordListHandler::Execute( GCPlayerShopRecordList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID							nShopID			= pPacket->GetShopID();		//�̵�ID
			BYTE										bEntryNum		= pPacket->GetEntryNum();
			GCPlayerShopRecordList::MessageEntry_t*		pEntryList		= pPacket->GetEntryList();
			INT											nStallPage		= pPacket->GetPage();

			RecordList_t* pRecordList = CDataPool::GetMe()->GetRecordList();
			pRecordList->CleanUp();
			RecordEntry_t	TempRecordEntry;

			for(INT i = 0; i<bEntryNum; i++)
			{
				TempRecordEntry.szContent = pEntryList[i].szContent;
				pRecordList->m_RecordEntryList[i] = TempRecordEntry;
			}
			pRecordList->m_nRecordNum = bEntryNum;

			// ���û�м�¼���͸�����Ļ��ʾ
			if( 0 == bEntryNum )
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCPlayerShopRecordListHandler_No_Message");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��ĵ���Ŀǰû�л�������¼��");
				return PACKET_EXE_CONTINUE;
			}
			//...��(����)��¼
			CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_MESSAGE, nStallPage);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
