/********************************************************************************
 *	文件名：	AbilityManager.h
 *	全路径：	d:\Prj\Server\Server\Ability\AbilityManager.h
 *	创建人：	胡繁
 *	创建时间：	
 *
 *	功能说明：	用于管理所有生活技能实例、配方表以及经验表
 *	修改记录：	2005 年 11 月 6 日	16:52，调整了一下结构
*********************************************************************************/

#ifndef __ABILITYMANAGER_H__
#define __ABILITYMANAGER_H__

#include "Type.h"
#include "AbilityLevelUpConfig.h"
#include "GameDefine.h"

//修改后最大7个生活技能
#define MAX_ABILITY					MAX_CHAR_ABILITY_NUM	

class Ability;
class Player;
struct _AbilityExpTable;
struct _ABILITY_PRESCRIPTION_TBL;

class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();

	enum AbilityClass
	{
		ABILITY_CLASS_INVALID			= -1,	// 非法
		ABILITY_CLASS_NOUSE				= 0,	// 占位技能

		ABILITY_CLASS_GATHERMINE,				// 采矿
		ABILITY_CLASS_REAP,						// 收割
		ABILITY_CLASS_CATCH	,					// 捕捉虫子

		ABILITY_CLASS_FOUNDRY,					// 精炼 - 铸造
		ABILITY_CLASS_TAILOR,					// 缝纫 - 制衣
		ABILITY_CLASS_ARTWORK,					// 精工 - 工艺
		ABILITY_CLASS_COOKING,					// 烹饪	
		ABILITY_CLASS_SIZE,
	};

	VOID								CleanUp();

	BOOL								Init();

	// 获得
	Ability*							GetAbility( AbilityID_t aid )
	{
		INT idx;
		idx = (INT)aid;

		//if( idx<=ABILITY_CLASS_INVALID || idx>=ABILITY_CLASS_SIZE )
		if( idx<=ABILITY_CLASS_NOUSE || idx>=ABILITY_CLASS_SIZE )//0会在表中找不到造成指针等于空modi:lby20071108
		{
			Assert(idx>ABILITY_CLASS_INVALID && idx<ABILITY_CLASS_SIZE);
			return NULL;
		}

		return m_pAbility[idx];
	}

	BYTE								MaxAbilityNum() { return (BYTE)MAX_ABILITY; }

	BOOL								InitAbility( const CHAR* filename );
	BOOL								InitPrescriptionList( const CHAR* filename );
	BOOL								InitAbilityExpTable( const CHAR* filename );

	_AbilityExpTable*					GetAbilityExpTbl()
	{
		return m_pAbilityExpTbl;
	}

	INT									GetAbilityExpTblRowCount()
	{
		return m_AbilityExpTblCount;
	}

	const _ABILITY_PRESCRIPTION_TBL*	GetPrescriptionList( )
	{
		return m_pPrescriptionList;
	}

	const AbilityLevelUpConfigManager *GetLevelUpConfigManager(VOID)const
	{
		return &m_AbilityLevelUpConfigManager;
	}
private:
	Ability*							m_pAbility[MAX_ABILITY];
	_AbilityExpTable*					m_pAbilityExpTbl;
	INT									m_AbilityExpTblCount;
	_ABILITY_PRESCRIPTION_TBL*			m_pPrescriptionList;

	AbilityLevelUpConfigManager			m_AbilityLevelUpConfigManager;
};

extern AbilityManager*					g_pAbilityManager;

#endif // __ABILITYMANAGER_H__
