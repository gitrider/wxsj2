//***************************************************************************************************
// 文件名称:	MenPai.h
// 系统名称:	
// 模块名称:	MenPai
// 功能说明:	所有门派的接口声明
//
// 相关文档:	
// 作    者:	liam
// 建立日期:	
// 审    核:	
// 软件版权:	
//
//修改记录:
//          1.
//				修 改 人:liam
//				修改时间:2007/10/30
//				修改内容:属性第一部分重构(具体参见文档)
//          2.
//				修 改 人:liam
//				修改时间:2007/11/02
//				修改内容:属性第二部分重构(具体参见文档)
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
			ID_INVALID					= -1,							//无效门派ID, 初始化用和判断非法用
			ID_SHAOLIN_GWS				= MATTRIBUTE_SHAOLIN_GWS,		//少林派[棍武僧]
			ID_SHAOLIN_TLH				= MATTRIBUTE_SHAOLIN_TLH,		//少林派[铁罗汉]
			ID_GAIBANG_GGB				= MATTRIBUTE_GAIBANG_GGB,		//丐帮[棍丐帮]
			ID_GAIBANG_ZGB				= MATTRIBUTE_GAIBANG_ZGB,		//丐帮[掌丐帮]
			ID_WUDANG_JWD				= MATTRIBUTE_WUDANG_JWD,		//武当派[剑武当]
			ID_WUDANG_QWD				= MATTRIBUTE_WUDANG_QWD,		//武当派[气武当]
			ID_TANGMEN_DT				= MATTRIBUTE_TANGMEN_DT,		//唐门[刀唐]
			ID_TANGMEN_XJT				= MATTRIBUTE_TANGMEN_XJT,		//唐门[陷阱唐]
			ID_XIAOYAO_XXY				= MATTRIBUTE_XIAOYAO_XXY,		//逍遥派[扇逍遥]
			ID_XIAOYAO_ZXY				= MATTRIBUTE_XIAOYAO_ZXY,		//逍遥派[指逍遥]
			ID_YAOWANGGU_KYW			= MATTRIBUTE_YAOWANGGU_KYW,		//药王谷[狂药王]
			ID_YAOWANGGU_YYW			= MATTRIBUTE_YAOWANGGU_YYW,		//药王谷[医药王]
			ID_TAOHUAGU_DTH				= MATTRIBUTE_TAOHUAGU_DTH,		//桃花岛[笛桃花]
			ID_TAOHUAGU_YTH				= MATTRIBUTE_TAOHUAGU_YTH,		//桃花岛[音桃花]
			ID_EMEI_CEM					= MATTRIBUTE_EMEI_CEM,			//峨嵋派[刺峨眉]
			ID_EMEI_FEM					= MATTRIBUTE_EMEI_FEM,			//峨嵋派[辅峨眉]
			ID_WUMENPAI					= MATTRIBUTE_WUMENPAI,			//无门派，如新手。。等
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
			//近程防御
			SHORT GetInitNearDefencePhysics(VOID) const;    //近程防御
			SHORT GetNearDefencePhysicsLevelRefix(VOID) const;   //等级对近程防御的影响系数
			//远程防御
			SHORT GetInitFarDefencePhysics(VOID) const;  //远程防御
			SHORT GetFarDefencePhysicsLevelRefix(VOID) const; //等级对远程防御影响
			//近程攻击
			SHORT GetInitNearAttackPhysics(VOID) const;    //近程攻击
			SHORT GetNearAttackPhysicsIntRefix(VOID) const;	    //内功对远程内功攻击的影响系数
			SHORT GetNearAttackPhysicsLevelRefix(VOID) const;   //等级对近程攻击的影响系数
			//远程攻击
			SHORT GetInitFarAttackPhysics(VOID) const;  //基础远程物攻
			SHORT GetFarAttackPhysicsStrRefix(VOID) const; //力道对远攻外功的影响
			SHORT GetFarAttackPhysicsLevelRefix(VOID) const; //等级对远程攻击影响

			SHORT GetInitToughness(VOID) const; //初始韧性   
			SHORT GetToughnessLevelRefix(VOID) const; //等级对韧性的影响系数

			SHORT GetInitAttackGold(VOID) const;    //初始金系攻击力
			SHORT GetInitAttackWood(VOID) const;    //初始木系攻击力
			SHORT GetInitAttackWater(VOID) const;   //初始水系攻击力
			SHORT GetInitAttackFire(VOID) const;    //初始火系攻击力
			SHORT GetInitAttackSoil(VOID) const;    //初始土系攻击力

			SHORT GetInitDefenceGold(VOID) const;    //初始金系防御力
			SHORT GetInitDefenceWood(VOID) const;    //初始木系防御力
			SHORT GetInitDefenceWater(VOID) const;   //初始水系防御力
			SHORT GetInitDefenceFire(VOID) const;    //初始火系防御力
			SHORT GetInitDefenceSoil(VOID) const;    //初始土系防御力

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

			//Attr Level 2 近程内功防御
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
