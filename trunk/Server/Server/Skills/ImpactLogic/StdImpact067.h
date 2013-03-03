/********************************************************************
	created:	2008/05/06
	created:	6:5:2008   21:43
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact067.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact067
	file ext:	h
	author:		Richard
	
	purpose:	坐骑给主人加的被动技
*********************************************************************/

#ifndef StdImpact067_h__
#define StdImpact067_h__

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

//第一个参数的取值
//CHAR_ATT_INVALID	= -1,
//CHAR_ATT_STR=0,			// 力量
//CHAR_ATT_CON,				// 体制
//CHAR_ATT_INT,				// 智力
//CHAR_ATT_DEX,				// 敏捷
//
//CHAR_ATT_HP,				// HP

//CHAR_ATT_MAX_HP,			// HP上限
//
//CHAR_ATT_RESTORE_HP,		// HP回复
//CHAR_ATT_RESTORE_MP,		// MP回复
//
//CHAR_ATT_HIT,				// 命中率
//CHAR_ATT_MISS,			// 闪避
//CHAR_ATT_CRIT_RATE,		// 会心率
//
//CHAR_ATT_SPEED,			// 移动速度
//CHAR_ATT_ATTACK_SPEED,	// 攻击速度
//
//CHAR_ATT_ATTACK_NEAR,	    // 近程攻击
//CHAR_ATT_DEFENCE_NEAR,	// 近程防御
//
//CHAR_ATT_ATTACK_FAR,	    // 远程攻击
//CHAR_ATT_DEFENCE_FAR,	    // 远程防御
//
//CHAR_ATT_ATTACK_MAGIC_NEAR,// 近程内功攻击
//CHAR_ATT_DEFENCE_MAGIC_NEAR,	// 近程内功防御
//
//CHAR_ATT_ATTACK_GOLD,		// 金攻击
//CHAR_ATT_DEFENCE_GOLD,	// 金防御
//
//CHAR_ATT_ATTACK_WOOD,		// 木攻击
//CHAR_ATT_DEFENCE_WOOD,	// 木防御

//CHAR_ATT_ATTACK_WATER,	// 水攻击
//CHAR_ATT_DEFENCE_WATER,	// 水防御

//CHAR_ATT_ATTACK_FIRE,		// 火攻击
//CHAR_ATT_DEFENCE_FIRE,	// 火防御

//CHAR_ATT_ATTACK_SOIL,		// 土攻击
//CHAR_ATT_DEFENCE_SOIL,	// 土防御

//CHAR_ATT_ANGER,			// 怒气值
//CHAR_ATT_SKILL_POINT,		// 连技点



namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact067_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_067,
				};

				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IMPACT_TYPE_ID=0,	//参考 第一个参数的取值
							IMPACT_PARA_ID,		//对应4个公式的ID,顺序为
												//力量参数=等级*成长率/1000+力量*力量资质/1000
												//敏捷参数=等级*成长率/1000+敏捷*敏捷资质/1000
												//智力参数=等级*成长率/1000+智力*智力资质/1000
												//体质参数=等级*成长率/1000+体质*体质资质/1000
							IMPACT_REFIX_RATE,	//影响率 0 ~ 1000, 上面4个公式中的分母（要除1000）
							IMPACT_MAX_REFIX
						};
				};
				enum 
				{
					MAX_REFIX = 5
				};
				StdImpact067_T() {};
				~StdImpact067_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixType(OWN_IMPACT const& rImp, INT iIndex) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IMPACT_TYPE_ID + ImpactDataDescriptorIndex_T::IMPACT_MAX_REFIX * iIndex,
						rImp.GetSkillLevel()
						);
				};
				INT GetRefixParaID(OWN_IMPACT const& rImp, INT iIndex) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IMPACT_PARA_ID + ImpactDataDescriptorIndex_T::IMPACT_MAX_REFIX * iIndex,
						rImp.GetSkillLevel()
						);
				};
				INT GetRefixRate(OWN_IMPACT const& rImp, INT iIndex) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IMPACT_REFIX_RATE + ImpactDataDescriptorIndex_T::IMPACT_MAX_REFIX * iIndex,
						rImp.GetSkillLevel()
						);
				};
		public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;

				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;



			protected:
				VOID MarkDirtyFlag( OWN_IMPACT & rImp, Obj_Character &rMe, INT iType ) const;
				INT FindIndex( OWN_IMPACT & rImp, Obj_Character& rMe, INT iType ) const;
				INT GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const;
				INT GetRefixResult( OWN_IMPACT & rImp, Obj_Character& rMe, INT iType ) const;
				INT GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe, INT iIndex ) const;
		private:
				
		};
	};
};
#endif // StdImpact067_h__
