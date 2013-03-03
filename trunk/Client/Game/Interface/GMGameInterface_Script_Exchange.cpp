
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Global.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Event/GMEventSystem.h"
#include "Network/NetManager.h"
#include "DataPool/GMDataPool.h"
#include "Object/Logic/character/obj_playermyself.h"
#include "DataPool/GMDP_CharacterData.h"
#include "Procedure\GameProcedure.h"
#include "GIUISystem.h"

#include "CGBankMoney.h"
#include "CGBankRemoveItem.h"

#include "ExchangeMsgDefine.h"
#include "CGExchangeApplyI.h"
#include "CGExchangeReplyI.h"
#include "CGExchangeCancel.h"
#include "CGExchangeSynchLock.h"
#include "CGExchangeOkIII.h"
#include "CGExchangeSynchMoneyII.h"
#include "CGExchangeSynchItemII.h"
#include "CGItemSynch.h"
#include "GIUtil.h"

#include "Object\Logic\Character\AI\GMAI_MySelf.h"
#include "procedure/GameProcedure.h"
#include "procedure/GamePro_Main.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Exchange Exchange::s_Exchange;
	LuaPlus::LuaObject* Exchange::s_pMetaTable = NULL;

	//�ĵ�����Ƿ����
	INT Exchange::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_Exchange)
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	//���뽻��
	INT Exchange::SendExchangeApply(LuaPlus::LuaState* state)
	{
		LuaStack args(state);		

		/// ---------------------------------------------------------------------
		std::string strChatObjName = "";

		// ���������������������Ҽ��˵��Ľ�������¼������Ľ���������ƣ��Ҳ�Ϊ��	20100524LL
		if ( args[2].IsString() )
		{
			strChatObjName = args[2].GetString();
			CObject_Character* pCharObj = CObjectManager::GetMe()->FindCharacterByName( strChatObjName );

			if ( pCharObj != NULL )
			{
				CObjectManager::GetMe()->SetMainTarget( pCharObj->GetServerID() );
			}
			else // �Ҳ�������ֱ�ӷ��أ���ֹ��ǰ����ѡ��Ŀ�꣬ͨ�������ѡ��Ľ�����Ҵ�ʱ�Ѳ��ڳ���������ʱ�������ǰѡ��Ŀ�귢�ͽ�������
				return 0;
		}

		/// ---------------------------------------------------------------------
		
		// �ж��Լ�״̬�ɷ���		20100709 BLL

		// ����Լ����ڰ�̯�����ܽ���( "�����ڰ�̯��" )
		if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMAI_Base_Info_You_Stalling" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// ��ͨ����״̬���ܽ���( "�����ڸ�����ˣ����ܽ���" )
		if( ( (CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI() )->IsFollow() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// ��Ӹ���״̬���ܽ���( "�����ڸ�����ˣ����ܽ���" )
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if( bTeamFollowMode == TRUE )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		// ��ͨ�ܲ�״̬���ܽ���( "�������н������ܽ���" )
		if( CObjectManager::GetMe()->GetMySelf()->IsMoving() )   
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange");
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// �Զ��ܲ�״̬���ܽ���( "�������н������ܽ���" )
		if(	CGameProcedure::s_pProcMain->IsAutoRun() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		/// --------------------------------------------------------------------------------------------

		// �ж϶Է�״̬�ɷ���
		if( this == &s_Exchange )
		{
			CObject* pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();

			if( pChar && g_theKernel.IsKindOf( pChar->GetClass(), GETCLASS(CObject_Character) ) )
			{
				// �������Ѿ���������ҷ��ͽ�������( "�������Ѿ�������������뽻��" )
				if ( ((CObject_Character*)pChar)->CharacterLogic_Get( CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )	
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Can_Exchange_Width_dead_Man" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					return 0;
				}

				// ���ж�˫�������ǲ��ǺϷ�
				if(pChar)
				{
					const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
					const fVector3 pOtherPos = ((CObject*)pChar)->GetPosition();

					FLOAT fDistance = KLU_GetDist( fVector2( pMePos.x, pMePos.z ), fVector2( pOtherPos.x, pOtherPos.z ) );

					// "��Է�����̫Զ������������"
					if( EXCHANGE_MAX_DISTANCE < fDistance )
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Too_Far_To_Exchange" );
						CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
						return 0;
					}
				}

				CGExchangeApplyI msg;
				msg.SetObjID( pChar->GetServerID() );
				CNetManager::GetMe()->SendPacket(&msg);

				// "���������Ѿ�����"
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Send_Exchange_Msg" );
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				return 0;
			}
		}

		return 0;
	}

	// �򿪽��׶Ի���
	INT Exchange::OpenExchangeFrame(LuaPlus::LuaState* state)
	{
		// �ж��Լ�״̬�Ƿ�Ϸ� 20100709 BLL

		// "�Լ��������������뽻��"
		if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Dead_Can_Not_Exchange");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			return 0;
		}

		// ����Լ����ڰ�̯�����ܽ���
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
			return 0;
		}

		/// --------------------------------------------------------------------------------------------

		// ��ͨ����״̬���ܽ���( "�����ڸ�����ˣ����ܽ���" )
		if( ( (CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI() )->IsFollow() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// ��Ӹ���״̬���ܽ���( "�����ڸ�����ˣ����ܽ���" )
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if( bTeamFollowMode == TRUE )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		// ��ͨ�ܲ�״̬���ܽ���( "�������н��У����ܽ���" )
		if( CObjectManager::GetMe()->GetMySelf()->IsMoving() )   
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange");
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// �Զ��ܲ�״̬���ܽ���( "�������н��У����ܽ���" )
		if(	CGameProcedure::s_pProcMain->IsAutoRun() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		/// --------------------------------------------------------------------------------------------

		// �ж϶Է�״̬�ɷ���

		// �����ݳ��б���ȡ��һ���������Լ����׵��˵�ID
		if( CDataPool::GetMe()->IsStillAnyAppInList() )
		{
			INT nUserID = CDataPool::GetMe()->GetFromAppList();

			tObject *pObj = CObjectManager::GetMe()->FindServerObject(nUserID);

			//��һ�������ж������������ڽ�������������룬��ʾʧ��
			if( pObj != NULL )
			{
				const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
				const fVector3 pOtherPos = ((CObject*)pObj)->GetPosition();

				FLOAT fDistance = KLU_GetDist( fVector2( pMePos.x, pMePos.z ), fVector2( pOtherPos.x, pOtherPos.z ) );

				if( EXCHANGE_MAX_DISTANCE < fDistance )
				{
					// "��Է�����̫Զ�����ܿ�������"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_TooFar_Accept_Exchange" );
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return 0;
				}

				if( ((CObject_Character*)(CObjectManager::GetMe()->FindServerObject(nUserID) ))->CharacterLogic_Get(CObject_Character::LOGIC_BASE) != CObject_Character::CHARACTER_LOGIC_IDLE )
				{
					// "�Ǹ��������������״̬�����ܽ���"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Can_Exchange_With_Player_in_Special_Status");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return 0;
				}

				CGExchangeReplyI msg;
				msg.SetObjID(nUserID);
				CNetManager::GetMe()->SendPacket(&msg);
			}
			else
			{					
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Out_Exchange_Range");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ܹ����׵ķ�Χ");

			}//if( pObj != NULL )

		}// if( CDataPool::GetMe()->IsStillAnyAppInList() )

		return 0;
	}

	// �����Լ����뽻�׵���Ʒ����
	// �Լ��ģ�"self"�����Է��ģ�"other"��
	INT Exchange::GetItemNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetItemNum param parameter error");
		}

		std::string str	= args[2].GetString();
		if( str == "self" )		//�Լ�
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetItemNum());
			return 1;
		}
		if( str == "other" )	//�Է�
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetItemNum());
			return 1;
		}
		return 0;
	}

	// ����Action
	INT Exchange::GetItemAction(LuaPlus::LuaState* state)
	{
		return 1;
	}

	//�������׵Ľ�Ǯ��
	INT Exchange::GetMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetMoney param parameter error");
		}
		INT nMoney;
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		std::string str	= args[2].GetString();
		if( str == "self" )		//�Լ�
			nMoney = CDataPool::GetMe()->MyExBox_GetMoney();
		else if( str == "other" )	//�Է�
			nMoney = CDataPool::GetMe()->OtExBox_GetMoney();
			

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ( (nMoney-nCopperCoin)/100 ) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}

		if ( nMoney >= 10000 )
		{
			nGoldCoin = (  ( (nMoney-nCopperCoin)/100 )-nSilverCoin  )/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger( nMoney );
		state->PushInteger( nGoldCoin );
		state->PushInteger( nSilverCoin );
		state->PushInteger( nCopperCoin );

		return 4;
	}

	//��������
	INT Exchange::LockExchange(LuaPlus::LuaState* state)
	{
		//����������ȡ��������Ϣ
		CGExchangeSynchLock msg;
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
			//�Լ���������״̬��ִ��ȡ�������Ĳ���
			msg.SetLockMyself(FALSE);
			//�����Լ�������
			CDataPool::GetMe()->MyExBox_SetLock(FALSE);
		}
		else
		{	
			//�Լ�������δ����״̬��ִ�������Ĳ���
			msg.SetLockMyself(TRUE);

			//�����Լ�������
			CDataPool::GetMe()->MyExBox_SetLock(TRUE);

		}

		CNetManager::GetMe()->SendPacket(&msg);

		//�����Լ��Ľ���
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}
	
	//��������״̬
	INT Exchange::IsLocked(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetMoney param parameter error");
		}

		BOOL bLocked;
		std::string str	= args[2].GetString();
		if( str == "self" )		//�Լ�
			bLocked = CDataPool::GetMe()->MyExBox_GetLock();
		else if( str == "other" )	//�Է�
			bLocked = CDataPool::GetMe()->OtExBox_GetLock();

		state->PushBoolean(bLocked==TRUE);
		return 1;
	}


	//ȷ������
	INT Exchange::AcceptExchange(LuaPlus::LuaState* state)
	{
		//���ͽ�����Ϣ
		CGExchangeOkIII msg;
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}

	// ����ȡ��
	INT Exchange::ExchangeCancel(LuaPlus::LuaState* state)
	{
		// ����ȡ��������Ϣ
		CGExchangeCancel msg;
		CNetManager::GetMe()->SendPacket(&msg);

		// ֪ͨ�����Ǯ�Ի���ر�
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);

		return 0;
	}

	// �򿪳�����棨��ʱ�����򿪽�Ǯ�����
	INT Exchange::OpenPetFrame(LuaPlus::LuaState* state)
	{
		// �򿪽�Ǯ�����
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"exch");
		return 0;
	}

	// ��������ý�Ǯ
	INT Exchange::GetMoneyFromInput(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:Exchange::GetMoney param parameter error");
		}

		UINT nMoney = static_cast<UINT>(args[2].GetInteger());

		//�����ʱ�Լ��Ѿ���������״̬�������������Ч
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
				
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_You_have_Locked_Exchange");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���Ѿ������˽���");
			return 0;
		}

		//��Ҫ�ȼ����ǲ�������ô���Ǯ�����û�У��������ֵ
		//ͬʱ��Ҫ���鲻��Ϊ����
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if (nMoney > (UINT)pCharData->Get_Money()) 
		{
				
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_You_Money_Is_Not_Enough");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Я���Ľ�Ǯ����");
			return 0;
		}
		CDataPool::GetMe()->MyExBox_SetMoney(nMoney);
		//���ͽ�Ǯ��Ϣ
		CGExchangeSynchMoneyII msg;
		msg.SetMoney(nMoney);
		CNetManager::GetMe()->SendPacket(&msg);


		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}

	//�������������б��Ƿ�Ϊ��
	INT Exchange::IsStillAnyAppInList(LuaPlus::LuaState* state)
	{
		state->PushBoolean(CDataPool::GetMe()->IsStillAnyAppInList() == TRUE);
		
		return 1;
	}

	//�������Լ����׵��˵�����
	INT Exchange::GetOthersName(LuaPlus::LuaState* state)
	{
		
		INT nObjId = CDataPool::GetMe()->GetExchangObjID();
		CObject_Character* pObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( nObjId ));
		if(pObj)
		{
			state->PushString(pObj->GetCharacterData()->Get_Name());
			return 1;
		}

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Can_Not_Find_Name");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		state->PushString("�Ҳ�������");
		return 1;
	}

	//ѡ��һ�������б��еĳ�����ȷ����Ĳ���
	INT Exchange::AddPet(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);
		//if (!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA:Exchange:AddPet[2] param parameter error");
		//}
		//INT nPetIndex = args[2].GetInteger();		
		//
		//SDATA_PET* SelectPet = CDataPool::GetMe()->Pet_GetPet(nPetIndex);
		//if( (!SelectPet) || (INVALID_ID == SelectPet->m_nIsPresent) )
		//{
		//	return 0;
		//}

		//// ������׽��濪��
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
		//{
		//	CGExchangeSynchItemII msg;
		//	msg.SetOpt(EXCHANGE_MSG::OPT_ADDPET);
		//	msg.SetPetGuid(SelectPet->m_GUID);

		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		////�����̯���濪��
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
		//{
		//	//��ѯ�����ǲ����Ѿ��ϼ�
		//	if(CDataPool::GetMe()->MyStallBox_IsPetOnStall(SelectPet->m_GUID))
		//	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��������Ѿ��ϼ���");
		//		return 0;
		//	}

		//	//���ϼܽ���
		//	CDataPool::GetMe()->MyStallBox_SetSelectpetGuid(SelectPet->m_GUID);
		//	CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"st_pet");
		//}
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
		//{
		//	//������ŵ��̵���ͷȥ
		//	CGItemSynch msg;
		//	msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
		//	msg.SetFromType(CGItemSynch::POS_PET);
		//	msg.SetToType(CGItemSynch::POS_PLAYERSHOPPET);
		//	msg.SetPetGUID(SelectPet->m_GUID);

		//	CGAutoMovePetFromBagToPlayerShop_t ExtraDataInfo;
		//	ExtraDataInfo.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
		//	ExtraDataInfo.m_nStallIndex		= (BYTE)CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		//	msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
		//	msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);

		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
		//{
		//	CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		//}
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PetSkillStudy"))
		//{
		//	CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		//}

		return 0;
	}
	// ��ó���ĸ���
	INT Exchange::GetPetNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetPetNum[2] param parameter error");
		}
		std::string str = args[2].GetString();
		if(str == "self")
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetCount());
		}
		else
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetCount());
		}

		return 1;
	}
	//
	INT Exchange::EnumPet(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}

		INT nIndex = args[3].GetInteger();
		std::string str = args[2].GetString();
		if(str == "self")
		{
			SDATA_PET* pPet = CDataPool::GetMe()->MyExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}
		else
		{
			SDATA_PET* pPet = CDataPool::GetMe()->OtExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}*/
		return 1;
	}
	//�ӽ�����������һ������
	INT Exchange::DelSelectPet(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);
		//if (!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA:Exchange::EnumPet[2] param parameter error");
		//}

		//INT nIndex = args[2].GetInteger();

		////���ӽ��׿������³������Ϣ
		//CGExchangeSynchItemII msg;
		//msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEPET);
		//msg.SetPetGuid(CDataPool::GetMe()->MyExBox_GetPet(nIndex)->m_GUID);

		//CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//��ʾ������ϸ��Ϣ
	INT Exchange::ViewPetDesc(LuaPlus::LuaState* state)//("self",g_nSelectPet);
	{
		/*LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::ViewPetDesc[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:Exchange::ViewPetDesc[3] param parameter error");
		}
		
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		SDATA_PET* pPet;
		if(str == "self")
		{
			pPet = CDataPool::GetMe()->MyExBox_GetPet( nIndex );
		}
		else
		{
			pPet = CDataPool::GetMe()->OtExBox_GetPet( nIndex );
		}
		if( (!pPet) || (INVALID_ID == pPet->m_nIsPresent) )
		{
			KLThrow("LUA:Exchange ViewPetDesc parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(pPet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);*/
		return 0;
	}
	
};


