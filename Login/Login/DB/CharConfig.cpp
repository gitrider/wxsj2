#include "stdafx.h" 
#include "CharConfig.h"
#include "Ini.h"
#include "DB_Struct.h"
#include "TimeManager.h"
#include "LoginTable.h"

FULLUSERDATA* CharConfig::pUserData = NULL;


CharConfig g_CharConfig;

CharConfig::CharConfig()
{
	__ENTER_FUNCTION
	pUserData	= new FULLUSERDATA;
	if(NULL==pUserData)
	{
		AssertEx(FALSE, "[CharConfig::CharConfig]: alloc memory for FULLUSERDATA failed!!");
		return;
	}
	pUserData->CleanUp();

	pUserData->m_bIsPasswdUnlock = FALSE;
	strncpy(pUserData->m_PrivateInfo.m_LuckWord,"这家伙很懒，什么也没有留下!",MAX_LUCKWORD_LENGTH);
	pUserData->m_PrivateInfo.m_LuckWord[sizeof(pUserData->m_PrivateInfo.m_LuckWord)-1]='\0';
	__LEAVE_FUNCTION
}

CharConfig::~CharConfig()
{
	SAFE_DELETE(pUserData);
}


BOOL	CharConfig::InitCharAttr(FULLUSERDATA* pData, INT nCountry, INT nMenPai)	
{
	__ENTER_FUNCTION	
	
	Assert(pData);
	pData->m_Human.m_StartScene = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_ENTERSCENCE);
	pData->m_Human.m_Position.m_fX = (FLOAT)(g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_BORNX));
	pData->m_Human.m_Position.m_fZ = (FLOAT)(g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_BORNY));
	pData->m_Human.m_CampData.m_nCampID = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_CAMP);

	pData->m_Human.m_HP = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIHP);
	pData->m_Human.m_Rage = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIRAGE);
	pData->m_Human.m_nPneuma = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIPNEUMA);
	pData->m_Human.m_Level = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INILEVEL);

	pData->m_Human.m_MaxVigor = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_MAX_VIGOR);
	pData->m_Human.m_Vigor = pData->m_Human.m_MaxVigor;
	pData->m_Human.m_Exp = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIEXP);
	pData->m_Human.m_MenPai = nMenPai;
	pData->m_Human.m_Money = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIMONEY);

	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_STR, 0);
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_CON, 0);
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_INT, 0);
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_DEX, 0);

	INT nSkillID = -1;
	for(int i=CHAR_INISKILL1; i<=CHAR_INISKILL1+MAX_INITSKILL_COUNT; ++i)
	{

		nSkillID = g_DefaultCharDataTbl.Get(nCountry, nMenPai, i);

		if(nSkillID == -1)
			break;
		else
		{
			pData->m_Skill.m_aSkill[i-CHAR_INISKILL1].m_nSkillID = nSkillID;
			++pData->m_Skill.m_Count;
		}
	}
	//pData->m_Ability.m_aABility[iSkillAbility].m_Level=f.ReadInt("ability", szLevel);
	//pData->m_Ability.m_aABility[iSkillAbility].m_Exp = f.ReadInt("ability", szExp);

	pData->m_bIsPasswdUnlock = FALSE;
	pData->m_Human.m_PostCode.m_PostCode = 1000010;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}




BOOL	CharConfig::GetCharChooseScene(DB_CHOOSE_SCENE* pInfo,CHAR_TYPE ctype)
{
	__ENTER_FUNCTION

	if(pInfo)
	{
		CFG_CHOOSE_SCENE* pGetInfo = g_DBSceneTable.GetSceneInfo(ctype);
		if(pGetInfo)
		{
			pInfo->mSceneCount = pGetInfo->mSceneCount;
			memcpy(pInfo->mSceneID,pGetInfo->mSceneID,sizeof(SceneID_t)*MAX_CHOOSE_SCENE_NUMBER);
		}
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

DBChooseSceneTable	g_DBSceneTable;

DBChooseSceneTable::DBChooseSceneTable()
{

	Ini ini("./Config/DefaultChar.ini");
	INT nValue;
	
	//INT iSceneCount = ini.ReadIntIfExist("startscene","scenecount");
	if(TRUE==ini.ReadIntIfExist("startscene","scenecount", nValue))
	{
		INT iSceneCount = nValue;
		if(iSceneCount>MAX_CHOOSE_SCENE_NUMBER)
			iSceneCount = MAX_CHOOSE_SCENE_NUMBER;

		g_DBSceneTable.mSceneTable[CT_NORMAL].mSceneCount = iSceneCount;
		for(INT i =0;i<iSceneCount;i++)
		{
			char szTmp0[32] = {0},szTmp1[32] = {0},szTmp2[32]={0};
			char szTmp[32] = {0};

			sprintf(szTmp0,"scene%d",i);
			sprintf(szTmp1,"pos_x%d",i);
			sprintf(szTmp2,"pos_z%d",i);
			//g_DBSceneTable.mSceneTable[CT_NORMAL].mSceneID[i] =	ini.ReadIntIfExist("startscene",szTmp0);
			if(TRUE==ini.ReadIntIfExist("startscene",szTmp0, nValue))
			{
				g_DBSceneTable.mSceneTable[CT_NORMAL].mSceneID[i] = nValue;
			}
			else
			{
				char szErr[1024];
				tsnprintf(szErr, sizeof(szErr), "[CharConfig::CharConfig]: Read ini failed!! section:%s; key:.%s", "startscene",szTmp0);
				AssertEx(FALSE, szErr);
			}
			//ini.ReadTextIfExist("startscene",szTmp1,szTmp,32);
			if(TRUE==ini.ReadTextIfExist("startscene",szTmp1, szTmp, sizeof(szTmp)))
			{
				g_DBSceneTable.mSceneTable[CT_NORMAL].mPos[i].m_fX= (FLOAT)(atof(szTmp));
			}
			else
			{
				char szErr[1024];
				tsnprintf(szErr, sizeof(szErr), "[CharConfig::CharConfig]: Read ini failed!! section:%s; key:.%s", "startscene",szTmp1);
				AssertEx(FALSE, szErr);
			}

			//ini.ReadTextIfExist("startscene",szTmp2,szTmp,32);
			if(TRUE==ini.ReadTextIfExist("startscene",szTmp2, szTmp, sizeof(szTmp)))
			{
				g_DBSceneTable.mSceneTable[CT_NORMAL].mPos[i].m_fZ = (FLOAT)(atof(szTmp));
			}
			else
			{
				char szErr[1024];
				tsnprintf(szErr, sizeof(szErr), "[CharConfig::CharConfig]: Read ini failed!! section:%s; key:.%s", "startscene",szTmp2);
				AssertEx(FALSE, szErr);
			}
		}
	}
	else
	{
		char szErr[1024];
		tsnprintf(szErr, sizeof(szErr), "[CharConfig::CharConfig]: Read ini failed!! section:%s; key:.%s", "startscene","scenecount");
		AssertEx(FALSE, szErr);
	}

}

CFG_CHOOSE_SCENE*	DBChooseSceneTable::GetSceneInfo(CHAR_TYPE ctype)
{
	__ENTER_FUNCTION
		
		Assert(ctype<CT_NUMBER);

		return &mSceneTable[ctype];	

	__LEAVE_FUNCTION

	return NULL;
}


BOOL	DBChooseSceneTable::GetPosBySceneID(SceneID_t sid,CHAR_TYPE ctype,WORLD_POS& pos)
{
	__ENTER_FUNCTION

	Assert(ctype<CT_NUMBER);

	CFG_CHOOSE_SCENE& Choose =	mSceneTable[ctype];	
	WORLD_POS	pos;

	for(INT i = 0;i<Choose.mSceneCount;i++)
	{
		if(Choose.mSceneID[i]==sid)
		{
			pos =  Choose.mPos[i];
			return TRUE;
		}
	}

	return FALSE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	DBChooseSceneTable::isValidSceneID(SceneID_t sid,CHAR_TYPE ctype)
{
	__ENTER_FUNCTION

	Assert(ctype<CT_NUMBER);

	CFG_CHOOSE_SCENE& Choose =	mSceneTable[ctype];	

	for(INT i = 0;i<Choose.mSceneCount;i++)
	{
		if(Choose.mSceneID[i]==sid)
		{
			return TRUE;
		}
	}

	return FALSE;

	__LEAVE_FUNCTION

	return FALSE;
}
