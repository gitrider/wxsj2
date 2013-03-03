
/** Tooltips接口
 */


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}
class CActionItem;

//
//脚本函数沙箱，这些函数应该由脚本调用
//
namespace SCRIPT_SANDBOX
{

class SuperTooltips
{
public:

	//查询Tooltip的Title
	//
	// 传入参数 
	//   
	// 传出参数
	//   0[String] - Title
	//
	INT		GetTitle(LuaPlus::LuaState* state);

	//查询Tooltip的Icon
	//
	// 传入参数 
	//   
	// 传出参数
	//   0[String] - Image(set:imageset image:imagename)
	//
	INT		GetIcon(LuaPlus::LuaState* state);

	//查询Tooltip的详细逻辑属性
	//
	// 传入参数 
	//   0[String] - AttributeName
	//   
	// 传出参数
	//   0[String] - AttributeValue
	//
	INT		GetAttributeValue(LuaPlus::LuaState* state);

	//查询Tooltip的描述说明
	//
	//
	//   
	// 传出参数
	//   0[String] - 描述字符串
	//
	INT		GetItemDesc(LuaPlus::LuaState* state);

	//查询Tooltip的物品等级
	//
	//
	//   
	// 传出参数
	//   0[int] - -1: 这个物品没有这个属性
	//
	INT		GetItemLevel(LuaPlus::LuaState* state);

	//查询Tooltip的物品耐久
	//
	//
	//   
	// 传出参数两个
	//   0[int] - 当前耐久 -1: 这个物品没有这个属性
	//   1[int] - 最大耐久 -1: 这个物品没有这个属性
	//
	INT		GetItemDur(LuaPlus::LuaState* state);

	//查询Tooltip的物品的修理次数
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 修理次数 -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemRepairCount(LuaPlus::LuaState* state);

	//查询Tooltip的物品的绑定信息
	//
	//
	//   
	// 传出参数1个
	//   -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemBindInfo(LuaPlus::LuaState* state);


	//查询Tooltip的物品的绑定信息
	//
	//
	//   
	// 传出参数1个
	//   -1: 这个物品没有这个属性
	//   4表示2次绑定
	//

	//查询Tooltip的装备的2次绑定信息
	INT		GetItemSecLevelBindInfo(LuaPlus::LuaState* state);


	//查询Tooltip的物品的绑定加成信息
	//
	INT	 GetItemBindAddInfo(LuaPlus::LuaState* state);

	//查询装备的颜色

	INT  GetDropVisColor(LuaPlus::LuaState* state);

	//查询Tooltip的物品的卖给npc的价格
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 修理次数 -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemPrice(LuaPlus::LuaState* state);

	//查询Tooltip的物品的职业
	//
	//
	//   
	// 传出参数1个
	//   -1: 这个物品没有这个属性
	//   

	INT      GetItemProfession(LuaPlus::LuaState* state);



	//得到装备牌型
 
	INT      GetEquipCard(LuaPlus::LuaState* state);
	//查询Tooltip得到物品是否唯一
	//
	//
	//   
	// 传出参数1个
	//   0: 表示这个物品不唯一
	//   
	INT		GetItemRuleOnly(LuaPlus::LuaState* state);

	//查询Tooltip得到装备已有孔洞的数量
	//
	//
	//   
	// 传出参数1个
	//     表示这个装备的孔数


	//得到装备孔数量


	INT		GetEquipHoleNum(LuaPlus::LuaState* state);

	//得到装备套装信息

	INT		GetEquipSetInfo(LuaPlus::LuaState* state);

	//得到装备牌型激活的信息

	INT		GetEquipSpritePaiInfo(LuaPlus::LuaState* state);


	//得到其他玩家装备小激活的信息
	INT		GetOtherEquipSpriteChainInfo(LuaPlus::LuaState* state);


	//得到装备小激活的信息
	INT		 GetEquipSpriteChainInfo(LuaPlus::LuaState* state);


	//得到其他玩家装备大激活的信息

	INT		GetOtherEquipSpriteAllActiveInfo(LuaPlus::LuaState* state);

	//得到装备大激活的信息
	INT		GetEquipSpriteAllActiveInfo(LuaPlus::LuaState* state);

	//得到装备套装信息

	INT		GetOtherEquipSetInfo(LuaPlus::LuaState* state);


	//得到装备套装激活的属性信息

	INT		GetEquipSetActiveAttr(LuaPlus::LuaState* state);
	//得到装备套装激活的属性信息

	INT		GetOtherEquipSetActiveAttr(LuaPlus::LuaState* state);


   //得到装备星级

	INT		GetStartLevel(LuaPlus::LuaState* state);

	//查询Tooltip的物品的制造者
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 修理次数 -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemManufacturer(LuaPlus::LuaState* state);

	//查询Tooltip的物品的白色属性
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	// INT		GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state);



	//查询Tooltip的物品的基本属性
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//

    INT	GetEquipBaseAttrInfo(LuaPlus::LuaState* state);


	//查询Tooltip的物品的附加属性
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	INT		GetEquipAddAttrInfo(LuaPlus::LuaState* state);



	// 查询Tooltip的装备的技能属性    
	INT	SuperTooltips::GetEquipSkillInfo(LuaPlus::LuaState* state);

	//查询Tooltip的物品的类型
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	INT	GetItemType(LuaPlus::LuaState* state);


	//查询Tooltip的装备的品质(白, 绿, 蓝)
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 1 : 白
	//            2 : 绿
    //            3 : 蓝
	//INT	GetItemEquipQuantity(LuaPlus::LuaState* state); //remove


	//查询Tooltip的装备的类型属性
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 物品类型
	INT	GetItemClassType(LuaPlus::LuaState* state);


	

	//
	// 得到最大修理次数
	// 传出参数1个
	// 0[int] - 最大修理次数
	//INT	GetEquipMaxRepair(LuaPlus::LuaState* state);


	
	//查询Tooltip的装备的宝石信息
	//
	//
	//   
	// 传出参数15个
	//   0[int] - 宝石个数
	//   1[str] - 宝石1的图标字符串 "" - 表示无图标
	//	 2[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 3[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 4[str] - 宝石附加属性的描述
	INT	GetGemInEquipInfo(LuaPlus::LuaState* state);

	INT GetNeedGemInfo(LuaPlus::LuaState* state);
	


	//----------------------------------------------------------------------------------------------------------------------
	// 心法函数
	//
	
	// 得到心法等级
	// 传出参数1个
	// 0[int] - 当前学会的心法等级.
	INT	GetXinfaLevel(LuaPlus::LuaState* state);


	//----------------------------------------------------------------------------------------------------------------------
	// 技能函数
	//

	// 得到技能等级
	// 传出参数1个
	// 0[int] - 当前学会的技能等级.
	INT	GetSkillLevel(LuaPlus::LuaState* state);

	// 得到是否学会技能
	// 传出参数1个
	// 0[int] - 当前学会的技能等级.
	INT	GetIsLearnedSkill(LuaPlus::LuaState* state);

	// 得到技能描述
	// 传出参数1个
	// 0[str] - 得到技能描述
	INT	GetSkillDesc(LuaPlus::LuaState* state);

	// 得到技能对应的心法等级
	//
	// 传出参数1个
	// 0[int] - 心法等级
	INT	GetSkillXinfaLevel(LuaPlus::LuaState* state);

	INT	IsPassiveSkill(LuaPlus::LuaState* state);




	//----------------------------------------------------------------------------------------------------------------------
	//
	// 宝石信息
	//

	// 得到宝石等级
	// 传出参数1个
	// 0[int] - 宝石等级
	INT	GetGemLevel(LuaPlus::LuaState* state);

	// 得到宝石属性
	// 传出参数1个
	// 0[str] - 宝石属性
	INT	GetGemArribInfo(LuaPlus::LuaState* state);



	//----------------------------------------------------------------------------------------------------------------------
	//
	// 魂珠信息
	//

	//得到魂珠的属性类型
	INT	GetFecthHeadInfo_TypeDesc( LuaPlus::LuaState* state );

	//得到魂珠的属性值
	INT	GetFecthHeadInfo_Value( LuaPlus::LuaState* state );

	//得到魂珠的属性数量
	INT	GetFecthHeadInfo_Num( LuaPlus::LuaState* state );


	//----------------------------------------------------------------------------------------------------------------------
	//
	// 得到物品的quality
	//

	// 传出参数1个
	// 0[int] - 药品或宝石品质
	INT	GetItemQuality(LuaPlus::LuaState* state);

	//得到装备的分类 普通装备还是高级装备
	 INT GetItemEquipQua(LuaPlus::LuaState* state);


	// 得到物品在表中的类型
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	GetItemTableType(LuaPlus::LuaState* state);


	
	// 得到配方需要的技能名字。
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	GetPrescrSkillName(LuaPlus::LuaState* state);

	// 得到配方需要的技能等级。
	// 传出参数1个
	// 0[int] - 表中的等级。
	INT	GetPrescrNeedSkillLevel(LuaPlus::LuaState* state);


	// 得到配方需要技能的当前等级。
	// 传出参数1个
	// 0[int] - 配方需要的当前等级。
	INT	GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state);


	// 向服务器请求item的详细信息
	// 
	// 
	INT	SendAskItemInfoMsg(LuaPlus::LuaState* state);

 
	// 得到装备是否鉴定
 
	 INT	IsEquipIdentify(LuaPlus::LuaState* state);

	//
	// npc商店是否打开。
	// 
	INT	IsNpcShopOpen(LuaPlus::LuaState* state);

	//是否存在
	INT IsPresent(LuaPlus::LuaState* state);

	//得到描述信息
	INT GetTypeDesc(LuaPlus::LuaState* state);

	//得到物品是否在背包中
	INT IsItemInPacket(LuaPlus::LuaState* state);
	INT GetItemIsOnStall(LuaPlus::LuaState* state);
	INT GetItemPriceOnStall(LuaPlus::LuaState* state);

	INT GetSkillMaxLevel(LuaPlus::LuaState* state);

	INT GetSkillType(LuaPlus::LuaState* state);

	INT GetSkillCoolDown(LuaPlus::LuaState* state);

	INT GetSkillAttackRange(LuaPlus::LuaState* state);

	INT GetSkillGatherTime(LuaPlus::LuaState* state);

	INT GetSkillLeadTime(LuaPlus::LuaState* state);

	INT GetNextLvSkillDesc(LuaPlus::LuaState* state);


	INT GetNextLvSkillGatherTime(LuaPlus::LuaState* state);

	INT GetNextLvSkillLeadTime(LuaPlus::LuaState* state);

	INT GetSkillLvUpLevelQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpPointQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpFahterSkillQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpMoneyQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpXpQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpItemQua(LuaPlus::LuaState* state);

	//得到是否是搜侠物品
	INT IsSouXiaItem( LuaPlus::LuaState* state );
	
	//得到坐骑增加速度          [7/21/2010 陈军龙]
	INT	GetMountAddtionSpeed(LuaPlus::LuaState* state);

	// 得到坐骑存在时限         [7/21/2010 陈军龙]
	INT	GetMountLife(LuaPlus::LuaState* state);

	// 得到坐骑获取时间         [7/21/2010 陈军龙]
	INT	GetMountObtainDate(LuaPlus::LuaState* state);

	// 得到坐骑剩余使用时间     [7/21/2010 陈军龙]
	INT	GetMountRemainTime(LuaPlus::LuaState* state);

	// 得到坐骑当前是否已绑定   [7/21/2010 陈军龙]
	INT	GetMountIsBinded(LuaPlus::LuaState* state);

	// 得到坐骑绑定类型         [7/21/2010 陈军龙]
	BOOL GetMountBindType(LuaPlus::LuaState* state);

public:
	//由游戏逻辑设置激活中的ActionItem
	VOID	SetActionItem(CActionItem* pActionitem);
	CActionItem* GetActionItem(void) { return m_pActionItem; }

protected:

	INT			 m_nActionId;
	CActionItem* m_pActionItem;

public:
	SuperTooltips();
	virtual ~SuperTooltips();

	static SuperTooltips s_SupperTooltip;
	static SuperTooltips s_SupperTooltip2;
	static LuaPlus::LuaObject* s_pMetaTable;
};




}