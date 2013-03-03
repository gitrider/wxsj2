/********************************************************************************
 *	�ļ�����	LuaFnTbl_Relation.h
 *	ȫ·����	d:\Prj\Server\Server\Script\LuaFnTbl_Relation.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 2 �� 28 ��	14:40
 *
 *	����˵����	��ϵϵͳ��صĵ�������
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __LUAFNTBL_RELATION_H__
#define __LUAFNTBL_RELATION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SceneManager.h"
#include "GWRelation.h"

namespace LuaFnTbl
{
	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	16:13
	 *	�������ƣ�	LuaFnIsFriend
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� ID
	 *
	 *	����˵����	�ж� selfId �� friendID �Ƿ�Ϊ����
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsFriend( Lua_State* L )
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t friendID;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		friendID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsFriend")
			GUID_t friendGUID;
			Obj_Human* pFriend;

			pFriend = pScene->GetHumanManager()->GetHuman( friendID );
			if( pFriend == NULL )
			{
				Assert( pFriend );
				return 0;
			}

			friendGUID = pFriend->GetGUID();
			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsFriend(friendGUID));
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}



	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	13:48
	 *	�������ƣ�	LuaFnGetFriendName
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendGUID
	 *					���ѵ� GUID
	 *
	 *	����˵����	�õ� GUID Ϊ friendGUID �ĺ��ѵ�����
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetFriendName( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  GUID_t friendGUID;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		friendGUID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetFriendName")

			const _OWN_RELATION* pRelation;

			pRelation = pHuman->GetHumanRelation()->GetRelationInfo(friendGUID);
			if( pRelation == NULL )
			{
				Assert(pRelation);
				return 0;
			}

			Lua_PushString(L, pRelation->m_Member.m_szMemberName);
			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	14:42
	 *	�������ƣ�	LuaFnGetFriendPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� ID
	 *
	 *	����˵����	�õ��� ID Ϊ friendID �ĺ��ѵ��Ѻö�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetFriendPoint( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t friendID;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		friendID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetFriendPoint")
			GUID_t friendGUID;
			Obj_Human* pFriend;

			pFriend = pScene->GetHumanManager()->GetHuman( friendID );
			if( pFriend == NULL )
			{
				Assert( pFriend );
				return 0;
			}

			friendGUID = pFriend->GetGUID();
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetFriendPoint(friendGUID));
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	20:07
	 *	�������ƣ�	LuaFnSetFriendPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� ID
	 *				friendPoint
	 *					�Ѻö�ֵ
	 *
	 *	����˵����	���� selfId �� friendID ���Ѻö�Ϊ friendPoint
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetFriendPoint( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t friendID;
	  INT nFriendPoint;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		friendID = Lua_ValueToNumber(L, 3);
		nFriendPoint = Lua_ValueToNumber(L, 4);

		BEGINHUMANDEFINE("LuaFnSetFriendPoint")

			INT nOldFriendPoint;
			GUID_t friendGUID;
			Obj_Human* pFriend;

			pFriend = pScene->GetHumanManager()->GetHuman( friendID );
			if( pFriend == NULL )
			{
				Assert( pFriend );
				return 0;
			}

			friendGUID = pFriend->GetGUID();
			nOldFriendPoint = pHuman->GetHumanRelation()->GetFriendPoint(friendGUID);

			if ( nFriendPoint < nOldFriendPoint )
			{ // ��
				pHuman->GetHumanRelation()->DecFriendPoint( friendGUID, nOldFriendPoint - nFriendPoint );
			}
			else if ( nFriendPoint > nOldFriendPoint )
			{ // ��
				pHuman->GetHumanRelation()->IncFriendPoint( friendGUID, nFriendPoint - nOldFriendPoint );
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	�����
	 *	��дʱ�䣺	2008-3-28	20:07
	 *	�������ƣ�	LuaFnSetDeadLinkFriendPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� GUID
	 *				friendPoint
	 *					�Ѻö�ֵ
	 *
	 *	����˵����	���� selfId �� friendID ���Ѻö�Ϊ friendPoint,
	 *				��LuaFnSetFriendPoint���������ں��ѿ��Դ�������״̬
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetDeadLinkFriendPoint( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  GUID_t friendGUID;
	  INT nFriendPoint;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		friendGUID = Lua_ValueToNumber(L, 3);
		nFriendPoint = Lua_ValueToNumber(L, 4);

		BEGINHUMANDEFINE("LuaFnSetDeadLinkFriendPoint")

			INT nOldFriendPoint = pHuman->GetHumanRelation()->GetFriendPoint(friendGUID);

			if ( nFriendPoint < nOldFriendPoint )
			{ // ��
				pHuman->GetHumanRelation()->DecFriendPoint( friendGUID, nOldFriendPoint - nFriendPoint );
			}
			else if ( nFriendPoint > nOldFriendPoint )
			{ // ��
				pHuman->GetHumanRelation()->IncFriendPoint( friendGUID, nFriendPoint - nOldFriendPoint );
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	12:14
	 *	�������ƣ�	LuaFnSetFriendPointByGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendGUID
	 *					���ѵ� GUID
	 *				friendPoint
	 *					�Ѻö�ֵ
	 *
	 *	����˵����	���� selfId �� friendGUID ���Ѻö�Ϊ friendPoint
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetFriendPointByGUID( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  GUID_t friendGUID;
	  INT nFriendPoint;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		friendGUID = Lua_ValueToNumber(L, 3);
		nFriendPoint = Lua_ValueToNumber(L, 4);

		BEGINHUMANDEFINE("LuaFnSetFriendPointByGUID")

			INT nOldFriendPoint;
			nOldFriendPoint = pHuman->GetHumanRelation()->GetFriendPoint(friendGUID);

			if ( nFriendPoint < nOldFriendPoint )
			{ // ��
				pHuman->GetHumanRelation()->DecFriendPoint( friendGUID, nOldFriendPoint - nFriendPoint );
			}
			else if ( nFriendPoint > nOldFriendPoint )
			{ // ��
				pHuman->GetHumanRelation()->IncFriendPoint( friendGUID, nFriendPoint - nOldFriendPoint );
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	15:04
	 *	�������ƣ�	LuaFnIsMarried
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�жϵ������Ƿ��ѻ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsMarried( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnIsMarried")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsMarried());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	12:17
	 *	�������ƣ�	LuaFnGetSpouseGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ���ż�� GUID
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetSpouseGUID( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetSpouseGUID")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetSpouseGUID());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	16:11
	 *	�������ƣ�	LuaFnMarry
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				spouseId
	 *					��ż ID
	 *
	 *	����˵����	sceneId �� spouseId ��Ϊ����
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnMarry( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t spouseId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		spouseId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnMarry")
			Obj_Human* pSpouse;
			pSpouse = pScene->GetHumanManager()->GetHuman( spouseId );
			if( pSpouse == NULL )
			{
				Assert( pSpouse );
				return 0;
			}

			// ���ݽṹ����
			pHuman->GetHumanRelation()->Marry( pSpouse->GetGUID() );
			pSpouse->GetHumanRelation()->Marry( pHuman->GetGUID() );

			// ���¿ͻ�����Ϣ
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pSpouseMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pSpouseRelation = pSpouseMsg->GetRelation();
			pSelfRelation->CleanUp();
			pSpouseRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pSpouseRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pSpouseRelation->SetGUID( pSpouse->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pSpouseRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pSpouse->GetGUID() );
			pSpouseRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pSpouseMsg, INVALID_ID );

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	19:24
	 *	�������ƣ�	LuaFnIsSpouses
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� selfId �� targetId �Ƿ��Ƿ���
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsSpouses( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsSpouses")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			// �ж�
			if( FALSE==pHuman->IsMySpouse(*pTarget)
			 || FALSE==pTarget->IsMySpouse(*pHuman)
			 )
			{
				Lua_PushNumber(L, FALSE);
			}
			else
			{
				Lua_PushNumber(L, TRUE);
			}

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	11:26
	 *	�������ƣ�	LuaFnUnMarry
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				spouseId
	 *					��ż ID
	 *
	 *	����˵����	sceneId �� spouseId ���
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnUnMarry( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t spouseId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		spouseId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnUnMarry")
			Obj_Human* pSpouse;
			pSpouse = pScene->GetHumanManager()->GetHuman( spouseId );
			if( pSpouse == NULL )
			{
				Assert( pSpouse );
				return 0;
			}

			// ���ݽṹ����
			pHuman->GetHumanRelation()->Divorce( );
			pSpouse->GetHumanRelation()->Divorce( );

			// ���¿ͻ�����Ϣ
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pSpouseMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pSpouseRelation = pSpouseMsg->GetRelation();
			pSelfRelation->CleanUp();
			pSpouseRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pSpouseRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pSpouseRelation->SetGUID( pSpouse->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pSpouseRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pSpouse->GetGUID() );
			pSpouseRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pSpouseMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	13:11
	 *	�������ƣ�	LuaFnDivorce
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	���������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnDivorce( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnDivorce")

			GUID_t SpouseGUID;
			HumanRelation* pHumanRelation;

			pHumanRelation = pHuman->GetHumanRelation();
			if( pHumanRelation == NULL )
			{
				Assert( pHumanRelation );
				return 0;
			}

			if( pHumanRelation->IsMarried() != TRUE )
			{
				Assert( pHumanRelation->IsMarried() );
				return 0;
			}

			SpouseGUID = pHumanRelation->GetSpouseGUID();
			pHumanRelation->Divorce( );

			// ���¿ͻ�����Ϣ
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID( SpouseGUID );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	17:07
	 *	�������ƣ�	LuaFnIsMaster
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� targetId �Ƿ��� selfId ��ʦ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsMaster( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsMaster")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsMaster( pTarget->GetGUID() ));

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:03
	 *	�������ƣ�	LuaFnIsPrentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� targetId �Ƿ��� selfId ��ͽ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsPrentice( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsPrentice")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsPrentice( pTarget->GetGUID() ));

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	20:17
	 *	�������ƣ�	LuaFnGetPrenticeGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				nIndex
	 *					�ڼ���ͽ��
	 *
	 *	����˵����	?
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetPrenticeGUID( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  INT nIndex;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		nIndex = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetPrenticeGUID")

			Assert( nIndex >= 0 && nIndex < 2 );

			GUID_t guid;
			guid = pHuman->GetHumanRelation()->GetNthPrentice(nIndex);

			if( guid == INVALID_ID )
			{
				Lua_PushNumber(L, -1);
			}
			else
			{
				Lua_PushNumber(L, guid);
			}

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:04
	 *	�������ƣ�	LuaFnHaveMaster
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�ж� selfId �Ƿ���ʦ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnHaveMaster( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnHaveMaster")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->HaveMaster());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:57
	 *	�������ƣ�	LuaFnGetMasterGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ�ʦ���� GUID
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetMasterGUID( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMasterGUID")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetMasterGUID());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	19:21
	 *	�������ƣ�	LuaFnGetMasterMoralPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	ȡ�� selfId ��ʦ�µ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetMasterMoralPoint( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMasterMoralPoint")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetMoralPoint());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	19:22
	 *	�������ƣ�	LuaFnSetMasterMoralPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uMoralPoint
	 *					ʦ�µ�
	 *
	 *	����˵����	����ʦ�µ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetMasterMoralPoint( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  UINT uMoralPoint;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		uMoralPoint = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetMasterMoralPoint")
			pHuman->GetHumanRelation()->SetMoralPoint(uMoralPoint);
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:07
	 *	�������ƣ�	LuaFnGetPrenticeCount
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	��Ҫͽ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetPrenticeCount( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetPrenticeCount")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetPrenticeCount());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:10
	 *	�������ƣ�	LuaFnGetPrenticeBetrayTime
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ����һ��ͽ����ʦ��ʱ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetPrenticeBetrayTime( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetPrenticeBetrayTime")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetPrenticeBetrayTime());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:11
	 *	�������ƣ�	LuaFnAprentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	selfId �� targetId Ϊʦ
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnAprentice( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnAprentice")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			pHuman->GetHumanRelation()->Aprentice( pTarget->GetGUID() );
			pTarget->GetHumanRelation()->Recruit( pHuman->GetGUID() );

			// ���¿ͻ�����Ϣ
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pMasterMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pMasterRelation = pMasterMsg->GetRelation();
			pSelfRelation->CleanUp();
			pMasterRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pMasterRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pMasterRelation->SetGUID( pTarget->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pMasterRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pTarget->GetGUID() );
			pMasterRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pMasterMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:18
	 *	�������ƣ�	LuaFnFinishAprentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	selfId �뿪 targetId ʦ�ǣ���ʽ��ʦ
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnFinishAprentice( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnFinishAprentice")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			pHuman->GetHumanRelation()->LeaveMaster();
			pTarget->GetHumanRelation()->ExpelPrentice( pHuman->GetGUID() );

			// ���¿ͻ�����Ϣ
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pMasterMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pMasterRelation = pMasterMsg->GetRelation();
			pSelfRelation->CleanUp();
			pMasterRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pMasterRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pMasterRelation->SetGUID( pTarget->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pMasterRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pTarget->GetGUID() );
			pMasterRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pMasterMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:47
	 *	�������ƣ�	LuaFnBetrayMaster
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	��ʦ
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnBetrayMaster( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnBetrayMaster")
			GUID_t guid = pHuman->GetHumanRelation()->GetMasterGUID();

			Assert( guid != INVALID_ID );

			pHuman->GetHumanRelation()->LeaveMaster();

			// ���¿ͻ�����Ϣ
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID( guid );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:47
	 *	�������ƣ�	LuaFnExpelPrentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				PrenticeGUID
	 *					ͽ�ܵ� GUID
	 *
	 *	����˵����	����ĳ��ͽ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnExpelPrentice( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  GUID_t PrenticeGUID;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		PrenticeGUID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnExpelPrentice")

			Assert( PrenticeGUID != INVALID_ID );

			pHuman->GetHumanRelation()->ExpelPrentice( PrenticeGUID ); 

			// ���¿ͻ�����Ϣ
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID( PrenticeGUID );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			return 0;
		ENDHUMANDEFINE


	LUA_LEAVE_FUNCTION

		return 0;
	}


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:40
	 *	�������ƣ�	LuaFnIsBrother
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� targetId �Ƿ��� selfId �Ľ���ֵ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsBrother( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsBrother")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsBrother( pTarget->GetGUID() ));

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}



	/****************************************************************************
	*	������Ա��	yuyazhou
	*	��дʱ�䣺	2010-5-21	
	*	�������ƣ�	LuaFnDelFriendByGUID
	*	����˵����
	*				sceneId
	*					�������
	*				selfId
	*					������ ID���޷�������
	*				myGUID
	*					���˵� GUID
	*				friendGUID
	*					���ѵ�GUID
	*
	*	����˵����	ͨ�����ѵ�GUID�������ߵĺ���ɾ��
	*	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnDelFriendByGUID(  Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT NowTime;
		GUID_t friendGUID;

		sceneId		= Lua_ValueToNumber(L, 1);
		selfId		= Lua_ValueToNumber(L, 2);
		NowTime		= Lua_ValueToNumber(L, 3);
		friendGUID	= Lua_ValueToNumber(L, 4);

		BEGINHUMANDEFINE("LuaFnDelFriendByGUID")

			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			
			// ���Ƿ�����ô������
			RELATION_TYPE RelationType = pMyRelation->GetRelationType( friendGUID );
			// ������Ǻ��ѣ������κβ���
			if ( RelationType != RELATION_TYPE_FRIEND )
				return 0;

			// ɾ�����ѹ���
			if( pMyRelation->DelRelation( RELATION_TYPE_FRIEND, friendGUID ) != TRUE )
			{
				return 0;
			}
			else
			{
				GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
				GW_RELATION* pRelation = pMsg->GetRelation();
				pRelation->CleanUp();
				pRelation->m_Type = REQ_DELFRIENDBYMAIL;
				pRelation->SetGUID( pHuman->GetGUID() );
				REQUEST_DEL_FRIEND_MAIL& Relation = pRelation->m_DelFriendByMail;
				Relation.CleanUp();

				Relation.SetGUID( friendGUID );
				Relation.SetTIME( NowTime );

				g_pServerManager->SendPacket( pMsg, INVALID_ID );

				g_pLog->FastSaveLog( LOG_FILE_1, "LuaFnTbl_Relation.h: GUID=%X del GUID=%X from friend list by mail.",
					pHuman->GetGUID(), friendGUID );
			}

		ENDHUMANDEFINE

			LUA_LEAVE_FUNCTION

			return 0;
	}


}


#endif // __LUAFNTBL_RELATION_H__

