#ifndef _PET_CONFIG_H_
#define _PET_CONFIG_H_

#define GENGU_NUM (5)

struct SPetConfig
{
	enum {
		VARIANCEPET_LEVEL_NUM = 8,
	};

	INT		 m_VariancePetRate;		//���ɱ�������
	INT		 m_BabyPetRate;			//���ɱ�������

	//Ұ����ȼ����ɸ��ʣ��ĸ�ֵ��
	INT		 m_WilenessPetRate_TakeLevel;	// Я���ȼ�����
	INT		 m_WilenessPetRate_Delta1;		// Я���ȼ�+,-1����	
	INT		 m_WilenessPetRate_Delta2;		// Я���ȼ�+,-2����	
	INT		 m_WilenessPetRate_Delta3;		// Я���ȼ�+,-3����	

	//�ɳ������ɸ��ʣ����ֵ��
	INT		 m_GrowRate0;
	INT		 m_GrowRate1;
	INT		 m_GrowRate2;
	INT		 m_GrowRate3;
	INT		 m_GrowRate4;

	// �������ȼ�����(10w)
	INT		m_aRateOfLevelVariancePet[VARIANCEPET_LEVEL_NUM];

	//�������
	struct SGenGu
	{
		INT		m_Begin;
		INT		m_End;
		INT		m_Rate;
	};
	SGenGu	m_vGenGu[GENGU_NUM];

	//���ʱ䶯��Χ�����5����
	INT		 m_IntelligenceRange;

	//�ƶ��ٶ�
	INT		 m_MoveSpeed;

	//�����ٶ�
	INT		 m_AttackSpeed;

	// AI�������
	INT		m_PetAI0_MagicRate;
	INT		m_PetAI1_MagicRate;
	INT		m_PetAI2_MagicRate;
	INT		m_PetAI3_MagicRate;

	// ��ʼHP���﹥��ħ���������ħ�������С����ܡ����ģ��˸���
	INT		m_BaseHP;
	//INT		m_BasePhyAttack;
	INT     m_BaseNearAttack;
	INT     m_BaseFarAttack;
	INT		m_BaseMgcAttack;
	//INT		m_BasePhyDefence;
	INT     m_BaseNearDefence;
	INT     m_BaseFarDefence;
	INT		m_BaseMgcDefence;
	INT		m_BaseHit;
	INT		m_BaseMiss;
	INT		m_BaseCritical;

	// ���ʶ�HP���������﹥��������ħ�������ʶ������������ħ�������ݶ����ܣ����ݶԻ�
	// ��,���ݶ����е�Ӱ��ϵ��
	FLOAT		m_Con_HP_Pram;
	//FLOAT		m_Str_PhyAttack_Pram;
	FLOAT       m_Str_NearAttack_Pram;
	//FLOAT		m_Spr_MgcAttack_Pram;
	//FLOAT		m_Con_PhyDefence_Pram;
	FLOAT		m_Int_MgcDefence_Pram;
	FLOAT		m_Dex_Miss_Pram;
	FLOAT		m_Dex_Critical_Pram;
	FLOAT		m_Dex_Hit_Pram;

	// �ȼ���HP���﹥��ħ���������ħ�������ܡ����ġ����е�Ӱ��ϵ��
	FLOAT		m_Level_HP_Pram;
	//FLOAT		m_Level_PhyAttack_Pram;
	FLOAT       m_Level_NearAttack_Pram;
	FLOAT       m_Level_FarAttack_Pram;

	FLOAT		m_Level_MgcAttack_Pram;
	FLOAT       m_Level_NearDefence_Pram;
	FLOAT       m_Level_FarDefence_Pram;
	//FLOAT		m_Level_PhyDefence_Pram;
	FLOAT		m_Level_MgcDefence_Pram;
	FLOAT		m_Level_Miss_Pram;
	FLOAT		m_Level_Critical_Pram;
	FLOAT		m_Level_Hit_Pram;

	//�������
	INT			m_Level_FindingHorseMate;
	INT			m_Happiness_FindingHorseMate;
	INT			m_Money_FindingHorseMate;


};

#endif