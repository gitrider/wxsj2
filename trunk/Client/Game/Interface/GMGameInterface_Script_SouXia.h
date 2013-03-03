/*
	搜侠录相关接口
*/

#pragma once


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

struct SOUXIA_DATA;
class CObject_Item;
class CObject_PlayerOther;

namespace SCRIPT_SANDBOX
{
	class SouXiaLu
	{
	public:
		SouXiaLu(){};
		virtual ~SouXiaLu(){};
		
		static SouXiaLu s_SouXiaLu;
		static LuaPlus::LuaObject* s_pMetaTable;

	public:

		//获得身上是否装备着搜侠录
		INT Lua_IsEquipSoiuXia( LuaPlus::LuaState* state );

		//	获得背包内指定类型的搜侠录记忆棒道具的数目
		INT Lua_GetSouXiaUsbNum( LuaPlus::LuaState* state );

		// 获得当前页面，USB记忆棒道具显示栏内，指定位置应该显示的物品 (所需要参数，显示栏第几页<INT>, 显示栏内第几个位置<INT> )
		INT Lua_GetSouXiaUsbItem( LuaPlus::LuaState* state );

		// 获得装备的搜侠录当前打开的类型的总的页数
		INT Lua_GetSouXiaPageNum_Type( LuaPlus::LuaState* state );

		// 获得当前打开的搜侠录道具的品质（1-4 分别代表类型的数量）
		INT Lua_GetSouXiaLevel( LuaPlus::LuaState* state );

		//	获得当前装备的搜侠录道具的额外参数（即，搜侠索引）
		INT Lua_GetSouXiaIndex( LuaPlus::LuaState* state );

		// 获得当前装备的搜侠录道具在当前打开的界面类型中已经学习的技能页数
		INT Lua_GetSouXiaPageNum( LuaPlus::LuaState* state );

		// 学习USB记忆棒的内容
		INT Lua_StudyByUSBItem( LuaPlus::LuaState* state );

		// 获得搜侠录道具已学列表中，第X页第Y个的技能的ID
		INT Lua_GetSouXiaSkillID( LuaPlus::LuaState* state );

		// 根据技能ID获得其ActionID
		INT Lua_GetSouXiaActionIDBySkillID( LuaPlus::LuaState* state );

		// 根据技能ID获得该技能的名字
		INT Lua_GetSouXiaSkillName( LuaPlus::LuaState* state );

		// 获得搜侠录道具已学列表中，第X个配方的配方ID
		INT Lua_GetSouXiaArmID( LuaPlus::LuaState* state );

		// 根据配方ID，获得合成后的武器的UI显示图标
		INT Lua_GetSouXiaArmIcon( LuaPlus::LuaState* state );

		// 根据配方ID，获得合成后的武器的ActionID， 名字， 说明
		INT Lua_GetSouXiaArmData( LuaPlus::LuaState* state );

		// 获取搜侠录已学列表中，第X个配方所需的材料
		INT Lua_GetSouXiaArmMatData( LuaPlus::LuaState* state );

		// 检索玩家背包特定位置的USB记忆棒道具内存储的具体的信息值（技能，配方产品，神兽，坐骑的ID）
		INT Lua_GetSouXia_Data( LuaPlus::LuaState* state );

		// 设置预览某个技能的效果
		INT Lua_SetPrewSkill( LuaPlus::LuaState* state );

		// 传入武器ID，显示武器外形的预览
		INT Lua_SetPrewArm( LuaPlus::LuaState* state );

		// 设置预览某个神兽
		INT Lua_SetPrewPet( LuaPlus::LuaState* state );

		//设置预览某个坐骑
		INT Lua_SetPrewZuoJi( LuaPlus::LuaState* state );

		// 清除武器外形的预览
		INT Lua_ClearPrewArm( LuaPlus::LuaState* state );

		// 初始化模型的Avatra
		INT Lua_InitModelAvart( LuaPlus::LuaState* state );

		// 点击武器配方界面的 “生产”按钮
		INT Lua_SouXiaLu_ArmMake( LuaPlus::LuaState* state );

		// 判断背包内某个位置上的搜侠录道具是否是空白搜侠录
		INT Lua_IsBlankSouXiaItem( LuaPlus::LuaState* state );

		//根据ActionID获得其中保存的技能ID
		INT Lua_GetSkillIdByActionID( LuaPlus::LuaState* state );

		//根据召唤技能ID获得将要召唤的物体的ModelID
		INT Lua_GetModelIDbySkillID( LuaPlus::LuaState* state );


		//搜侠录拆卸功能
		INT Lua_SouXiaLu_Dismantle_Accept( LuaPlus::LuaState* state );

		//搜侠录销毁功能
		INT Lua_SouXiaLu_Destory_Accept( LuaPlus::LuaState* state );

		//搜侠录炼化功能
		INT Lua_SouXiaLu_Complex_Accept( LuaPlus::LuaState* state );

		//搜侠录缝合功能
		INT Lua_SouXiaLu_Sew_Accept( LuaPlus::LuaState* state );

		INT Lua_SouXia_SetLock( LuaPlus::LuaState* state );
		
		//清除掉技能快捷栏中的搜侠技能
		INT Lua_Clear_SouXiaSkill( LuaPlus::LuaState* state );

		//设置快捷栏上搜侠技能可使用的次数
		INT Lua_SetMainMenuBarSouXiaSkillNum( LuaPlus::LuaState* state );

		//获得当前对话的NPC的ID
		INT Lua_SouXia_NpcId( LuaPlus::LuaState* state );

	public:
		
		//获得身上是否装备有搜侠录
		BOOL IsEquipSouXia();

		//判断一个道具是否是指定类型的记忆棒道具( 0.全部  1.技能  2.武器  3.神兽  4.坐骑 )
		BOOL IsSouXiaUsbItem( INT type, INT pos );
		BOOL IsSouXiaUsbItem( INT type, tObject_Item* pItem );

		//判断一个道具是否是搜侠录相关物品( type == 0.搜侠录道具  1.USB记忆棒  2.拆解道具  3.炼化道具  4.缝合道具)
		BOOL IsSouXiaItem( INT pos, INT type);

		//判断背包内的一个道具是否是空白的搜侠录道具( return true:是空白  false:不是空白 )
		BOOL IsBlankSouXiaItem( INT pos );

		//判断一个道具是否是搜侠录残页
		BOOL IsSouXiadeformityItem( INT pos );
		BOOL IsSouXiadeformityItem( tObject_Item* pItem );

		//判断一个搜侠录残页是否是空白残页
		BOOL IsBlankSouXiadeformity( INT pos );
		BOOL IsBlankSouXiadeformity( tObject_Item* pItem );

		//得到搜侠录残页的类型（最好和 IsSouXiadeformityItem 函数配合使用）
		INT GetSouXiadeformityType( INT pos );
		INT GetSouXiadeformityType( tObject_Item* pItem );

		//得到搜侠录残页的详细数据
		SOUXIA_DATA* GetSouXiadeformityData( INT pos );
		SOUXIA_DATA* GetSouXiadeformityData( tObject_Item* pItem );

		//判断一个技能是否是搜侠技能
		BOOL IsSouXiaSkill( SkillID_t nSkillID );

		//获得装备框搜侠录位置内存放的搜侠录
		tObject_Item* GetHEquipSouxia();

		//获得当前装备的搜侠录道具的详细数据
		SOUXIA_DATA* GetHEquipSouxiaInfo();

		//获得搜侠录记忆棒道具的类型（return 1,2,3,4 ---- 1.技能  2.武器  3.神兽  4.坐骑 ）
		INT GetSouXiaUsbType( tObject_Item* pItem );

		//根据材料的类型和等级，计算出材料的ID索引
		INT GetMatIndex( int type, int level);	

		// 根据TableID，计算出玩家背包中有多少此物品
		INT GetItemNum_ByIdTable( INT IdTable );

		// 检索玩家背包特定位置的USB记忆棒道具内存储的具体的信息值（技能，配方，神兽，坐骑的ID）
		INT GetSouXiaUsb_Data( INT bagIndex );

		//	获得背包等容器内指定位置物品的actionID
		INT GetSouXiaEnumActionId( INT nPosIndex, LPCTSTR szFilter );

		//设置物品锁定
		VOID SetLockSouXiaItem( INT pos, BOOL lock );

		//更新快捷栏上指定搜侠技能ID的次数
		VOID UpdateMainMenuBarSouxiaSkillIDNum( INT SkillId, INT num );

		//根据ActionID获得技能ID
		INT GetSkillIdByActionID( INT nActionId );

		//获得当前装备的搜侠录中指定技能的总的使用次数
		INT GetSouXiaSkillNum( INT SkillId );

		//根据召唤技能ID获得将要召唤的物体的ModelID
		INT GetModelIDbySkillID( INT SkillID );

		//设置快捷栏上搜侠技能可使用的次数
		VOID SetMainMenuBarSouXiaSkillNum( INT ActionId, INT Index );

		//显示搜侠残页界面
		VOID ShowSouXiaDeformity( tObject_Item* pItem );

		std::vector<INT>	m_vPageUsbID;	//存储， 背包中所有符合当前条件的USB记忆棒道具在背包中的位置
		std::map<INT, CObject_Item*> m_ProItem;			//保存搜侠中用到的物品，每个物品编号只创建一个物品

	};
}