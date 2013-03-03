#pragma once

#include "../DataPool/GMDP_Struct_Pet.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CObject_Character;

namespace SCRIPT_SANDBOX
{
	class Mount
	{
	//LuaPlus界面相关接口
	public:
		// 查询 骑宠名字
		INT		GetMountName(LuaPlus::LuaState* state);

		// 查询 骑宠昵称
		INT		GetMountNickName(LuaPlus::LuaState* state);

		// 查询 骑宠级别
		INT		GetMountLevel(LuaPlus::LuaState* state);

		// 查询 骑宠性别
		INT		GetMountGender(LuaPlus::LuaState* state);

		// 查询 骑宠代数
		INT		GetMountEraCount(LuaPlus::LuaState* state);

		// 查询 骑宠是否可交配
		INT		IsCanBeMating(LuaPlus::LuaState* state);

		// 查询 骑宠ID
		INT		GetMountGUID(LuaPlus::LuaState* state);

		// 查询 骑宠配偶ID
		INT		GetSpouseID(LuaPlus::LuaState* state);

		// 查询 骑宠列表
		INT		GetMountList(LuaPlus::LuaState* state);

		// 查询 骑宠快乐值
		INT		GetMountHappiness(LuaPlus::LuaState* state);

		// 查询 骑宠成长率
		INT		GetMountGrowRate(LuaPlus::LuaState* state);

		// 查询 骑宠根骨
		INT		GetMountBasic(LuaPlus::LuaState* state);

		// 查询 骑宠悟性
		INT		GetMountSavvy(LuaPlus::LuaState* state);

		// 查询 骑宠力量
		INT		GetMountStr(LuaPlus::LuaState* state);

		// 查询 骑宠敏捷
		INT		GetMountDex(LuaPlus::LuaState* state);

		// 查询 骑宠体力
		INT		GetMountCon(LuaPlus::LuaState* state);

		// 查询 骑宠智力
		INT		GetMountInt(LuaPlus::LuaState* state);

		// 查询 骑宠潜能
		INT		GetMountPotential(LuaPlus::LuaState* state);

		// 查询 骑宠力量资质
		INT		GetMountStrApt(LuaPlus::LuaState* state);

		// 查询 骑宠敏捷资质
		INT		GetMountDexApt(LuaPlus::LuaState* state);

		// 查询 骑宠体力资质
		INT		GetMountConApt(LuaPlus::LuaState* state);

		// 查询 骑宠智力资质
		INT		GetMountIntApt(LuaPlus::LuaState* state);		

		// 查询 骑宠UI模型名称
		INT		GetUIModelName(LuaPlus::LuaState* state);
		
		// 查询 坐骑UI模型名称
	    INT     GetMountUIModelName(LuaPlus::LuaState* state);

		// 查询 骑宠技能
		INT		GetMountSkill(LuaPlus::LuaState* state);

		// 查询 坐骑Item的Action
    	INT     GetMountAction(LuaPlus::LuaState* state);

		// 加点确认
		INT		SetAttrAddRequest(LuaPlus::LuaState* state);

		// 是否骑乘中
		INT		IsMounting(LuaPlus::LuaState* state);

		// 上马
		INT		Mounting(LuaPlus::LuaState* state);
        
		// 下马
		INT		Dismount(LuaPlus::LuaState* state);

		// 放生
		INT		FreeMount(LuaPlus::LuaState* state);

		// 召唤坐骑  [7/14/2010 陈军龙]
		INT     CallMount(LuaPlus::LuaState* state);

	    // 召回坐骑  [7/14/2010 陈军龙]
	    INT     ReCallMount(LuaPlus::LuaState* state);

		// 丢弃坐骑  [7/14/2010 陈军龙]
	    INT     DiscardMount(LuaPlus::LuaState* state);

		// 是否可繁殖
		INT		CanMating(LuaPlus::LuaState* state);

		// 查询 加锁状态
		INT		IsLocked(LuaPlus::LuaState* state);
		



		// 目标骑宠名字
		INT		GetTargetMountName(LuaPlus::LuaState* state);

		// 目标骑宠的昵称
		INT		GetTargetMountNickName(LuaPlus::LuaState* state);

		// 目标骑宠级别
		INT		GetTargetMountLevel(LuaPlus::LuaState* state);

		// 目标骑宠性别
		INT		GetTargetMountGender(LuaPlus::LuaState* state);

		// 目标骑宠ID
		INT		GetTargetMountID(LuaPlus::LuaState* state);

		// 目标配偶ID
		INT		GetTargetSpouseID(LuaPlus::LuaState* state);

		// 目标力量
		INT		GetTargetMountStr(LuaPlus::LuaState* state);

		// 目标体力
		INT		GetTargetMountCon(LuaPlus::LuaState* state);

		// 目标敏捷
		INT		GetTargetMountDex(LuaPlus::LuaState* state);

		// 目标智力
		INT		GetTargetMountInt(LuaPlus::LuaState* state);

		// 目标力量资质
		INT		GetTargetMountStrApt(LuaPlus::LuaState* state);

		// 目标敏捷资质
		INT		GetTargetMountDexApt(LuaPlus::LuaState* state);

		// 目标体力资质
		INT		GetTargetMountConApt(LuaPlus::LuaState* state);

		// 目标智力资质
		INT		GetTargetMountIntApt(LuaPlus::LuaState* state);

		// 目标根骨
		INT		GetTargetMountBasic(LuaPlus::LuaState* state);

		// 目标悟性
		INT		GetTargetMountSavvy(LuaPlus::LuaState* state);

		// 目标成长率
		INT		GetTargetMountGrowRate(LuaPlus::LuaState* state);

		// 目标UI模型名
		INT		GetTargetUIModelName(LuaPlus::LuaState* state);

		// 请求目标骑宠属性
		INT		TargetMountAttrRequest(LuaPlus::LuaState* state);

		// 目标技能
		INT		GetTargetMountSkill(LuaPlus::LuaState* state);




		// 得到 当前出战的坐骑
		INT		GetBattleMountIndex(LuaPlus::LuaState* state);
        
		// 得到 当前UI选择的骑宠
		INT		GetSelectMount(LuaPlus::LuaState* state);

		// 设置 当前UI选择的骑宠
		INT		SetSelectMount(LuaPlus::LuaState* state);

		// 解除物品锁定状态
		INT		ItemUnLock(LuaPlus::LuaState* state);

		// 使用物品
		INT		UseMountItem(LuaPlus::LuaState* state);

		// 申请改变坐骑的名字
		INT		ChangeMountName(LuaPlus::LuaState* state);

		// 鉴定骑宠成长率
		INT		RequestIdentifyMount(LuaPlus::LuaState* state);

		// 发送交配锁定或解锁消息
		INT		RequestMyMountMatingLocked(LuaPlus::LuaState* state);

		// 发送打开繁殖界面消息
		INT		RequestTogleMatingPage(LuaPlus::LuaState* state);

		// 发送确认或取消交配消息
		INT		RequestMating(LuaPlus::LuaState* state);

		// 发送领取繁殖坐骑消息
		INT		RequestGainMatingMount(LuaPlus::LuaState* state);



	protected:
		// 当前UI列表中选择的骑宠
		PET_GUID_t		m_nSelectMount;
		

	public:
		Mount(void){}
		~Mount(void){}

		static Mount s_Mount;
		static LuaPlus::LuaObject* s_pMetaTable;

		
	
	};
}

