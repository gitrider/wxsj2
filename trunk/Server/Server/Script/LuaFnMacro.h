//-----------------------------------------------------------------------------
// �ļ��� : LuaFnMacro.h
// ģ��	:	Script
// ����	 :  ����ű�ϵͳ��Ҫ�ĺ�
// �޸���ʷ:
//	yangfei����
//-----------------------------------------------------------------------------
#ifndef __LUAFNMACRO_H__
#define __LUAFNMACRO_H__


namespace LuaFnTbl
{
#ifdef  __WINDOWS__
#define BEGINSCENEDEFINE(FUNCNAME) 	 { \
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"����ID���� "##FUNCNAME);	if(pScene) {	\
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	}
#define ENDSCENEDEFINE } 

#define BEGINCHARCTERDEFINE(FUNCNAME) 	 { \
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"����ID���� "##FUNCNAME);	if(pScene) { \
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));	AssertEx(pObj,"selfId���� "##FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
	Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj��Obj_Characterָ��ת������ "##FUNCNAME);			if(pCharacter) {\
	}
#define ENDCHARCTERDEFINE 		}	} }

#define  BEGINHUMANDEFINE(FUNCNAME) 	 { \
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));		AssertEx(pScene,"����ID���� "##FUNCNAME);   if(pScene) { \
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));		AssertEx(pObj,"selfId���� "##FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
	Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj��Obj_Characterָ��ת������ "##FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN) { \
	Obj_Human* pHuman = (Obj_Human*)pCharacter; AssertEx(pHuman,"Obj_Character��Obj_Humanָ��ת������ "##FUNCNAME);if(pHuman) {\
	}
#define  ENDHUMANDEFINE }	}	}	} 

#define  BEGINMONSTERDEFINE(FUNCNAME) 	 {\
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"����ID���� "##FUNCNAME);	if(pScene) {\
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));    	AssertEx(pObj,"selfId���� "##FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) {\
	Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj��Obj_Characterָ��ת������ "##FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_MONSTER) { \
	Obj_Monster* pMonster = (Obj_Monster*)pCharacter;	AssertEx(pMonster,"Obj_Character��Obj_Monsterָ��ת������ "##FUNCNAME);	if(pMonster) {\
	}
#define  ENDMONSTERDEFINE }	}	}	} 
#elif __LINUX__
#define BEGINSCENEDEFINE(FUNCNAME) 	 { \
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"����ID���� "FUNCNAME);	if(pScene) {	\
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	}
#define ENDSCENEDEFINE } 

#define BEGINCHARCTERDEFINE(FUNCNAME) 	 { \
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"����ID���� "FUNCNAME);	if(pScene) { \
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));	AssertEx(pObj,"selfId���� "FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
	Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj��Obj_Characterָ��ת������ "FUNCNAME);			if(pCharacter) {\
	}
#define ENDCHARCTERDEFINE 		}	} }

#define  BEGINHUMANDEFINE(FUNCNAME) 	 { \
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));		AssertEx(pScene,"����ID���� "FUNCNAME);   if(pScene) { \
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));		AssertEx(pObj,"selfId���� "FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
	Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj��Obj_Characterָ��ת������ "FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN) { \
	Obj_Human* pHuman = (Obj_Human*)pCharacter; AssertEx(pHuman,"Obj_Character��Obj_Humanָ��ת������ "FUNCNAME);if(pHuman) {\
	}
#define  ENDHUMANDEFINE }	}	}	} 

#define  BEGINMONSTERDEFINE(FUNCNAME) 	 {\
	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"����ID���� "FUNCNAME);	if(pScene) {\
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
	Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));    	AssertEx(pObj,"selfId���� "FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) {\
	Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj��Obj_Characterָ��ת������ "FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_MONSTER) { \
	Obj_Monster* pMonster = (Obj_Monster*)pCharacter;	AssertEx(pMonster,"Obj_Character��Obj_Monsterָ��ת������ "FUNCNAME);	if(pMonster) {\
	}
#define  ENDMONSTERDEFINE }	}	}	} 

#endif
	

	#define RETURNFALSE {	Lua_PushNumber(L,-1);	return 1;}
}

#pragma  warning(disable : 4244)



//����Quake3��strncpy����
VOID Q_strncpyz( CHAR *dest, const CHAR *src, INT destsize ) 
{
	__ENTER_FUNCTION

	if ( !src ) {
		AssertEx(src,"null pointer in Q_strncpyz");
		return;
	}
	if ( destsize < 1 ) {
		AssertEx(destsize>0,"dest buffer is null in Q_strncpyz");
		return;
	}

	strncpy( dest, src, destsize-1 );
	dest[destsize-1] = 0;

	__LEAVE_FUNCTION
}

#endif
