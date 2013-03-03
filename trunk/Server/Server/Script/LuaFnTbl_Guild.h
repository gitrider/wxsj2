//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Guild.h
// ģ��	:	Script
// ����	 :  ����ű��ӿ�
// �޸���ʷ:
//				sunyu ����
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_GUILD_H__
#define __LUAFNTBL_GUILD_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCGuildApply.h"
#include "Obj_Human.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GWGuild.h"
#include "GameDefine_Guild.h"
namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-21
	*     �������� : LuaFnGuildList
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*/
	INT	LuaFnGuildList(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId ;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGuildList")
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);
			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_ASKLIST;

			GUILD_CGW_ASKLIST* pPacketAskList = (GUILD_CGW_ASKLIST*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ASKLIST));

			pPacketAskList->m_uStart = 0;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		ENDHUMANDEFINE
		return	0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-21
	*     �������� : LuaFnGuildCreate
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*/
	INT	LuaFnGuildCreate(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGuildCreate")
			GCGuildApply Msg;
			Msg.SetNpcId(targetId);
			pHuman->GetPlayer()->SendPacket( &Msg );
		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaAddGuildUserPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					���ӵİ�ṱ�׶�
    *				
    */
    INT	LuaAddGuildUserPoint(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nGP ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nGP     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaAddGuildUserPoint")

        INT nTotal  =   nGP + pHuman->GetGuildPoint();
        if (nTotal > MAX_GUILD_USER_POINT)
            nTotal  =   MAX_GUILD_USER_POINT;

        pHuman->SetGuildPoint(nTotal);
        pHuman->SendMsg_RefeshAttrib();
        pHuman->SendMsg_RefeshGP(nGP , GUILD_PACKET_GW_ADD_GP);

        ENDHUMANDEFINE
        return	0;

        LUA_LEAVE_FUNCTION
        return 0 ;
    }


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaSubGuildUserPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					��ṱ�׶�
    *				
    */
    INT	LuaSubGuildUserPoint(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nGP ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nGP     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaSubGuildUserPoint")

        INT nTotal  =   pHuman->GetGuildPoint()-nGP;
        if (nTotal < 0)
            nTotal  =   0;

        pHuman->SetGuildPoint(nTotal);
        pHuman->SendMsg_RefeshAttrib();
        pHuman->SendMsg_RefeshGP(nGP , GUILD_PACKET_GW_SUB_GP);

        ENDHUMANDEFINE
        return	0;

        LUA_LEAVE_FUNCTION
        return 0 ;
    }

    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaAddTotalGP
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					���ӵİ�ṱ�׶�
    *				
    */
    INT	LuaAddTotalGP(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nGP ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nGP     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaAddTotalGP")

        pHuman->SendMsg_RefeshGP(nGP , GUILD_PACKET_GW_ADD_GP);

        ENDHUMANDEFINE
        return	0;

        LUA_LEAVE_FUNCTION
        return 0 ;
    }

    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaSubTotalGP
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					���ӵİ�ṱ�׶�
    *				
    */
    INT	LuaSubTotalGP(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nGP ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nGP     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaAddTotalGP")

        pHuman->SendMsg_RefeshGP(nGP , GUILD_PACKET_GW_SUB_GP);

        ENDHUMANDEFINE
            return	0;

        LUA_LEAVE_FUNCTION
            return 0 ;
    }

}
#endif