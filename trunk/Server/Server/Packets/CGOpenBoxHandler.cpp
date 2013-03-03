//#include "stdafx.h"
#include "stdafx.h"
#include "CGOpenBox.h"
#include "GamePlayer.h"
#include "GCOpenBox.h"
#include "OpenBox.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "HumanItemLogic.h"


UINT CGOpenBoxHandler::Execute( CGOpenBox* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	//UINT uBoxId = pPacket->GetBoxID();

	// //这里还需要通过boxid得到对应的脚本id
	//UINT uScriptId = 999666;
	//
	//

	//pScene->GetLuaInterface()->ExeScript_DD( 
	//	 uScriptId, 
	//	"OnDefaultFun",
	//	pScene->SceneID(),
	//	pHuman->GetID()
	//	);


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
