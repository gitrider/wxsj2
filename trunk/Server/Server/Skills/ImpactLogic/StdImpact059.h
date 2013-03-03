/********************************************************************
	created:	2007/11/24
	created:	24:11:2007   13:56
	filename: 	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic\StdImpact059.h
	file path:	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact059
	file ext:	h
	author:		Richard
	
	purpose:	��֪(��ͬ����)����ʹ�ò���Ч��,ά��ʹ��������Ĵ˼��ܵ����Ժ�״̬

				1 ���ȶ�ȡImpact�����С�����������¼��m_uBallCount��

				2 ��ǰ��С��ȫ��ΨһID��ʼ��Ϊ0,Ϊ�Ƿ�ID

				3 ��ȫ��С��������л�ȡ���ܻ�ȡС���ȫ��ID,�����ڴ��������м���
                  �����п��ܻ����ظ�ID,��ɴ���

			    4 ÿ�������Է���ʱOnIntervalOver��Ϣ����ᱻ����,��OnIntervalOver
				  ����Ŀ��ִ������,�ҵ���ЧĿ��ʱ,�ж�Ŀ���Ƿ��Ѿ�������ͬ��Ч��
				  ����������Ŀ��,����,�ж�С�������Ƿ�>0,�����,��Ŀ�귢��Ч��,
				  ����С�������1

			    5 ��������ɢʱ�����κ����⴦��

				6 OnActive ����Ҫ���⴦��

				7 ��������Ч���ļ����߼�IDӦΪIMPACTS_TO_TARGET,Ŀ��Ӧ�����Լ�

				8 �� skl_template �����Ŀ��ѡ���߼�Ӧ���� TARGET_SELF��0��

				9 �ڴ��༼��ʹ��ʱ��ֻ��ʹ��������ע���Ч�����ɣ�����Ĺ�������Ϊ
				  �ɱ�Ч��������ʵ��
*********************************************************************/
#ifndef StdImpact059_h__24_11_2007_14_43
#define StdImpact059_h__24_11_2007_14_43

#include "ImpactLogic.h"


namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact059_T : public ImpactLogic_T
		{
			struct StdImpact059_Ball
			{
				UINT m_uUniqueID;
				Obj_Character* m_pCharacter;
				StdImpact059_Ball()
				{
					memset( this, 0, sizeof(StdImpact059_Ball) );
				}
			};
			public:
				enum
				{
					ID = STD_IMPACT_059,
				};
				StdImpact059_T();
				~StdImpact059_T();
				//��std_impact���Ĵ洢�����ܶ�ȡС�������
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				//�Ƿ�Ϊ�����Լ���
				BOOL IsOverTimed(VOID)  const { return TRUE; }
				//�Ƿ������Է���
				BOOL IsIntervaled(VOID) const { return TRUE;}

			public: //message handler
				//Ч������ʱ����Ϣ����
				virtual VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				//������ʱ�䵽�˻���ô���Ϣ����,������ЧĿ��,��Ŀ��ע��Ч��,ά��
				//С��ļ���
				virtual VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
				//�ӳ����������Ϸ���Ŀ�����rTargets�У�rTargetsΪ�������
				BOOL ScanUnitForTarget(OWN_IMPACT& rImp, Obj_Character& rMe, FLOAT fX,FLOAT fZ, OBJLIST& rTargets) const;
				//���ݻ��и��ʵ���Ϣ�жϺϷ���Ŀ���Ƿ���ı�����
				BOOL HitThisTarget(Obj_Character& rMe, Obj_Character& rTar, SkillID_t nSkillID) const;
				//ʵ�ʵĸ��ʼ���
				BOOL IsHit(Obj_Character& rMe, Obj_Character& rTar, INT nAccuracy) const;
				VOID BroadcastTargetListMessage( OWN_IMPACT& rImp, Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& rHitFlagsForObjList ) const;
				INT GetSubImpactDataIndexByIndex(OWN_IMPACT const& rImp, INT nIndex) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						nIndex,
						rImp.GetSkillLevel()
						);
				}
				//�ж��Ƿ������Ŀ��Ͷ��Ч������Ҫ��������Ҫ����:
				// 1 Ŀ�������Ƿ��Ѿ��������Buff
				// 2 ���Buff�Ƿ���Ͷ���߷�����
				//��� 1 ������������Ͷ��
				//��� 1 ���� 2 ������������Ͷ�ͣ�������Ͷ��
				BOOL IsCanSendToUnit( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* pChar ) const;
				//************************************
				// Method:    GetAccuracy
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::GetAccuracy
				// Access:    protected 
				// Returns:   VOID
				// Qualifier: const
				// Parameter: SkillID_t nSkillID
				// ���ڶ��Ч���ṫ��һ��Ч�����������ڴ˶����в��ܱ���Ч��������ص�����
				// ������ÿһ�η���ʱ��OWN_IMPACT�����л�ȡ��ص���Ϣ���˷����������OWN_IMPACT
				// �����е�skillID��ȡ�����ʲ���ʱ������m_iAccuracy�С�m_iAccuracy����ֱ��ʹ��
				//************************************
				VOID GetAccuracy( SkillID_t nSkillID ) const;
				//************************************
				// Method:    DoHitTarget
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::DoHitTarget
				// Access:    protected 
				// Returns:   VOID
				// Qualifier: const
				// Parameter: [in]OWN_IMPACT& rImp
				// Parameter: [in]Obj_Character& rMe
				// Parameter: [out]OBJLIST & rTargets Ŀ���б�
				// Parameter: [out]HitFlagsForOBJLIST_T & HitFlagList �Ƿ���еı���б���rTargetsһһ��Ӧ
				// Parameter: [out]UINT & nCount ʵ�ʻ��е�Ŀ�����
				//************************************
				VOID DoHitTarget( OWN_IMPACT& rImp, Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& HitFlagList, UINT& nCount ) const;
				//************************************
				// Method:    SendLayerChangeMsg
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::SendLayerChangeMsg
				// Access:    protected 
				// Returns:   VOID
				// Qualifier:
				// Parameter: OWN_IMPACT & rImp
				// Parameter: Obj_Character & rMe
				// ��ͻ��˷���buff�����仯����Ϣ
				//************************************
				VOID SendLayerChangeMsg( OWN_IMPACT& rImp, Obj_Character& rMe ) const;

				//************************************
				// Method:    ComputeLayerChange
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::ComputeLayerChange
				// Access:    protected 
				// Returns:   VOID
				// Qualifier: const
				// Parameter: OWN_IMPACT & rImp
				// Parameter: Obj_Character & rMe
				// Parameter: INT nCount
				// Parameter: INT iCurrentLayer
				// ����buff��ĸı�
				//************************************
				VOID ComputeLayerChange( OWN_IMPACT& rImp, Obj_Character& rMe, INT nCount, INT iCurrentLayer ) const;

				//************************************
				// Method:    LayerFadeOut
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::LayerFadeOut
				// Access:    protected 
				// Returns:   BOOL ��������б仯ΪTRUE,����ΪFALSE
				// Qualifier: const
				// Parameter: OWN_IMPACT & rImp
				//************************************
				BOOL LayerFadeOut( OWN_IMPACT& rImp ) const;

			private:
				////��֪��ͬ�༼��С��ĵ�ǰ����, ��InitFromData�г�ʼ��Ϊ����е�С��������
				////��������¼��template�����
				//UINT m_uBallCount;
				////��ǰ��С�����
				//UINT m_uCurrentBallCount;
				StdImpact059_Ball	m_aBallUniqueID[OBJLIST::MAX_OBJ_LIST_SIZE];
				INT					m_iAccuracy;

		} DI_SelfIntervalOverSkill_T;
	};
};

#endif //StdImpact059_h__24_11_2007_14_43
