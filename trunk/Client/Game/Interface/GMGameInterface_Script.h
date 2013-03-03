
/**	GMGameInterface_Script.h
/**	
 *
 *	功能：	
 *
 *	修改记录：
 *			080306	Lua_ShowGuildInfoDlg
 *			
 *			080314	Friend::IsPlayerIsFriend
 *					Friend::SetCurrentSelect
 *					增加 FriendSearcher 类， 用于实现好友的搜索
 *			080317	ThrowToEnemyList
 *					OpenSearcher 打开搜索界面
 *			080317	NpcShop::GetItemPrice
 *			080317	NpcShop::NpcShopBuy
 *			080317	NpcShop::NpcShopSell
 *			080403	DataPool中添加主界面系统提示
 *			080409  Lua_OpenEquipAll     wz
 *			080410	修改OpenMail, 增加OpenMailBrowser, OpenMailEdit
 *			
 *			080404	增加 DaytipsData 类， 用于实现每日功能提醒
 *			080414	增加 QuestLogData 类， 用于实现个人任务功能
 *			
 *			080404	增加 DaytipsData 类， 用于实现每日功能提醒
 *			080414	增加 QuestLogData 类， 用于实现个人任务功能
 *			080424	增加 QuickChange 类， 用于实现一键换装
 */

#pragma once

#include <time.h>
#include "GIDBC_Struct.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\GMDP_Struct_Daytips.h"
#include "DataPool\GMDP_Struct_QuestLog.h"


#include "Type.h"


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CObject_Character;
class tActionItem;
class CObject_Item;

namespace SCRIPT_SANDBOX
{

	//======$
	INT Lua_ClickTest(LuaPlus::LuaState* state);


	//登陆界面中，用于点击确定后的操作(-- for debug)
	INT	Lua_ServerLogin(LuaPlus::LuaState* state);

	//要求退出程序
	INT Lua_QuitApplication(LuaPlus::LuaState* state);

	//要求打开/关闭技能书
	INT	Lua_ToggleSkillBook(LuaPlus::LuaState* state);
	//要求打开技能书
	INT Lua_OpenSkillBook(LuaPlus::LuaState* state);

	//要求打开/关闭宠物界面
	INT	Lua_TogglePetPage(LuaPlus::LuaState* state);

	//要求打开生活技能界面
	INT Lua_OpenLifePage(LuaPlus::LuaState* state);

	//要求打开普通技能界面
	INT Lua_OpenCommonSkillPage(LuaPlus::LuaState* state);
	//打开包裹
	INT Lua_ToggleContainer(LuaPlus::LuaState* state);

	//打开任务
	INT Lua_ToggleMission(LuaPlus::LuaState* state);

	//关闭所有二级菜单
	INT Lua_CloseSecondMenu(LuaPlus::LuaState* state);

	//输出提示信息
	INT Lua_PushDebugMessage(LuaPlus::LuaState* state);

	// 更新一键换装套装信息
	INT Lua_UpdateChangeSuit(LuaPlus::LuaState* pState);			
	//得到某类操作对象的数目
	INT	Lua_GetActionNum(LuaPlus::LuaState* state);

	//导出数据池内从服务器发送来的老师的门派信息
	//INT Lua_GetMenpaiInfo(LuaPlus::LuaState* state);

	//导出心法升级需要的金钱
	INT Lua_GetUplevelXinfaSpendMoney(LuaPlus::LuaState* state);

	//导出心法升级需要的经验值
	INT Lua_GetUplevelXinfaSpendExp(LuaPlus::LuaState* state);

	//导出当前心法等级
	INT Lua_GetXinfaLevel(LuaPlus::LuaState* state);

	// 获取当前时间
	INT Lua_GetCurrentTime( LuaPlus::LuaState* state);

	//获得当前的鼠标位置（屏幕坐标）
	INT Lua_GetCurMousePos( LuaPlus::LuaState* state);

	//加入门派
	//INT Lua_MenpaiFrameJoin(LuaPlus::LuaState* state);

	//学习门派心法 
	INT lua_SkillsStudyFrame_study(LuaPlus::LuaState* state);

	//得到操作对象
	INT	Lua_EnumAction(LuaPlus::LuaState* state);
		//得到操作对象
	INT	Lua_EnumActionForEquip(LuaPlus::LuaState* state);
	//从技能 id 得到CActionItem
	INT	Lua_GetAction_SkillID(LuaPlus::LuaState* state);

	//从物品 id 得到CActionItem
	INT	Lua_GetAction_ItemID(LuaPlus::LuaState* state);

	//显示右键菜单
	INT	Lua_ShowContexMenu(LuaPlus::LuaState* state);

	//任务窗口选项按下
	INT Lua_QuestFrameOptionClicked(LuaPlus::LuaState* state);

	//窗口是否Show的状态
	INT	Lua_IsWindowShow(LuaPlus::LuaState* state);

	//开始合成
	INT Lua_ComposeItem_Begin(LuaPlus::LuaState* state);

	//取消合成
	INT Lua_ComposeItem_Cancel(LuaPlus::LuaState* state);

	//接受任务
	INT Lua_QuestFrameAcceptClicked(LuaPlus::LuaState* state);

	//谢绝任务
	INT Lua_QuestFrameRefuseClicked(LuaPlus::LuaState* state);

	//继续任务
	INT Lua_QuestFrameMissionContinue(LuaPlus::LuaState* state);

	//任务完成
	INT Lua_QuestFrameMissionComplete(LuaPlus::LuaState* state);

	//任务放弃
	INT Lua_QuestFrameMissionAbnegate(LuaPlus::LuaState* state);

	//要求打开/关闭合成界面
	INT	Lua_ToggleComposeWnd(LuaPlus::LuaState* state);
	//枚举所有的配方
	INT	Lua_EnumMethod(LuaPlus::LuaState* state); 
	// 得到某一种配方的数量
	INT Lua_GetMethodNumber( LuaPlus::LuaState* state); 
	//调试信息输出
	INT Lua_AxTrace(LuaPlus::LuaState* state);

	// 根据ID得到场景名字
	INT Lua_GetSceneNameFromID( LuaPlus::LuaState* state );

	// 得到当前场景得尺寸
	INT Lua_GetSceneSize( LuaPlus::LuaState* state); 

	// 得到当前场景id
	INT Lua_GetSceneID( LuaPlus::LuaState* state); 

	// 得到当前小地图得尺寸
	INT Lua_GetMinimapSize( LuaPlus::LuaState* state);

	// 发送聊天信息
	INT Lua_SendChatMessage( LuaPlus::LuaState* state);

	INT Lua_GetCurChatProperty( LuaPlus::LuaState* state );
	// 关闭打开大地图
	INT Lua_ToggleLargeMap( LuaPlus::LuaState* state );
	INT Lua_ToggleSceneMap( LuaPlus::LuaState* state );
	INT Lua_OpenMinimap( LuaPlus::LuaState* state );
	INT Lua_UpdateMinimap( LuaPlus::LuaState* state );

	// 关闭大地图
	INT Lua_CloseLargeMap( LuaPlus::LuaState* state );

	// 关闭场景地图
	INT Lua_CloseSceneMap( LuaPlus::LuaState* state );

	// 更新场景地图，世界地图列表 (_WX@7)
	INT Lua_UpdateLargeMapList( LuaPlus::LuaState* state );
	INT Lua_UpdateSceneMapList( LuaPlus::LuaState* state );

	// 枚举大地图列表中的列表的数量
	INT Lua_LargeMapListNum( LuaPlus::LuaState* state );
	// 枚举场景地图列表中的列表的数量
	INT Lua_SceneMapListNum( LuaPlus::LuaState* state );

	// 枚举大地图列表中的数据
	INT Lua_LargeMapList( LuaPlus::LuaState* state );

	// 枚举场景地图NPC列表中的数据
	INT Lua_SceneMapList( LuaPlus::LuaState* state );


	// 往场景地图列表中添加数据

	// 找北
	INT Lua_ResetCamera( LuaPlus::LuaState* state );
	//脚本里可以控制其他角色的功能函数

	//打开关闭商人交易界面
	INT Lua_OpenBooth( LuaPlus::LuaState* state );
	INT Lua_CloseBooth( LuaPlus::LuaState* state);
	INT Lua_RepairAll( LuaPlus::LuaState* state);
	INT Lua_RepairOne( LuaPlus::LuaState* state);
	INT Lua_CancelRepairState( LuaPlus::LuaState* state);
	INT Lua_GetCurrentSceneName( LuaPlus::LuaState* state );
	INT Lua_GetOperationType( LuaPlus::LuaState* state );
	INT Lua_GetShopType( LuaPlus::LuaState* state );


	//打开关闭称号界面
	INT Lua_OpenTitleList( LuaPlus::LuaState* state);
	INT Lua_CloseTitleList( LuaPlus::LuaState* state); 

	INT Lua_OpenWindow( LuaPlus::LuaState* state);
	INT Lua_CloseWindow( LuaPlus::LuaState* state); 
	INT Lua_ToggleWindow( LuaPlus::LuaState* state); 
	INT Lua_SplitUnifiedString( LuaPlus::LuaState* state);

	INT Lua_OpenCharacter( LuaPlus::LuaState* state );

	INT Lua_OpenEquip( LuaPlus::LuaState* state );
	// 打开个人装备 个人属性 个人称号  一键换装 骑乘属性  个人资料
	INT	Lua_OpenEquipAll( LuaPlus::LuaState* state );

	// 关闭个人装备 个人属性 个人称号  一键换装 骑乘属性  个人资料
	INT	Lua_CloseEquipAll( LuaPlus::LuaState* state );
	INT Lua_GetActionInfo( LuaPlus::LuaState* state );

	// 打开装备界面
	INT Lua_OpenSelfEquip( LuaPlus::LuaState* state );

	//关闭装备 骑乘 换装 资料 称号 属性ui
	INT   Lua_EquipAllCloseUI(LuaPlus::LuaState* state);

	//关闭装备 骑乘 换装 资料 称号 属性ui
	INT   Lua_EquipAllOpenUI(LuaPlus::LuaState* state);

	//触发快捷键之后，判断是否可以进行一键换装
	INT	Lua_IsEquipExChange(LuaPlus::LuaState* state);

	//更新装备 骑乘 换装 资料 称号 属性ui位置
	
	INT   Lua_UpdataEquipAllPos(LuaPlus::LuaState* state);
	//请求升级
	INT Lua_AskLevelUp(LuaPlus::LuaState* state);
	//发送gm指令
	INT Lua_SendGMCommand(LuaPlus::LuaState* state);

	INT Lua_ShowSupterTooltip( LuaPlus::LuaState* state);

	INT Lua_Get_XParam_INT(LuaPlus::LuaState* state);

	INT Lua_Get_XParam_STR(LuaPlus::LuaState* state);

	INT Lua_Clear_XSCRIPT(LuaPlus::LuaState* state);

	INT Lua_Send_XSCRIPT(LuaPlus::LuaState* state);

	INT Lua_Set_XSCRIPT_Parameter(LuaPlus::LuaState* state);

	INT Lua_Set_XSCRIPT_Function_Name(LuaPlus::LuaState* state);

	INT Lua_Set_XSCRIPT_ScriptID(LuaPlus::LuaState* state);

	INT Lua_Set_XSCRIPT_ParamCount(LuaPlus::LuaState* state);

	// 设置UI摄像机
	INT Lua_Set_UICamera(LuaPlus::LuaState* state);

	INT Lua_SaveAutoAttributePoint( LuaPlus::LuaState* state);
	INT Lua_GetAutoAttributePoint( LuaPlus::LuaState* state);

//--------------------------------------------------------------------------
// 组队相关

	INT Lua_ShowTeamFuncMenu(LuaPlus::LuaState* state);		// 队员或队长的功能菜单.
	INT Lua_ShowTeamInfoDlg(LuaPlus::LuaState* state);		// 显示队伍信息对话框.
	INT Lua_FlashTeamButton(LuaPlus::LuaState* state);		// 闪烁按钮.
	INT Lua_SetCurSelMember(LuaPlus::LuaState* state);		// 设置当前选择的队员

// 组队相关
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// 帮会

	// 显示帮会邀请确认对话框
	INT Lua_ShowGuildInfoDlg(LuaPlus::LuaState* state);

//--------------------------------------------------------------------------
	// 向服务器请求排行榜
	INT Lua_SendTopList(LuaPlus::LuaState* state);

	// 向Lua脚本发消息
	INT Lua_PushEvent(LuaPlus::LuaState* state);
	
	// 得到俳榜的名称
	//INT Lua_GetTopListName(LuaPlus::LuaState* state);

	// 设置一个全局变量
	INT Lua_SetGlobalInteger(LuaPlus::LuaState* state);
	// 获得一个全局变量
	INT Lua_GetGlobalInteger(LuaPlus::LuaState* state);

	// 要装备的详细信息
	INT Lua_AskEquipDetial(LuaPlus::LuaState* state);

	// 销毁物品
	INT Lua_DiscardItem(LuaPlus::LuaState* state);

	// 取消销毁
	INT Lua_DiscardItemCancelLocked(LuaPlus::LuaState* state);

	// 播放声音
	INT Lua_PlaySound(LuaPlus::LuaState* state);

	// 关闭所有窗口
	INT	Lua_CloseAllWindow( LuaPlus::LuaState* state);

	// 打开或关闭某个特定的窗口(_WX@7)
	//INT Lua_CloseOneWindow( LuaPlus::LuaState* state);

	// target窗口弹出的菜单
	INT	Lua_OpenTargetMenu( LuaPlus::LuaState* state);

	INT Lua_GetMousePos( LuaPlus::LuaState* state);

	// 显示系统提示对话框。 
	INT Lua_ShowSystemTipInfo( LuaPlus::LuaState* state);

	// 打开更改二级密码对话框。 
//	INT Lua_OpenChangeMinorPasswordDlg( LuaPlus::LuaState* state);

	// 打开解锁密码。 
//	INT Lua_OpenUnLockeMinorPasswordDlg( LuaPlus::LuaState* state);

	// 打开设置密码界面。 
//	INT Lua_OpenSetMinorPasswordDlg( LuaPlus::LuaState* state);

	// 设置密码。 
	INT Lua_SendSetMinorPassword( LuaPlus::LuaState* state);

	// 修改密码。 
	INT Lua_SendModifyMinorPassword( LuaPlus::LuaState* state);

	// 解锁密码。 
	INT Lua_UnLockMinorPassword( LuaPlus::LuaState* state);

	// 强制解锁密码。 
	INT Lua_ForceUnLockMinorPassword( LuaPlus::LuaState* state);

	// 修改密码。 
	INT Lua_ModifyMinorPassword( LuaPlus::LuaState* state);

	// 安全时间。 
	INT Lua_SetPasswordProtectTime( LuaPlus::LuaState* state);

	// 检测输入密码
//	INT Lua_CheckInputPassword( LuaPlus::LuaState* state );

	// 输入密码的状态
//	INT Lua_SetInputPasswordState( LuaPlus::LuaState* state );
//	INT Lua_GetInputPasswordState( LuaPlus::LuaState* state );


	//// 播放音乐。参数是音乐的ID  
	//INT Lua_PlayMusic( LuaPlus::LuaState* state);
	//
	//// 停止音乐。参数是音乐的句柄  
	//INT Lua_StopMusic( LuaPlus::LuaState* state);//

	// 打开8个主功能按钮 Nick 2007.11.22
	INT Lua_OpenMainFuncBtn( LuaPlus::LuaState* state);
 
	// 打开关闭一键换装界面
	INT Lua_OpenQuickChangeEquip( LuaPlus::LuaState* state);

	// 装备打孔。 
	INT Lua_Equip_Add_Hole( LuaPlus::LuaState* state);
 
 	// 装备打孔条件。 
	INT Lua_GetAddHoleCondition( LuaPlus::LuaState* state);
   
	// 装备孔的数目。 
	INT Lua_GetEquipHoleNumForAddHole( LuaPlus::LuaState* state) ;


	// 打星条件信息
	INT   Lua_AddStart_Equip_Info(LuaPlus::LuaState* state) ;

	INT   Lua_AddStartAutoMatic_Attr_Info(LuaPlus::LuaState* state);

	INT   Lua_Get_Equip_StarInfo(LuaPlus::LuaState* state);


	//装备炼化
	INT	Lua_Equip_Refining( LuaPlus::LuaState* state );

	//装备附魔
	INT	Lua_Equip_Fumo( LuaPlus::LuaState* state );

	//装备退魔
	INT	Lua_Equip_Tuimo( LuaPlus::LuaState* state );

	
	// 特修条件信息
	INT   Lua_SpecialRepair_Info(LuaPlus::LuaState* state);  

	// 弹出确认全部修理界面 
	INT   Lua_PopRepairSureWnd( LuaPlus::LuaState* state) ;
 


	//取得打造和合成装备 配方的材料〈按编号取得〉 
	INT  Lua_GetPackFormulation(LuaPlus::LuaState* state);

	// 配方数量
	INT  Lua_EnumPackFormulationNum(LuaPlus::LuaState* state);

	//枚举玩家可用配方配方  
	INT  Lua_EnumPackProCanUse(LuaPlus::LuaState* state) ;


	//枚举配方名字 用于打造和合成装备界面
	INT  Lua_EnumPackFormulation(LuaPlus::LuaState* state);

	//打开装备改造界面
	INT     Lua_Open_Rework_Equip(LuaPlus::LuaState* state);
	//或的改造材料信息
	INT     Lua_GetReWorkMatInfo(LuaPlus::LuaState* state);

	// ---分析装备拆解后的材料数量和等级
	INT     Lua_EquipDismantle_Material_Info(LuaPlus::LuaState* state);

	//取得玩家 是否有相应等级的材料                          打造和合成装备 
	INT  Lua_GetPackForMatOnGrade(LuaPlus::LuaState* state);
	//取得玩家 相应等级的材料icon                  
	INT  Lua_GetPackForMatIconOnGrade(LuaPlus::LuaState* state);

	//取得玩家 是否有相应等级的材料  用于改造                         

	INT  Lua_GetPackForMatOnGradeReWork(LuaPlus::LuaState* state);

	INT  CleanItemForTip(LuaPlus::LuaState* state);

	INT  CreatItemForTip(LuaPlus::LuaState* state);

	INT  Lua_IB_Shop(LuaPlus::LuaState* state);// add by gh
 
class Character
{
public:

	// 显示目标头像的右键菜单
	INT	ShowTargetContexMenu(LuaPlus::LuaState* state);

	// 取得HP return (percent)
	INT		GetHPPercent(LuaPlus::LuaState* state);
	//取得MP return (percent)
	INT		GetMPPercent(LuaPlus::LuaState* state);
	//取得Rage return (percent)
	INT		GetRagePercent(LuaPlus::LuaState* state);

	INT		GetScenePos( LuaPlus::LuaState* state);

	
	//----------------------------------------------------------------------
	// 组队相关

	//请求组队
	INT	SendTeamRequest(LuaPlus::LuaState* state);

	//请求加入队伍
	INT	SendTeamApply(LuaPlus::LuaState* state);

	// 组队相关
	//----------------------------------------------------------------------

	// 跟随
	INT		Follow(LuaPlus::LuaState* state);
	
// 共有的数据
	// 得到角色的位置
	virtual INT		GetPos( LuaPlus::LuaState* state);
	//取得名字
	virtual INT		GetName(LuaPlus::LuaState* state);
	//对象是否存在 "target", "pet", ...
	virtual INT		IsPresent(LuaPlus::LuaState* state);
	// 得到角色身上的buff的数量
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );
	

	// 得到等级
	INT				GetLevel(LuaPlus::LuaState* state);
	INT				GetData(LuaPlus::LuaState* state);
	INT				SendAskDetail(LuaPlus::LuaState* state);
	INT				Set_To_Private(LuaPlus::LuaState* state);
	INT				Close_Before_TargetEquip_UI(LuaPlus::LuaState* state);
	INT				TargetEquip_ChangeModel(LuaPlus::LuaState* state);

	INT				GetDialogNpcSeverID( LuaPlus::LuaState* state );

	virtual INT		GetBuffIconNameByIndex( LuaPlus::LuaState* state );
	virtual INT		GetDialogNpcName( LuaPlus::LuaState* state );
	virtual INT		GetShopNpcName( LuaPlus::LuaState* state );
	virtual INT		GetXinfaNpcName(LuaPlus::LuaState* state);


public:

	 // 当前的target 是否是队友
	 INT	IsTargetTeamMember(LuaPlus::LuaState* state);

	 // 当前的target 是否合法 
	 INT	IsTargetValide(LuaPlus::LuaState* state);

	 // main target 窗口显示队友信息。 

	// 显示名字
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// 显示hp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// 显示mp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// 显示怒气
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// 显示等级
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// 显示头像名字。
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);


protected:
	Character() {};
	virtual ~Character() {};

private:
	CObject_Character*		_GetTargetAsCharacter(VOID);

public:
	//主选择对象
	static Character s_Target;
};

//脚本里可以控制玩家的功能函数
class PlayerMySelf : public Character
{
public:
	
	//获得角色当前对话的NPC id
	INT	GetCurDialogNpcId(LuaPlus::LuaState*state);

	// 得到当前的称号.
	INT	GetCurTitle(LuaPlus::LuaState* state);

	// 不选择称号
	INT SetNullAgname(LuaPlus::LuaState* state);
	
	//----------------------------------------------------------------------------------------------------------------------------------------------
	// 组队相关
	
	// 离开队伍
	INT LeaveTeam(LuaPlus::LuaState* state);	

	// 解散队伍
	INT DismissTeam(LuaPlus::LuaState* state);	

	// 删除一个选中的队员
	INT KickTeamMember(LuaPlus::LuaState* state);
	
	// 删除当前选中队员
	INT KickTeamMemberByIndex(LuaPlus::LuaState* state);			

	//自建队伍
	INT	CreateTeamSelf(LuaPlus::LuaState* state);

	//任命队长消息
	INT	AppointLeader(LuaPlus::LuaState* state);

	// 是否处于组队跟随状态
	INT InTeamFollowMode(LuaPlus::LuaState* state);

	// 取消跟随
	INT StopFollow(LuaPlus::LuaState* state);

	// 组队跟随
	INT TeamFrame_AskTeamFollow(LuaPlus::LuaState* state);

	// 同意加入队伍(有人邀请我)
	INT	SendAgreeJoinTeam(LuaPlus::LuaState* state);
	// 拒绝加入队伍(有人邀请我)
	INT SendRejectJoinTeam(LuaPlus::LuaState* state);

	// 同意加入队伍(有人申请加入队伍)
	INT	SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state);

	// 拒绝加入队伍(有人申请加入队伍)
	INT SendRejectJoinTeam_Apply(LuaPlus::LuaState* state);

	// 同意加入队伍(队员邀请某人加入队伍)
	INT	SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// 拒绝加入队伍(队员邀请某人加入队伍)
	INT SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// 同意进入组队跟随模式
	INT SendAgreeTeamFollow(LuaPlus::LuaState* state);

	// 拒绝进入组队跟随模式
	INT SendRefuseTeamFollow(LuaPlus::LuaState* state);

	// 得到队员guid
	INT GetTeamMemberGUID(LuaPlus::LuaState* state);

	// 修改组队状态下物品拾取模式
	INT ModifyTeamAllocationMode(LuaPlus::LuaState* state);

	// 获得组队状态下物品拾取模式
	INT GetTeamAllocationMode(LuaPlus::LuaState* state);

	// 组队相关
	//-----------------------------------------------------------------------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------------------------------------------------------------------
	// 装备属性界面
	//

	// 发送手动调整属性点数 
	INT SendAskManualAttr(LuaPlus::LuaState* state);


	//
	// 装备属性界面
	//-----------------------------------------------------------------------------------------------------------------------------------------------



	virtual INT		GetName(LuaPlus::LuaState* state);
	virtual INT		GetData( LuaPlus::LuaState* state );
	// 得到角色的某项生活技能的信息
	virtual INT		GetAbilityInfo( LuaPlus::LuaState* state );
	// 得到角色是否学习过某项生活技能
	virtual INT		GetAbilityStudy( LuaPlus::LuaState* state );
	// 得到角色的某项战斗技能的信息
	virtual INT		GetSkillInfo( LuaPlus::LuaState* state );
	// 按索引得到角色的某项战斗技能的信息 Nick 2007.11.23
	virtual INT		GetIndexSkillInfo( LuaPlus::LuaState* state );
	// 得到技能总数 Nick 2007.11.23
	virtual INT		GetSkillNum( LuaPlus::LuaState* state );
	// 得到角色的某项心法的信息
	virtual INT		GetXinfaInfo( LuaPlus::LuaState* state );
	// 请求学习生活技能
	virtual INT		AskLeanAbility(LuaPlus::LuaState* state);

	// 得到角色的位置
	virtual INT		GetPos( LuaPlus::LuaState* state);
	// 角色是否存在
	virtual INT		IsPresent(LuaPlus::LuaState* state);

	//称号相关
	virtual INT		GetAgnameNum(LuaPlus::LuaState* state);
	virtual INT		GetAgnameType(LuaPlus::LuaState* state);
	virtual INT		EnumAgname(LuaPlus::LuaState* state);
	virtual INT		GetCurrentAgname(LuaPlus::LuaState* state);
	virtual INT		AskChangeCurrentAgname(LuaPlus::LuaState* state);
	virtual INT		UpdateTitleState(LuaPlus::LuaState* state);
	virtual INT		GetAgnameInfo(LuaPlus::LuaState* state);

	virtual INT		GetAllAgnameNum(LuaPlus::LuaState* state);
	virtual INT		GetTableAgnameInfo(LuaPlus::LuaState* state);

	
	// 得到角色身上的buff的数量
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );

	virtual INT		GetBuffIconNameByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffToolTipsByIndex( LuaPlus::LuaState* state );
	virtual INT		DispelBuffByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffTimeTextByIndex( LuaPlus::LuaState* state );

	// 复活对话框相关
	virtual INT		SendReliveMessage_OutGhost( LuaPlus::LuaState* state );
	virtual INT		SendReliveMessage_Relive( LuaPlus::LuaState* state );

	// 右键菜单相关
	virtual INT		ShowMySelfContexMenu(LuaPlus::LuaState* state);
	// 左键选中自己
	virtual INT		SelectMyselfAsTarget(LuaPlus::LuaState* state);

	// 设置二级密码
	INT	SetSupperPassword(LuaPlus::LuaState* state);


	// 获取密码保护系统设置选项
	INT Lua_PasswordGetData( LuaPlus::LuaState* state );
	// 保存密码保护系统设置选项
	INT Lua_SavePasswordSetup( LuaPlus::LuaState* state );
	// 获取某类型的设置状态
	INT Lua_PasswordTypeGetState( LuaPlus::LuaState* state );


	// 密码检测流程
	INT Lua_CheckPasswdProcedure( LuaPlus::LuaState* state );
	// 处理输入密码后的流程
	INT Lua_InputPasswdProcedure( LuaPlus::LuaState* state );


protected:

	// 保护事件处理
	VOID ProtectEventHandling( INT nType );

	// 针对装备的操作保护
	INT			m_nQuestIndex;
	STRING		m_sQuestScript;

protected:

	PlayerMySelf() : m_nQuestIndex(-1), m_sQuestScript("")  {};
	virtual ~PlayerMySelf() {};

public:

	static PlayerMySelf s_MySelf;
};

//数据池
class DataPool
{
public:
	//取得当前对话npc数据
	INT		GetNPCEventList_Num(LuaPlus::LuaState* state);
	INT		GetNPCEventList_Item(LuaPlus::LuaState* state);

	//-----------------------------------------------------------
	//任务信息
	INT		GetMissionInfo_Num(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Text(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Bonus(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Kind(LuaPlus::LuaState* state);

	// 是否是每日活动任务 20100728 BLL 
	INT		GetMissionInfo_IsDayTip(LuaPlus::LuaState* state);

	// 获取每日活动的可接次数和完成次数 20100728 BLL 
	INT		GetMissionInfo_DayTipCount(LuaPlus::LuaState* state);

	//任务需求信息
	INT		GetMissionDemand_Num(LuaPlus::LuaState* state);
	INT		CloseMissionFrame(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Text(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Item(LuaPlus::LuaState* state);
	//Continue
	INT		GetMissionContinue_Num(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Text(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Bonus(LuaPlus::LuaState* state);

	// 取任务跟踪列表
	INT		GetScoutMission(LuaPlus::LuaState* state);
	INT		SetScoutMission(LuaPlus::LuaState* state);
	INT		GetScoutMission_Num(LuaPlus::LuaState* state);

	// Npc自动走路
	INT		NpcAutoFindPath(LuaPlus::LuaState* state);

	// 在场景地图窗口点击鼠标移动到其他位置时，如果当前是Npc自动寻路状态，则重置自动寻路的参数值	20100531 BLL
	INT		ResetNpcAutoFindPath(LuaPlus::LuaState* state);	

	// NPC跨场景自动寻路
	INT		NpcOutSceneAutoFindPath(LuaPlus::LuaState* state);

	//玩家当前任务信息
	INT		GetPlayerMission_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMission_InUse(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Description(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMissionCustom_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionCustom(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Memo(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Name(LuaPlus::LuaState* state);
	INT		GetPlayerMission_BillName(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Info(LuaPlus::LuaState* state);

	INT		GetPlayerMission_ItemCountNow(LuaPlus::LuaState* state);

	INT		GetPlayerMission_Variable(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Display(LuaPlus::LuaState* state);
	INT		GetPlayerMission_RemainTime(LuaPlus::LuaState* state);
	INT		Mission_Abnegate_Popup(LuaPlus::LuaState* state);
	INT		Mission_Abnegate_Special_Quest_Popup(LuaPlus::LuaState* state);
	INT		EnumPlayerMission_ItemAction(LuaPlus::LuaState* state);
	INT		GetPlayerMission_DataRound(LuaPlus::LuaState* state);

	INT		GetPlayerMissionDemandKill_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_NPC(LuaPlus::LuaState* state);
/*	INT		GetPlayerMissionBonus_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Memo(LuaPlus::LuaState* state);
*/	INT		GetPlayerMission_Level(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Kind(LuaPlus::LuaState* state);
	INT		GetPlayerMission_IsElite(LuaPlus::LuaState* state);
	//获得玩家当前任务所在场景的ID 名字
	INT		GetPlayerMission_SceneID(LuaPlus::LuaState* state );


	//取得配方数据
	//取得配方的数量
	INT		GetPrescrList_Num(LuaPlus::LuaState* state);
	//取得配方的名字
	INT		GetPrescrList_Item(LuaPlus::LuaState* state);
	//取得配方所需的生活技能ID
	INT		GetPrescrList_Item_LifeAbility(LuaPlus::LuaState* state);
	//取得配方的产品
	INT		GetPrescrList_Item_Result(LuaPlus::LuaState* state);
	//取得配方所需的材料
	INT		GetPrescrList_Item_Requirement(LuaPlus::LuaState* state);
	//取得配方的需要工具
	INT		GetPrescrList_Item_NeedTool(LuaPlus::LuaState* state);
	//取得配方的详细解释
	INT		GetPrescrList_Item_Desc(LuaPlus::LuaState* state);

	
	//根据生活技能类型，取得生活技能配方容器中的第N个相关数据
	INT		GetPrescrList_Item_ListPos(LuaPlus::LuaState* state);




	// 获得货架的信息 
	INT		GetBoothList_Num(LuaPlus::LuaState* state); // 得到有多少个货物
	INT		GetBoothGood_ID( LuaPlus::LuaState* state ); // 得到货物的id 
	INT		GetBoothGood_Name(LuaPlus::LuaState* state); // 得到货物的名字
	INT		GetBoothGood_Price(LuaPlus::LuaState* state);// 得到货物的价格
	

	//-----------------------------------------------------------------------------------------------------------------------------------
	// 组队相关. 
	
	//------------------------------
	//这些函数供左边的头像界面显示.
	INT		GetTeamMemGUIDByUIIndex(LuaPlus::LuaState* state);			// 得到队员的信息
	INT		GetTeamMemInfo(LuaPlus::LuaState* state);			// 得到队员的信息
	INT		GetTeamMemCount(LuaPlus::LuaState* state);			// 得到队员的个数
	INT	    IsTeamLeader(LuaPlus::LuaState* state);				// 是否是队长
	INT		SelectAsTargetByUIIndex(LuaPlus::LuaState* state);	// 通过队友头像, 选中队友作为target
	INT		GetSelfGUID(LuaPlus::LuaState* state);
	INT		GetTeamMemPosByUIIndex(LuaPlus::LuaState* state);		//得到队员的位置信息，用于跨场景寻路



	//------------------------------
	// 打开邀请界面.

	//这些函数供组队信息窗口使用.  邀请接口.   
	INT		GetInviteTeamCount(LuaPlus::LuaState* state);		// 得到邀请队伍的个数.
	INT     GetInviteTeamMemberInfo(LuaPlus::LuaState* state);	// 得到队伍的信息.	
	INT		GetInviteTeamMemberCount(LuaPlus::LuaState* state);	// 得到某个邀请队伍的队员的个数.


	
	//-----------------------------
	// 打开队伍信息.

	// 是否在组队状态中
	INT		GetMyTeamFlag(LuaPlus::LuaState* state);

	// 得到当前队伍人数
	INT		GetMyTeamMemberCount(LuaPlus::LuaState* state);

	// 自己和玩家(或npc)间的距离，返回 -1 表示看不到目标。
	INT		GetDistanceBySeverID(LuaPlus::LuaState* state);

	// 得到另一个队友的severid
	INT		GetAnotherTeamMemberSeverID(LuaPlus::LuaState* state);

	// 得到clientid
	INT		GetClientIDBySeverID(LuaPlus::LuaState* state);

	
	// 由于队伍中的自己的信息, 不通过队伍信息返回, 每次打开界面的时候要刷新自己的信息
	INT     SetSelfInfo(LuaPlus::LuaState* state);

	// 队长打开队伍信息时, 选中的人物
	INT     SetSelectTeamMember_Leader(LuaPlus::LuaState* state);

	// 通过索引得到队员信息, 供队员打开界面使用.
	INT	GetTeamMemInfoByIndex(LuaPlus::LuaState* state);	// 得到队员的信息

	// 得到队员的ui模型名字
	INT	GetTeamMemUIModelNameByIndex(LuaPlus::LuaState* state);

	// 得到申请者的ui模型名字
	INT GetApplyMemberUIModelName(LuaPlus::LuaState* state);		

	// 得到队伍ui模型名字
	INT GetInviteTeamMemberUIModelName(LuaPlus::LuaState* state);	

	// 得到队员ui模型名字
	INT	GetTeamMemUIModelName(LuaPlus::LuaState* state);		

	// 队员与自己是否在同一个场景中
	INT	IsTeamMemberInScene(LuaPlus::LuaState* state);		

	//获得自己是否是队长
	BOOL    GetMyTeamLeaderFlag( LuaPlus::LuaState* state);


	//------------------------------
    // 打开申请人界面

	//这些函数供组队信息窗口使用.  申请接口. 
	INT		GetApplyMemberCount(LuaPlus::LuaState* state);		// 得到申请者个数.
	INT     GetApplyMemberInfo(LuaPlus::LuaState* state);		// 得到申请者的个数.		
	INT     SetCurSelApply(LuaPlus::LuaState* state);			// 设置当前选择的申请者.
	INT     EraseApply(LuaPlus::LuaState* state);				// 删除一个申请者
	INT     ClearAllApply(LuaPlus::LuaState* state);			// 清空所有的申请者.



	//------------------------------
	// 公共数据接口.

	// 设置组队对话框打开标记.
	INT		SetTeamFrameOpenFlag(LuaPlus::LuaState* state);		// 设置队伍对话框打开标记


	//--------------------------------------------------------------------------------------------------------------------------------------



	//包裹数据
	INT		GetPacketItem( LuaPlus::LuaState* state );
	INT		GetPacketItemMaxNumber( LuaPlus::LuaState* state );
	INT     GetPacketItemFroId( LuaPlus::LuaState* state );
	INT     GetPacketItemAllNum( LuaPlus::LuaState* state );



/** 好友系统接口
*/

	// 得到某一组的好友数量
	INT		GetFriendNumber( LuaPlus::LuaState* state );
	// 得到某一组的好友的具体数据
	INT		GetFriend( LuaPlus::LuaState* state );

	// 添加一个好友到某一组
	INT		AddFriend( LuaPlus::LuaState* state );
	// 向系统提出删除好友请求
	INT		AskDelFriend( LuaPlus::LuaState* state );
	// 确认删除一个好友
	INT		DelFriend( LuaPlus::LuaState* state );

	// 放到不同的列表中
	INT     ThrowToBlackList( LuaPlus::LuaState* state );
	INT		ThrowToFriendList( LuaPlus::LuaState* state );
	INT     ThrowToEnemyList( LuaPlus::LuaState* state );
	INT     ThrowToList( LuaPlus::LuaState* state );

	// 单独处理通过右键点击玩家对象加为好友
	INT		AddFriendByModel( LuaPlus::LuaState* state );

	// 修改个人设置
	INT		FriendSeting( LuaPlus::LuaState* state );

	// 显示好友信息
	INT		ShowFriendInfo( LuaPlus::LuaState* state );
	// 调用脚本， 准备显示信息
	INT		LuaShowFriendInfo( LuaPlus::LuaState* state );
	INT		ShowChatInfo(LuaPlus::LuaState* state);
	
	// 心情
	INT		SetMood( LuaPlus::LuaState* state );
	INT		GetMood( LuaPlus::LuaState* state );

	INT		OpenFriendList(LuaPlus::LuaState* state);

	// 获取邮件数量
	INT		GetMailNumber( LuaPlus::LuaState* state );
	// 获取邮件内容
	INT		GetMail( LuaPlus::LuaState* state );

	//LAIM
	INT     MailAutoRestoreGet( LuaPlus::LuaState* state){return 0;}
	INT     ChatAutoRestoreGet( LuaPlus::LuaState* state){return 0;}
	INT		AutoRestoreGetContent( LuaPlus::LuaState* state){return 0;}

	// 打开邮件界面
	INT 	OpenMailBrowser(LuaPlus::LuaState* state);
	INT 	OpenMailEdit(LuaPlus::LuaState* state);
	// 发送邮件
	INT		SendMail( LuaPlus::LuaState* state );

	INT		GetNextMail(LuaPlus::LuaState* state );

	// 通过名字获取好友
	INT     GetFriendByName( LuaPlus::LuaState* state );

	// 打开历史
	INT		OpenHistroy( LuaPlus::LuaState* state );

	// 添加信息到历史聊天记录
	INT		AddChatHistoryMessage( LuaPlus::LuaState* state );

	// 确定添加申请玩家到好友列表
	INT		SureAddFriend( LuaPlus::LuaState* state );

	// 拒绝加为好友
	INT		RejectAddFriend( LuaPlus::LuaState* state );

	//	判断玩家是否是自己最近联系人
	INT		IsTempFriendByName( LuaPlus::LuaState* state );

	//	判断玩家是否在自己的黑名单中
	INT		IsInBlackListByName( LuaPlus::LuaState* state );

	//  通过guid判断玩家是否在黑名单中
	INT		IsInBlackListByGuid( LuaPlus::LuaState* state );

	//	判断玩家是否在自己的仇人列表中
	INT		IsInEnemyListByName( LuaPlus::LuaState* state);


//改变发型发色
	INT		Change_MyHairColor( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle_Item( LuaPlus::LuaState* state );
	INT		Get_MyHairColor( LuaPlus::LuaState* state );
	INT		Get_MyHairStyle( LuaPlus::LuaState* state );
//检查String是否合法
	INT		Check_StringCode( LuaPlus::LuaState* state );


//================
// 界面系统提示


	// 添加事件
	INT		SystemTip_AddEventInfo( LuaPlus::LuaState* state );
	// 删除事件
	INT		SystemTip_DelEventInfo( LuaPlus::LuaState* state );

	// 获取某一事件的信息
	INT		SystemTip_GetEventInfo( LuaPlus::LuaState* state );
	// 设置某一事件的状态
	INT		SystemTip_SetEventState( LuaPlus::LuaState* state );


	// 获取现有事件数量
	INT		SystemTip_GetEventNum( LuaPlus::LuaState* state );

	INT		Lua_ReloadWindowScript(LuaPlus::LuaState* state);


public:

	INT		IsCanDoAction( LuaPlus::LuaState* state );
	INT		Change_RectifyColor( LuaPlus::LuaState* state );
	INT		Change_GetColorLumination(LuaPlus::LuaState* state);

public:

	// 显示名字
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// 显示hp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// 显示mp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// 显示怒气
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// 显示等级
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// 显示头像名字。
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);

	INT	ToggleShowPetList(LuaPlus::LuaState* state);

	INT	GetNPCIDByServerID(LuaPlus::LuaState* state);

	INT GetUIColor(LuaPlus::LuaState* state);

	// 设置二级密码
	INT	SetSupperPass(LuaPlus::LuaState* state);

	//临时声明
	INT TargetFrame_Update_BuffNum_Team(LuaPlus::LuaState* state);
	INT TargetFrame_Update_BuffInfo_Team(LuaPlus::LuaState* state);

public:

	static DataPool s_DataPool;
	static LuaPlus::LuaObject* s_pMetaTable;

};



// 当前正在对话的生活技能老师
class AbilityTeacher
{
public:
	//取得当前老师是否存在
	INT		IsPresent(LuaPlus::LuaState* state);
	//得到当前老师所能教的生活技能ID
	INT		GetAbilityID(LuaPlus::LuaState* state);
	//得到升级到下一级所需要的经验值
	INT		GetNeedExp(LuaPlus::LuaState* state);
	//得到升级到下一级所需要的金钱
	INT		GetNeedMoney(LuaPlus::LuaState* state);
	//得到升级到下一级所需要的熟练度
	INT		GetNeedSkillExp(LuaPlus::LuaState* state);
	// 获得服务器升级的一些数据
	INT		GetServerData(LuaPlus::LuaState* state);

public:
	static AbilityTeacher s_AbilityTeacher;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Pet
{
public:
	//取得当前宠物是否存在
	INT		IsPresent(LuaPlus::LuaState* state);
	//得到宠物的数量
	INT		GetPet_Count(LuaPlus::LuaState* state);
	//得到第几只宠物的名字
	INT		GetPetList_Appoint(LuaPlus::LuaState* state);
	//宠物参战
	INT		Go_Fight(LuaPlus::LuaState* state);
	//宠物休息，收回
	INT		Go_Relax(LuaPlus::LuaState* state);
	//宠物放生
	INT		Go_Free(LuaPlus::LuaState* state);
	//宠物喂食
	INT		Feed(LuaPlus::LuaState* state);
	INT		Dome(LuaPlus::LuaState* state);
	//得到宠物的名字
	INT		GetName(LuaPlus::LuaState* state);
	//获得宠物的ID
	INT		GetID(LuaPlus::LuaState* state);
	//获得配偶的GUID
	INT		GetConsort(LuaPlus::LuaState* state);
	//得到宠物的性别
	INT		GetSex(LuaPlus::LuaState* state);
	//获得宠物的寿命
	INT		GetNaturalLife(LuaPlus::LuaState* state);
	INT		GetMaxLife(LuaPlus::LuaState* state);
	INT		GetLoyalgGade(LuaPlus::LuaState* state);
	//得到宠物的等级
	INT		GetLevel(LuaPlus::LuaState* state);
	//获得宠物的潜能
	INT		GetPotential(LuaPlus::LuaState* state);
	//得到宠物是第几代宠
	INT		GetType(LuaPlus::LuaState* state);
	//得到宠物的快乐度
	INT		GetHappy(LuaPlus::LuaState* state);
	//得到宠物的力量资质
	INT		GetStrAptitude(LuaPlus::LuaState* state);
	//获得宠物体力资质
	INT		GetPFAptitude(LuaPlus::LuaState* state);
	//获得宠物身法资质
	INT		GetDexAptitude(LuaPlus::LuaState* state);
//	INT		GetIntAptitude(LuaPlus::LuaState* state);
	//获得宠物定力资质
	INT		GetStaAptitude(LuaPlus::LuaState* state);
	//获得宠物当前的血量
	INT		GetHP(LuaPlus::LuaState* state);
	//获得宠物的最大血量
	INT		GetMaxHP(LuaPlus::LuaState* state);
	INT		GetMP(LuaPlus::LuaState* state);
	INT		GetMaxMP(LuaPlus::LuaState* state);

	//获得宠物的力量
	INT		GetStr(LuaPlus::LuaState* state);
	INT		GetInt(LuaPlus::LuaState* state);
	//获得宠物的身法
	INT		GetDex(LuaPlus::LuaState* state);
	//获得宠物的体力
	INT		GetPF(LuaPlus::LuaState* state);
	//获得宠物的定力
	INT		GetSta(LuaPlus::LuaState* state);
	//获得宠物的根骨
	INT		GetBasic(LuaPlus::LuaState* state);
	//获得宠物当前的经验和升级所需要的经验
	INT		GetExp(LuaPlus::LuaState* state);

	INT		GetNearAttack(LuaPlus::LuaState* state);
	INT		GetNearRecovery(LuaPlus::LuaState* state);
	INT		GetFarAttack(LuaPlus::LuaState* state);
	INT		GetFarRecovery(LuaPlus::LuaState* state);

//	INT		GetPhysicsAttack(LuaPlus::LuaState* state);
	INT		GetMagicAttack(LuaPlus::LuaState* state);
//	INT		GetPhysicsRecovery(LuaPlus::LuaState* state);
	INT		GetMagicRecovery(LuaPlus::LuaState* state);

	//获得宠物的闪避率
	INT		GetMiss(LuaPlus::LuaState* state);
	//获得宠物的命中率
	INT		GetShootProbability(LuaPlus::LuaState* state);
	//宠物属性加点
	INT		Add_Attribute(LuaPlus::LuaState* state);
	//宠物改名字
	INT		Change_Name(LuaPlus::LuaState* state);
	//宠物是否处于参战状态
	INT		GetIsFighting(LuaPlus::LuaState* state);
	INT		SetModel(LuaPlus::LuaState* state);
	//获得宠物的会心率
	INT		GetDeathWarrant(LuaPlus::LuaState* state);

	//获得宠物的第N个技能的ID
	INT		GetSkillID(LuaPlus::LuaState* state);
	
	//yangjun add for petskillstudy
	//设置宠物技能学习的宠物模型
	INT		SetSkillStudyModel(LuaPlus::LuaState* state);
	//显示目标宠物
	INT		ShowTargetPet(LuaPlus::LuaState* state);
	INT		SkillStudy_Do(LuaPlus::LuaState* state);
	INT		SkillStudyUnlock(LuaPlus::LuaState* state);
	INT		SkillStudy_MenPaiSkill_Created(LuaPlus::LuaState* state);
	INT		SkillStudy_MenPaiSkill_Clear(LuaPlus::LuaState* state);
	INT		GetGUID(LuaPlus::LuaState* state);
	INT		ShowPetList(LuaPlus::LuaState* state);

	//yangjun add for petcontexmenu
	INT		ShowMyPetContexMenu(LuaPlus::LuaState* state);
	INT		HandlePetMenuItem(LuaPlus::LuaState* state);

	//yangjun add for petImpact
	INT		GetPetPortraitByIndex(LuaPlus::LuaState* state);
	INT		GetPetImpactNum(LuaPlus::LuaState* state);
	INT		GetPetImpactIconNameByIndex(LuaPlus::LuaState* state);
	//得到操作对象，得到宠物的技能ID
	INT		Lua_EnumPetSkill(LuaPlus::LuaState* state);
	INT		Select_Pet(LuaPlus::LuaState* state);
	//获得宠物的性格
	INT		GetAIType(LuaPlus::LuaState* state);
	INT		GetSkillPassive(LuaPlus::LuaState* state);
	// 宠物放生确认
	INT		Free_Confirm(LuaPlus::LuaState* state);

public:
	static Pet s_Pet;
	static LuaPlus::LuaObject* s_pMetaTable;
};


class TargetPet
{
public:
	//取得当前宠物是否存在
	INT		IsPresent(LuaPlus::LuaState* state);
//	INT		GetPet_Count(LuaPlus::LuaState* state);
//	INT		GetPetList_Appoint(LuaPlus::LuaState* state);
//	INT		Go_Fight(LuaPlus::LuaState* state);
//	INT		Go_Relax(LuaPlus::LuaState* state);

	INT		GetName(LuaPlus::LuaState* state);
	INT		GetID(LuaPlus::LuaState* state);
	INT		GetConsort(LuaPlus::LuaState* state);
	INT		GetSex(LuaPlus::LuaState* state);
	INT		GetNaturalLife(LuaPlus::LuaState* state);
	INT		GetMaxLife(LuaPlus::LuaState* state);
	INT		GetLoyalgGade(LuaPlus::LuaState* state);
	INT		GetLevel(LuaPlus::LuaState* state);
	INT		GetType(LuaPlus::LuaState* state);
	INT		GetHappy(LuaPlus::LuaState* state);
	INT		GetStrAptitude(LuaPlus::LuaState* state);
	INT		GetPFAptitude(LuaPlus::LuaState* state);
	INT		GetDexAptitude(LuaPlus::LuaState* state);
//	INT		GetIntAptitude(LuaPlus::LuaState* state);
//	INT		GetStaAptitude(LuaPlus::LuaState* state);
	
	INT		GetHP(LuaPlus::LuaState* state);
	INT		GetMaxHP(LuaPlus::LuaState* state);
	INT		GetMP(LuaPlus::LuaState* state);
	INT		GetMaxMP(LuaPlus::LuaState* state);

	INT		GetStr(LuaPlus::LuaState* state);
	INT		GetInt(LuaPlus::LuaState* state);
	INT		GetDex(LuaPlus::LuaState* state);
	INT		GetPF(LuaPlus::LuaState* state);
	INT		GetSta(LuaPlus::LuaState* state);

	INT		GetBasic(LuaPlus::LuaState* state);
	INT		GetPotential(LuaPlus::LuaState* state);
	INT		GetExp(LuaPlus::LuaState* state);

	INT		GetNearAttack(LuaPlus::LuaState* state);
	INT		GetNearRecovery(LuaPlus::LuaState* state);
	INT		GetFarAttack(LuaPlus::LuaState* state);
	INT		GetFarRecovery(LuaPlus::LuaState* state);

//	INT		GetPhysicsAttack(LuaPlus::LuaState* state);
	INT		GetMagicAttack(LuaPlus::LuaState* state);
//	INT		GetPhysicsRecovery(LuaPlus::LuaState* state);
	INT		GetMagicRecovery(LuaPlus::LuaState* state);

	INT		GetMiss(LuaPlus::LuaState* state);
	INT		GetShootProbability(LuaPlus::LuaState* state);

//	INT		Add_Attribute(LuaPlus::LuaState* state);
//	INT		Change_Name(LuaPlus::LuaState* state);

	INT		GetIsFighting(LuaPlus::LuaState* state);
	INT		SetModel(LuaPlus::LuaState* state);
	INT		CopyMyPet(LuaPlus::LuaState* state);
	INT		GetDeathWarrant(LuaPlus::LuaState* state);
	INT		GetAIType(LuaPlus::LuaState* state);

public:
	static TargetPet s_TargetPet;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Bank
{
public:
	//的到银行是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//打开存钱界面（输入存钱数量的界面）
	INT OpenSaveFrame(LuaPlus::LuaState* state);
	//打开取钱界面（输入取钱数量的界面）
	INT OpenGetFrame(LuaPlus::LuaState* state);
	//金钱的转换
	INT GetInputMoney(LuaPlus::LuaState* state);
	//发送存钱消息
	INT SaveMoneyToBank(LuaPlus::LuaState* state);
	//发送取钱消息
	INT GetMoneyFromBank(LuaPlus::LuaState* state);
	//导出银行的金钱数
	INT GetBankMoney(LuaPlus::LuaState* state);
	//发送一个移动银行物品到背包的消息
	INT MoveItemToPacket(LuaPlus::LuaState* state);
	//导出现在拥有的租赁箱的个数
	INT GetRentBoxNum(LuaPlus::LuaState* state);
	//导出指定租赁箱的开始数和格子数
	INT GetRentBoxInfo(LuaPlus::LuaState* state);
	//设置当前的租赁箱
	INT SetCurRentIndex(LuaPlus::LuaState* state);
	//转换货币
	INT TransformCoin(LuaPlus::LuaState* state);
	//关闭银行
	INT Close(LuaPlus::LuaState* state);
	//获得操作物品
	INT EnumItem(LuaPlus::LuaState* state);

	// 打开密码输入界面
	INT OpenInputFrame(LuaPlus::LuaState* state);
public:
	static Bank s_Bank;
	static LuaPlus::LuaObject* s_pMetaTable;
};

// 交易
class Exchange
{
public:

	// 的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	// 申请交易
	INT SendExchangeApply(LuaPlus::LuaState* state);
	// 打开交易对话框
	INT OpenExchangeFrame(LuaPlus::LuaState* state);
	// 导出自己放入交易的物品个数
	INT GetItemNum(LuaPlus::LuaState* state);
	// 导出Action
	INT GetItemAction(LuaPlus::LuaState* state);
	// 导出金钱数
	INT GetMoney(LuaPlus::LuaState* state);
	// 锁定交易
	INT LockExchange(LuaPlus::LuaState* state);
	//导出锁定状态
	INT IsLocked(LuaPlus::LuaState* state);
	// 确定交易
	INT AcceptExchange(LuaPlus::LuaState* state);
	// 交易取消
	INT ExchangeCancel(LuaPlus::LuaState* state);
	// 打开宠物界面（暂时用来打开金钱输入框）
	INT OpenPetFrame(LuaPlus::LuaState* state);
	// 从输入框获得金钱
	INT GetMoneyFromInput(LuaPlus::LuaState* state);
	//导出交易请求列表，是否为空
	INT IsStillAnyAppInList(LuaPlus::LuaState* state);
	//导出以自己交易的人的名字
	INT GetOthersName(LuaPlus::LuaState* state);
	//选择一个宠物列表中的宠物，点击确定后的操作
	INT AddPet(LuaPlus::LuaState* state);
	// 获得宠物的个数
	INT GetPetNum(LuaPlus::LuaState* state);
	//
	INT EnumPet(LuaPlus::LuaState* state);
	//从交易栏上拿下一个宠物
	INT DelSelectPet(LuaPlus::LuaState* state);
	//显示宠物详细信息
	INT ViewPetDesc(LuaPlus::LuaState* state);

public:
	static Exchange s_Exchange;
	static LuaPlus::LuaObject* s_pMetaTable;
};


// 生活技能
class LifeAbility
{
public:
	//通过生活技能ID和级别 得到当前技能级别的详细信息
	INT GetLifeAbility_Data(LuaPlus::LuaState* state);
	//根据生活技能ID获得其ActionID
	INT GetActionID_ByAbilityID(LuaPlus::LuaState*state);
	// 得到生活技能的数量
//	INT GetLifeAbility_Count(LuaPlus::LuaState* state);
	// 通过编号去查Ability的编号
	INT GetLifeAbility_Number(LuaPlus::LuaState* state);
	// 通过编号去查该配方，共需要几个材料
	INT GetPrescr_Material_Number(LuaPlus::LuaState* state);
	// 通过材料的编号，查询材料的详细信息，返回图标和名称。
	INT	GetPrescr_Material(LuaPlus::LuaState* state);
	// 通过序号去查第几个配方的编号
	INT GetPrescrList_Item_FromNum(LuaPlus::LuaState* state);
	// 计算身上原料总共可以合成多少个这个配方。
	INT GetPrescr_Item_Maximum(LuaPlus::LuaState* state);
	// 计算原料数量。
	INT GetPrescr_Material_Hold_Count(LuaPlus::LuaState* state);
	// 刷新合成界面
	INT Update_Synthesize( LuaPlus::LuaState* state);
	// 打开宝石合成/镶嵌界面
	INT Open_Compose_Gem_Page( LuaPlus::LuaState* state);
	// 开始镶嵌
	INT	Do_Enchase( LuaPlus::LuaState* state);
	// 开始合成
	INT	Do_Combine( LuaPlus::LuaState* state);
	// 返回装备上，第i颗宝石的图标信息
	INT	GetEquip_Gem( LuaPlus::LuaState* state);
	// 返回装备上，总共有多少颗宝石。
	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// 返回装备上，总共可以镶嵌多少个宝石	
//	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// 返回宝石的级别。
//	INT Compound_Preparation( LuaPlus::LuaState* state );
	// 是否可以镶嵌
	INT	Can_Enchase( LuaPlus::LuaState* state );
	// 是否可以合成
	INT Can_Combine( LuaPlus::LuaState* state );
	// 物品可以放到镶嵌界面这个位置。
	INT Enchase_Preparation( LuaPlus::LuaState* state );
	// 物品可以放到合成界面这个位置。
	INT Compound_Preparation( LuaPlus::LuaState* state );
	// 把物品栏某位置的物品置灰。
	INT Lock_Packet_Item( LuaPlus::LuaState* state );
	// 取得配方的详细描述
	INT	GetPrescr_Explain(LuaPlus::LuaState* state);
	// 生活技能界面关心的Npc
	INT GetNpcId( LuaPlus::LuaState* state );
	
	//获取宝石的Action
	INT GetGemActionInfo( LuaPlus::LuaState* state );
	
	INT SetPrecalcInfo( LuaPlus::LuaState* state );

	//根据IDtable来获取生活技能所需材料的详细信息
	INT		GetPrescrList_Item_RequirementData(LuaPlus::LuaState* state);

	//根据ID来获取背包中此物品的数量
	INT		GetRequirementNum_ById(LuaPlus::LuaState* state);

protected:
	std::map<INT,CObject_Item*> m_Requirement;			//保存生活技能中用到的物品，每个物品编号只创建一个物品

public:
	static LifeAbility s_LifeAbility;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//有可能将买和买分开来处理，
//class Stall

class StallSale
{
public:

	//的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//导出本地摊位费
	INT GetPosTax(LuaPlus::LuaState* state);
	//导出本地税率
	INT GetTradeTax(LuaPlus::LuaState* state);
	//提交商品价格
	INT ReferItemPrice(LuaPlus::LuaState* state);
	// 更改摊位名字
	INT ModifStallName(LuaPlus::LuaState* state);
	// 更改Item价格
	INT ModifItemPrice(LuaPlus::LuaState* state);
	// 设置保存状态
	INT SetSaveStates(LuaPlus::LuaState* state);
	// 收摊确认
	INT ApplyCloseStall(LuaPlus::LuaState* state);
	// 收摊走人
	INT CloseStall(LuaPlus::LuaState* state);
	// 玩家开市		20100706 BLL
	INT StallOpen(LuaPlus::LuaState* state);
	// 商品的重新定价
	INT ItemReprice(LuaPlus::LuaState* state);
	// 发送开始摆摊的消息开始摆摊
	INT AgreeBeginStall(LuaPlus::LuaState* state);
	//删除货架上的一个物品（下架）
	INT DeleteItem(LuaPlus::LuaState* state);
	//获得指定位置的定价
	INT GetPrice(LuaPlus::LuaState* state);
	//打开摊位信息
	INT OpenMessageSale(LuaPlus::LuaState* state);
	//更改摊位介绍（广告语）
	INT ApplyAd(LuaPlus::LuaState* state);
	//关闭
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//获得摊位名
	INT GetStallName(LuaPlus::LuaState* state);
	//获得广告语
	INT GetAdvertise(LuaPlus::LuaState* state);
	//导出自己的GUID
	INT GetGuid(LuaPlus::LuaState* state);
	// 获得宠物的个数
	INT GetPetNum(LuaPlus::LuaState* state);
	// 获得宠物
	INT EnumPet(LuaPlus::LuaState* state);
	// 宠物上架
	INT PetUpStall(LuaPlus::LuaState* state);
	// 宠物更改价格
	INT PetReprice(LuaPlus::LuaState* state);
	// 获得缺省页
	INT GetDefaultPage(LuaPlus::LuaState* state);
	// 设置缺省页
	INT SetDefaultPage(LuaPlus::LuaState* state);
	// 显示宠物
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//设置选中的自己摊位上的宠物
	INT SetSelectPet(LuaPlus::LuaState* state);

public:
	static StallSale s_StallSale;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBuy
{
public:
	//的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//导出本地税率
	INT GetTradeTax(LuaPlus::LuaState* state);
	//打开别人的摊位
	INT OpenStall(LuaPlus::LuaState* state);
	//获得指定位置的定价
	INT GetPrice(LuaPlus::LuaState* state);
	//购买物品
	INT BuyItem(LuaPlus::LuaState* state);
	//打开摊位信息
	INT OpenMessageBuy(LuaPlus::LuaState* state);
	//获得摊位名
	INT GetStallName(LuaPlus::LuaState* state);
	//获得广告语
	INT GetAdvertise(LuaPlus::LuaState* state);
	//关闭
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//导出自己的GUID
	INT GetGuid(LuaPlus::LuaState* state);
	//导出摊主的名字
	INT GetStallerName(LuaPlus::LuaState* state);
	// 获得宠物的个数
	INT GetPetNum(LuaPlus::LuaState* state);
	// 获得宠物
	INT EnumPet(LuaPlus::LuaState* state);
	// 获得缺省页
	INT GetDefaultPage(LuaPlus::LuaState* state);
	//购买宠物
	INT BuyPet(LuaPlus::LuaState* state);
public:
	static StallBuy s_StallBuy;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBbs
{
public:
	//获得信息的条数
	INT GetMessageNum(LuaPlus::LuaState* state);
	//获得
	INT EnumMessage(LuaPlus::LuaState* state);
	//添加一条新的信息
	INT AddMessage(LuaPlus::LuaState* state);
	//回复一条信息
	INT ReplyMessage(LuaPlus::LuaState* state);


public:
	static StallBbs s_StallBbs;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class MissionReply
{
public:
	//的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//打开宠物列表
	INT OpenPetFrame(LuaPlus::LuaState* state);
	//点击"Continue的操作"
	INT OnContinue(LuaPlus::LuaState* state);
	//
	INT EnumItem(LuaPlus::LuaState* state);


public:
	static MissionReply s_MissionReply;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class NpcShop
{
public:
	//关闭NpcShop
	INT Close(LuaPlus::LuaState* state);
	//获得商店NpcID
	INT GetNpcId(LuaPlus::LuaState* state);
	//获得商店物品价格
	INT GetItemPrice(LuaPlus::LuaState* state);
	//商店买入
	INT NpcShopBuy(LuaPlus::LuaState* state);
	//商店卖出
	INT NpcShopSell(LuaPlus::LuaState* state);

	INT GetItemPriceType(LuaPlus::LuaState* state);

public:
	static NpcShop s_NpcShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class QuickChange
{
public:
	// 一键换装：换装
	INT Lua_ChangeSuit( LuaPlus::LuaState* state);

	// 一键换装：保存命名
	INT Lua_SaveSuitName( LuaPlus::LuaState* state);

	// 一键换装：保存装备
	INT Lua_SaveSuit( LuaPlus::LuaState* state);

	// 一键换装：读取装备
	INT Lua_ReadSuit( LuaPlus::LuaState* state);

	// 一键换装：设置当前套装
	INT Lua_SetCurrentSuit( LuaPlus::LuaState* state);

	//一键换装：得到套装的名字
	INT Lua_GetSuitName( LuaPlus::LuaState* state );

public:
	static QuickChange s_QuickChange;
	static LuaPlus::LuaObject* s_pMetaTable;

private:
};

// 玩家商店
class PlayerShop
{
public:
	INT CreateShop(LuaPlus::LuaState* state);
	// 获得商店的数量
	INT GetShopNum(LuaPlus::LuaState* state);
	// 
	INT EnumShop(LuaPlus::LuaState* state);
	// 
	INT AskOpenShop(LuaPlus::LuaState* state);
	// 
	INT EnumItem(LuaPlus::LuaState* state);
	// 
	INT EnumShopInfo(LuaPlus::LuaState* state);
	// 
	INT GetSelectIndex(LuaPlus::LuaState* state);
	// 从玩家商店取回物品到自己的背包
	INT RetackItem(LuaPlus::LuaState* state);
	//
	INT InputMoney(LuaPlus::LuaState* state);
	//上架（下架）
	INT UpStall(LuaPlus::LuaState* state);
	//
	INT	SetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT	GetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT DealMoney(LuaPlus::LuaState* state);
	// 向服务器提交
	INT ApplyMoney(LuaPlus::LuaState* state);
	//
	INT InfoMoney(LuaPlus::LuaState* state);
	//
	INT GetStallNum(LuaPlus::LuaState* state);
	//
	INT BuyItem(LuaPlus::LuaState* state);
	//
	INT IsSelectOnSale(LuaPlus::LuaState* state);
	//下架
	INT DownSale(LuaPlus::LuaState* state);
	//清空当前的选择
	INT ClearSelect(LuaPlus::LuaState* state);
	//获得金钱
	INT GetMoney(LuaPlus::LuaState* state);
	//获得商业指数
	INT GetCommercialFactor(LuaPlus::LuaState* state);
	//获得店名
	INT GetShopInfo(LuaPlus::LuaState* state);
	//检查一个Stall是否Open
	INT IsOpenStall(LuaPlus::LuaState* state);
	//开张一个柜台
	INT OpenStall(LuaPlus::LuaState* state);
	//请求一个Stall的数据
	INT AskStallData(LuaPlus::LuaState* state);
	//宠物
	INT EnumPet(LuaPlus::LuaState* state);
	//设置当前选中的宠物
	INT SetSelectPet(LuaPlus::LuaState* state);
	//获得当前能开的商店的类型
	INT GetCanOpenShopType(LuaPlus::LuaState* state);
	//获得当前选中的宠物Index
	INT SetCurSelectPetIndex(LuaPlus::LuaState* state);
	//获得选择物品或者宠物的价格
	INT GetObjPrice(LuaPlus::LuaState* state);
	//修改商店的内容
	INT Modify(LuaPlus::LuaState* state);
	//获得选中的物品（宠物）的名称
	INT GetObjName(LuaPlus::LuaState* state);
	//显示宠物详细信息
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//清除现在选择的 Position,,在进行操作的时候，可以使用检测这个值是不是为－1 来标志是不是界面有选中物品
	INT ClearSelectPos(LuaPlus::LuaState* state);
	//盘出商店
	INT Transfer(LuaPlus::LuaState* state);
	// 查看当前的商店是不是处于盘店状态
	INT IsSaleOut(LuaPlus::LuaState* state);
	// 购买商店
	INT BuyShop(LuaPlus::LuaState* state);
	//关闭
	INT CloseShop(LuaPlus::LuaState* state);
	//打开（关闭）Message
	INT OpenMessage(LuaPlus::LuaState* state);
	//获得Message的数量
	INT GetMessageNum(LuaPlus::LuaState* state);
	//当前页的Message的数量
	INT GetCurPageMessageNum(LuaPlus::LuaState* state);
	//获得Message
	INT EnumMessage(LuaPlus::LuaState* state);
	//操作合伙人
	INT DealFriend(LuaPlus::LuaState* state);
	//获得合伙人的数量
	INT GetFriendNum(LuaPlus::LuaState* state);
	//获得合伙人
	INT EnumFriend(LuaPlus::LuaState* state);
	//扩张（缩减）店铺
	INT ChangeShopNum(LuaPlus::LuaState* state);
	// 获得访问的商店管理Npc的当前场景ID，（用来做CareObject）
	INT GetNpcId(LuaPlus::LuaState* state);

public:
	static PlayerShop s_PlayerShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

// 好友相关
class Friend
{
public:

	// 关闭
	INT Close(LuaPlus::LuaState* state);
	
	// 查询名字
	INT EnumName(LuaPlus::LuaState* state);
	
	// 打开右键菜单
	INT OpenMenu( LuaPlus::LuaState* state);
	
	INT AskTeam(LuaPlus::LuaState* state);

	// 打开分组
	INT OpenGrouping( LuaPlus::LuaState* state);

	// 创建新组
	INT CreateGrouping( LuaPlus::LuaState* state);
	
	// 显示好友相关的输入对话框
	INT ShowFriendInputDlg( LuaPlus::LuaState* state);

	// 显示添加好友对话框
	INT ShowAddNewFriendDlg( LuaPlus::LuaState* state);

	// 得到有多少条历史消息
	INT GetHistroyNumber( LuaPlus::LuaState* state);
	// 得到历史数据
	INT GetHistroyData(LuaPlus::LuaState* state);

	// 界面选择的好友列表
	INT	SetCurrentTeam( LuaPlus::LuaState* state );
	INT GetCurrentTeam( LuaPlus::LuaState* state );
	
	// 拉人界面的一个“同意”和“不同意”处理
	INT CallOf(LuaPlus::LuaState* state);
	// 点击显示心情的操作
	INT ViewFeel(LuaPlus::LuaState* state);
	// 测试心情是不是在自己的头顶
	INT IsMoodInHead(LuaPlus::LuaState* state);

	// 根据名字判断是否是好友
	INT IsPlayerIsFriend(LuaPlus::LuaState* state);

	// 设置当前选择
	INT SetCurrentSelect(LuaPlus::LuaState* state);

	// 获取自定义组的信息
	INT GetNewGroupInfo(LuaPlus::LuaState* state);


	// 打开搜索界面
	INT OpenSearcher(LuaPlus::LuaState* state);


public:

	static Friend s_Friend;
	static LuaPlus::LuaObject* s_pMetaTable;
	
	// 当前选中的组，
	int		m_nCurTeam; 
	
	Friend()
	{
		m_nCurTeam = 0; 
	};

};	// class friend

// 好友查找
class FriendSearcher
{
public:

	static FriendSearcher s_FriendSearcher;
	static LuaPlus::LuaObject* s_pMetaTable;

	FriendSearcher()
	{
	};

public:

	// 清空好友列表
	INT ClearFingerList( LuaPlus::LuaState* state );
	// 获取页数
	INT GetFriendPageNumber( LuaPlus::LuaState* state );

	// 获取一页中的好友数量
	INT GetFriendNumberInPage( LuaPlus::LuaState* state );
	// 获取一页中选择的好友参数
	INT GetFriendFromPage( LuaPlus::LuaState* state );

	// 通过id搜索
	INT FriendSearchByID( LuaPlus::LuaState* state );
	// 通过名字搜索
	INT FriendSearchByName( LuaPlus::LuaState* state );
	// 
//	INT FriendSearchByNameAndId( LuaPlus::LuaState* state );
	// 高级搜索
	// nPosition， 职业， 帮会， 性别， 等级下限， 等级上限
	INT FriendSearchAdvance( LuaPlus::LuaState* state );


private:

};	// class FriendSearcher


// 每日提醒功能
class DaytipsData
{
public:
	static DaytipsData s_DaytipsData;
	static LuaPlus::LuaObject* s_pMetaTable;

	DaytipsData();
	~DaytipsData();

	INT FillCycleMissionList(LuaPlus::LuaState* state);	
	INT FillFixedMissionList(LuaPlus::LuaState* state );  // 根据当前时间更新每日活动列表     20100716 BLL
	INT RecommendMission(LuaPlus::LuaState* state );
	INT GetWeek(LuaPlus::LuaState* state );

	// 获取一条活动记录  20100716 BLL
	INT GetMissionByIndex(LuaPlus::LuaState* state );	 

	// 获取今日活动个数  20100716 BLL
	INT GetMissionCount( LuaPlus::LuaState* state );	

	// 是否是活动任务 20100723 BLL
	VOID IsDayTipsMission( INT nMisID, BOOL& IsDayTipMission, BOOL& bTodayCanSee );

	// 获取一项活动的可接次数    20100728 BLL
	INT	    GetMissionCanDoCount( INT nMissionID );
private:

	struct DayTipsNode
	{
		_DBC_DAY_TIPS* m_pDayTipNode;	
		INT			   m_nTimeType;		// 根据当前时间设置的时间段类型，1，正在进行活动 2，即将进行活动，3，过期活动
		
		DayTipsNode()
		{
			m_pDayTipNode = NULL;
			m_nTimeType   = -1;	// 类型未知
		}
	};

	struct CompFunc
	{
		bool operator()( DayTipsNode p1, DayTipsNode p2) const
		{
			tm* tm1 = &p1.m_pDayTipNode->m_VailidTimeSlice.m_Begin;
			tm* tm2 = &p2.m_pDayTipNode->m_VailidTimeSlice.m_Begin;

			return (tm1->tm_hour * 60 + tm1->tm_min) <= (tm2->tm_hour * 60 + tm2->tm_min);
		}
	};

	typedef std::set< DayTipsNode, CompFunc > DayTipSet;

	DayTipSet			m_fixedMission;		// 显示到活动界面的数据队列

private:
	VOID	ResetDaytipSet();
	// 计算一时间点和时间段的关系
	INT		TimeSegmentValue(const VALID_TIME_SLICE* pSlice, const tm* pCheck, int* pValue);
	// 公共条件检测
	BOOL	CommonCheck(const _DBC_DAY_TIPS* pAttr, const CCharacterData* pData);

	// 是否是今日可见活动   20100716 BLL
	BOOL	IsTodayMission( const _DBC_DAY_TIPS* pDayTip );

	// 获取已参加一项活动的次数    20100716 BLL
	INT		GetMissionDoneCount( INT nMissionID );

	// 本周是本月第几周    20100716 BLL
	INT		GetWeekNO();

	// 加载今日活动列表	 20100719 BLL
	VOID    InitializeMissionList();

};


// 个人任务功能
class QuestLogData
{
public:
	static QuestLogData s_QuestLogData;
	static LuaPlus::LuaObject* s_pMetaTable;

	QuestLogData() {};

public:
	// 已接任务数量
	INT DoneMissionNum(LuaPlus::LuaState* state);

	// 可接任务数量
	INT CanDoMissionNum(LuaPlus::LuaState* state );

	// 当前已接任务数量
	INT DoneMissionNum_CurScene(LuaPlus::LuaState* state );

	// 当前可接任务数量
	INT CanDoMissionNum_CurScene(LuaPlus::LuaState* state );

	// 得到可接任务列表
	INT GetMissionList(LuaPlus::LuaState* state );

	INT GetMissionInfoByMissionID(LuaPlus::LuaState* state);

	INT GetMissionInfoByClientIndex(LuaPlus::LuaState* state);

	INT GetMissionRewordNumByIndex(LuaPlus::LuaState* state);

	INT GetMissionRewordInfo(LuaPlus::LuaState* state);

private:


private:



};

//---------------------------------------------------------------------------------------------------------------------------------------
	//
	// 登录信息导出函数.
	//
	class CGameProduce_Login
	{
	public: 

		CGameProduce_Login();
		~CGameProduce_Login();

	public:
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 选择服务器
		//

		//得到区域服务器的个数.
		INT GetAreaCount(LuaPlus::LuaState* state);
		
		//得到区域服务器的名字.
		INT GetAreaName(LuaPlus::LuaState* state);

		//得到区域服务器的提供商.

		//得到区域服务器中, login server 的个数.
		INT GetAreaLoginServerCount(LuaPlus::LuaState* state);

		//得到区域服务器的信息
		INT GetAreaServerInfo(LuaPlus::LuaState* state);

		// 得到login server 的信息.
		INT GetAreaLoginServerInfo(LuaPlus::LuaState* state);

		// 选择一个服务器
		INT SelectLoginServer(LuaPlus::LuaState* state);

		// 自动选择一个与当前选择的网络供应商匹配的login server
		INT AutoSelLoginServer(LuaPlus::LuaState* state);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 帐号输入界面
		//

		// 连接到login server
		INT ConnectToLoginServer(LuaPlus::LuaState* state);

		// 退到服务器选择界面
		INT ExitToSelectServer(LuaPlus::LuaState* state);

		// 验证用户名和密码
		INT CheckAccount(LuaPlus::LuaState* state);

		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 人物选择界面
		//

		// 从人物选择界面切换到帐号输入界面
		INT ChangeToAccountInputDlgFromSelectRole(LuaPlus::LuaState* state);

		// 从人物选择界面切换到人物创建界面
		INT ChangeToCreateRoleDlgFromSelectRole(LuaPlus::LuaState* state);

		// 从人物创建界面切换到人物选择界面
		INT ChangeToSelectRoleDlgFromCreateRole(LuaPlus::LuaState* state);

		// 得到角色的个数
		INT GetRoleCount(LuaPlus::LuaState* state);

		// 得到角色的信息
		INT GetRoleInfo(LuaPlus::LuaState* state);

		// 得到角色的ui模型名字
		INT GetRoleUIModleName(LuaPlus::LuaState* state);

		// 创建角色
		INT CreateRole(LuaPlus::LuaState* state);

		// 删除角色
		INT DelRole(LuaPlus::LuaState* state);

		// 发送进入游戏消息
		INT SendEnterGameMsg(LuaPlus::LuaState* state);
		// 切换场景
		INT ChangeSceneMsg(LuaPlus::LuaState* state);

		// 询问是否删除角色
		INT DelRole_YesNo(LuaPlus::LuaState* state);

		// 询问是否退出游戏
		INT ExitGame_YesNo(LuaPlus::LuaState* state);

		// 询问是否退到帐号输入界面
		INT ExitToAccountInput_YesNo(LuaPlus::LuaState* state);

		// 删除选择的角色
		INT DelSelRole(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 关于头像部分.
		//

		// 得到女主角头像的个数
		INT GetWomanFaceCount(LuaPlus::LuaState* state);

		// 得到男主角头像的个数
		INT GetManFaceCount(LuaPlus::LuaState* state);

		// 得到女主角头像的名称
		INT GetWomanFaceName(LuaPlus::LuaState* state);

		// 得到男主角头像的名称
		INT GetManFaceName(LuaPlus::LuaState* state);

		// 得到主角头像的名称
		INT GetFaceName(LuaPlus::LuaState* state);

		// 设置头像id
		INT SetFaceId(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 关于脸形部分.
		//
		
		// 得到脸形model的个数
		INT GetFaceModelCount(LuaPlus::LuaState* state);

		// 得到脸形模型的名字
		INT GetFaceModelName(LuaPlus::LuaState* state);

		// 设置脸形模型id
		INT SetFaceModelId(LuaPlus::LuaState* state);
		INT CreateRotateRole(LuaPlus::LuaState* state);
		INT ChangeSelectRole(LuaPlus::LuaState* state);
		INT SelRotateRole(LuaPlus::LuaState* state);
		INT GetLastLoginAreaAndServerIndex(LuaPlus::LuaState* state);
		INT PlayAnimation(LuaPlus::LuaState* state);

		// 用于创建人物界面显示

		// 设置性别
		INT SetGender( LuaPlus::LuaState* state );
		// 设置职业
		INT SetProfession( LuaPlus::LuaState* state );



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 关于发形部分.
		//
		
		// 得到脸形model的个数
		INT GetHairModelCount(LuaPlus::LuaState* state);

		// 得到脸形模型的名字
		INT GetHairModelName(LuaPlus::LuaState* state);

		// 设置脸形模型id
		INT SetHairModelId(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 网络状态部分。
		//
		
		// 关闭网络连接。 
		INT CloseNetConnect(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 声音部分。
		//
		
		//  开始播放音乐。
		INT PlayMusic(LuaPlus::LuaState* state);

		//  停止播放音乐。
		INT StopMusic(LuaPlus::LuaState* state);

		/**重新加载window的脚本
		*/
		INT Lua_ReloadWindowScript(LuaPlus::LuaState* state);


	public:

		static CGameProduce_Login s_GameProduceLogin;
		static LuaPlus::LuaObject* s_pMetaTable;

	};	

	class MailAnalyze
	{
	public:
		MailAnalyze(void){};
		~MailAnalyze(void){};

	public:
		STRING			ReplaceItemElementInfo1( const STRING& strSrc );
		STRING			CreateItemElementInfo1( const STRING& strSrc );
	};


	class IBShop
	{
	public:
		//关闭IBShop
		INT IBClose(LuaPlus::LuaState* state);
		//商店买入
		INT IBShopBuy(LuaPlus::LuaState* state);
		//获得商店物品价格
		INT Lua_GetIBItemPrice(LuaPlus::LuaState* state);

		//得到IB商城内物品的操作对象
		INT	Lua_EnumActionFoIBShop(LuaPlus::LuaState* state);

		//得到IB商城内符合条件的物品的数量
		INT	Lua_GetIBShopAccordNum(LuaPlus::LuaState* state);

	public:
		//判断商城内的一个物品，是否符合过滤规则，是否被显示
		BOOL IsActionFilter( tActionItem* pActionItem, INT type, INT type2 );

	public:
		static IBShop s_IBShop;
		static LuaPlus::LuaObject* s_pMetaTable;
	};
};	//namespace SCRIPT_SANDBOX


