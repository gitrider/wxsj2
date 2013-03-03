
/**	GMGameInterface_Script.h
/**	
 *
 *	���ܣ�	
 *
 *	�޸ļ�¼��
 *			080306	Lua_ShowGuildInfoDlg
 *			
 *			080314	Friend::IsPlayerIsFriend
 *					Friend::SetCurrentSelect
 *					���� FriendSearcher �࣬ ����ʵ�ֺ��ѵ�����
 *			080317	ThrowToEnemyList
 *					OpenSearcher ����������
 *			080317	NpcShop::GetItemPrice
 *			080317	NpcShop::NpcShopBuy
 *			080317	NpcShop::NpcShopSell
 *			080403	DataPool�����������ϵͳ��ʾ
 *			080409  Lua_OpenEquipAll     wz
 *			080410	�޸�OpenMail, ����OpenMailBrowser, OpenMailEdit
 *			
 *			080404	���� DaytipsData �࣬ ����ʵ��ÿ�չ�������
 *			080414	���� QuestLogData �࣬ ����ʵ�ָ���������
 *			
 *			080404	���� DaytipsData �࣬ ����ʵ��ÿ�չ�������
 *			080414	���� QuestLogData �࣬ ����ʵ�ָ���������
 *			080424	���� QuickChange �࣬ ����ʵ��һ����װ
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


	//��½�����У����ڵ��ȷ����Ĳ���(-- for debug)
	INT	Lua_ServerLogin(LuaPlus::LuaState* state);

	//Ҫ���˳�����
	INT Lua_QuitApplication(LuaPlus::LuaState* state);

	//Ҫ���/�رռ�����
	INT	Lua_ToggleSkillBook(LuaPlus::LuaState* state);
	//Ҫ��򿪼�����
	INT Lua_OpenSkillBook(LuaPlus::LuaState* state);

	//Ҫ���/�رճ������
	INT	Lua_TogglePetPage(LuaPlus::LuaState* state);

	//Ҫ�������ܽ���
	INT Lua_OpenLifePage(LuaPlus::LuaState* state);

	//Ҫ�����ͨ���ܽ���
	INT Lua_OpenCommonSkillPage(LuaPlus::LuaState* state);
	//�򿪰���
	INT Lua_ToggleContainer(LuaPlus::LuaState* state);

	//������
	INT Lua_ToggleMission(LuaPlus::LuaState* state);

	//�ر����ж����˵�
	INT Lua_CloseSecondMenu(LuaPlus::LuaState* state);

	//�����ʾ��Ϣ
	INT Lua_PushDebugMessage(LuaPlus::LuaState* state);

	// ����һ����װ��װ��Ϣ
	INT Lua_UpdateChangeSuit(LuaPlus::LuaState* pState);			
	//�õ�ĳ������������Ŀ
	INT	Lua_GetActionNum(LuaPlus::LuaState* state);

	//�������ݳ��ڴӷ���������������ʦ��������Ϣ
	//INT Lua_GetMenpaiInfo(LuaPlus::LuaState* state);

	//�����ķ�������Ҫ�Ľ�Ǯ
	INT Lua_GetUplevelXinfaSpendMoney(LuaPlus::LuaState* state);

	//�����ķ�������Ҫ�ľ���ֵ
	INT Lua_GetUplevelXinfaSpendExp(LuaPlus::LuaState* state);

	//������ǰ�ķ��ȼ�
	INT Lua_GetXinfaLevel(LuaPlus::LuaState* state);

	// ��ȡ��ǰʱ��
	INT Lua_GetCurrentTime( LuaPlus::LuaState* state);

	//��õ�ǰ�����λ�ã���Ļ���꣩
	INT Lua_GetCurMousePos( LuaPlus::LuaState* state);

	//��������
	//INT Lua_MenpaiFrameJoin(LuaPlus::LuaState* state);

	//ѧϰ�����ķ� 
	INT lua_SkillsStudyFrame_study(LuaPlus::LuaState* state);

	//�õ���������
	INT	Lua_EnumAction(LuaPlus::LuaState* state);
		//�õ���������
	INT	Lua_EnumActionForEquip(LuaPlus::LuaState* state);
	//�Ӽ��� id �õ�CActionItem
	INT	Lua_GetAction_SkillID(LuaPlus::LuaState* state);

	//����Ʒ id �õ�CActionItem
	INT	Lua_GetAction_ItemID(LuaPlus::LuaState* state);

	//��ʾ�Ҽ��˵�
	INT	Lua_ShowContexMenu(LuaPlus::LuaState* state);

	//���񴰿�ѡ���
	INT Lua_QuestFrameOptionClicked(LuaPlus::LuaState* state);

	//�����Ƿ�Show��״̬
	INT	Lua_IsWindowShow(LuaPlus::LuaState* state);

	//��ʼ�ϳ�
	INT Lua_ComposeItem_Begin(LuaPlus::LuaState* state);

	//ȡ���ϳ�
	INT Lua_ComposeItem_Cancel(LuaPlus::LuaState* state);

	//��������
	INT Lua_QuestFrameAcceptClicked(LuaPlus::LuaState* state);

	//л������
	INT Lua_QuestFrameRefuseClicked(LuaPlus::LuaState* state);

	//��������
	INT Lua_QuestFrameMissionContinue(LuaPlus::LuaState* state);

	//�������
	INT Lua_QuestFrameMissionComplete(LuaPlus::LuaState* state);

	//�������
	INT Lua_QuestFrameMissionAbnegate(LuaPlus::LuaState* state);

	//Ҫ���/�رպϳɽ���
	INT	Lua_ToggleComposeWnd(LuaPlus::LuaState* state);
	//ö�����е��䷽
	INT	Lua_EnumMethod(LuaPlus::LuaState* state); 
	// �õ�ĳһ���䷽������
	INT Lua_GetMethodNumber( LuaPlus::LuaState* state); 
	//������Ϣ���
	INT Lua_AxTrace(LuaPlus::LuaState* state);

	// ����ID�õ���������
	INT Lua_GetSceneNameFromID( LuaPlus::LuaState* state );

	// �õ���ǰ�����óߴ�
	INT Lua_GetSceneSize( LuaPlus::LuaState* state); 

	// �õ���ǰ����id
	INT Lua_GetSceneID( LuaPlus::LuaState* state); 

	// �õ���ǰС��ͼ�óߴ�
	INT Lua_GetMinimapSize( LuaPlus::LuaState* state);

	// ����������Ϣ
	INT Lua_SendChatMessage( LuaPlus::LuaState* state);

	INT Lua_GetCurChatProperty( LuaPlus::LuaState* state );
	// �رմ򿪴��ͼ
	INT Lua_ToggleLargeMap( LuaPlus::LuaState* state );
	INT Lua_ToggleSceneMap( LuaPlus::LuaState* state );
	INT Lua_OpenMinimap( LuaPlus::LuaState* state );
	INT Lua_UpdateMinimap( LuaPlus::LuaState* state );

	// �رմ��ͼ
	INT Lua_CloseLargeMap( LuaPlus::LuaState* state );

	// �رճ�����ͼ
	INT Lua_CloseSceneMap( LuaPlus::LuaState* state );

	// ���³�����ͼ�������ͼ�б� (_WX@7)
	INT Lua_UpdateLargeMapList( LuaPlus::LuaState* state );
	INT Lua_UpdateSceneMapList( LuaPlus::LuaState* state );

	// ö�ٴ��ͼ�б��е��б������
	INT Lua_LargeMapListNum( LuaPlus::LuaState* state );
	// ö�ٳ�����ͼ�б��е��б������
	INT Lua_SceneMapListNum( LuaPlus::LuaState* state );

	// ö�ٴ��ͼ�б��е�����
	INT Lua_LargeMapList( LuaPlus::LuaState* state );

	// ö�ٳ�����ͼNPC�б��е�����
	INT Lua_SceneMapList( LuaPlus::LuaState* state );


	// ��������ͼ�б����������

	// �ұ�
	INT Lua_ResetCamera( LuaPlus::LuaState* state );
	//�ű�����Կ���������ɫ�Ĺ��ܺ���

	//�򿪹ر����˽��׽���
	INT Lua_OpenBooth( LuaPlus::LuaState* state );
	INT Lua_CloseBooth( LuaPlus::LuaState* state);
	INT Lua_RepairAll( LuaPlus::LuaState* state);
	INT Lua_RepairOne( LuaPlus::LuaState* state);
	INT Lua_CancelRepairState( LuaPlus::LuaState* state);
	INT Lua_GetCurrentSceneName( LuaPlus::LuaState* state );
	INT Lua_GetOperationType( LuaPlus::LuaState* state );
	INT Lua_GetShopType( LuaPlus::LuaState* state );


	//�򿪹رճƺŽ���
	INT Lua_OpenTitleList( LuaPlus::LuaState* state);
	INT Lua_CloseTitleList( LuaPlus::LuaState* state); 

	INT Lua_OpenWindow( LuaPlus::LuaState* state);
	INT Lua_CloseWindow( LuaPlus::LuaState* state); 
	INT Lua_ToggleWindow( LuaPlus::LuaState* state); 
	INT Lua_SplitUnifiedString( LuaPlus::LuaState* state);

	INT Lua_OpenCharacter( LuaPlus::LuaState* state );

	INT Lua_OpenEquip( LuaPlus::LuaState* state );
	// �򿪸���װ�� �������� ���˳ƺ�  һ����װ �������  ��������
	INT	Lua_OpenEquipAll( LuaPlus::LuaState* state );

	// �رո���װ�� �������� ���˳ƺ�  һ����װ �������  ��������
	INT	Lua_CloseEquipAll( LuaPlus::LuaState* state );
	INT Lua_GetActionInfo( LuaPlus::LuaState* state );

	// ��װ������
	INT Lua_OpenSelfEquip( LuaPlus::LuaState* state );

	//�ر�װ�� ��� ��װ ���� �ƺ� ����ui
	INT   Lua_EquipAllCloseUI(LuaPlus::LuaState* state);

	//�ر�װ�� ��� ��װ ���� �ƺ� ����ui
	INT   Lua_EquipAllOpenUI(LuaPlus::LuaState* state);

	//������ݼ�֮���ж��Ƿ���Խ���һ����װ
	INT	Lua_IsEquipExChange(LuaPlus::LuaState* state);

	//����װ�� ��� ��װ ���� �ƺ� ����uiλ��
	
	INT   Lua_UpdataEquipAllPos(LuaPlus::LuaState* state);
	//��������
	INT Lua_AskLevelUp(LuaPlus::LuaState* state);
	//����gmָ��
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

	// ����UI�����
	INT Lua_Set_UICamera(LuaPlus::LuaState* state);

	INT Lua_SaveAutoAttributePoint( LuaPlus::LuaState* state);
	INT Lua_GetAutoAttributePoint( LuaPlus::LuaState* state);

//--------------------------------------------------------------------------
// ������

	INT Lua_ShowTeamFuncMenu(LuaPlus::LuaState* state);		// ��Ա��ӳ��Ĺ��ܲ˵�.
	INT Lua_ShowTeamInfoDlg(LuaPlus::LuaState* state);		// ��ʾ������Ϣ�Ի���.
	INT Lua_FlashTeamButton(LuaPlus::LuaState* state);		// ��˸��ť.
	INT Lua_SetCurSelMember(LuaPlus::LuaState* state);		// ���õ�ǰѡ��Ķ�Ա

// ������
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// ���

	// ��ʾ�������ȷ�϶Ի���
	INT Lua_ShowGuildInfoDlg(LuaPlus::LuaState* state);

//--------------------------------------------------------------------------
	// ��������������а�
	INT Lua_SendTopList(LuaPlus::LuaState* state);

	// ��Lua�ű�����Ϣ
	INT Lua_PushEvent(LuaPlus::LuaState* state);
	
	// �õ�ٽ�������
	//INT Lua_GetTopListName(LuaPlus::LuaState* state);

	// ����һ��ȫ�ֱ���
	INT Lua_SetGlobalInteger(LuaPlus::LuaState* state);
	// ���һ��ȫ�ֱ���
	INT Lua_GetGlobalInteger(LuaPlus::LuaState* state);

	// Ҫװ������ϸ��Ϣ
	INT Lua_AskEquipDetial(LuaPlus::LuaState* state);

	// ������Ʒ
	INT Lua_DiscardItem(LuaPlus::LuaState* state);

	// ȡ������
	INT Lua_DiscardItemCancelLocked(LuaPlus::LuaState* state);

	// ��������
	INT Lua_PlaySound(LuaPlus::LuaState* state);

	// �ر����д���
	INT	Lua_CloseAllWindow( LuaPlus::LuaState* state);

	// �򿪻�ر�ĳ���ض��Ĵ���(_WX@7)
	//INT Lua_CloseOneWindow( LuaPlus::LuaState* state);

	// target���ڵ����Ĳ˵�
	INT	Lua_OpenTargetMenu( LuaPlus::LuaState* state);

	INT Lua_GetMousePos( LuaPlus::LuaState* state);

	// ��ʾϵͳ��ʾ�Ի��� 
	INT Lua_ShowSystemTipInfo( LuaPlus::LuaState* state);

	// �򿪸��Ķ�������Ի��� 
//	INT Lua_OpenChangeMinorPasswordDlg( LuaPlus::LuaState* state);

	// �򿪽������롣 
//	INT Lua_OpenUnLockeMinorPasswordDlg( LuaPlus::LuaState* state);

	// ������������档 
//	INT Lua_OpenSetMinorPasswordDlg( LuaPlus::LuaState* state);

	// �������롣 
	INT Lua_SendSetMinorPassword( LuaPlus::LuaState* state);

	// �޸����롣 
	INT Lua_SendModifyMinorPassword( LuaPlus::LuaState* state);

	// �������롣 
	INT Lua_UnLockMinorPassword( LuaPlus::LuaState* state);

	// ǿ�ƽ������롣 
	INT Lua_ForceUnLockMinorPassword( LuaPlus::LuaState* state);

	// �޸����롣 
	INT Lua_ModifyMinorPassword( LuaPlus::LuaState* state);

	// ��ȫʱ�䡣 
	INT Lua_SetPasswordProtectTime( LuaPlus::LuaState* state);

	// �����������
//	INT Lua_CheckInputPassword( LuaPlus::LuaState* state );

	// ���������״̬
//	INT Lua_SetInputPasswordState( LuaPlus::LuaState* state );
//	INT Lua_GetInputPasswordState( LuaPlus::LuaState* state );


	//// �������֡����������ֵ�ID  
	//INT Lua_PlayMusic( LuaPlus::LuaState* state);
	//
	//// ֹͣ���֡����������ֵľ��  
	//INT Lua_StopMusic( LuaPlus::LuaState* state);//

	// ��8�������ܰ�ť Nick 2007.11.22
	INT Lua_OpenMainFuncBtn( LuaPlus::LuaState* state);
 
	// �򿪹ر�һ����װ����
	INT Lua_OpenQuickChangeEquip( LuaPlus::LuaState* state);

	// װ����ס� 
	INT Lua_Equip_Add_Hole( LuaPlus::LuaState* state);
 
 	// װ����������� 
	INT Lua_GetAddHoleCondition( LuaPlus::LuaState* state);
   
	// װ���׵���Ŀ�� 
	INT Lua_GetEquipHoleNumForAddHole( LuaPlus::LuaState* state) ;


	// ����������Ϣ
	INT   Lua_AddStart_Equip_Info(LuaPlus::LuaState* state) ;

	INT   Lua_AddStartAutoMatic_Attr_Info(LuaPlus::LuaState* state);

	INT   Lua_Get_Equip_StarInfo(LuaPlus::LuaState* state);


	//װ������
	INT	Lua_Equip_Refining( LuaPlus::LuaState* state );

	//װ����ħ
	INT	Lua_Equip_Fumo( LuaPlus::LuaState* state );

	//װ����ħ
	INT	Lua_Equip_Tuimo( LuaPlus::LuaState* state );

	
	// ����������Ϣ
	INT   Lua_SpecialRepair_Info(LuaPlus::LuaState* state);  

	// ����ȷ��ȫ��������� 
	INT   Lua_PopRepairSureWnd( LuaPlus::LuaState* state) ;
 


	//ȡ�ô���ͺϳ�װ�� �䷽�Ĳ��ϡ������ȡ�á� 
	INT  Lua_GetPackFormulation(LuaPlus::LuaState* state);

	// �䷽����
	INT  Lua_EnumPackFormulationNum(LuaPlus::LuaState* state);

	//ö����ҿ����䷽�䷽  
	INT  Lua_EnumPackProCanUse(LuaPlus::LuaState* state) ;


	//ö���䷽���� ���ڴ���ͺϳ�װ������
	INT  Lua_EnumPackFormulation(LuaPlus::LuaState* state);

	//��װ���������
	INT     Lua_Open_Rework_Equip(LuaPlus::LuaState* state);
	//��ĸ��������Ϣ
	INT     Lua_GetReWorkMatInfo(LuaPlus::LuaState* state);

	// ---����װ������Ĳ��������͵ȼ�
	INT     Lua_EquipDismantle_Material_Info(LuaPlus::LuaState* state);

	//ȡ����� �Ƿ�����Ӧ�ȼ��Ĳ���                          ����ͺϳ�װ�� 
	INT  Lua_GetPackForMatOnGrade(LuaPlus::LuaState* state);
	//ȡ����� ��Ӧ�ȼ��Ĳ���icon                  
	INT  Lua_GetPackForMatIconOnGrade(LuaPlus::LuaState* state);

	//ȡ����� �Ƿ�����Ӧ�ȼ��Ĳ���  ���ڸ���                         

	INT  Lua_GetPackForMatOnGradeReWork(LuaPlus::LuaState* state);

	INT  CleanItemForTip(LuaPlus::LuaState* state);

	INT  CreatItemForTip(LuaPlus::LuaState* state);

	INT  Lua_IB_Shop(LuaPlus::LuaState* state);// add by gh
 
class Character
{
public:

	// ��ʾĿ��ͷ����Ҽ��˵�
	INT	ShowTargetContexMenu(LuaPlus::LuaState* state);

	// ȡ��HP return (percent)
	INT		GetHPPercent(LuaPlus::LuaState* state);
	//ȡ��MP return (percent)
	INT		GetMPPercent(LuaPlus::LuaState* state);
	//ȡ��Rage return (percent)
	INT		GetRagePercent(LuaPlus::LuaState* state);

	INT		GetScenePos( LuaPlus::LuaState* state);

	
	//----------------------------------------------------------------------
	// ������

	//�������
	INT	SendTeamRequest(LuaPlus::LuaState* state);

	//����������
	INT	SendTeamApply(LuaPlus::LuaState* state);

	// ������
	//----------------------------------------------------------------------

	// ����
	INT		Follow(LuaPlus::LuaState* state);
	
// ���е�����
	// �õ���ɫ��λ��
	virtual INT		GetPos( LuaPlus::LuaState* state);
	//ȡ������
	virtual INT		GetName(LuaPlus::LuaState* state);
	//�����Ƿ���� "target", "pet", ...
	virtual INT		IsPresent(LuaPlus::LuaState* state);
	// �õ���ɫ���ϵ�buff������
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );
	

	// �õ��ȼ�
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

	 // ��ǰ��target �Ƿ��Ƕ���
	 INT	IsTargetTeamMember(LuaPlus::LuaState* state);

	 // ��ǰ��target �Ƿ�Ϸ� 
	 INT	IsTargetValide(LuaPlus::LuaState* state);

	 // main target ������ʾ������Ϣ�� 

	// ��ʾ����
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// ��ʾhp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// ��ʾmp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// ��ʾŭ��
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// ��ʾ�ȼ�
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// ��ʾͷ�����֡�
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);


protected:
	Character() {};
	virtual ~Character() {};

private:
	CObject_Character*		_GetTargetAsCharacter(VOID);

public:
	//��ѡ�����
	static Character s_Target;
};

//�ű�����Կ�����ҵĹ��ܺ���
class PlayerMySelf : public Character
{
public:
	
	//��ý�ɫ��ǰ�Ի���NPC id
	INT	GetCurDialogNpcId(LuaPlus::LuaState*state);

	// �õ���ǰ�ĳƺ�.
	INT	GetCurTitle(LuaPlus::LuaState* state);

	// ��ѡ��ƺ�
	INT SetNullAgname(LuaPlus::LuaState* state);
	
	//----------------------------------------------------------------------------------------------------------------------------------------------
	// ������
	
	// �뿪����
	INT LeaveTeam(LuaPlus::LuaState* state);	

	// ��ɢ����
	INT DismissTeam(LuaPlus::LuaState* state);	

	// ɾ��һ��ѡ�еĶ�Ա
	INT KickTeamMember(LuaPlus::LuaState* state);
	
	// ɾ����ǰѡ�ж�Ա
	INT KickTeamMemberByIndex(LuaPlus::LuaState* state);			

	//�Խ�����
	INT	CreateTeamSelf(LuaPlus::LuaState* state);

	//�����ӳ���Ϣ
	INT	AppointLeader(LuaPlus::LuaState* state);

	// �Ƿ�����Ӹ���״̬
	INT InTeamFollowMode(LuaPlus::LuaState* state);

	// ȡ������
	INT StopFollow(LuaPlus::LuaState* state);

	// ��Ӹ���
	INT TeamFrame_AskTeamFollow(LuaPlus::LuaState* state);

	// ͬ��������(����������)
	INT	SendAgreeJoinTeam(LuaPlus::LuaState* state);
	// �ܾ��������(����������)
	INT SendRejectJoinTeam(LuaPlus::LuaState* state);

	// ͬ��������(��������������)
	INT	SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state);

	// �ܾ��������(��������������)
	INT SendRejectJoinTeam_Apply(LuaPlus::LuaState* state);

	// ͬ��������(��Ա����ĳ�˼������)
	INT	SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// �ܾ��������(��Ա����ĳ�˼������)
	INT SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// ͬ�������Ӹ���ģʽ
	INT SendAgreeTeamFollow(LuaPlus::LuaState* state);

	// �ܾ�������Ӹ���ģʽ
	INT SendRefuseTeamFollow(LuaPlus::LuaState* state);

	// �õ���Աguid
	INT GetTeamMemberGUID(LuaPlus::LuaState* state);

	// �޸����״̬����Ʒʰȡģʽ
	INT ModifyTeamAllocationMode(LuaPlus::LuaState* state);

	// ������״̬����Ʒʰȡģʽ
	INT GetTeamAllocationMode(LuaPlus::LuaState* state);

	// ������
	//-----------------------------------------------------------------------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------------------------------------------------------------------
	// װ�����Խ���
	//

	// �����ֶ��������Ե��� 
	INT SendAskManualAttr(LuaPlus::LuaState* state);


	//
	// װ�����Խ���
	//-----------------------------------------------------------------------------------------------------------------------------------------------



	virtual INT		GetName(LuaPlus::LuaState* state);
	virtual INT		GetData( LuaPlus::LuaState* state );
	// �õ���ɫ��ĳ������ܵ���Ϣ
	virtual INT		GetAbilityInfo( LuaPlus::LuaState* state );
	// �õ���ɫ�Ƿ�ѧϰ��ĳ�������
	virtual INT		GetAbilityStudy( LuaPlus::LuaState* state );
	// �õ���ɫ��ĳ��ս�����ܵ���Ϣ
	virtual INT		GetSkillInfo( LuaPlus::LuaState* state );
	// �������õ���ɫ��ĳ��ս�����ܵ���Ϣ Nick 2007.11.23
	virtual INT		GetIndexSkillInfo( LuaPlus::LuaState* state );
	// �õ��������� Nick 2007.11.23
	virtual INT		GetSkillNum( LuaPlus::LuaState* state );
	// �õ���ɫ��ĳ���ķ�����Ϣ
	virtual INT		GetXinfaInfo( LuaPlus::LuaState* state );
	// ����ѧϰ�����
	virtual INT		AskLeanAbility(LuaPlus::LuaState* state);

	// �õ���ɫ��λ��
	virtual INT		GetPos( LuaPlus::LuaState* state);
	// ��ɫ�Ƿ����
	virtual INT		IsPresent(LuaPlus::LuaState* state);

	//�ƺ����
	virtual INT		GetAgnameNum(LuaPlus::LuaState* state);
	virtual INT		GetAgnameType(LuaPlus::LuaState* state);
	virtual INT		EnumAgname(LuaPlus::LuaState* state);
	virtual INT		GetCurrentAgname(LuaPlus::LuaState* state);
	virtual INT		AskChangeCurrentAgname(LuaPlus::LuaState* state);
	virtual INT		UpdateTitleState(LuaPlus::LuaState* state);
	virtual INT		GetAgnameInfo(LuaPlus::LuaState* state);

	virtual INT		GetAllAgnameNum(LuaPlus::LuaState* state);
	virtual INT		GetTableAgnameInfo(LuaPlus::LuaState* state);

	
	// �õ���ɫ���ϵ�buff������
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );

	virtual INT		GetBuffIconNameByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffToolTipsByIndex( LuaPlus::LuaState* state );
	virtual INT		DispelBuffByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffTimeTextByIndex( LuaPlus::LuaState* state );

	// ����Ի������
	virtual INT		SendReliveMessage_OutGhost( LuaPlus::LuaState* state );
	virtual INT		SendReliveMessage_Relive( LuaPlus::LuaState* state );

	// �Ҽ��˵����
	virtual INT		ShowMySelfContexMenu(LuaPlus::LuaState* state);
	// ���ѡ���Լ�
	virtual INT		SelectMyselfAsTarget(LuaPlus::LuaState* state);

	// ���ö�������
	INT	SetSupperPassword(LuaPlus::LuaState* state);


	// ��ȡ���뱣��ϵͳ����ѡ��
	INT Lua_PasswordGetData( LuaPlus::LuaState* state );
	// �������뱣��ϵͳ����ѡ��
	INT Lua_SavePasswordSetup( LuaPlus::LuaState* state );
	// ��ȡĳ���͵�����״̬
	INT Lua_PasswordTypeGetState( LuaPlus::LuaState* state );


	// ����������
	INT Lua_CheckPasswdProcedure( LuaPlus::LuaState* state );
	// ������������������
	INT Lua_InputPasswdProcedure( LuaPlus::LuaState* state );


protected:

	// �����¼�����
	VOID ProtectEventHandling( INT nType );

	// ���װ���Ĳ�������
	INT			m_nQuestIndex;
	STRING		m_sQuestScript;

protected:

	PlayerMySelf() : m_nQuestIndex(-1), m_sQuestScript("")  {};
	virtual ~PlayerMySelf() {};

public:

	static PlayerMySelf s_MySelf;
};

//���ݳ�
class DataPool
{
public:
	//ȡ�õ�ǰ�Ի�npc����
	INT		GetNPCEventList_Num(LuaPlus::LuaState* state);
	INT		GetNPCEventList_Item(LuaPlus::LuaState* state);

	//-----------------------------------------------------------
	//������Ϣ
	INT		GetMissionInfo_Num(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Text(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Bonus(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Kind(LuaPlus::LuaState* state);

	// �Ƿ���ÿ�ջ���� 20100728 BLL 
	INT		GetMissionInfo_IsDayTip(LuaPlus::LuaState* state);

	// ��ȡÿ�ջ�ĿɽӴ�������ɴ��� 20100728 BLL 
	INT		GetMissionInfo_DayTipCount(LuaPlus::LuaState* state);

	//����������Ϣ
	INT		GetMissionDemand_Num(LuaPlus::LuaState* state);
	INT		CloseMissionFrame(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Text(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Item(LuaPlus::LuaState* state);
	//Continue
	INT		GetMissionContinue_Num(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Text(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Bonus(LuaPlus::LuaState* state);

	// ȡ��������б�
	INT		GetScoutMission(LuaPlus::LuaState* state);
	INT		SetScoutMission(LuaPlus::LuaState* state);
	INT		GetScoutMission_Num(LuaPlus::LuaState* state);

	// Npc�Զ���·
	INT		NpcAutoFindPath(LuaPlus::LuaState* state);

	// �ڳ�����ͼ���ڵ������ƶ�������λ��ʱ�������ǰ��Npc�Զ�Ѱ·״̬���������Զ�Ѱ·�Ĳ���ֵ	20100531 BLL
	INT		ResetNpcAutoFindPath(LuaPlus::LuaState* state);	

	// NPC�糡���Զ�Ѱ·
	INT		NpcOutSceneAutoFindPath(LuaPlus::LuaState* state);

	//��ҵ�ǰ������Ϣ
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
	//�����ҵ�ǰ�������ڳ�����ID ����
	INT		GetPlayerMission_SceneID(LuaPlus::LuaState* state );


	//ȡ���䷽����
	//ȡ���䷽������
	INT		GetPrescrList_Num(LuaPlus::LuaState* state);
	//ȡ���䷽������
	INT		GetPrescrList_Item(LuaPlus::LuaState* state);
	//ȡ���䷽����������ID
	INT		GetPrescrList_Item_LifeAbility(LuaPlus::LuaState* state);
	//ȡ���䷽�Ĳ�Ʒ
	INT		GetPrescrList_Item_Result(LuaPlus::LuaState* state);
	//ȡ���䷽����Ĳ���
	INT		GetPrescrList_Item_Requirement(LuaPlus::LuaState* state);
	//ȡ���䷽����Ҫ����
	INT		GetPrescrList_Item_NeedTool(LuaPlus::LuaState* state);
	//ȡ���䷽����ϸ����
	INT		GetPrescrList_Item_Desc(LuaPlus::LuaState* state);

	
	//������������ͣ�ȡ��������䷽�����еĵ�N���������
	INT		GetPrescrList_Item_ListPos(LuaPlus::LuaState* state);




	// ��û��ܵ���Ϣ 
	INT		GetBoothList_Num(LuaPlus::LuaState* state); // �õ��ж��ٸ�����
	INT		GetBoothGood_ID( LuaPlus::LuaState* state ); // �õ������id 
	INT		GetBoothGood_Name(LuaPlus::LuaState* state); // �õ����������
	INT		GetBoothGood_Price(LuaPlus::LuaState* state);// �õ�����ļ۸�
	

	//-----------------------------------------------------------------------------------------------------------------------------------
	// ������. 
	
	//------------------------------
	//��Щ��������ߵ�ͷ�������ʾ.
	INT		GetTeamMemGUIDByUIIndex(LuaPlus::LuaState* state);			// �õ���Ա����Ϣ
	INT		GetTeamMemInfo(LuaPlus::LuaState* state);			// �õ���Ա����Ϣ
	INT		GetTeamMemCount(LuaPlus::LuaState* state);			// �õ���Ա�ĸ���
	INT	    IsTeamLeader(LuaPlus::LuaState* state);				// �Ƿ��Ƕӳ�
	INT		SelectAsTargetByUIIndex(LuaPlus::LuaState* state);	// ͨ������ͷ��, ѡ�ж�����Ϊtarget
	INT		GetSelfGUID(LuaPlus::LuaState* state);
	INT		GetTeamMemPosByUIIndex(LuaPlus::LuaState* state);		//�õ���Ա��λ����Ϣ�����ڿ糡��Ѱ·



	//------------------------------
	// ���������.

	//��Щ�����������Ϣ����ʹ��.  ����ӿ�.   
	INT		GetInviteTeamCount(LuaPlus::LuaState* state);		// �õ��������ĸ���.
	INT     GetInviteTeamMemberInfo(LuaPlus::LuaState* state);	// �õ��������Ϣ.	
	INT		GetInviteTeamMemberCount(LuaPlus::LuaState* state);	// �õ�ĳ���������Ķ�Ա�ĸ���.


	
	//-----------------------------
	// �򿪶�����Ϣ.

	// �Ƿ������״̬��
	INT		GetMyTeamFlag(LuaPlus::LuaState* state);

	// �õ���ǰ��������
	INT		GetMyTeamMemberCount(LuaPlus::LuaState* state);

	// �Լ������(��npc)��ľ��룬���� -1 ��ʾ������Ŀ�ꡣ
	INT		GetDistanceBySeverID(LuaPlus::LuaState* state);

	// �õ���һ�����ѵ�severid
	INT		GetAnotherTeamMemberSeverID(LuaPlus::LuaState* state);

	// �õ�clientid
	INT		GetClientIDBySeverID(LuaPlus::LuaState* state);

	
	// ���ڶ����е��Լ�����Ϣ, ��ͨ��������Ϣ����, ÿ�δ򿪽����ʱ��Ҫˢ���Լ�����Ϣ
	INT     SetSelfInfo(LuaPlus::LuaState* state);

	// �ӳ��򿪶�����Ϣʱ, ѡ�е�����
	INT     SetSelectTeamMember_Leader(LuaPlus::LuaState* state);

	// ͨ�������õ���Ա��Ϣ, ����Ա�򿪽���ʹ��.
	INT	GetTeamMemInfoByIndex(LuaPlus::LuaState* state);	// �õ���Ա����Ϣ

	// �õ���Ա��uiģ������
	INT	GetTeamMemUIModelNameByIndex(LuaPlus::LuaState* state);

	// �õ������ߵ�uiģ������
	INT GetApplyMemberUIModelName(LuaPlus::LuaState* state);		

	// �õ�����uiģ������
	INT GetInviteTeamMemberUIModelName(LuaPlus::LuaState* state);	

	// �õ���Աuiģ������
	INT	GetTeamMemUIModelName(LuaPlus::LuaState* state);		

	// ��Ա���Լ��Ƿ���ͬһ��������
	INT	IsTeamMemberInScene(LuaPlus::LuaState* state);		

	//����Լ��Ƿ��Ƕӳ�
	BOOL    GetMyTeamLeaderFlag( LuaPlus::LuaState* state);


	//------------------------------
    // �������˽���

	//��Щ�����������Ϣ����ʹ��.  ����ӿ�. 
	INT		GetApplyMemberCount(LuaPlus::LuaState* state);		// �õ������߸���.
	INT     GetApplyMemberInfo(LuaPlus::LuaState* state);		// �õ������ߵĸ���.		
	INT     SetCurSelApply(LuaPlus::LuaState* state);			// ���õ�ǰѡ���������.
	INT     EraseApply(LuaPlus::LuaState* state);				// ɾ��һ��������
	INT     ClearAllApply(LuaPlus::LuaState* state);			// ������е�������.



	//------------------------------
	// �������ݽӿ�.

	// ������ӶԻ���򿪱��.
	INT		SetTeamFrameOpenFlag(LuaPlus::LuaState* state);		// ���ö���Ի���򿪱��


	//--------------------------------------------------------------------------------------------------------------------------------------



	//��������
	INT		GetPacketItem( LuaPlus::LuaState* state );
	INT		GetPacketItemMaxNumber( LuaPlus::LuaState* state );
	INT     GetPacketItemFroId( LuaPlus::LuaState* state );
	INT     GetPacketItemAllNum( LuaPlus::LuaState* state );



/** ����ϵͳ�ӿ�
*/

	// �õ�ĳһ��ĺ�������
	INT		GetFriendNumber( LuaPlus::LuaState* state );
	// �õ�ĳһ��ĺ��ѵľ�������
	INT		GetFriend( LuaPlus::LuaState* state );

	// ���һ�����ѵ�ĳһ��
	INT		AddFriend( LuaPlus::LuaState* state );
	// ��ϵͳ���ɾ����������
	INT		AskDelFriend( LuaPlus::LuaState* state );
	// ȷ��ɾ��һ������
	INT		DelFriend( LuaPlus::LuaState* state );

	// �ŵ���ͬ���б���
	INT     ThrowToBlackList( LuaPlus::LuaState* state );
	INT		ThrowToFriendList( LuaPlus::LuaState* state );
	INT     ThrowToEnemyList( LuaPlus::LuaState* state );
	INT     ThrowToList( LuaPlus::LuaState* state );

	// ��������ͨ���Ҽ������Ҷ����Ϊ����
	INT		AddFriendByModel( LuaPlus::LuaState* state );

	// �޸ĸ�������
	INT		FriendSeting( LuaPlus::LuaState* state );

	// ��ʾ������Ϣ
	INT		ShowFriendInfo( LuaPlus::LuaState* state );
	// ���ýű��� ׼����ʾ��Ϣ
	INT		LuaShowFriendInfo( LuaPlus::LuaState* state );
	INT		ShowChatInfo(LuaPlus::LuaState* state);
	
	// ����
	INT		SetMood( LuaPlus::LuaState* state );
	INT		GetMood( LuaPlus::LuaState* state );

	INT		OpenFriendList(LuaPlus::LuaState* state);

	// ��ȡ�ʼ�����
	INT		GetMailNumber( LuaPlus::LuaState* state );
	// ��ȡ�ʼ�����
	INT		GetMail( LuaPlus::LuaState* state );

	//LAIM
	INT     MailAutoRestoreGet( LuaPlus::LuaState* state){return 0;}
	INT     ChatAutoRestoreGet( LuaPlus::LuaState* state){return 0;}
	INT		AutoRestoreGetContent( LuaPlus::LuaState* state){return 0;}

	// ���ʼ�����
	INT 	OpenMailBrowser(LuaPlus::LuaState* state);
	INT 	OpenMailEdit(LuaPlus::LuaState* state);
	// �����ʼ�
	INT		SendMail( LuaPlus::LuaState* state );

	INT		GetNextMail(LuaPlus::LuaState* state );

	// ͨ�����ֻ�ȡ����
	INT     GetFriendByName( LuaPlus::LuaState* state );

	// ����ʷ
	INT		OpenHistroy( LuaPlus::LuaState* state );

	// �����Ϣ����ʷ�����¼
	INT		AddChatHistoryMessage( LuaPlus::LuaState* state );

	// ȷ�����������ҵ������б�
	INT		SureAddFriend( LuaPlus::LuaState* state );

	// �ܾ���Ϊ����
	INT		RejectAddFriend( LuaPlus::LuaState* state );

	//	�ж�����Ƿ����Լ������ϵ��
	INT		IsTempFriendByName( LuaPlus::LuaState* state );

	//	�ж�����Ƿ����Լ��ĺ�������
	INT		IsInBlackListByName( LuaPlus::LuaState* state );

	//  ͨ��guid�ж�����Ƿ��ں�������
	INT		IsInBlackListByGuid( LuaPlus::LuaState* state );

	//	�ж�����Ƿ����Լ��ĳ����б���
	INT		IsInEnemyListByName( LuaPlus::LuaState* state);


//�ı䷢�ͷ�ɫ
	INT		Change_MyHairColor( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle_Item( LuaPlus::LuaState* state );
	INT		Get_MyHairColor( LuaPlus::LuaState* state );
	INT		Get_MyHairStyle( LuaPlus::LuaState* state );
//���String�Ƿ�Ϸ�
	INT		Check_StringCode( LuaPlus::LuaState* state );


//================
// ����ϵͳ��ʾ


	// ����¼�
	INT		SystemTip_AddEventInfo( LuaPlus::LuaState* state );
	// ɾ���¼�
	INT		SystemTip_DelEventInfo( LuaPlus::LuaState* state );

	// ��ȡĳһ�¼�����Ϣ
	INT		SystemTip_GetEventInfo( LuaPlus::LuaState* state );
	// ����ĳһ�¼���״̬
	INT		SystemTip_SetEventState( LuaPlus::LuaState* state );


	// ��ȡ�����¼�����
	INT		SystemTip_GetEventNum( LuaPlus::LuaState* state );

	INT		Lua_ReloadWindowScript(LuaPlus::LuaState* state);


public:

	INT		IsCanDoAction( LuaPlus::LuaState* state );
	INT		Change_RectifyColor( LuaPlus::LuaState* state );
	INT		Change_GetColorLumination(LuaPlus::LuaState* state);

public:

	// ��ʾ����
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// ��ʾhp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// ��ʾmp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// ��ʾŭ��
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// ��ʾ�ȼ�
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// ��ʾͷ�����֡�
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);

	INT	ToggleShowPetList(LuaPlus::LuaState* state);

	INT	GetNPCIDByServerID(LuaPlus::LuaState* state);

	INT GetUIColor(LuaPlus::LuaState* state);

	// ���ö�������
	INT	SetSupperPass(LuaPlus::LuaState* state);

	//��ʱ����
	INT TargetFrame_Update_BuffNum_Team(LuaPlus::LuaState* state);
	INT TargetFrame_Update_BuffInfo_Team(LuaPlus::LuaState* state);

public:

	static DataPool s_DataPool;
	static LuaPlus::LuaObject* s_pMetaTable;

};



// ��ǰ���ڶԻ����������ʦ
class AbilityTeacher
{
public:
	//ȡ�õ�ǰ��ʦ�Ƿ����
	INT		IsPresent(LuaPlus::LuaState* state);
	//�õ���ǰ��ʦ���̵ܽ������ID
	INT		GetAbilityID(LuaPlus::LuaState* state);
	//�õ���������һ������Ҫ�ľ���ֵ
	INT		GetNeedExp(LuaPlus::LuaState* state);
	//�õ���������һ������Ҫ�Ľ�Ǯ
	INT		GetNeedMoney(LuaPlus::LuaState* state);
	//�õ���������һ������Ҫ��������
	INT		GetNeedSkillExp(LuaPlus::LuaState* state);
	// ��÷�����������һЩ����
	INT		GetServerData(LuaPlus::LuaState* state);

public:
	static AbilityTeacher s_AbilityTeacher;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Pet
{
public:
	//ȡ�õ�ǰ�����Ƿ����
	INT		IsPresent(LuaPlus::LuaState* state);
	//�õ����������
	INT		GetPet_Count(LuaPlus::LuaState* state);
	//�õ��ڼ�ֻ���������
	INT		GetPetList_Appoint(LuaPlus::LuaState* state);
	//�����ս
	INT		Go_Fight(LuaPlus::LuaState* state);
	//������Ϣ���ջ�
	INT		Go_Relax(LuaPlus::LuaState* state);
	//�������
	INT		Go_Free(LuaPlus::LuaState* state);
	//����ιʳ
	INT		Feed(LuaPlus::LuaState* state);
	INT		Dome(LuaPlus::LuaState* state);
	//�õ����������
	INT		GetName(LuaPlus::LuaState* state);
	//��ó����ID
	INT		GetID(LuaPlus::LuaState* state);
	//�����ż��GUID
	INT		GetConsort(LuaPlus::LuaState* state);
	//�õ�������Ա�
	INT		GetSex(LuaPlus::LuaState* state);
	//��ó��������
	INT		GetNaturalLife(LuaPlus::LuaState* state);
	INT		GetMaxLife(LuaPlus::LuaState* state);
	INT		GetLoyalgGade(LuaPlus::LuaState* state);
	//�õ�����ĵȼ�
	INT		GetLevel(LuaPlus::LuaState* state);
	//��ó����Ǳ��
	INT		GetPotential(LuaPlus::LuaState* state);
	//�õ������ǵڼ�����
	INT		GetType(LuaPlus::LuaState* state);
	//�õ�����Ŀ��ֶ�
	INT		GetHappy(LuaPlus::LuaState* state);
	//�õ��������������
	INT		GetStrAptitude(LuaPlus::LuaState* state);
	//��ó�����������
	INT		GetPFAptitude(LuaPlus::LuaState* state);
	//��ó���������
	INT		GetDexAptitude(LuaPlus::LuaState* state);
//	INT		GetIntAptitude(LuaPlus::LuaState* state);
	//��ó��ﶨ������
	INT		GetStaAptitude(LuaPlus::LuaState* state);
	//��ó��ﵱǰ��Ѫ��
	INT		GetHP(LuaPlus::LuaState* state);
	//��ó�������Ѫ��
	INT		GetMaxHP(LuaPlus::LuaState* state);
	INT		GetMP(LuaPlus::LuaState* state);
	INT		GetMaxMP(LuaPlus::LuaState* state);

	//��ó��������
	INT		GetStr(LuaPlus::LuaState* state);
	INT		GetInt(LuaPlus::LuaState* state);
	//��ó������
	INT		GetDex(LuaPlus::LuaState* state);
	//��ó��������
	INT		GetPF(LuaPlus::LuaState* state);
	//��ó���Ķ���
	INT		GetSta(LuaPlus::LuaState* state);
	//��ó���ĸ���
	INT		GetBasic(LuaPlus::LuaState* state);
	//��ó��ﵱǰ�ľ������������Ҫ�ľ���
	INT		GetExp(LuaPlus::LuaState* state);

	INT		GetNearAttack(LuaPlus::LuaState* state);
	INT		GetNearRecovery(LuaPlus::LuaState* state);
	INT		GetFarAttack(LuaPlus::LuaState* state);
	INT		GetFarRecovery(LuaPlus::LuaState* state);

//	INT		GetPhysicsAttack(LuaPlus::LuaState* state);
	INT		GetMagicAttack(LuaPlus::LuaState* state);
//	INT		GetPhysicsRecovery(LuaPlus::LuaState* state);
	INT		GetMagicRecovery(LuaPlus::LuaState* state);

	//��ó����������
	INT		GetMiss(LuaPlus::LuaState* state);
	//��ó����������
	INT		GetShootProbability(LuaPlus::LuaState* state);
	//�������Լӵ�
	INT		Add_Attribute(LuaPlus::LuaState* state);
	//���������
	INT		Change_Name(LuaPlus::LuaState* state);
	//�����Ƿ��ڲ�ս״̬
	INT		GetIsFighting(LuaPlus::LuaState* state);
	INT		SetModel(LuaPlus::LuaState* state);
	//��ó���Ļ�����
	INT		GetDeathWarrant(LuaPlus::LuaState* state);

	//��ó���ĵ�N�����ܵ�ID
	INT		GetSkillID(LuaPlus::LuaState* state);
	
	//yangjun add for petskillstudy
	//���ó��＼��ѧϰ�ĳ���ģ��
	INT		SetSkillStudyModel(LuaPlus::LuaState* state);
	//��ʾĿ�����
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
	//�õ��������󣬵õ�����ļ���ID
	INT		Lua_EnumPetSkill(LuaPlus::LuaState* state);
	INT		Select_Pet(LuaPlus::LuaState* state);
	//��ó�����Ը�
	INT		GetAIType(LuaPlus::LuaState* state);
	INT		GetSkillPassive(LuaPlus::LuaState* state);
	// �������ȷ��
	INT		Free_Confirm(LuaPlus::LuaState* state);

public:
	static Pet s_Pet;
	static LuaPlus::LuaObject* s_pMetaTable;
};


class TargetPet
{
public:
	//ȡ�õ�ǰ�����Ƿ����
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
	//�ĵ������Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//�򿪴�Ǯ���棨�����Ǯ�����Ľ��棩
	INT OpenSaveFrame(LuaPlus::LuaState* state);
	//��ȡǮ���棨����ȡǮ�����Ľ��棩
	INT OpenGetFrame(LuaPlus::LuaState* state);
	//��Ǯ��ת��
	INT GetInputMoney(LuaPlus::LuaState* state);
	//���ʹ�Ǯ��Ϣ
	INT SaveMoneyToBank(LuaPlus::LuaState* state);
	//����ȡǮ��Ϣ
	INT GetMoneyFromBank(LuaPlus::LuaState* state);
	//�������еĽ�Ǯ��
	INT GetBankMoney(LuaPlus::LuaState* state);
	//����һ���ƶ�������Ʒ����������Ϣ
	INT MoveItemToPacket(LuaPlus::LuaState* state);
	//��������ӵ�е�������ĸ���
	INT GetRentBoxNum(LuaPlus::LuaState* state);
	//����ָ��������Ŀ�ʼ���͸�����
	INT GetRentBoxInfo(LuaPlus::LuaState* state);
	//���õ�ǰ��������
	INT SetCurRentIndex(LuaPlus::LuaState* state);
	//ת������
	INT TransformCoin(LuaPlus::LuaState* state);
	//�ر�����
	INT Close(LuaPlus::LuaState* state);
	//��ò�����Ʒ
	INT EnumItem(LuaPlus::LuaState* state);

	// �������������
	INT OpenInputFrame(LuaPlus::LuaState* state);
public:
	static Bank s_Bank;
	static LuaPlus::LuaObject* s_pMetaTable;
};

// ����
class Exchange
{
public:

	// �ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	// ���뽻��
	INT SendExchangeApply(LuaPlus::LuaState* state);
	// �򿪽��׶Ի���
	INT OpenExchangeFrame(LuaPlus::LuaState* state);
	// �����Լ����뽻�׵���Ʒ����
	INT GetItemNum(LuaPlus::LuaState* state);
	// ����Action
	INT GetItemAction(LuaPlus::LuaState* state);
	// ������Ǯ��
	INT GetMoney(LuaPlus::LuaState* state);
	// ��������
	INT LockExchange(LuaPlus::LuaState* state);
	//��������״̬
	INT IsLocked(LuaPlus::LuaState* state);
	// ȷ������
	INT AcceptExchange(LuaPlus::LuaState* state);
	// ����ȡ��
	INT ExchangeCancel(LuaPlus::LuaState* state);
	// �򿪳�����棨��ʱ�����򿪽�Ǯ�����
	INT OpenPetFrame(LuaPlus::LuaState* state);
	// ��������ý�Ǯ
	INT GetMoneyFromInput(LuaPlus::LuaState* state);
	//�������������б��Ƿ�Ϊ��
	INT IsStillAnyAppInList(LuaPlus::LuaState* state);
	//�������Լ����׵��˵�����
	INT GetOthersName(LuaPlus::LuaState* state);
	//ѡ��һ�������б��еĳ�����ȷ����Ĳ���
	INT AddPet(LuaPlus::LuaState* state);
	// ��ó���ĸ���
	INT GetPetNum(LuaPlus::LuaState* state);
	//
	INT EnumPet(LuaPlus::LuaState* state);
	//�ӽ�����������һ������
	INT DelSelectPet(LuaPlus::LuaState* state);
	//��ʾ������ϸ��Ϣ
	INT ViewPetDesc(LuaPlus::LuaState* state);

public:
	static Exchange s_Exchange;
	static LuaPlus::LuaObject* s_pMetaTable;
};


// �����
class LifeAbility
{
public:
	//ͨ�������ID�ͼ��� �õ���ǰ���ܼ������ϸ��Ϣ
	INT GetLifeAbility_Data(LuaPlus::LuaState* state);
	//���������ID�����ActionID
	INT GetActionID_ByAbilityID(LuaPlus::LuaState*state);
	// �õ�����ܵ�����
//	INT GetLifeAbility_Count(LuaPlus::LuaState* state);
	// ͨ�����ȥ��Ability�ı��
	INT GetLifeAbility_Number(LuaPlus::LuaState* state);
	// ͨ�����ȥ����䷽������Ҫ��������
	INT GetPrescr_Material_Number(LuaPlus::LuaState* state);
	// ͨ�����ϵı�ţ���ѯ���ϵ���ϸ��Ϣ������ͼ������ơ�
	INT	GetPrescr_Material(LuaPlus::LuaState* state);
	// ͨ�����ȥ��ڼ����䷽�ı��
	INT GetPrescrList_Item_FromNum(LuaPlus::LuaState* state);
	// ��������ԭ���ܹ����Ժϳɶ��ٸ�����䷽��
	INT GetPrescr_Item_Maximum(LuaPlus::LuaState* state);
	// ����ԭ��������
	INT GetPrescr_Material_Hold_Count(LuaPlus::LuaState* state);
	// ˢ�ºϳɽ���
	INT Update_Synthesize( LuaPlus::LuaState* state);
	// �򿪱�ʯ�ϳ�/��Ƕ����
	INT Open_Compose_Gem_Page( LuaPlus::LuaState* state);
	// ��ʼ��Ƕ
	INT	Do_Enchase( LuaPlus::LuaState* state);
	// ��ʼ�ϳ�
	INT	Do_Combine( LuaPlus::LuaState* state);
	// ����װ���ϣ���i�ű�ʯ��ͼ����Ϣ
	INT	GetEquip_Gem( LuaPlus::LuaState* state);
	// ����װ���ϣ��ܹ��ж��ٿű�ʯ��
	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// ����װ���ϣ��ܹ�������Ƕ���ٸ���ʯ	
//	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// ���ر�ʯ�ļ���
//	INT Compound_Preparation( LuaPlus::LuaState* state );
	// �Ƿ������Ƕ
	INT	Can_Enchase( LuaPlus::LuaState* state );
	// �Ƿ���Ժϳ�
	INT Can_Combine( LuaPlus::LuaState* state );
	// ��Ʒ���Էŵ���Ƕ�������λ�á�
	INT Enchase_Preparation( LuaPlus::LuaState* state );
	// ��Ʒ���Էŵ��ϳɽ������λ�á�
	INT Compound_Preparation( LuaPlus::LuaState* state );
	// ����Ʒ��ĳλ�õ���Ʒ�ûҡ�
	INT Lock_Packet_Item( LuaPlus::LuaState* state );
	// ȡ���䷽����ϸ����
	INT	GetPrescr_Explain(LuaPlus::LuaState* state);
	// ����ܽ�����ĵ�Npc
	INT GetNpcId( LuaPlus::LuaState* state );
	
	//��ȡ��ʯ��Action
	INT GetGemActionInfo( LuaPlus::LuaState* state );
	
	INT SetPrecalcInfo( LuaPlus::LuaState* state );

	//����IDtable����ȡ�����������ϵ���ϸ��Ϣ
	INT		GetPrescrList_Item_RequirementData(LuaPlus::LuaState* state);

	//����ID����ȡ�����д���Ʒ������
	INT		GetRequirementNum_ById(LuaPlus::LuaState* state);

protected:
	std::map<INT,CObject_Item*> m_Requirement;			//������������õ�����Ʒ��ÿ����Ʒ���ֻ����һ����Ʒ

public:
	static LifeAbility s_LifeAbility;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//�п��ܽ������ֿ�������
//class Stall

class StallSale
{
public:

	//�ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//��������̯λ��
	INT GetPosTax(LuaPlus::LuaState* state);
	//��������˰��
	INT GetTradeTax(LuaPlus::LuaState* state);
	//�ύ��Ʒ�۸�
	INT ReferItemPrice(LuaPlus::LuaState* state);
	// ����̯λ����
	INT ModifStallName(LuaPlus::LuaState* state);
	// ����Item�۸�
	INT ModifItemPrice(LuaPlus::LuaState* state);
	// ���ñ���״̬
	INT SetSaveStates(LuaPlus::LuaState* state);
	// ��̯ȷ��
	INT ApplyCloseStall(LuaPlus::LuaState* state);
	// ��̯����
	INT CloseStall(LuaPlus::LuaState* state);
	// ��ҿ���		20100706 BLL
	INT StallOpen(LuaPlus::LuaState* state);
	// ��Ʒ�����¶���
	INT ItemReprice(LuaPlus::LuaState* state);
	// ���Ϳ�ʼ��̯����Ϣ��ʼ��̯
	INT AgreeBeginStall(LuaPlus::LuaState* state);
	//ɾ�������ϵ�һ����Ʒ���¼ܣ�
	INT DeleteItem(LuaPlus::LuaState* state);
	//���ָ��λ�õĶ���
	INT GetPrice(LuaPlus::LuaState* state);
	//��̯λ��Ϣ
	INT OpenMessageSale(LuaPlus::LuaState* state);
	//����̯λ���ܣ�����
	INT ApplyAd(LuaPlus::LuaState* state);
	//�ر�
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//���̯λ��
	INT GetStallName(LuaPlus::LuaState* state);
	//��ù����
	INT GetAdvertise(LuaPlus::LuaState* state);
	//�����Լ���GUID
	INT GetGuid(LuaPlus::LuaState* state);
	// ��ó���ĸ���
	INT GetPetNum(LuaPlus::LuaState* state);
	// ��ó���
	INT EnumPet(LuaPlus::LuaState* state);
	// �����ϼ�
	INT PetUpStall(LuaPlus::LuaState* state);
	// ������ļ۸�
	INT PetReprice(LuaPlus::LuaState* state);
	// ���ȱʡҳ
	INT GetDefaultPage(LuaPlus::LuaState* state);
	// ����ȱʡҳ
	INT SetDefaultPage(LuaPlus::LuaState* state);
	// ��ʾ����
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//����ѡ�е��Լ�̯λ�ϵĳ���
	INT SetSelectPet(LuaPlus::LuaState* state);

public:
	static StallSale s_StallSale;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBuy
{
public:
	//�ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//��������˰��
	INT GetTradeTax(LuaPlus::LuaState* state);
	//�򿪱��˵�̯λ
	INT OpenStall(LuaPlus::LuaState* state);
	//���ָ��λ�õĶ���
	INT GetPrice(LuaPlus::LuaState* state);
	//������Ʒ
	INT BuyItem(LuaPlus::LuaState* state);
	//��̯λ��Ϣ
	INT OpenMessageBuy(LuaPlus::LuaState* state);
	//���̯λ��
	INT GetStallName(LuaPlus::LuaState* state);
	//��ù����
	INT GetAdvertise(LuaPlus::LuaState* state);
	//�ر�
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//�����Լ���GUID
	INT GetGuid(LuaPlus::LuaState* state);
	//����̯��������
	INT GetStallerName(LuaPlus::LuaState* state);
	// ��ó���ĸ���
	INT GetPetNum(LuaPlus::LuaState* state);
	// ��ó���
	INT EnumPet(LuaPlus::LuaState* state);
	// ���ȱʡҳ
	INT GetDefaultPage(LuaPlus::LuaState* state);
	//�������
	INT BuyPet(LuaPlus::LuaState* state);
public:
	static StallBuy s_StallBuy;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBbs
{
public:
	//�����Ϣ������
	INT GetMessageNum(LuaPlus::LuaState* state);
	//���
	INT EnumMessage(LuaPlus::LuaState* state);
	//���һ���µ���Ϣ
	INT AddMessage(LuaPlus::LuaState* state);
	//�ظ�һ����Ϣ
	INT ReplyMessage(LuaPlus::LuaState* state);


public:
	static StallBbs s_StallBbs;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class MissionReply
{
public:
	//�ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//�򿪳����б�
	INT OpenPetFrame(LuaPlus::LuaState* state);
	//���"Continue�Ĳ���"
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
	//�ر�NpcShop
	INT Close(LuaPlus::LuaState* state);
	//����̵�NpcID
	INT GetNpcId(LuaPlus::LuaState* state);
	//����̵���Ʒ�۸�
	INT GetItemPrice(LuaPlus::LuaState* state);
	//�̵�����
	INT NpcShopBuy(LuaPlus::LuaState* state);
	//�̵�����
	INT NpcShopSell(LuaPlus::LuaState* state);

	INT GetItemPriceType(LuaPlus::LuaState* state);

public:
	static NpcShop s_NpcShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class QuickChange
{
public:
	// һ����װ����װ
	INT Lua_ChangeSuit( LuaPlus::LuaState* state);

	// һ����װ����������
	INT Lua_SaveSuitName( LuaPlus::LuaState* state);

	// һ����װ������װ��
	INT Lua_SaveSuit( LuaPlus::LuaState* state);

	// һ����װ����ȡװ��
	INT Lua_ReadSuit( LuaPlus::LuaState* state);

	// һ����װ�����õ�ǰ��װ
	INT Lua_SetCurrentSuit( LuaPlus::LuaState* state);

	//һ����װ���õ���װ������
	INT Lua_GetSuitName( LuaPlus::LuaState* state );

public:
	static QuickChange s_QuickChange;
	static LuaPlus::LuaObject* s_pMetaTable;

private:
};

// ����̵�
class PlayerShop
{
public:
	INT CreateShop(LuaPlus::LuaState* state);
	// ����̵������
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
	// ������̵�ȡ����Ʒ���Լ��ı���
	INT RetackItem(LuaPlus::LuaState* state);
	//
	INT InputMoney(LuaPlus::LuaState* state);
	//�ϼܣ��¼ܣ�
	INT UpStall(LuaPlus::LuaState* state);
	//
	INT	SetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT	GetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT DealMoney(LuaPlus::LuaState* state);
	// ��������ύ
	INT ApplyMoney(LuaPlus::LuaState* state);
	//
	INT InfoMoney(LuaPlus::LuaState* state);
	//
	INT GetStallNum(LuaPlus::LuaState* state);
	//
	INT BuyItem(LuaPlus::LuaState* state);
	//
	INT IsSelectOnSale(LuaPlus::LuaState* state);
	//�¼�
	INT DownSale(LuaPlus::LuaState* state);
	//��յ�ǰ��ѡ��
	INT ClearSelect(LuaPlus::LuaState* state);
	//��ý�Ǯ
	INT GetMoney(LuaPlus::LuaState* state);
	//�����ҵָ��
	INT GetCommercialFactor(LuaPlus::LuaState* state);
	//��õ���
	INT GetShopInfo(LuaPlus::LuaState* state);
	//���һ��Stall�Ƿ�Open
	INT IsOpenStall(LuaPlus::LuaState* state);
	//����һ����̨
	INT OpenStall(LuaPlus::LuaState* state);
	//����һ��Stall������
	INT AskStallData(LuaPlus::LuaState* state);
	//����
	INT EnumPet(LuaPlus::LuaState* state);
	//���õ�ǰѡ�еĳ���
	INT SetSelectPet(LuaPlus::LuaState* state);
	//��õ�ǰ�ܿ����̵������
	INT GetCanOpenShopType(LuaPlus::LuaState* state);
	//��õ�ǰѡ�еĳ���Index
	INT SetCurSelectPetIndex(LuaPlus::LuaState* state);
	//���ѡ����Ʒ���߳���ļ۸�
	INT GetObjPrice(LuaPlus::LuaState* state);
	//�޸��̵������
	INT Modify(LuaPlus::LuaState* state);
	//���ѡ�е���Ʒ�����������
	INT GetObjName(LuaPlus::LuaState* state);
	//��ʾ������ϸ��Ϣ
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//�������ѡ��� Position,,�ڽ��в�����ʱ�򣬿���ʹ�ü�����ֵ�ǲ���Ϊ��1 ����־�ǲ��ǽ�����ѡ����Ʒ
	INT ClearSelectPos(LuaPlus::LuaState* state);
	//�̳��̵�
	INT Transfer(LuaPlus::LuaState* state);
	// �鿴��ǰ���̵��ǲ��Ǵ����̵�״̬
	INT IsSaleOut(LuaPlus::LuaState* state);
	// �����̵�
	INT BuyShop(LuaPlus::LuaState* state);
	//�ر�
	INT CloseShop(LuaPlus::LuaState* state);
	//�򿪣��رգ�Message
	INT OpenMessage(LuaPlus::LuaState* state);
	//���Message������
	INT GetMessageNum(LuaPlus::LuaState* state);
	//��ǰҳ��Message������
	INT GetCurPageMessageNum(LuaPlus::LuaState* state);
	//���Message
	INT EnumMessage(LuaPlus::LuaState* state);
	//�����ϻ���
	INT DealFriend(LuaPlus::LuaState* state);
	//��úϻ��˵�����
	INT GetFriendNum(LuaPlus::LuaState* state);
	//��úϻ���
	INT EnumFriend(LuaPlus::LuaState* state);
	//���ţ�����������
	INT ChangeShopNum(LuaPlus::LuaState* state);
	// ��÷��ʵ��̵����Npc�ĵ�ǰ����ID����������CareObject��
	INT GetNpcId(LuaPlus::LuaState* state);

public:
	static PlayerShop s_PlayerShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

// �������
class Friend
{
public:

	// �ر�
	INT Close(LuaPlus::LuaState* state);
	
	// ��ѯ����
	INT EnumName(LuaPlus::LuaState* state);
	
	// ���Ҽ��˵�
	INT OpenMenu( LuaPlus::LuaState* state);
	
	INT AskTeam(LuaPlus::LuaState* state);

	// �򿪷���
	INT OpenGrouping( LuaPlus::LuaState* state);

	// ��������
	INT CreateGrouping( LuaPlus::LuaState* state);
	
	// ��ʾ������ص�����Ի���
	INT ShowFriendInputDlg( LuaPlus::LuaState* state);

	// ��ʾ��Ӻ��ѶԻ���
	INT ShowAddNewFriendDlg( LuaPlus::LuaState* state);

	// �õ��ж�������ʷ��Ϣ
	INT GetHistroyNumber( LuaPlus::LuaState* state);
	// �õ���ʷ����
	INT GetHistroyData(LuaPlus::LuaState* state);

	// ����ѡ��ĺ����б�
	INT	SetCurrentTeam( LuaPlus::LuaState* state );
	INT GetCurrentTeam( LuaPlus::LuaState* state );
	
	// ���˽����һ����ͬ�⡱�͡���ͬ�⡱����
	INT CallOf(LuaPlus::LuaState* state);
	// �����ʾ����Ĳ���
	INT ViewFeel(LuaPlus::LuaState* state);
	// ���������ǲ������Լ���ͷ��
	INT IsMoodInHead(LuaPlus::LuaState* state);

	// ���������ж��Ƿ��Ǻ���
	INT IsPlayerIsFriend(LuaPlus::LuaState* state);

	// ���õ�ǰѡ��
	INT SetCurrentSelect(LuaPlus::LuaState* state);

	// ��ȡ�Զ��������Ϣ
	INT GetNewGroupInfo(LuaPlus::LuaState* state);


	// ����������
	INT OpenSearcher(LuaPlus::LuaState* state);


public:

	static Friend s_Friend;
	static LuaPlus::LuaObject* s_pMetaTable;
	
	// ��ǰѡ�е��飬
	int		m_nCurTeam; 
	
	Friend()
	{
		m_nCurTeam = 0; 
	};

};	// class friend

// ���Ѳ���
class FriendSearcher
{
public:

	static FriendSearcher s_FriendSearcher;
	static LuaPlus::LuaObject* s_pMetaTable;

	FriendSearcher()
	{
	};

public:

	// ��պ����б�
	INT ClearFingerList( LuaPlus::LuaState* state );
	// ��ȡҳ��
	INT GetFriendPageNumber( LuaPlus::LuaState* state );

	// ��ȡһҳ�еĺ�������
	INT GetFriendNumberInPage( LuaPlus::LuaState* state );
	// ��ȡһҳ��ѡ��ĺ��Ѳ���
	INT GetFriendFromPage( LuaPlus::LuaState* state );

	// ͨ��id����
	INT FriendSearchByID( LuaPlus::LuaState* state );
	// ͨ����������
	INT FriendSearchByName( LuaPlus::LuaState* state );
	// 
//	INT FriendSearchByNameAndId( LuaPlus::LuaState* state );
	// �߼�����
	// nPosition�� ְҵ�� ��ᣬ �Ա� �ȼ����ޣ� �ȼ�����
	INT FriendSearchAdvance( LuaPlus::LuaState* state );


private:

};	// class FriendSearcher


// ÿ�����ѹ���
class DaytipsData
{
public:
	static DaytipsData s_DaytipsData;
	static LuaPlus::LuaObject* s_pMetaTable;

	DaytipsData();
	~DaytipsData();

	INT FillCycleMissionList(LuaPlus::LuaState* state);	
	INT FillFixedMissionList(LuaPlus::LuaState* state );  // ���ݵ�ǰʱ�����ÿ�ջ�б�     20100716 BLL
	INT RecommendMission(LuaPlus::LuaState* state );
	INT GetWeek(LuaPlus::LuaState* state );

	// ��ȡһ�����¼  20100716 BLL
	INT GetMissionByIndex(LuaPlus::LuaState* state );	 

	// ��ȡ���ջ����  20100716 BLL
	INT GetMissionCount( LuaPlus::LuaState* state );	

	// �Ƿ��ǻ���� 20100723 BLL
	VOID IsDayTipsMission( INT nMisID, BOOL& IsDayTipMission, BOOL& bTodayCanSee );

	// ��ȡһ���ĿɽӴ���    20100728 BLL
	INT	    GetMissionCanDoCount( INT nMissionID );
private:

	struct DayTipsNode
	{
		_DBC_DAY_TIPS* m_pDayTipNode;	
		INT			   m_nTimeType;		// ���ݵ�ǰʱ�����õ�ʱ������ͣ�1�����ڽ��л 2���������л��3�����ڻ
		
		DayTipsNode()
		{
			m_pDayTipNode = NULL;
			m_nTimeType   = -1;	// ����δ֪
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

	DayTipSet			m_fixedMission;		// ��ʾ�����������ݶ���

private:
	VOID	ResetDaytipSet();
	// ����һʱ����ʱ��εĹ�ϵ
	INT		TimeSegmentValue(const VALID_TIME_SLICE* pSlice, const tm* pCheck, int* pValue);
	// �����������
	BOOL	CommonCheck(const _DBC_DAY_TIPS* pAttr, const CCharacterData* pData);

	// �Ƿ��ǽ��տɼ��   20100716 BLL
	BOOL	IsTodayMission( const _DBC_DAY_TIPS* pDayTip );

	// ��ȡ�Ѳμ�һ���Ĵ���    20100716 BLL
	INT		GetMissionDoneCount( INT nMissionID );

	// �����Ǳ��µڼ���    20100716 BLL
	INT		GetWeekNO();

	// ���ؽ��ջ�б�	 20100719 BLL
	VOID    InitializeMissionList();

};


// ����������
class QuestLogData
{
public:
	static QuestLogData s_QuestLogData;
	static LuaPlus::LuaObject* s_pMetaTable;

	QuestLogData() {};

public:
	// �ѽ���������
	INT DoneMissionNum(LuaPlus::LuaState* state);

	// �ɽ���������
	INT CanDoMissionNum(LuaPlus::LuaState* state );

	// ��ǰ�ѽ���������
	INT DoneMissionNum_CurScene(LuaPlus::LuaState* state );

	// ��ǰ�ɽ���������
	INT CanDoMissionNum_CurScene(LuaPlus::LuaState* state );

	// �õ��ɽ������б�
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
	// ��¼��Ϣ��������.
	//
	class CGameProduce_Login
	{
	public: 

		CGameProduce_Login();
		~CGameProduce_Login();

	public:
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ѡ�������
		//

		//�õ�����������ĸ���.
		INT GetAreaCount(LuaPlus::LuaState* state);
		
		//�õ����������������.
		INT GetAreaName(LuaPlus::LuaState* state);

		//�õ�������������ṩ��.

		//�õ������������, login server �ĸ���.
		INT GetAreaLoginServerCount(LuaPlus::LuaState* state);

		//�õ��������������Ϣ
		INT GetAreaServerInfo(LuaPlus::LuaState* state);

		// �õ�login server ����Ϣ.
		INT GetAreaLoginServerInfo(LuaPlus::LuaState* state);

		// ѡ��һ��������
		INT SelectLoginServer(LuaPlus::LuaState* state);

		// �Զ�ѡ��һ���뵱ǰѡ������繩Ӧ��ƥ���login server
		INT AutoSelLoginServer(LuaPlus::LuaState* state);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �ʺ��������
		//

		// ���ӵ�login server
		INT ConnectToLoginServer(LuaPlus::LuaState* state);

		// �˵�������ѡ�����
		INT ExitToSelectServer(LuaPlus::LuaState* state);

		// ��֤�û���������
		INT CheckAccount(LuaPlus::LuaState* state);

		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����ѡ�����
		//

		// ������ѡ������л����ʺ��������
		INT ChangeToAccountInputDlgFromSelectRole(LuaPlus::LuaState* state);

		// ������ѡ������л������ﴴ������
		INT ChangeToCreateRoleDlgFromSelectRole(LuaPlus::LuaState* state);

		// �����ﴴ�������л�������ѡ�����
		INT ChangeToSelectRoleDlgFromCreateRole(LuaPlus::LuaState* state);

		// �õ���ɫ�ĸ���
		INT GetRoleCount(LuaPlus::LuaState* state);

		// �õ���ɫ����Ϣ
		INT GetRoleInfo(LuaPlus::LuaState* state);

		// �õ���ɫ��uiģ������
		INT GetRoleUIModleName(LuaPlus::LuaState* state);

		// ������ɫ
		INT CreateRole(LuaPlus::LuaState* state);

		// ɾ����ɫ
		INT DelRole(LuaPlus::LuaState* state);

		// ���ͽ�����Ϸ��Ϣ
		INT SendEnterGameMsg(LuaPlus::LuaState* state);
		// �л�����
		INT ChangeSceneMsg(LuaPlus::LuaState* state);

		// ѯ���Ƿ�ɾ����ɫ
		INT DelRole_YesNo(LuaPlus::LuaState* state);

		// ѯ���Ƿ��˳���Ϸ
		INT ExitGame_YesNo(LuaPlus::LuaState* state);

		// ѯ���Ƿ��˵��ʺ��������
		INT ExitToAccountInput_YesNo(LuaPlus::LuaState* state);

		// ɾ��ѡ��Ľ�ɫ
		INT DelSelRole(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����ͷ�񲿷�.
		//

		// �õ�Ů����ͷ��ĸ���
		INT GetWomanFaceCount(LuaPlus::LuaState* state);

		// �õ�������ͷ��ĸ���
		INT GetManFaceCount(LuaPlus::LuaState* state);

		// �õ�Ů����ͷ�������
		INT GetWomanFaceName(LuaPlus::LuaState* state);

		// �õ�������ͷ�������
		INT GetManFaceName(LuaPlus::LuaState* state);

		// �õ�����ͷ�������
		INT GetFaceName(LuaPlus::LuaState* state);

		// ����ͷ��id
		INT SetFaceId(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �������β���.
		//
		
		// �õ�����model�ĸ���
		INT GetFaceModelCount(LuaPlus::LuaState* state);

		// �õ�����ģ�͵�����
		INT GetFaceModelName(LuaPlus::LuaState* state);

		// ��������ģ��id
		INT SetFaceModelId(LuaPlus::LuaState* state);
		INT CreateRotateRole(LuaPlus::LuaState* state);
		INT ChangeSelectRole(LuaPlus::LuaState* state);
		INT SelRotateRole(LuaPlus::LuaState* state);
		INT GetLastLoginAreaAndServerIndex(LuaPlus::LuaState* state);
		INT PlayAnimation(LuaPlus::LuaState* state);

		// ���ڴ������������ʾ

		// �����Ա�
		INT SetGender( LuaPlus::LuaState* state );
		// ����ְҵ
		INT SetProfession( LuaPlus::LuaState* state );



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ���ڷ��β���.
		//
		
		// �õ�����model�ĸ���
		INT GetHairModelCount(LuaPlus::LuaState* state);

		// �õ�����ģ�͵�����
		INT GetHairModelName(LuaPlus::LuaState* state);

		// ��������ģ��id
		INT SetHairModelId(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����״̬���֡�
		//
		
		// �ر��������ӡ� 
		INT CloseNetConnect(LuaPlus::LuaState* state);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �������֡�
		//
		
		//  ��ʼ�������֡�
		INT PlayMusic(LuaPlus::LuaState* state);

		//  ֹͣ�������֡�
		INT StopMusic(LuaPlus::LuaState* state);

		/**���¼���window�Ľű�
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
		//�ر�IBShop
		INT IBClose(LuaPlus::LuaState* state);
		//�̵�����
		INT IBShopBuy(LuaPlus::LuaState* state);
		//����̵���Ʒ�۸�
		INT Lua_GetIBItemPrice(LuaPlus::LuaState* state);

		//�õ�IB�̳�����Ʒ�Ĳ�������
		INT	Lua_EnumActionFoIBShop(LuaPlus::LuaState* state);

		//�õ�IB�̳��ڷ�����������Ʒ������
		INT	Lua_GetIBShopAccordNum(LuaPlus::LuaState* state);

	public:
		//�ж��̳��ڵ�һ����Ʒ���Ƿ���Ϲ��˹����Ƿ���ʾ
		BOOL IsActionFilter( tActionItem* pActionItem, INT type, INT type2 );

	public:
		static IBShop s_IBShop;
		static LuaPlus::LuaObject* s_pMetaTable;
	};
};	//namespace SCRIPT_SANDBOX


