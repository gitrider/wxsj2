#pragma once
#include "..\Obj_Static.h"

/**
	Tripper Object
*/

class CTripperObject :	public CObject_Static
{
public:
	// ���ָ��ķ���
	virtual BOOL FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// �Ҽ�ָ��ķ���
	virtual BOOL FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
protected:
	KL_DECLARE_DYNAMIC(CTripperObject);
};