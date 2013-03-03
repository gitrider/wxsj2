///////////////////////////////////////////////////////////////////////////////
// 文件名：Attributes.h
// 程序员：高骐
// 功能说明：角色层属性
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "Type.h"
#include "GameUtil.h"

class CharIntAttrs_T
{
	public:
		typedef enum
		{
			ATTR_STR,
			//ATTR_SPR,
			ATTR_CON,
			ATTR_INT,
			ATTR_DEX,
			ATTR_MAX_HP,
			ATTR_HP_REGENERATE,
			ATTR_MAX_MP,
			ATTR_MP_REGENERATE,
			ATTR_MAX_PNEUMA,								//最大元气值
			ATTR_PNEUMA_REGENERATE,							//元气回复值
			ATTR_MAX_STRIKE_POINT,
			ATTR_MAX_RAGE,
			ATTR_RAGE_REGENERATE,
			ATTR_CAMP,
			ATTR_MODEL_ID,
			ATTR_MOUNT_ID,
			ATTR_EXP_REFIX,
			ATTR_STEALTH_LEVEL,
			ATTR_DETECT_LEVEL,
			//ATTR_ATTACK_PHY,
			//add by liam
			ATTR_ATTACK_NEAR,
			ATTR_ATTACK_FAR,
			ATTR_DEFENCE_NEAR,
			ATTR_DEFENCE_FAR,
			ATTR_TOUGHNESS,
			ATTR_NEARATT_REDUCE,
			ATTR_FARATT_REDUCE,
			ATTR_MAGIC_NEAR_ATT_REDUCE,						//近程内功减免
			ATTR_MAGIC_FAR_ATT_REDUCE,						//近程外功减免
			ATTR_DREAD_RESIST,
			ATTR_COMA_RESIST,
			ATTR_HUSH_RESIST,
			ATTR_UNARMY_RESIST,
			ATTR_ATTACKSPEED_RESIST,
			ATTR_SKILLSPEED_RESIST,
			ATTR_MOVEREDUCE_RESIST,
			//ATTR_DEFENCE_PHY,

			ATTR_ATTACK_MAGIC_NEAR,							//近程内功攻击力
			ATTR_ATTACK_MAGIC_FAR,							//近程外功攻击力
			ATTR_DEFENCE_MAGIC_NEAR,						//近程内功防御力
			ATTR_DEFENCE_MAGIC_FAR,							//近程内功防御力

			ATTR_ATTACK_GOLD,								//金攻击
			ATTR_RESIST_GOLD,								//金抗性

			ATTR_ATTACK_WOOD,
			ATTR_RESIST_WOOD,

			ATTR_ATTACK_WATER,
			ATTR_RESIST_WATER,

			ATTR_ATTACK_FIRE,
			ATTR_RESIST_FIRE,

			ATTR_ATTACK_SOIL,
			ATTR_RESIST_SOIL,

			ATTR_MISS,
			ATTR_HIT,
			ATTR_CRITICAL,
			ATTR_MOVE_SPEED,
			ATTR_ATTACK_SPEED,
			ATTR_REDUCE_SLOWER_DURATION,
			ATTR_REDUCE_WEAKEN_DURATION,
			ATTR_REDUCE_FAINT_DURATION,
			ATTR_REDUCE_POISONED_DURATION,
			ATTR_VISION_RANGE,

			ATTR_MAX_ENERGY,
			ATTR_MAX_VIGOR,
			ATTR_ENERGY_REGENERATE,
			ATTR_VIGOR_REGENERATE,
			MAX_INDEX,
		} Index_T;
		CharIntAttrs_T()
		{
			ResetAttrs();
		}; 
		~CharIntAttrs_T(){};
		INT GetAttr(Index_T const nIdx) const {return m_aAttrs[nIdx];}
		VOID SetAttr(Index_T const nIdx, INT const nValue){m_aAttrs[nIdx]=nValue;};
		VOID ResetAttrs(VOID)
		{	
			memset((VOID*)m_aAttrs, '\0', sizeof(m_aAttrs));
			m_aAttrs[ATTR_MOUNT_ID]=INVALID_ID;

		};
	private:
		INT m_aAttrs[MAX_INDEX];
};
class CharBoolAttrs_T
{
	public:
		typedef enum
		{
			ATTR_ALIVE,
			ATTR_IN_COMBAT,
			ATTR_CAN_MOVE,
			ATTR_CAN_ACTION1,
			ATTR_CAN_ACTION2,
			ATTR_DISABLE_FLAG2,
			ATTR_UNBREAKABLE,
			ATTR_IN_STEALTH_MODE,
			ATTR_CAN_DETECT_STEALTH,
			MAX_INDEX,
		}Index_T;
		CharBoolAttrs_T()
		{
			ResetAttrs();
		};
		~CharBoolAttrs_T(){};
		BOOL GetAttr(Index_T const nIdx) const {return m_FlagSet.GetFlagByIndex(nIdx);}
		VOID SetAttr(Index_T const nIdx, BOOL bFlag){TRUE==bFlag?m_FlagSet.MarkFlagByIndex(nIdx):m_FlagSet.ClearFlagByIndex(nIdx);}
		VOID ResetAttrs(VOID)
		{
			SetAttr(ATTR_ALIVE, TRUE);
			SetAttr(ATTR_IN_COMBAT, FALSE);
			SetAttr(ATTR_CAN_MOVE, TRUE);
			SetAttr(ATTR_CAN_ACTION1,TRUE);
			SetAttr(ATTR_CAN_ACTION2,TRUE);
			SetAttr(ATTR_UNBREAKABLE,FALSE);
			SetAttr(ATTR_IN_STEALTH_MODE, FALSE);
			SetAttr(ATTR_CAN_DETECT_STEALTH, FALSE);
		};
	private:
		BitFlagSet_T<MAX_INDEX> m_FlagSet;
		
};
class CharIntAttrRefixs_T
{
	public:
		typedef enum
		{
			REFIX_STR,
			//REFIX_SPR,
			REFIX_CON,
			REFIX_INT,
			REFIX_DEX,
			REFIX_MAX_HP,
			REFIX_HP_REGENERATE,
			REFIX_MAX_MP,
			REFIX_MP_REGENERATE,
			REFIX_MAX_PNEUMA,
			REFIX_PNEUMA_REGENERATE,
			REFIX_MAX_STRIKE_POINT,
			REFIX_MAX_RAGE,
			REFIX_RAGE_REGENERATE,
			REFIX_CAMP,
			REFIX_MODEL_ID,
			REFIX_MOUNT_ID,
			REFIX_EXP_REFIX,
			REFIX_STEALTH_LEVEL,
			REFIX_DETECT_LEVEL,
			//REFIX_ATTACK_PHY,
			//add by liam
			REFIX_ATTACK_NEAR,
			REFIX_ATTACK_FAR,
			REFIX_DEFENCE_NEAR,
			REFIX_DEFENCE_FAR,
			REFIX_TOUGHNESS,
			REFIX_NEARATT_REDUCE,
			REFIX_FARATT_REDUCE,
			REFIX_MAGIC_NEAR_ATT_REDUCE,
			REFIX_MAGIC_FAR_ATT_REDUCE,
			REFIX_DREAD_RESIST,
			REFIX_COMA_RESIST,
			REFIX_HUSH_RESIST,
			REFIX_UNARMY_RESIST,
			REFIX_ATTACKSPEED_RESIST,
			REFIX_SKILLSPEED_RESIST,
			REFIX_MOVEREDUCE_RESIST,

			//REFIX_DEFENCE_PHY,
			REFIX_ATTACK_MAGIC_NEAR,
			REFIX_ATTACK_MAGIC_FAR,
			REFIX_DEFENCE_MAGIC_NEAR,
			REFIX_DEFENCE_MAGIC_FAR,

			REFIX_ATTACK_GOLD,
			REFIX_RESIST_GOLD,

			REFIX_ATTACK_WOOD,
			REFIX_RESIST_WOOD,

			REFIX_ATTACK_WATER,
			REFIX_RESIST_WATER,

			REFIX_ATTACK_FIRE,
			REFIX_RESIST_FIRE,

			REFIX_ATTACK_SOIL,
			REFIX_RESIST_SOIL,

			REFIX_MISS,
			REFIX_HIT,
			REFIX_CRITICAL,
			REFIX_MOVE_SPEED,
			REFIX_ATTACK_SPEED,
			REFIX_REDUCE_SLOWER_DURATION,
			REFIX_REDUCE_WEAKEN_DURATION,
			REFIX_REDUCE_FAINT_DURATION,
			REFIX_REDUCE_POISONED_DURATION,
			REFIX_VISION_RANGE,
			MAX_INDEX,
		}Index_T;
		CharIntAttrRefixs_T()
		{
			ResetAttrs();
		};
		~CharIntAttrRefixs_T(){};
		INT GetAttr(Index_T const nIdx) const {return m_aAttrRefixs[nIdx];}
		VOID SetAttr(Index_T const nIdx, INT const nValue){m_aAttrRefixs[nIdx]=nValue;};
		VOID ResetAttrs(VOID)
		{
			memset((VOID*)m_aAttrRefixs, '\0', sizeof(m_aAttrRefixs));	
			m_aAttrRefixs[REFIX_CAMP]=INVALID_ID;
			m_aAttrRefixs[REFIX_MODEL_ID]=INVALID_ID;
			m_aAttrRefixs[REFIX_MOUNT_ID]=INVALID_ID;
		}
	private:
		INT m_aAttrRefixs[MAX_INDEX];
};
/*
class CharBoolAttrRefixs_T
{
	public:
		typedef enum
		{
			REFIX_ALIVE,
			REFIX_IN_COMBAT,
			REFIX_CAN_MOVE,
			REFIX_CAN_ACTION,
			REFIX_UNBREAKABLE,
			MAX_INDEX,
		}Index_T;
		CharBoolAttrRefixs_T()
		{
			ResetAttrs();
		};
		~CharBoolAttrRefixs_T(){};
		BOOL GetAttr(Index_T const nIdx) const {return m_FlagSet.GetFlagByIndex(nIdx);}
		VOID SetAttr(Index_T const nIdx, BOOL bFlag){TRUE==bFlag?m_FlagSet.MarkFlagByIndex(nIdx):m_FlagSet.ClearFlagByIndex(nIdx);}
		VOID ResetAttrs(VOID)
		{
			SetAttr(REFIX_ALIVE, TRUE);
			SetAttr(REFIX_IN_COMBAT, FALSE);
			SetAttr(REFIX_CAN_MOVE, TRUE);
			SetAttr(REFIX_CAN_ACTION,TRUE);
			SetAttr(REFIX_UNBREAKABLE,FALSE);
		};
	private:
		BitFlagSet_T<MAX_INDEX> m_FlagSet;
		
};
*/
#endif //ATTRIBUTES_H
