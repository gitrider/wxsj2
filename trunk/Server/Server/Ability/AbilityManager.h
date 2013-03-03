/********************************************************************************
 *	�ļ�����	AbilityManager.h
 *	ȫ·����	d:\Prj\Server\Server\Ability\AbilityManager.h
 *	�����ˣ�	����
 *	����ʱ�䣺	
 *
 *	����˵����	���ڹ������������ʵ�����䷽���Լ������
 *	�޸ļ�¼��	2005 �� 11 �� 6 ��	16:52��������һ�½ṹ
*********************************************************************************/

#ifndef __ABILITYMANAGER_H__
#define __ABILITYMANAGER_H__

#include "Type.h"
#include "AbilityLevelUpConfig.h"
#include "GameDefine.h"

//�޸ĺ����7�������
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
		ABILITY_CLASS_INVALID			= -1,	// �Ƿ�
		ABILITY_CLASS_NOUSE				= 0,	// ռλ����

		ABILITY_CLASS_GATHERMINE,				// �ɿ�
		ABILITY_CLASS_REAP,						// �ո�
		ABILITY_CLASS_CATCH	,					// ��׽����

		ABILITY_CLASS_FOUNDRY,					// ���� - ����
		ABILITY_CLASS_TAILOR,					// ���� - ����
		ABILITY_CLASS_ARTWORK,					// ���� - ����
		ABILITY_CLASS_COOKING,					// ���	
		ABILITY_CLASS_SIZE,
	};

	VOID								CleanUp();

	BOOL								Init();

	// ���
	Ability*							GetAbility( AbilityID_t aid )
	{
		INT idx;
		idx = (INT)aid;

		//if( idx<=ABILITY_CLASS_INVALID || idx>=ABILITY_CLASS_SIZE )
		if( idx<=ABILITY_CLASS_NOUSE || idx>=ABILITY_CLASS_SIZE )//0���ڱ����Ҳ������ָ����ڿ�modi:lby20071108
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
