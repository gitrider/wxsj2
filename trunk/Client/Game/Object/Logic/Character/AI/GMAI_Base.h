
/** AI_Base.h
 */

#pragma once


#include "GameCommand.h"



class CObject_Character;

class CAI_Base
{
public:

	/** �߼��� */
	virtual BOOL		Tick( VOID ) = 0;

	/** ѹ��һ��ָ�� */
	virtual BOOL		PushCommand( const SCommand_AI *pCmd ) = 0;
	virtual BOOL		PushCommand_MoveTo(FLOAT fDestX, FLOAT fDestZ) = 0;
	virtual BOOL		PushCommand_Jump(VOID)	= 0;
	virtual BOOL		PushCommand_UseSkill(INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir) = 0;

	//test5
	// ѹ�뼼�ܴ���
	virtual BOOL		PushCommand_OperateSkill( INT nLogicCount, INT nSkillId, INT nTargetId, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir ) = 0;


	/** �жϵ�ǰ AI */
	virtual VOID		OnAIStopped( VOID ) = 0;

	/** ���� */
	virtual VOID		Reset(VOID) = 0;

protected:

	/** AIָ�� */
	virtual RC_RESULT	OnCommand( const SCommand_AI *pCmd ) = 0;

public:

	CAI_Base(CObject_Character* pCharObj);
	virtual ~CAI_Base(VOID);

	CObject_Character*	GetCharacter(VOID)									{ return m_pCharacterObj; }

protected:

	CObject_Character*	m_pCharacterObj;

};
