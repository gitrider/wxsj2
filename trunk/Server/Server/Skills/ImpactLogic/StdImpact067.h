/********************************************************************
	created:	2008/05/06
	created:	6:5:2008   21:43
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact067.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact067
	file ext:	h
	author:		Richard
	
	purpose:	��������˼ӵı�����
*********************************************************************/

#ifndef StdImpact067_h__
#define StdImpact067_h__

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

//��һ��������ȡֵ
//CHAR_ATT_INVALID	= -1,
//CHAR_ATT_STR=0,			// ����
//CHAR_ATT_CON,				// ����
//CHAR_ATT_INT,				// ����
//CHAR_ATT_DEX,				// ����
//
//CHAR_ATT_HP,				// HP

//CHAR_ATT_MAX_HP,			// HP����
//
//CHAR_ATT_RESTORE_HP,		// HP�ظ�
//CHAR_ATT_RESTORE_MP,		// MP�ظ�
//
//CHAR_ATT_HIT,				// ������
//CHAR_ATT_MISS,			// ����
//CHAR_ATT_CRIT_RATE,		// ������
//
//CHAR_ATT_SPEED,			// �ƶ��ٶ�
//CHAR_ATT_ATTACK_SPEED,	// �����ٶ�
//
//CHAR_ATT_ATTACK_NEAR,	    // ���̹���
//CHAR_ATT_DEFENCE_NEAR,	// ���̷���
//
//CHAR_ATT_ATTACK_FAR,	    // Զ�̹���
//CHAR_ATT_DEFENCE_FAR,	    // Զ�̷���
//
//CHAR_ATT_ATTACK_MAGIC_NEAR,// �����ڹ�����
//CHAR_ATT_DEFENCE_MAGIC_NEAR,	// �����ڹ�����
//
//CHAR_ATT_ATTACK_GOLD,		// �𹥻�
//CHAR_ATT_DEFENCE_GOLD,	// �����
//
//CHAR_ATT_ATTACK_WOOD,		// ľ����
//CHAR_ATT_DEFENCE_WOOD,	// ľ����

//CHAR_ATT_ATTACK_WATER,	// ˮ����
//CHAR_ATT_DEFENCE_WATER,	// ˮ����

//CHAR_ATT_ATTACK_FIRE,		// �𹥻�
//CHAR_ATT_DEFENCE_FIRE,	// �����

//CHAR_ATT_ATTACK_SOIL,		// ������
//CHAR_ATT_DEFENCE_SOIL,	// ������

//CHAR_ATT_ANGER,			// ŭ��ֵ
//CHAR_ATT_SKILL_POINT,		// ������



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
							IMPACT_TYPE_ID=0,	//�ο� ��һ��������ȡֵ
							IMPACT_PARA_ID,		//��Ӧ4����ʽ��ID,˳��Ϊ
												//��������=�ȼ�*�ɳ���/1000+����*��������/1000
												//���ݲ���=�ȼ�*�ɳ���/1000+����*��������/1000
												//��������=�ȼ�*�ɳ���/1000+����*��������/1000
												//���ʲ���=�ȼ�*�ɳ���/1000+����*��������/1000
							IMPACT_REFIX_RATE,	//Ӱ���� 0 ~ 1000, ����4����ʽ�еķ�ĸ��Ҫ��1000��
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
