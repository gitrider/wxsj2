#include "StdAfx.h"
#include "GMTripperObj.h"
#include "GIException.h"

//TD_IMPLEMENT_DYNAMIC_VIRTUAL(CTripperObject, NULL);
KL_IMPLEMENT_DYNAMIC_VIRTUAL(CTripperObject, GETCLASS(CObject_Static));

// ���ָ��ķ���
BOOL CTripperObject::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	//if(Tripper_CanOperation())
	{
		pOutCmd->m_typeMouse = SCommand_Mouse::MCT_HIT_TRIPPEROBJ;
		pOutCmd->m_adwParam[0] = GetServerID();
	}
	/*else
	{
		pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	}*/
	return TRUE;
}

// �Ҽ�ָ��ķ���
BOOL CTripperObject::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	if(Tripper_CanOperation())
	{
		pOutCmd->m_typeMouse = SCommand_Mouse::MCT_CONTEXMENU;
		pOutCmd->m_adwParam[0] = GetServerID();
	}
	else
	{
		pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	}
	return TRUE;
}
