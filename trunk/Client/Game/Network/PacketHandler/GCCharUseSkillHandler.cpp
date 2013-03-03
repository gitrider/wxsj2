#include "stdafx.h"
#include "GCCharUseSkill.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Object\Logic\Character\Obj_Character.h"
#include "..\..\Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\Game\DataPool\GMDP_Struct_Skill.h"
#include "..\Game\DataPool\GMDP_CharacterData.h"
#include "..\..\GameInterface\Include\GIDBC_Struct.h"

#include "GIException.h"

uint GCCharUseSkillHandler::Execute( GCCharUseSkill* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	INT  nSkillID	= pPacket->GetSkillID();
	INT  nSenderID  = pPacket->GetSenderID();

	CObject_Character* pCharacter = (CObject_Character*)CObjectManager::GetMe()->FindServerObject( nSenderID );

	if ( pCharacter == NULL )
	{
		return PACKET_EXE_CONTINUE;
	}

	// 释放技能时，角色头顶冒技能名称
	pCharacter->DisplaySkillInfo( nSkillID );		

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
