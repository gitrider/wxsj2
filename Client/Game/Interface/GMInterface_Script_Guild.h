
/** 
 *
 *	功能：	帮会相关接口
 *	
 *	修改记录：
 *			080303	增加 “邀请入帮”GuildInvite
 *			080304	增加 AdjustMemberTitle 修改成员的称号
 *			080305	DestoryFamily 删除家族
 *					ShowInputDialog 显示帮会相关的输入对话框
 *					AgreeInvite 同意邀请
 *			080307	CharacterCheck 字符检测
 *					CreateFamily 创建家族
 *					CreateFamilyConfirm 创建家族确认
 *					AdjustFamilyMember 添加或删除家族成员
 *					TransferMemberToFamily 将成员移到家族中
 *					KickMemberFormFamily 从家族中删除成员
 *					InviteMemberToFamily 邀请成员到家族
 *					FamilyInviteConfirm 家族邀请的确认
 *					GetFamilyMembersInfo 获得家族的成员详细信息
 *			080310	SetMemberListSelect 保存每次成员列表中的选择
 *			080311	GetFamilyMemberIndex 
 *					SetLuaFamilyNum
 *					GetLuaFamilyNum
 *			
 *			
 */

#include "Type.h"


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};

namespace SCRIPT_SANDBOX
{
	class Guild
	{

	//LuaPlus界面相关接口
	public:
		//创建帮会
		INT	CreateGuild(LuaPlus::LuaState* state);

		//创建帮会确认
		INT CreateGuildConfirm(LuaPlus::LuaState* state);

		//向World申请帮会详细信息
		INT AskGuildDetailInfo(LuaPlus::LuaState* state);

		//向World申请帮会成员信息
		INT AskGuildMembersInfo(LuaPlus::LuaState* state);

		//向World申请帮会职位信息
		INT AskGuildAppointPosInfo(LuaPlus::LuaState* state);

		//获得现有帮会的总数
		INT	GetGuildNum(LuaPlus::LuaState* state);

		//获得其他帮会的信息
		INT GetGuildInfo(LuaPlus::LuaState* state);

		//加入帮会
		INT JoinGuild(LuaPlus::LuaState* state);

		//邀请入帮
		INT GuildInvite(LuaPlus::LuaState* state);
		//显示邀请入帮的窗口
		INT ShowInputDialog(LuaPlus::LuaState* state);

		//退出帮会
		INT	QuitGuild(LuaPlus::LuaState* state);

		//踢出帮会,拒绝申请
		INT KickGuild(LuaPlus::LuaState* state);

		//接纳会员
		INT RecruitGuild(LuaPlus::LuaState* state);

		//获得自己的帮众信息
		INT GetMembersNum(LuaPlus::LuaState* state);

		//Lua显示里list控件的位置
		INT GetShowMembersIdx(LuaPlus::LuaState* state);
		INT GetShowTraineesIdx(LuaPlus::LuaState* state);

		//获得自己的帮众详细信息
		INT	GetMembersInfo(LuaPlus::LuaState* state);

		//获得自己的帮派信息
		INT GetMyGuildInfo(LuaPlus::LuaState* state);

		//获得自己帮派的详细信息
		INT GetMyGuildDetailInfo(LuaPlus::LuaState* state);

		//获得自己的帮派权利
		INT GetMyGuildPower(LuaPlus::LuaState* state);

        //修改现有帮众职位
		INT	AdjustMemberAuth(LuaPlus::LuaState* state);

		//帮会让位给别人
		INT ChangeGuildLeader(LuaPlus::LuaState* state);

		//帮会删除
		INT	DestoryGuild(LuaPlus::LuaState* state);

		//修改帮会信息（宗旨）
		INT FixGuildInfo(LuaPlus::LuaState* state);

		//帮会禅让
		INT DemisGuild(LuaPlus::LuaState* state);

		//准备帮会成员数据
		INT PrepareMembersInfomation(LuaPlus::LuaState* state);

		//修改帮会成员的称号
		INT AdjustMemberTitle(LuaPlus::LuaState* state);

		//同意加帮的邀请
		INT AgreeInvite(LuaPlus::LuaState* state);


		//保存每次成员列表中的选择
		INT SetMemberListSelect(LuaPlus::LuaState* state);

		//帮会相关的菜单
		INT OpenMenu(LuaPlus::LuaState* state);


		//================
		// 家族

		//创建家族
		INT	CreateFamily(LuaPlus::LuaState* state);

		//创建家族确认
		INT CreateFamilyConfirm(LuaPlus::LuaState* state);

		//家族删除
		INT	DestoryFamilyConfirm(LuaPlus::LuaState* state);
		INT	DestoryFamily(LuaPlus::LuaState* state);

		//获得家族的成员详细信息
		INT	GetFamilyMembersInfo(LuaPlus::LuaState* state);

		//对家族成员进行操作（添加或删除）
		INT AdjustFamilyMember(LuaPlus::LuaState* state);

		//移动成员到家族
		INT TransferMemberToFamily(LuaPlus::LuaState* state);

		//从家族中踢出
		INT KickMemberFormFamily(LuaPlus::LuaState* state);

		//家族邀请
		INT InviteMemberToFamily(LuaPlus::LuaState* state);

		//家族邀请确认
		INT FamilyInviteConfirm(LuaPlus::LuaState* state);

		//获取某家族成员数量
		INT GetFamilyMemberNum(LuaPlus::LuaState* state);
		//获取家族成员在成员列表中的索引
		INT GetFamilyMemberIndex(LuaPlus::LuaState* state);

		//保存脚本中当前显示的家族数量
		INT SetLuaFamilyNum(LuaPlus::LuaState* state);
		INT GetLuaFamilyNum(LuaPlus::LuaState* state);

		// 家族数据（索引，名称，位置）
		INT SetFamilyWindowData(LuaPlus::LuaState* state);
		INT GetFamilyWindowData(LuaPlus::LuaState* state);
		// 删除保存的指定数据
		INT DelFamilyWindowData(LuaPlus::LuaState* state);


	//共用函数
	public:
		//显示提示信息
		// msgType		消息号，用来在字典里获取相应的文字
		// bTalk		需要显示在聊天窗口
		// bTip			需要显示在屏幕中间的提示
		VOID ShowMsg(INT msgType, BOOL bTalk = FALSE, BOOL bTip = TRUE);

		struct Name_Idx
		{
			STRING	m_MemberName;		//对应DataPool中GuildMemberInfo_t结构里的m_szName
			INT		m_MemberIdx;		//对应DataPool中GuildMemberInfo_t的索引值
			INT		m_Position;			//在帮中的职位

			Name_Idx()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}

			VOID	CleanUp()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}
		};

		struct ShowColor
		{
			STRING	m_OnlineLeaderColor;		//在线领导的显示颜色
			STRING	m_LeaveLeaderColor;			//离线领导的显示颜色
			STRING	m_OnlineMemberColor;		//在线帮众显示颜色
			STRING	m_LeaveMemberColor;			//离线帮众显示颜色
			STRING	m_OnlineTraineeColor;		//在线申请人显示颜色
			STRING	m_LeaveTraineeColor;		//离线申请人显示颜色

			ShowColor()
			{
				m_OnlineLeaderColor = "#B";			//蓝色
				m_OnlineMemberColor = "#W";			//白色
				m_OnlineTraineeColor = "#W";

				m_LeaveLeaderColor = "#c808080";	//灰色
				m_LeaveMemberColor = "#c808080";
				m_LeaveTraineeColor = "#c808080";
			}
		};


		// 检测字符合法性
		BOOL CharacterCheck( const STRING& str );

	private:
		//帮会成立时间转换
		VOID ConvertServerTime(INT dTime, STRING& strTime);

		//更新帮会相应的显示信息
		VOID PerpareMembersBeforeShow();

		//选择显示颜色
		STRING GetShowColor_For_Lua(INT idx);

		std::vector<STRING>		m_MsgArray;			//配合ShowMsg函数使用
		INT						m_iMemberBak;		//改变职位时的备份是要修改哪个玩家的职位信息

		// 保存成员列表中的选择
		INT						m_iMemberSelect;


		//成员列表
		typedef		std::vector<Name_Idx>	MEMBER_TABLE;
		typedef		std::vector<INT>		MEMBER_INDEX_TABLE;

		MEMBER_INDEX_TABLE						m_AllMembers;			//所有成员
		MEMBER_INDEX_TABLE						m_AllTrainees;			//所有预备成员
		ShowColor								m_LuaShowColors;		//Lua里显示的颜色

		typedef		std::map< INT, MEMBER_INDEX_TABLE >		FAMILY_MEMBER_INDEX_TABLE;
		FAMILY_MEMBER_INDEX_TABLE				m_FamilyMembers;		// 帮会中家族成员
		INT										m_iFamilyNum;			// 家族的数量
		INT										m_iCurFamilyNum;		// 保存脚本中当前以有的家族数量

	public:

		Guild();
		virtual ~Guild();

		static Guild s_Guild;
		static LuaPlus::LuaObject* s_pMetaTable;
	};


	class SimpleCmpFunc
	{
	public:
		static BOOL less_strcmp(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);
		static BOOL less_strcoll(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);

		static BOOL less_position(const Guild::Name_Idx&s1, const Guild::Name_Idx& s2);
	};
};