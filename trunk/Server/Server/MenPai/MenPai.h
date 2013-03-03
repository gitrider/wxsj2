//***************************************************************************************************
// �ļ�����:	MenPai.h
// ϵͳ����:	
// ģ������:	MenPai
// ����˵��:	�������ɵĽӿ�����
//
// ����ĵ�:	
// ��    ��:	liam
// ��������:	
// ��    ��:	
// �����Ȩ:	
//
//�޸ļ�¼:
//          1.
//				�� �� ��:liam
//				�޸�ʱ��:2007/10/30
//				�޸�����:���Ե�һ�����ع�(����μ��ĵ�)
//          2.
//				�� �� ��:liam
//				�޸�ʱ��:2007/11/02
//				�޸�����:���Եڶ������ع�(����μ��ĵ�)
//***************************************************************************************************
#ifndef MENPAI_H
#define MENPAI_H

#include "Type.h"
#include "Obj_Human.h"

namespace MenPai_Module
{
	class MenPai_T
{
	public:
		typedef enum
		{
			ID_INVALID					= -1,							//��Ч����ID, ��ʼ���ú��жϷǷ���
			ID_SHAOLIN_GWS				= MATTRIBUTE_SHAOLIN_GWS,		//������[����ɮ]
			ID_SHAOLIN_TLH				= MATTRIBUTE_SHAOLIN_TLH,		//������[���޺�]
			ID_GAIBANG_GGB				= MATTRIBUTE_GAIBANG_GGB,		//ؤ��[��ؤ��]
			ID_GAIBANG_ZGB				= MATTRIBUTE_GAIBANG_ZGB,		//ؤ��[��ؤ��]
			ID_WUDANG_JWD				= MATTRIBUTE_WUDANG_JWD,		//�䵱��[���䵱]
			ID_WUDANG_QWD				= MATTRIBUTE_WUDANG_QWD,		//�䵱��[���䵱]
			ID_TANGMEN_DT				= MATTRIBUTE_TANGMEN_DT,		//����[����]
			ID_TANGMEN_XJT				= MATTRIBUTE_TANGMEN_XJT,		//����[������]
			ID_XIAOYAO_XXY				= MATTRIBUTE_XIAOYAO_XXY,		//��ң��[����ң]
			ID_XIAOYAO_ZXY				= MATTRIBUTE_XIAOYAO_ZXY,		//��ң��[ָ��ң]
			ID_YAOWANGGU_KYW			= MATTRIBUTE_YAOWANGGU_KYW,		//ҩ����[��ҩ��]
			ID_YAOWANGGU_YYW			= MATTRIBUTE_YAOWANGGU_YYW,		//ҩ����[ҽҩ��]
			ID_TAOHUAGU_DTH				= MATTRIBUTE_TAOHUAGU_DTH,		//�һ���[���һ�]
			ID_TAOHUAGU_YTH				= MATTRIBUTE_TAOHUAGU_YTH,		//�һ���[���һ�]
			ID_EMEI_CEM					= MATTRIBUTE_EMEI_CEM,			//������[�̶�ü]
			ID_EMEI_FEM					= MATTRIBUTE_EMEI_FEM,			//������[����ü]
			ID_WUMENPAI					= MATTRIBUTE_WUMENPAI,			//�����ɣ������֡�����
			NUMBER_OF_MENPAI

		} MenPaiID_T;
		enum
		{
			ID= INVALID_ID,
		};
		
			MenPai_T(){};
			~MenPai_T(){};
			virtual MenPaiID_T GetID(VOID) const{return ID_INVALID;};
			//MaxHP
			SHORT GetInitMaxHP(VOID) const;
			SHORT GetMaxHPConRefix(VOID) const;
			SHORT GetMaxHPLevelRefix(VOID) const;
			//HP Regenerate
			SHORT GetInitHPRegenerate(VOID) const;
			SHORT GetHPRegenerateLevelRefix(VOID) const;
			//MaxMP
			SHORT GetInitMaxMP(VOID) const;
			SHORT GetMaxMPIntRefix(VOID) const;
			SHORT GetMaxMPLevelRefix(VOID) const;
			//MP Regenerate
			SHORT GetInitMPRegenerate(VOID) const;
			SHORT GetMPRegenerateLevelRefix(VOID) const;

			//MaxPneuma
			SHORT GetInitMaxPneuma(VOID) const;
			SHORT GetMaxPneumaLevelRefix(VOID) const;
			//Pneuma Regenerate
			SHORT GetInitPneumaRegenerate(VOID) const;
			SHORT GetPneumaRegenerateLevelRefix(VOID) const;

			//MaxRage
			SHORT GetInitMaxRage(VOID) const;
			SHORT GetMaxRageIntRefix(VOID) const;
			SHORT GetMaxRageLevelRefix(VOID) const;
			//Rage Regenerate
			SHORT GetInitRageRegenerate(VOID) const;
			SHORT GetRageRegenerateLevelRefix(VOID) const;

			//Attr Level 1 Sstr
			SHORT GetBaseStrLevelupRefix(VOID) const;
			SHORT GetStrLevelupRefix(SHORT const nLevel) const;

			//Attr Level 1 Spr
			/*SHORT	GetInitSpr(VOID) const;
			SHORT GetSprLevelupRefix(SHORT const nLevel) const;*/
			//Attr Level 1 Con
			SHORT GetInitCon(VOID) const;
			SHORT GetBaseConLevelupRefix(VOID) const;
			SHORT GetConLevelupRefix(SHORT const nLevel) const;
			//Attr Level 1 Int
			SHORT GetInitInt(VOID) const;
			SHORT GetBaseIntLevelupRefix() const;
			SHORT GetIntLevelupRefix(SHORT const nLevel) const;
			SHORT GetIntToughness() const;
			//Attr Level 1 Dex
			SHORT GetInitDex(VOID) const;
			SHORT GetBaseDexLevelupRefix(VOID) const;
			SHORT GetDexLevelupRefix(SHORT const nLevel) const;

			//Attr Level 2 Attack Physics
			//SHORT GetInitAttackPhysics(VOID) const;
			//SHORT GetAttackPhysicsStrRefix(VOID) const;
			//SHORT GetAttackPhysicsLevelRefix(VOID) const;
			////Attr Level 2 Defence Physics
			//SHORT GetInitDefencePhysics(VOID) const;
			//SHORT GetDefencePhysicsLevelRefix(VOID) const;

			//add by liam---------------------------------------
			//���̷���
			SHORT GetInitNearDefencePhysics(VOID) const;    //���̷���
			SHORT GetNearDefencePhysicsLevelRefix(VOID) const;   //�ȼ��Խ��̷�����Ӱ��ϵ��
			//Զ�̷���
			SHORT GetInitFarDefencePhysics(VOID) const;  //Զ�̷���
			SHORT GetFarDefencePhysicsLevelRefix(VOID) const; //�ȼ���Զ�̷���Ӱ��
			//���̹���
			SHORT GetInitNearAttackPhysics(VOID) const;    //���̹���
			SHORT GetNearAttackPhysicsIntRefix(VOID) const;	    //�ڹ���Զ���ڹ�������Ӱ��ϵ��
			SHORT GetNearAttackPhysicsLevelRefix(VOID) const;   //�ȼ��Խ��̹�����Ӱ��ϵ��
			//Զ�̹���
			SHORT GetInitFarAttackPhysics(VOID) const;  //����Զ���﹥
			SHORT GetFarAttackPhysicsStrRefix(VOID) const; //������Զ���⹦��Ӱ��
			SHORT GetFarAttackPhysicsLevelRefix(VOID) const; //�ȼ���Զ�̹���Ӱ��

			SHORT GetInitToughness(VOID) const; //��ʼ����   
			SHORT GetToughnessLevelRefix(VOID) const; //�ȼ������Ե�Ӱ��ϵ��

			SHORT GetInitAttackGold(VOID) const;    //��ʼ��ϵ������
			SHORT GetInitAttackWood(VOID) const;    //��ʼľϵ������
			SHORT GetInitAttackWater(VOID) const;   //��ʼˮϵ������
			SHORT GetInitAttackFire(VOID) const;    //��ʼ��ϵ������
			SHORT GetInitAttackSoil(VOID) const;    //��ʼ��ϵ������

			SHORT GetInitDefenceGold(VOID) const;    //��ʼ��ϵ������
			SHORT GetInitDefenceWood(VOID) const;    //��ʼľϵ������
			SHORT GetInitDefenceWater(VOID) const;   //��ʼˮϵ������
			SHORT GetInitDefenceFire(VOID) const;    //��ʼ��ϵ������
			SHORT GetInitDefenceSoil(VOID) const;    //��ʼ��ϵ������

			SHORT GetMaxVigorLevelupPoint(VOID) const;
			SHORT GetLevelRestoreVigor(VOID) const;

			SHORT GetLevelAttackGold(VOID) const;    
			SHORT GetLevelAttackWood(VOID) const;    
			SHORT GetLevelAttackWater(VOID) const;   
			SHORT GetLevelAttackFire(VOID) const;    
			SHORT GetLevelAttackSoil(VOID) const;    

			SHORT GetLevelDefenceGold(VOID) const;   
			SHORT GetLevelDefenceWood(VOID) const;   
			SHORT GetLevelDefenceWater(VOID) const;  
			SHORT GetLevelDefenceFire(VOID) const;   
			SHORT GetLevelDefenceSoil(VOID) const;   

			SHORT GetRestoreRageFromEXP(VOID) const;
			SHORT GetBaseVigorRestore(VOID) const;

			//-----------------------------------------------------

			//Attr Level 2 
			SHORT GetInitAttackMagicNear(VOID) const;
			SHORT GetInitAttackMagicFar(VOID) const;

			SHORT GetAttackMagicNearIniRefix(VOID) const;
			SHORT GetAttackMagicFarIniRefix(VOID) const;

			SHORT GetAttackMagicNearLevelRefix(VOID) const;
			SHORT GetAttackMagicFarLevelRefix(VOID) const;

			//Attr Level 2 �����ڹ�����
			SHORT GetInitDefenceMagicNear(VOID) const;
			SHORT GetInitDefenceMagicFar(VOID) const;

			SHORT GetDefenceMagicNearLevelRefix(VOID) const;
			SHORT GetDefenceMagicFarLevelRefix(VOID) const;

			//Attr Level 2 Hit
			SHORT GetInitHit(VOID) const;
			SHORT GetHitStrRefix(VOID) const;
			SHORT GetHitLevelRefix(VOID) const;
			//Attr Level 2 Critical
			SHORT GetInitCritical(VOID) const;
			SHORT GetCriticalIntRefix(VOID) const;
			SHORT GetCriticalDexRefix(VOID) const;
			SHORT GetCriticalLevelRefix(VOID) const;
			//Attr Level 2 Miss
			SHORT GetInitMiss(VOID) const;
			SHORT GetMissDexRefix(VOID) const;
			SHORT GetMissLevelRefix(VOID) const;
			//Attr Attack Fluctuation
			SHORT GetAttackFluctuation(VOID) const;
			// Need more menpai special Refix functions
			//event handler
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const;
			virtual VOID OnDamageTarget(Obj_Human& rMe, Obj_Character& rTar, INT& nDamage) const;
			virtual VOID OnMyTrapActivated(Obj_Human& rMe, Obj_Special& rTrap) const {};
			virtual VOID OnHealTarget(Obj_Human& rMe, INT nHealedHp) const {};
			virtual VOID OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const {};
	protected:
	
	private:
			//Rage
			virtual INT TransferValueToRage(INT nDamage) const {return 0;};
			//Strike 
	};
};
#endif //MENPAI_H
