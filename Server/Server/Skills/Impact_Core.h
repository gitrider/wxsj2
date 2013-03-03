///////////////////////////////////////////////////////////////////////////////
// 文件名：Impact_Core.h
// 程序员：高骐
// 功能说明：效果内核模块
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef IMPACT_CORE_H
#define IMPACT_CORE_H

#include "Type.h"
#include "Obj_Character.h"
#include "DataRecords.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class ImpactCore_T
		{
			public:
				ImpactCore_T(){ m_uUniqueID = 1; }//0为无效ID
				~ImpactCore_T(){};
				//根据nDataIndex对应的效果逻辑初始化rImp
				BOOL InitImpactFromData(ID_t nDataIndex, OWN_IMPACT& rImp, Obj_Character& rTar, INT nReserveParam1=0, INT nReserveParam2=0) const;
				BOOL SendImpactToUnit(Obj_Character& rTar, ID_t nDataIndex, ObjID_t nSender, Time_t nDelayTime=0, BOOL bCriticalFlag=FALSE, INT nRefixRate=0, INT nReserveParam1=0, INT nReserveParam2=0) const;//modify by gh for SoulBead

				//************************************
				// Method:    SendImpactToUnit
				// FullName:  Combat_Module::Impact_Module::ImpactCore_T::SendImpactToUnit
				// Access:    public 
				// Returns:   BOOL
				// Qualifier: const
				// Parameter: Obj_Character & rTar
				// Parameter: ID_t nDataIndex
				// Parameter: ObjID_t nSender
				// Parameter: SkillID_t nSkillID
				// Parameter: Time_t nDelayTime
				// Parameter: BOOL bCriticalFlag
				// Parameter: INT nRefixRate
				// 为先知类技能重载的方法，特别之处在于，skillID,由投送者直接指定
				//************************************
				BOOL SendImpactToUnit(Obj_Character& rTar, ID_t nDataIndex, ObjID_t nSender, SkillID_t nSkillID, Time_t nDelayTime=0, BOOL bCriticalFlag=FALSE, INT nRefixRate=0) const;

				//获取效果系统的统一uniqueID
				UINT GetUniqueID();
			private:
				UINT m_uUniqueID;
		};
		//impact
		ImpactData_T const* Impact_GetDataBlock(ID_t nDataIndex);
		//两个效果是否有互斥关系
		BOOL Impact_IsImpactsABMutexed(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB);
		//判断效果A是否可以替换效果B
		BOOL Impact_CanImpactAReplaceImpactB(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB);
		Obj* Impact_GetCaster(Obj& rMe, OWN_IMPACT& rImp);
		ID_t Impact_GetLogicID(OWN_IMPACT const& rImp);
		BOOL Impact_IsOverTimed(OWN_IMPACT& rImp);
		ID_t Impact_GetImpactID(OWN_IMPACT& rImp);
		ID_t Impact_GetMutexID(OWN_IMPACT& rImp);
		//获得同一个角色造成多个相同效果是否互斥
		BOOL Impact_GetMutexByCasterFlag(OWN_IMPACT& rImp);
		ID_t Impact_GetLevel(OWN_IMPACT& rImp);
		ID_t Impact_GetStandFlag(OWN_IMPACT& rImp);
		BOOL Impact_IsRemainOnCorpse(OWN_IMPACT& rImp);
		BOOL Impact_CanBeDispeled(OWN_IMPACT& rImp);
		BOOL Impact_CanBeCanceled(OWN_IMPACT& rImp);
		BOOL Impact_NeedChannelSupport(OWN_IMPACT& rImp);
		BOOL Impact_IsFadeOutWhenUnitOnDamage(OWN_IMPACT& rImp);
		BOOL Impact_IsFadeOutWhenUnitStartActions(OWN_IMPACT& rImp);
		BOOL Impact_IsFadeOutWhenUnitOffline(OWN_IMPACT& rImp);
		BOOL Impact_IsStillTimingWhenUnitOffline(OWN_IMPACT& rImp);
		ID_t Impact_IgnoreFliter(OWN_IMPACT& rImp);
		Time_t Impact_GetTableContinuance(OWN_IMPACT& rImp);
		Time_t Impact_GetInterval(OWN_IMPACT& rImp);
		INT	Impact_GetImpactDataDescriptorValueByIndex(INT nDataIndex, INT nIndex, INT iLevel );
		BOOL Impact_IsImpactInCollection(OWN_IMPACT& rImp, INT nCollection);
	};
};
extern Combat_Module::Impact_Module::ImpactCore_T g_ImpactCore;
#endif //IMPACT_CORE_H
