///////////////////////////////////////////////////////////////////////////////
// �ļ�����Impact_Core.h
// ����Ա������
// ����˵����Ч���ں�ģ��
//
// �޸ļ�¼��
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
				ImpactCore_T(){ m_uUniqueID = 1; }//0Ϊ��ЧID
				~ImpactCore_T(){};
				//����nDataIndex��Ӧ��Ч���߼���ʼ��rImp
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
				// Ϊ��֪�༼�����صķ������ر�֮�����ڣ�skillID,��Ͷ����ֱ��ָ��
				//************************************
				BOOL SendImpactToUnit(Obj_Character& rTar, ID_t nDataIndex, ObjID_t nSender, SkillID_t nSkillID, Time_t nDelayTime=0, BOOL bCriticalFlag=FALSE, INT nRefixRate=0) const;

				//��ȡЧ��ϵͳ��ͳһuniqueID
				UINT GetUniqueID();
			private:
				UINT m_uUniqueID;
		};
		//impact
		ImpactData_T const* Impact_GetDataBlock(ID_t nDataIndex);
		//����Ч���Ƿ��л����ϵ
		BOOL Impact_IsImpactsABMutexed(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB);
		//�ж�Ч��A�Ƿ�����滻Ч��B
		BOOL Impact_CanImpactAReplaceImpactB(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB);
		Obj* Impact_GetCaster(Obj& rMe, OWN_IMPACT& rImp);
		ID_t Impact_GetLogicID(OWN_IMPACT const& rImp);
		BOOL Impact_IsOverTimed(OWN_IMPACT& rImp);
		ID_t Impact_GetImpactID(OWN_IMPACT& rImp);
		ID_t Impact_GetMutexID(OWN_IMPACT& rImp);
		//���ͬһ����ɫ��ɶ����ͬЧ���Ƿ񻥳�
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
