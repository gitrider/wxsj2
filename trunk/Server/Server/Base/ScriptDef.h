// ScriptDef.h
//
/////////////////////////////////////////////////////////

#ifndef __SCRIPTDEF_H__
#define __SCRIPTDEF_H__

#define ABILITY_LOGIC_SCRIPT			701601
#define GEM_COMPOUND_SCRIPT				701602
#define SUBTRAIN_EXPRATE_SCRIPT			800025
#define SUBTRAIN_MONEYRATE_SCRIPT		800026

#define	DEF_EVENT_ENTRY_FUNC_NAME		("OnDefaultEvent")
#define	DEF_ON_KILL_OBJECT_FUNC_NAME	("OnKillObject")
#define	DEF_ON_ITEM_CHANGED_FUNC_NAME	("OnItemChanged")
#define	DEF_ON_PET_CHANGED_FUNC_NAME	("OnPetChanged")
#define	DEF_ON_ENTER_AREA_FUNC_NAME		("OnEnterArea")
#define	DEF_ON_LEAVE_AREA_FUNC_NAME		("OnLeaveArea")
#define DEF_EVENT_ON_TIMER				("OnTimer")

// ���������ͨ�ϳ��䷽ʹ��
// ����ʹ�ü�麯��
#define DEF_ABILITY_CHECK				("AbilityCheck")
// �ϳɽ����������������
#define DEF_ABILITY_CONSUME				("AbilityConsume")
// �ϳɳɹ���������Ʒ
#define DEF_ABILITY_PRODUCE				("AbilityProduce")

// �ϳɽ���ʱ�ĳɹ��ʼ���
#define DEF_ABILITY_SUCCESSFUL_CHECK	("CheckForResult")
// �䷽�ϳɽ���ʱ������������
#define DEF_ABILITY_GAIN_EXPERIENCE		("CompoundGainExperience")

// �ϳɳɹ�ʱ�����Ʒ��Ʒ�ʣ�Quality��
#define DEF_ABILITY_CALC_QUALITY		("CalcQuality")

// �����Ǳ�ʯ����
#define DEF_GEM_COUNT_CHECK				("TooManyGems")
#define DEF_IS_GEM_FIT_EQUIP			("IsGemFitEquip")
#define DEF_GEM_CONFLICT_CHECK			("IsGemConflict")
#define DEF_GEM_EMBED_PROCESS			("EmbedProc")

#define DEF_GEM_COMPOUND				("GemCompound")

#define DEF_SUBTRAIN_EXPRATE			("SubTrainExpRate")
#define DEF_SUBTRAIN_MONEYRATE			("SubTrainMoneyRate")
#endif