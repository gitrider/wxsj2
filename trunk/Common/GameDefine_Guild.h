/********************************************************************************
 *	文件名：	GameDefine_Guild.h
 *	全路径：	d:\Prj\Common\GameDefine_Guild.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 5 日	11:06
 *
 *	功能说明：	帮会的一些状态数据以及操作返回值
 *	修改记录：
*********************************************************************************/

#ifndef __GAMEDEFINE_GUILD_H__
#define __GAMEDEFINE_GUILD_H__

#define MAX_NUM_PER_POSITION		    20
#define GUILD_CREATE_LEVEL_LIMIT        20			//创建帮会所需等级
#define GUILD_CREATE_MONEY_LIMIT        500000		//创建帮会所需金钱
#define GUILD_CREATE_ITEM_INDEX         11990001    //ItemIndex
#define GUILD_REJOIN_TIME_LIMIT         24*3600     //24小时
#define GUILD_FAMILY_USER_MAX           20          //黄金家族人数上限
#define GUILD_NORMAL_FAMILY_SIZE        12          //普通家族人数上限
#define MAX_GUILD_USER_POINT            99999       //帮会个人最高贡献度
#define MAX_GUILD_TOTAL_POINT           9999999     //帮会总贡献度


enum GUILD_STATUS
{
	GUILD_STATUS_INVALID			= 0,
	GUILD_STATUS_NORMAL,
};
//帮会职位级别
enum GUILD_POSITION
{
    GUILD_POSITION_INVALID			= 0,

    GUILD_POSITION_TRAINEE,					// 预备帮众，待批准
    GUILD_POSITION_MEMBER,					// 普通帮众，未非配

    GUILD_POSITION_ELITE_MEMBER,			// 精英帮众，已分配家族
    GUILD_POSITION_FAMILY_BOSS,             // 家族长

    GUILD_POSITION_ASS_CHIEFTAIN,			// 副帮主
    GUILD_POSITION_CHIEFTAIN,				// 帮主

    GUILD_POSITION_SIZE,					// 职位级别数
};
//帮会权限
enum GUILD_AUTHORITY
{
	GUILD_AUTHORITY_INVALID			= 0,

	GUILD_AUTHORITY_ASSIGN			= 0x1,		// 职务调动权
	GUILD_AUTHORITY_AUTHORIZE		= 0x2,		// 权限调整权
	GUILD_AUTHORITY_RECRUIT			= 0x4,		// 接收帮众权
	GUILD_AUTHORITY_EXPEL			= 0x8,		// 开除帮众权
	GUILD_AUTHORITY_DEMISE			= 0x10,		// 禅让权
	GUILD_AUTHORITY_WITHDRAW		= 0x20,		// 支取帮资权
	GUILD_AUTHORITY_DEPOSIT			= 0x40,		// 存入金额权
	GUILD_AUTHORITY_LEAVE			= 0x80,		// 离开帮会权（这个权限有点多余）
	GUILD_AUTHORITY_DIMISS			= 0x100,	// 解散帮会权
	GUILD_AUTHORITY_ADDFAMILYUSER   = 0x200,	// 添加家族成员权限
    GUILD_AUTHORITY_DELFAMILYUSER   = 0x400,    // 删除家族成员权限


	GUILD_AUTHORITY_NUMBER			= 8,	// 权限数量

	// 帮众的通用权限
	GUILD_AUTHORITY_MEMBER			= GUILD_AUTHORITY_DEPOSIT | GUILD_AUTHORITY_LEAVE,
	GUILD_AUTHORITY_ASSCHIEFTAIN	= GUILD_AUTHORITY_ASSIGN | GUILD_AUTHORITY_RECRUIT | GUILD_AUTHORITY_EXPEL | GUILD_AUTHORITY_LEAVE,
	GUILD_AUTHORITY_CHIEFTAIN		= 0x17F,	// 没有离开帮会的权限
};
//帮会返回信息
enum GUILD_RETURN_TYPE
{
	GUILD_RETURN_INVALID			= -1,

	// 创建部分
	GUILD_RETURN_CREATE,					// 创建成功
	GUILD_RETURN_RESPONSE,					// 响应成功

	// 加入部分
	GUILD_RETURN_JOIN,						// 加入申请列表等待批准
    GUILD_RETURN_INVIT,                     // 邀请加入帮会回复

	// 权限操作
	GUILD_RETURN_PROMOTE,					// 0001，任免职务，升职
	GUILD_RETURN_DEMOTE,					// 0001，任免职务，降职
	GUILD_RETURN_AUTHORIZE,					// 0002，调整权限，授权
	GUILD_RETURN_DEPRIVE_AUTHORITY,			// 0002，调整权限，解除权限
	GUILD_RETURN_RECRUIT,					// 0003，接收帮众
	GUILD_RETURN_EXPEL,						// 0004，开除帮众
	GUILD_RETURN_DEMISE,					// 0005，禅让
	GUILD_RETURN_WITHDRAW,					// 0006，支取金额
	GUILD_RETURN_DEPOSIT,					// 0007，存入金额
	GUILD_RETURN_LEAVE,						// 0008，离开帮会
	GUILD_RETURN_REJECT,					// 0009，拒绝申请
	
	GUILD_RETURN_FOUND,						// 正式成立（人数达到）
	GUILD_RETURN_DISMISS,					// 解散帮会
	GUILD_RETURN_CHANGEDESC,				// 修改帮会宗旨

	GUILD_RETURN_NAME,						// 刷新帮会名

	//新增，以后再整理
	GUILD_RETURN_CREATE_FAILED,				// 创建帮会失败
    GUILD_RETURN_CREATE_FAMILY,             // 创建家族
    GUILD_RETURN_ADD_FAMILY_USER,           // 添加家族成员
    GUILD_RETURN_REMOVE_FAMILY_USER,        // 删除家族成员
    GUILD_RETURN_MODIFY_DESC,               // 修改帮会宗旨
    GUILD_RETURN_FAMILY_DISMISS,            // 解散家族
    GUILD_RETURN_BEADD_FAMILY,              // 通知被添加为家族成员    
    GUILD_RETURN_LEVEL_UP,                  // 帮会升级    
    GUILD_RETURN_ALREADY_IN,                // 已经在其他帮会
    GUILD_RETURN_ERASE_ITEM_FAIL,           // 扣除帮助令失败

	GUILD_RETURN_SIZE,
};
//帮会错误消息
enum GUILD_ERROR_TYPE
{
	// 创建部分
	GUILD_ERROR_NOTHING				= 0,	// 没什么错误……
	GUILD_ERROR_WANTING				= -1,	// 不符合创建条件，只给出错误符，具体的错误描述由脚本发送
	GUILD_ERROR_INVALID_NAME		= -2,	// 名字非法
	GUILD_ERROR_DUPLICATED_NAME		= -3,	// 重名
	GUILD_ERROR_GUILD_FULL			= -4,	// 不能创建更多帮会

	// 加入部分
	GUILD_ERROR_MEMBER_FULL			= -5,	// 帮会满员了
	GUILD_ERROR_PROPOSER_FULL		= -6,	// 申请人已满
	GUILD_ERROR_IN_GUILD			= -7,	// 玩家已加入帮会
	GUILD_ERROR_NOT_EXIST			= -8,	// 帮会不存在

	// 权限操作
	GUILD_ERROR_UNAUTHORIZED		= -9,	// 没有权限
	GUILD_ERROR_NO_VACANCY			= -10,	// 0001，任免职务，没有职位空缺
	GUILD_ERROR_NO_QUALIFICATION	= -11,	// 0002，调整权限，被调整人员（职位）不够资格
	GUILD_ERROR_CASHBOX_FULL		= -12,	// 0007，存入金额，帮会金库已满

	GUILD_ERROR_ALREADY_MEMBER		= -13,	// 此人已经是帮会成员
	GUILD_ERROR_MEMBER_NOT_EXIST	= -14,	// 此人不在帮会中

	//禅让
	GUILD_ERROR_NO_ASS_CHIEF		= -15,	// 没有副帮主
	GUILD_ERROR_GUILD_ALREADY_EXIST = -16,	//	帮会已存在

	//这个职位人数已满，不能任命
	GUILD_ERROR_POS_FULL			= -17,	//这个职位人数已满，不能任命

	//新增部分，ID以后再整理
	GUILD_ERROR_LEVEL_TOO_LOW		= -18,	// 等级太低不能创建帮会
	GUILD_ERROR_LEVEL_TO_JOIN		= -19,	// 等级太低不能加入帮会


    GUILD_ERROR_PASSWORD		    = -20,	// 密码错误
    GUILD_ERROR_WAITTING		    = -21,	// 申请待确认
    GUILD_ERROR_FAMILY_DIMISS       = -22,	// 解散家族失败
	// 解散部分

	GUILD_ERROR						= -64,	// 不明错误
};
//帮会包类型
enum GUILD_PACKET_TYPE
{
	GUILD_PACKET_INVALID			= -1,
	GUILD_PACKET_CG_ASKLIST,				// 询问帮会列表(UCHAR)
	GUILD_PACKET_CG_CREATE,					// 创建帮会
 	GUILD_PACKET_CG_JOIN,					// 加入帮会
	GUILD_PACKET_CG_ASKINFO,				// 询问帮会信息
	GUILD_PACKET_CG_APPOINT,				// 帮会任免
	GUILD_PACKET_CG_ADJUSTAUTHORITY,		// 调整帮会权限
	GUILD_PACKET_CG_RECRUIT,				// 帮会收人
	GUILD_PACKET_CG_EXPEL,					// 帮会踢人
	GUILD_PACKET_CG_WITHDRAW,				// 提取帮资
	GUILD_PACKET_CG_DEPOSIT,				// 存入帮资
	GUILD_PACKET_CG_LEAVE,					// 离开帮会
	GUILD_PACKET_CG_DISMISS,				// 解散帮会
	GUILD_PACKET_CG_DEMISE,					// 解散帮会
	GUILD_PACKET_CG_CHANGEDESC,				// 更改帮会宗旨
    GUILD_PACKET_CG_INVITE,                 // 邀请加入帮会
    GUILD_PACKET_CG_INVITE_CONFIRM,         // 帮会邀请加入确认
    GUILD_PACKET_CG_CREATE_FAMILY,          // 请求创建家族
    GUILD_PACKET_CG_ADD_FAMILY_USER,        // 添加家族成员
    GUILD_PACKET_CG_REMOVE_FAMILY_USER,     // 删除家族成员
    GUILD_PACKET_CG_MODIFY_DESC,            // 修改帮会宗旨
    GUILD_PACKET_CG_MODIFY_TITLE_NAME,      // 修改帮会称号
    GUILD_PACKET_CG_FAMILY_DISMISS,         // 解散家族
    GUILD_PACKET_CG_LEVELUP,                // 升级帮会

	GUILD_PACKET_CG_GW_SEPARATOR,			// 分隔符

	GUILD_PACKET_GW_ASKLIST,				// 询问帮会列表(GUID_t, UCHAR)
	GUILD_PACKET_GW_CREATE,					// 创建帮会
	GUILD_PACKET_GW_JOIN,					// 加入帮会
	GUILD_PACKET_GW_ASKINFO,				// 询问帮会信息
	GUILD_PACKET_GW_APPOINT,				// 帮会任免
	GUILD_PACKET_GW_ADJUSTAUTHORITY,		// 调整帮会权限
	GUILD_PACKET_GW_RECRUIT,				// 帮会收人
	GUILD_PACKET_GW_EXPEL,					// 帮会踢人
	GUILD_PACKET_GW_WITHDRAW,				// 提取帮资
	GUILD_PACKET_GW_DEPOSIT,				// 存入帮资
	GUILD_PACKET_GW_LEAVE,					// 离开帮会
	GUILD_PACKET_GW_DISMISS,				// 解散帮会
	GUILD_PACKET_GW_DEMISE,					// 禅让
	GUILD_PACKET_GW_CHANGEDESC,				// 更改帮会宗旨
    GUILD_PACKET_GW_INVITE,                 // 邀请加入帮会
    GUILD_PACKET_GW_INVITE_CONFIRM,         // 帮会邀请加入确认
    GUILD_PACKET_GW_CREATE_FAMILY,          // 请求创建家族
    GUILD_PACKET_GW_ADD_FAMILY_USER,        // 添加家族成员
    GUILD_PACKET_GW_REMOVE_FAMILY_USER,     // 删除家族成员
    GUILD_PACKET_GW_MODIFY_DESC,            // 修改帮会宗旨
    GUILD_PACKET_GW_MODIFY_TITLE_NAME,      // 修改帮会称号
    GUILD_PACKET_GW_FAMILY_DISMISS,         // 解散家族
    GUILD_PACKET_GW_LEVELUP,                // 升级帮会
    GUILD_PACKET_GW_ASKMEMBERLIST,			// 帮会成员信息列表
    GUILD_PACKET_GW_ADD_GP,                 // 增加帮会总帮贡
    GUILD_PACKET_GW_SUB_GP,                 // 减少帮会总帮会
    GUILD_PACKET_GW_LEVEL_INFO,             // 帮会等级信息
	
	GUILD_PACKET_GW_WG_SEPARATOR,			// 分隔符

	GUILD_PACKET_WG_LIST,					// 帮会列表
	GUILD_PACKET_WG_MEMBER_LIST,			// 帮众列表
	GUILD_PACKET_WG_GUILD_INFO,				// 帮会信息
	GUILD_PACKET_WG_APPOINT_INFO,			// 帮会可任命信息
	GUILD_PACKET_WG_SELF_GUILD_INFO,		// 个人帮派信息
	GUILD_PACKET_WG_CONFIRM_GUILD_INVITE,	// 转发帮会邀请消息
    GUILD_PACKET_WG_MODIFY_TITLE_NAME,		// 通知客户端修改帮会称号
    GUILD_PACKET_WG_ADD_GP,                 // 增加帮会总帮贡
    GUILD_PACKET_WG_SUB_GP,                 // 减少帮会总帮会


	GUILD_PACKET_WG_GC_SEPARATOR,			// 分隔符

	GUILD_PACKET_GC_LIST,					// 帮会列表
	GUILD_PACKET_GC_MEMBER_LIST,			// 帮众列表
	GUILD_PACKET_GC_GUILD_INFO,				// 帮会信息
	GUILD_PACKET_GC_APPOINT_INFO,			// 帮会可任命信息
	GUILD_PACKET_GC_SELF_GUILD_INFO,		// 个人帮派信息
	GUILD_PACKET_GC_CONFIRM_GUILD_INVITE,	// 确认帮会邀请
    GUILD_PACKET_GC_MODIFY_TITLE_NAME,      // 通知客户端修改帮会称号
    GUILD_PACKET_GC_FAMILY_DISMISS,         // 解散家族
};


#endif // __GAMEDEFINE_GUILD_H__
