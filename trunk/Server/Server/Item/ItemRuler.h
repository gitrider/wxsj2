/********************************************************************
	��������:	2008��1��24��
	�ļ�����:	ItemRuler.h
	������:		��Ѯ
	
	�ļ����ܣ� ��Ʒϵͳ�Ĺ�����,���紴������\�������\ʹ�ù����\ʰȡ����
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _ITEM_RULER_H_
#define _ITEM_RULER_H_


#include "ItemTable.h"
#include "Item.h"
#include "ItemHelper.h"

struct ITEM_LOG_PARAM;

class Obj_ItemRuler
{
public:

	//Item��������

	VOID	CreateItemFromMonsterDrop(INT iMonsterID,
									  INT iPlayerLvl,
									  BOOL bTeam,
									  FLOAT fControlValue,
									  Obj_ItemContaner& OutContaner);
};


class ItemCreateRuler
{

public:

	static	BOOL		CreateEquipAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM_TYPE itemType,INT iPlayerJob,_ITEM& it,INT& quality, INT iMonsterLvl=0, INT iMonsterType=0);
	static	BOOL		CreateGemAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateGemAttrib(UINT TableIndex,_ITEM& it);
	static	BOOL		CreateMedicAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateTaskItemAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateMaterialAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateQuestItemAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateStoreMapAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateExtraBagAttrib(_ITEM_TYPE itemType,_ITEM& it);
	//	add by gh 2010/05/05
	static	BOOL		CreateSouXiaAttrib(_ITEM_TYPE itemType, _ITEM& it);
	static	BOOL		CreateSoulBeadAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static  BOOL	    CreateMountAttrib(_ITEM_TYPE itemType,_ITEM& it);      // [7/8/2010 �¾���]
private:
	static	BOOL	CaculateDropEquipEffectAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM& it, INT iPlayerJob=-1, INT iMonsterLvl=0, INT iMonsterType=0);
	static	BOOL    CaculateCreateEquipEffectAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM& it, INT iPlayerJob=0);
	static	BOOL    CaculateEquipEffectAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM& it, INT iPlayerLvl=0);
};






/*����Exp���������
 *	
 */
class MonsterExpCaculateRuler
{
public:

	uint				CaculateBaseExp(INT iMonsterLevel,INT iKillerLevel,uint MonsterBaseExp, BOOL bTeam);
};


/*
 *		������Ʒ����
 *		�Լ��̶�����
 */

class Obj_Monster;
class Obj_Human;

class MonsterDropRuler
{
public:

	static	BOX_DISTRIBUTE_RULER	
						GetMonsterDropRuler(Obj_Monster*	pMonster);

	static	BOOL		CaculateCommOwnerList(Obj_Monster*	pMonster);
	static	BOOL		CaculateBossOwnerList(Obj_Monster*	pMonster);

	static	BOOL		CaculateCommDropRuler(Obj_Monster*	pMonster, BOX_DISTRIBUTE_RULER DropRuler);


	static	BOOL		CaculateBossDropRuler(Obj_Monster*	pMonster, BOX_DISTRIBUTE_RULER DropRuler);

};

/* ��Ʒ��������
 *	
 */

class ItemRulerCheck
{
public:
	/* �����鷽��
	 *	
	 *		��������    
	 *			ITEM_RULER_LIST ruler			������
	 *		
	 *			const CHAR& iIndex				����ID
	 *
	 *		����
	 *
	 *			BOOL							0 ��ʾû��ͨ��  1 ��ʾͨ���ù���
	 *
	 */
	static BOOL	CheckRuler(ITEM_RULER_LIST ruler,const CHAR& iRuler)
	{

		__ENTER_FUNCTION

		INT iRulerID = iRuler;
		switch(ruler)
		{
		case IRL_DISCARD:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanDiscard();
			}
			break;
		case IRL_TILE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanTile();
			}
			break;
		case IRL_SHORTCUT:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanShortCut();
			}
			break;
		case IRL_CANSELL:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanSell();
			}
			break;
		case IRL_CANEXCHANGE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanExchange();
			}
			break;
		case IRL_CANUSE:
			{
				return	g_ItemTable.GetRuleValueByID(iRulerID)->CanUse();
			}
			break;
		case IRL_PICKBIND:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isPickBind();
			}
			break;
		case IRL_EQUIPBIND:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isEquipBind();
			}
			break;
		case IRL_UNIQUE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isUnique();
			}
			break;
		case IRL_IDENT:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isIdent();
			}
			break;	
		case IRL_DUMMYITEM:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isDummyItem();
			}
			break;
		case IRL_BANK:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isBack();
			}
			break;
		case IRL_CONSUME:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isConsume();
			}
			break;
		default:
			{
				Assert(FALSE);
			}
			break;
		}
		return FALSE;

	__LEAVE_FUNCTION

		return FALSE ;
	
	}



	/*
	 *		��鷽��
	 *		
	 *		��������    
	 *			ITEM_RULER_LIST ruler			������
	 *		
	 *			const CHAR& iIndex				����ID
	 *		
	 *		����
	 *		
	 *			BOOL							0 ��ʾû��ͨ��  1 ��ʾͨ���ù���
	 *	
	 *	
	 *	
	 *
	 */

	static	BOOL	CheckTypeRuler(ITEM_RULER_LIST ruler,const UINT uItemIndex);
	
};







#endif