///////////////////////////////////////////////////////////////////////////////
// 文件名：Combat_Core.h
// 程序员：高骐
// 功能说明：战斗逻辑内核
//
// 修改记录：
// 祝方亮  增加注释 目的在于建立文档的同时更快
//		   更准确的掌握类功能，此次修改只限于增加注释
//		   不做任何代码修改。2007-10-29 
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef COMBAT_CORE_H
#define COMBAT_CORE_H

#include "Type.h"
#include "Obj_Character.h"
#include "SkillInfo.h"

using namespace Combat_Module;
using namespace Impact_Module;

namespace Combat_Module
{
	typedef enum
	{
		DAMAGE_SPECIAL_EFFECT_NEAR=0,
		DAMAGE_SPECIAL_EFFECT_FAR,
		DAMAGE_SPECIAL_EFFECT_MAGIC_NEAR,
		DAMAGE_SPECIAL_EFFECT_MAGIC_FAR,
		DAMAGE_SPECIAL_EFFECT_GOLD,
		DAMAGE_SPECIAL_EFFECT_WOOD,
		DAMAGE_SPECIAL_EFFECT_WATER,
		DAMAGE_SPECIAL_EFFECT_FIRE,
		DAMAGE_SPECIAL_EFFECT_SOIL,
	} DamageSEID_T;
	
	class CombatCore_T
	{
		//注: 请在每次使用时声明一个实体，内含成员变量，非多线程支持
		public:
			CombatCore_T(){Reset();};
			~CombatCore_T(){};
			INT CalculateHitRate(Obj_Character& rAttacker, Obj_Character& rTag);
			BOOL IsHit(INT nHitRate, INT nRand);
			//************************************
			// Method:    IsCriticalHit
			// FullName:  Combat_Module::CombatCore_T::IsCriticalHit
			// Access:    public 
			// Returns:   BOOL
			// Qualifier:
			// Parameter: INT nCriticalRate 攻击方爆击率
			// Parameter: INT nRand	概率用随机数 0~100
			// Parameter: INT iToughness 受击方韧性
			//************************************
			BOOL IsCriticalHit(Obj_Character& rAttacker, INT nCriticalRate, INT nRand, INT iToughness);
			VOID Reset();
			BOOL GetResultImpact(Obj_Character& rAttacker, Obj_Character& rDefenceer, OWN_IMPACT& rImp);
		public:
			INT GetAdditionalAttackNear(VOID) const {return m_nAdditionalAttackNear;};
			VOID SetAdditionalAttackNear(INT nValue) {m_nAdditionalAttackNear=nValue;};

			INT GetAdditionalAttackFar(VOID) const {return m_nAdditionalAttackFar;};
			VOID SetAdditionalAttackFar(INT nValue) {m_nAdditionalAttackFar=nValue;};


			INT GetAdditionalAttackMagicNear(VOID) const {return m_nAdditionalAttackMagicNear;};
			INT GetAdditionalAttackMagicFar(VOID) const {return m_nAdditionalAttackMagicFar;};

			VOID SetAdditionalAttackMagicNear(INT nValue) {m_nAdditionalAttackMagicNear=nValue;};
			VOID SetAdditionalAttackMagicFar(INT nValue) {m_nAdditionalAttackMagicFar=nValue;};

			INT GetAdditionalAttackGold(VOID) const {return m_nAdditionalAttackGold;};
			VOID SetAdditionalAttackGold(INT nValue) {m_nAdditionalAttackGold=nValue;};

			INT GetAdditionalAttackWood(VOID) const {return m_nAdditionalAttackWood;};
			VOID SetAdditionalAttackWood(INT nValue) {m_nAdditionalAttackWood=nValue;};

			INT GetAdditionalAttackWater(VOID) const {return m_nAdditionalAttackWater;};
			VOID SetAdditionalAttackWater(INT nValue) {m_nAdditionalAttackWater=nValue;};

			INT GetAdditionalAttackFire(VOID) const {return m_nAdditionalAttackFire;};
			VOID SetAdditionalAttackFire(INT nValue) {m_nAdditionalAttackFire=nValue;};

			INT GetAdditionalAttackSoil(VOID) const {return m_nAdditionalAttackSoil;};
			VOID SetAdditionalAttackSoil(INT nValue) {m_nAdditionalAttackSoil=nValue;}

		protected:
			INT NearDamage		(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack=0,   INT nAdditionDefence=0);
			INT FarDamage		(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack=0,   INT nAdditionDefence=0);
			
			INT MagicalNearDamage	(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack=0,   INT nAdditionDefence=0);
			INT MagicalFarDamage	(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack=0,   INT nAdditionDefence=0);

			INT GoldDamage		(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
			INT WoodDamage		(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);

			INT WaterDamage	(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
			INT FireDamage		(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
			INT SoilDamage		(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
			
			INT NormalDamage( Obj_Character &rTar, INT nAttack, INT nDefence, INT iType );
			INT AttrDamage( Obj_Character &rTar, INT &nResist, INT nAttack, INT iType );
		private:
			INT m_nAdditionalAttackNear;
			INT m_nAdditionalAttackFar;
			INT m_nAdditionalAttackMagicNear;
			INT m_nAdditionalAttackMagicFar;

			INT m_nAdditionalAttackGold;
			INT m_nAdditionalAttackWood;

			INT m_nAdditionalAttackWater;
			INT m_nAdditionalAttackFire;
			INT m_nAdditionalAttackSoil;
			
			INT m_nAdditionalDefenceNear;
			INT m_nAdditionalDefenceFar;
			INT m_nAdditionalDefenceMagicNear;
			INT m_nAdditionalDefenceMagicFar;

			INT m_nAdditionalDefenceGold;
			INT m_nAdditionalDefenceWood;

			INT m_nAdditionalDefenceWater;
			INT m_nAdditionalDefenceFire;
			INT m_nAdditionalDefenceSoil;
	};
};
#endif //SKILL_CORE_H
