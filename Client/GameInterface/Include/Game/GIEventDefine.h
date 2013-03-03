
/**	GIEventDefine.h
 *
 *	功能：	事件处理
 *
 *	描述：  Nick 080314
 *			向lua脚本发送的消息的枚举定义，
 *          lua脚本使用了与枚举值对应的整数。
 *			所以新添加枚举值一定要加到最后。以避免lua脚本消息出错。
 *	
 *			Nick 080327
 *			新增在脚本里PushEvent用与GAME_EVENT_ID对应的字符串的功能。
 *			所以该枚举可以随意加了。
 *	
 *
 *	修改记录：
 *			080305	GE_GUILD_INVITE_OBJECT
 *			080306	GE_GUILD_NOTIFY_INTIVE
 *					GE_GUILD_INTIVE_CONFIRM
 *					GE_GUILD_MODIFY_TITLE
 *			080308	GE_GUILD_CREATE_FAMILY
 *					GE_GUILD_CREATE_FAMILY_CONFIRM
 *					GE_GUILD_DESTORY_FAMILY_CONFIRM
 *			080314	GE_OPEN_FRIEND_SEARCH
 *			080318	GE_FRIEND_CREATE_GROUP
 *			080319	GE_OPEN_FRIEND_SEARCH_REASON
 *			080320	GE_OPEN_BUY_CONFIRM
 *			080320	GE_OPEN_SALE_CONFIRM
 *			080324	GE_QUICK_CHANGE_NAME
 *			080325	GE_ADD_GROUP
 *			080327	GE_ITEM_LINK
 *			080331	GE_SHOW_TITLE
 *					GE_UPDATE_TITLE
 *			080402	GE_SHOW_FRIEND_INFO
 *			080407	GE_PERSONAL_MESSAGE		// 人物消息
 *					GE_SYSTEM_MESSAGE		// 系统消息
 *					GE_SHOW_SYSTEM_PROMPT
 *			080410	GE_SHOW_SUPERTOOLTIP_TITLE,		// 显示称号提示
 *					GE_RICH_SAFE_GUARD,				// 保护范围界面
 *					GE_RICH_SAFE_TIME,				// 安全时限弹出界面
 *					GE_RICH_SET_PASSWORD,			// 设置密码、修改密码界面
 *					GE_RICH_SETUP,					// 财产保护功能设置界面
 *					GE_UNITE_GF_SHOW
 *			080418	GE_INPUT_PASSWD_COMMAND
 *			080331	GE_OPEN_DAY_TIPS
 *					GE_UPDATE_DAYTIPS_TIME	
 *					GE_UPDATE_DAY_TIPS
 *					GE_UPDATE_CALENDAR
 *					GE_OPEN_TOP_LIST
 *			080410	GE_UPDATE_TOP_LIST
 *
 *			080417	GE_OPEN_MISSION_SCOUT
 *					GE_UPDATE_MISSION_SCOUT
 *			080331	GE_OPEN_DAY_TIPS
 *					GE_UPDATE_DAYTIPS_TIME	
 *					GE_UPDATE_DAY_TIPS
 *					GE_UPDATE_CALENDAR
 *					GE_OPEN_TOP_LIST
 *			080410	GE_UPDATE_TOP_LIST
 *
 *			080417	GE_OPEN_MISSION_SCOUT
 *					GE_UPDATE_MISSION_SCOUT
 *			080421	GE_OPEN_BANK_EVENT
 *					GE_QUEST_INPUT_PASSWD_COMMAND
 *					GE_PACKET_OPEN_SALE
 *					GE_COUNTRY_MANAGER_INFO,		// 国家信息
 *					GE_COUNTRY_MANAGER_ADJUST_AUTH,	// 国家官职调整
 *			080422	GE_INPUT_COUNTRY_DLG
 *					GE_COUNTRY_KING_CREATE_CONFIRM
 *			080425	GE_COUNTRY_MANAGER_CREATE_KING
 *					GE_COUNTRY_MANAGER_REQUEST_INFO
 *
 */

#pragma once



/// 
enum GAME_EVENT_ID
{
	GE_APPLICATION_INITED,			/// 游戏程序初始化
	GE_ON_SCENE_TRANS,				/// 场景转移
	GE_SCENE_TRANSED,				/// 场景转移后
									/// arg0- 小地图的名称
									//
	GE_ON_SCENE_UPDATED,			/// 场景更新(一般用于玩家城市)
									/// arg0- 小地图的名称
									//
	GE_ON_SERVER_TRANS,				/// 服务器转移
	GE_PLAYER_ENTER_WORLD,			/// 进入世界
	GE_PLAYER_LEAVE_WORLD,			/// 退出世界
	GE_TEAM_CHANG_WORLD,			/// 队友改变场景了
	GE_SKILL_CHANGED,				/// 玩家的某项技能发生改变,
									// arg0 - 技能id
	GE_PACKAGE_ITEM_CHANGED,		/// 包裹中的某个物品发生改变
									// arg0 - 物品在包裹中的编号

	GE_MAINTARGET_CHANGED,			/// 主选择对象更改
									// arg0 - 对象ServerId

	
	GE_MAINTARGET_OPEND,			// 当target 窗口选择是队友的情况， 打开窗口
									

	GE_TOGLE_SKILL_BOOK,			// 打开/关闭技能书
	GE_CLOSE_SKILL_BOOK,			// 关闭技能书

	GE_TOGLE_PET_PAGE,				// 打开宠物界面
	GE_TOGLE_PETATTRIBUTE_PAGE,		//打开宠物详细属性界面
	GE_UPDATE_PETATTRIBUTE_PAGE,	//刷新宠物详细属性界面
	GE_PETPAGE_CLOSE,		//宠物界面关闭了
	GE_TOGLE_LIFE_PAGE,				// 打开生活技能界面
	GE_TOGLE_COMMONSKILL_PAGE,		// 打开普通技能界面
	GE_SKILL_UPDATE,				//玩家技能数据发生改变

	GE_TOGLE_CONTAINER,				// 打开/关闭包裹

	//-----------------------------------------------------
	//角色属性
	GE_UNIT_HP,						//某个单元的HP发生变化
									// arg0 - 单元名称
									//		"player"	- 玩家自己
									//		"target"	- 主目标
									//		"pet"		- 自己的宠物
									//		"party*"	- 队友(1-4)
									//		"partypet*"	- 队友的宠物(1-4)
									//		...
	GE_UNIT_MP,						//某个单元的MANA发生变化
									// arg0 - 单元名称(同上)
	GE_UNIT_PENUMA,					//某个单元的元气发生变化
									// arg0 - 单元名称(同上)

	GE_UNIT_RAGE,					//某个单元的怒气发生变化
									// arg0 - 单元名称(同上)

//	GE_UNIT_XINFA,					//某个单元的心法等级发生变化
									// arg0 - 单元名称(同上)
									// arg1 - 心法名称

	GE_UNIT_EXP,					//某个单元的经验值发生变化
									// arg0 - 单元名称(同上)

	GE_UNIT_MONEY,					//某个单元的钱发生变化
									// arg0 - 单元名称(同上)

	GE_UNIT_RACE_ID,				// 数据表中的ID
	GE_UNIT_NAME,					// 角色的名字，从服务器传来
	GE_UNIT_CAMP_ID,				// 阵营ID
	GE_UNIT_LEVEL,					// 等级
	GE_UNIT_MOVE_SPEED,				// 移动的速度
	GE_UNIT_FIGHT_STATE,			// 战斗状态	
	GE_UNIT_MAX_EXP,				// 最大EXP
	GE_UNIT_TITLE,					// 称号
	GE_UNIT_STRIKEPOINT,			// 连击点
	GE_UNIT_RELATIVE,				// 归属问题

	//-------------
	//一级战斗属性
	GE_UNIT_STR,					// 外功
	GE_UNIT_SPR,					// 内功
	GE_UNIT_CON,					// 身法
	GE_UNIT_INT,					// 体魄
	GE_UNIT_DEX,					// 智慧
	GE_UNIT_POINT_REMAIN,			// 剩余待分配点数

	//-------------
	//二级战斗属性
	GE_UNIT_ATT_NEAR,				// 近攻
	GE_UNIT_DEF_NEAR,				// 近防
	GE_UNIT_ATT_FAR,				// 远攻
	GE_UNIT_DEF_FAR,				// 远防

//	GE_UNIT_ATT_PHYSICS,			// 物理攻击力			
	GE_UNIT_ATT_MAGIC_NEAR,				// 近程内功			
	GE_UNIT_ATT_MAGIC_FAR,				// 远程内功			
//	GE_UNIT_DEF_PHYSICS,			// 物理防御力			
	GE_UNIT_DEF_MAGIC_NEAR,			// 近程内功防御力	
	GE_UNIT_DEF_MAGIC_FAR,			//远程内功防御力
	GE_UNIT_MAX_HP,					// 最大生命点			
	GE_UNIT_MAX_MP,					// 最大魔法点		
	GE_UNIT_MAX_PNEUMA,				// 最大元气点	
	GE_UNIT_MAX_RAGE,				// 最大怒气点
	GE_UNIT_HP_RE_SPEED,			// HP恢复速度  点/秒	
	GE_UNIT_MP_RE_SPEED,			// MP恢复速度  点/秒	
	GE_UNIT_TOUGHNESS,				// 韧性
	GE_UNIT_HIT,					// 命中率				
	GE_UNIT_MISS,					// 闪避率				
	GE_UNIT_CRIT_RATE,				// 会心率				
	GE_UNIT_ATT_SPEED,				// 攻击速度	
	GE_UNIT_ATT_GOLD,				// 近攻击	
	GE_UNIT_DEF_GOLD,				// 金防御	
	GE_UNIT_ATT_FIRE,				// 火攻击	
	GE_UNIT_DEF_FIRE,				// 火防御	
	GE_UNIT_ATT_WATER,				// 水攻击	
	GE_UNIT_DEF_WATER,				// 水防御	
	GE_UNIT_ATT_WOOD,				// 木攻击	
	GE_UNIT_DEF_WOOD,				// 木防御
	GE_UNIT_ATT_SOIL,				// 土攻击	
	GE_UNIT_DEF_SOIL,				// 土防御
	GE_UNIT_VIGOR,					// 活力值
	GE_UNIT_MAX_VIGOR,				// 活力值上限
	GE_UNIT_ENERGY,					// 精力值
	GE_UNIT_MAX_ENERGY,				// 精力值上限

	GE_UNIT_MENPAI,					// 门派号
	GE_UNIT_HAIR_MESH,				// -> DBC_CHARACTER_HAIR_GEO
	GE_UNIT_HAIR_COLOR,				// 头发颜色
	GE_UNIT_FACE_MESH,				// -> DBC_CHARACTER_HEAD_GEO		char_head.tab
	GE_UNIT_FACE_IMAGE,				// 头像信息改变
	GE_UNIT_EQUIP_VER,				// 角色的装备状态版本号，用于和服务器同步数据的依据
	GE_UNIT_EQUIP,					// 装备表(含武器)
	GE_UNIT_EQUIP_WEAPON,			// 武器


	//--------------------------------------------------------------------------------

	GE_SHOW_CONTEXMENU,				//显示右键菜单
									// arg0 - 菜单类
									//		"player"		- 玩家自己
									//		"other_player"  - 其他玩家
									//		"npc"			- npc
									// arg1 - 鼠标位置_x
									// arg2 - 鼠标位置_y

	GE_TOGLE_COMPOSE_WINDOW,		// 打开关闭合成界面

	GE_TOGLE_CONSOLE,				// 显示控制台

	GE_ON_SKILL_ACTIVE,				// 某个技能开始使用
									// arg0 - 技能ID

	GE_MAINTARGET_BUFF_REFRESH,		// 更新角色的buff队列事件

	GE_POSITION_MODIFY,				// 人物得位置改变了

	GE_CHAT_MESSAGE,				// 得到聊天信息
									/*
									|  arg0 - 聊天类型
									|		"near"		- 附近玩家
									|		"scene"		- 场景
									|		"sys"		- 系统
									|		"team"		- 队伍
									|		"guild"		- 帮会
									|		"user"		- 自建
									|		"private"	- 私聊
									|
									|  arg1 - 说话者的名字
									|
									|  arg2 - 说话内容
									|
									*/

	GE_CHAT_CHANNEL_CHANGED,		//聊天频道发生改变
	GE_CHAT_CHANGE_PRIVATENAME,		//修改聊天中的密语对象
	GE_CHAT_CHANGE_COPY,				//聊天超链接弹出菜单中的复制命令
	GE_CHAT_TAB_CREATE,				//创建聊天页面
	GE_CHAT_TAB_CREATE_FINISH,		//创建聊天页面完成
	GE_CHAT_TAB_CONFIG,				//配置聊天页面
	GE_CHAT_TAB_CONFIG_FINISH,		//配置聊天页面完成
	GE_CHAT_FACEMOTION_SELECT,		//聊天表情选择
	GE_CHAT_TEXTCOLOR_SELECT,		//聊天文字颜色选择
	GE_CHAT_CONTEX_MENU,			//聊天相关的快捷菜单
	GE_CHAT_ACTSET,					//聊天动作命令
	GE_CHAT_ACT_SELECT,				//聊天动作命令界面显示
	GE_CHAT_ACT_HIDE,				//聊天动作命令界面关闭，因为动作命令界面比较特殊，需要支持托拽。
	GE_CHAT_PINGBI_LIST,			//聊天屏蔽界面
	GE_CHAT_PINGBI_UPDATE,			//屏蔽列表更新
	GE_CHAT_ADJUST_MOVE_CTL,		//通知聊天窗口调整大小
	GE_CHAT_INPUTLANGUAGE_CHANGE,	//输入法变更
	GE_CHAT_SHOWUSERINFO,			//聊天查看玩家信息界面
	GE_CHAT_LOAD_TAB_CONFIG,		//聊天页面配置信息

	GE_ACCELERATE_KEYSEND,			//键盘快捷键发送
	
	GE_LOOT_OPENED,					// 箱子打开
	GE_LOOT_SLOT_CLEARED,			// 箱子中的某个位置清空
									// arg0 - 箱子中的位置

	GE_LOOT_CLOSED,					// 箱子关闭

	GE_PROGRESSBAR_SHOW,			// 进度条显示
	GE_PROGRESSBAR_HIDE,			// 进度条隐藏
	GE_PROGRESSBAR_WIDTH,			// 进度条宽度调整
									// arg0 - 调整到的百分比 1 < x < 100

	GE_CHANGE_BAR,					//改变工具条上的Action
									// arg0 - Bar类型
									// arg1 - Index
									// arg2 - ActionItem ID

	GE_COLSE_SECOND_MENU,			/// 关闭所有二级菜单
	GE_TOGLE_MISSION,				/// 接收任务界面
	GE_UPDATE_MISSION,				/// 刷新任务列表
	GE_REPLY_MISSION,				/// 提交任务界面
	GE_UPDATE_REPLY_MISSION,		/// 刷新提交任务界面

	GE_TOGLE_LARGEMAP,				// 关闭打开大地图
	GE_TOGLE_CLOSELARGEMAP,			// 关闭大地图
	GE_TOGLE_SCENEMAP,				// 关闭打开场景地图
	GE_TOGLE_CLOSESCENEMAP,			// 关闭场景地图
	GE_OPEN_MINIMAP,				// 打开小地图
	GE_OPEN_MINIMAPEXP,				// 打开扩展小地图

	GE_QUEST_EVENTLIST,				//打开和npc第一次对话时的可执行脚本列表
	GE_QUEST_INFO,					//打开和npc第二次对话(在接任务时，看到的任务信息，等待接受)
	GE_QUEST_REGIE,					//漕运任务
	GE_QUEST_CONTINUE_DONE,			//接受任务后，再次和npc对话，所得到的任务需求信息，(任务完成)
	GE_QUEST_CONTINUE_NOTDONE,		//接受任务后，再次和npc对话，所得到的任务需求信息，(任务未完成)
	GE_QUEST_AFTER_CONTINUE,		//点击“继续之后”，奖品选择界面
	GE_QUEST_TIPS,					//游戏过程中的任务进展提示信息
	GE_QUEST_INPUT_PASSWD_COMMAND,	// 通过输入命令打开物品操作

	GE_TOGLE_COMPOSEITEM,			// 打开/关闭 合成界面
									// arg0 - 生活技能的名称
	GE_TOGLE_COMPOSEGEM,			// 打开/关闭 宝石合成界面
									// 
	GE_ON_LIFEABILITY_ACTIVE,		// 某个生活技能开始使用
									// arg0 - 技能ID
									// arg1 - 配方
									// arg2 - 平台
	GE_NEW_DEBUGMESSAGE,			//新的debug string弹出，在屏幕上用listbox显示。

	//-------------------------------------------------------------------------------------------------------
	GE_QUICK_CHANGE_EQUIP,			// 打开关闭一键换装界面
	GE_UPDATE_QUICK_CHANGE,			// 更新一键换装界面
	GE_QUICK_CHANGE_SUCCESS,		// 一键换装成功
	GE_UPDATE_QUICK_CHANGE_EQUIPNAME,		//更新一键换装套装的名字


	//-------------------------------------------------------------------------------------------------------
	// 人物属性界面
	GE_UPDATE_EQUIP,				// 更新装备
	GE_OPEN_BOOTH,					// 打开货架栏
	GE_CLOSE_BOOTH,					// 关闭货架栏
	GE_MANUAL_ATTR_SUCCESS_EQUIP,	// 手动调节属性成功.
	GE_CUR_TITLE_CHANGEED,			// 当前人物的称号改变了.

	GE_UPDATE_BOOTH,				// 货架更新

	GE_OPEN_CHARACTOR,				// 打开人物属性栏
	GE_OPEN_EQUIP,					// 打开装备栏
	GE_TOGLE_JOINSCHOOL,			// 打开加入门派的对话框



	GE_OPEN_SOUXIA,			//打开搜侠录界面
	GE_CLOSE_SOUXIA,			//关闭搜侠录界面
	GE_UPDATE_SOUXIA_TAB,

	GE_OPEN_SOUXIASEW,				//打开搜侠录缝合界面
	GE_OPEN_SOUXIACOMPLEX,		//打开搜侠录炼化界面
	GE_OPEN_SOUXIADESTORY,		//打开搜侠录销毁界面

	GE_OPEN_SOUXIADISMANTLE,	//打开搜侠录拆解界面
	GE_ADD_SOUXIADISMANTLE_ITEM,	//搜侠录拆解界面中安放搜侠录道具
	GE_ADD_SOUXIADISMANTLE_DIS,		//搜侠录拆解界面中安放搜侠录拆解道具
	GE_SHOW_SOUXIADISMANTLE_BUILD,	//搜侠录拆解界面中显示生成道具

	GE_ADD_SOUXIADESTORY_ITEM,			//搜侠录销毁界面中安放搜侠录道具

	GE_ADD_SOUXIACOMPLEX_ITEM1,			//搜侠录炼化界面中安放搜侠录道具1
	GE_ADD_SOUXIACOMPLEX_ITEM2,			//搜侠录炼化界面中安放搜侠录道具2
	GE_ADD_SOUXIACOMPLEX_DIS,			//搜侠录炼化界面中安放搜侠录炼化道具
	GE_SHOW_SOUXIACOMPLEX_BUILD,		//搜侠录炼化界面中显示生成的物品

	GE_ADD_SOUXIASEW_ITEM1,			//搜侠录缝合界面中安放搜侠录道具
	GE_ADD_SOUXIASEW_DISITEM,			//搜侠录缝合界面中安放搜侠录残页道具
	GE_ADD_SOUXIASEW_DIS,			//搜侠录缝合界面中安放搜侠录缝合道具
	GE_SHOW_SOUXIASEW_BUILD,	//搜侠录缝合界面中显示生成的物品
	
	// Nick 2007.11.22
	GE_OPEN_MAIN_FUNC_BTN,			// 主功能按钮，用于打开8个主功能按钮

 
	GE_OPEN_ADD_HOLE,				//打开打孔界面
	GE_UPDATE_ADD_HOLE,				//更新打孔界面
	GE_ADD_HOLE_SUCCESS,			//成功打孔
    
	GE_OPEN_BANG_DING,				//打开绑定界面				
	GE_UPDATE_BANG_DING,			//更新绑定界面		     
	GE_ADD_BANG_DING_SUCCESS,		//绑定成功

	GE_OPEN_DA_XING,				//打开打星界面	
	GE_UPDATE_DA_XING,				//更新打星界面	
	GE_OPEN_DA_XING_SUCCESS,		//打星成功	

	GE_OPEN_SP_REPAIR,				//打开特修界面	
	GE_UPDATE_SP_REPAIR,			//更新特修界面	
	GE_OPEN_SP_REPAIR_SUCCESS,		//特修成功	
	GE_OPEN_REPAIR_SURE,			//弹出修理消费提示对话框

	GE_OPEN_SYNTHESIZE,				//打开合成与打造对话框
	GE_CLOSE_SYNTHESIZE,			//关闭合成与打造对话框

	GE_OPEN_EQUIP_REWORK,			//打开改造对话框
	GE_CLOSE_EQUIP_REWORK,			//关闭改造对话框

	GE_OPEN_EQUIP_DISASS,			//打开拆解装备对话框
	GE_UPDATE_EQUIP_DISASS,         //更新
	GE_CLOSE_EQUIP_DISASS,			//关闭拆解装备对话框
									
	GE_UPDATE_CONTAINER,			// 包裹状态更新
	GE_IMPACT_SELF_UPDATE,			// 自己的特效更新 
	GE_IMPACT_SELF_UPDATE_TIME,		// 自己的特效时间的更新 
	GE_TOGLE_SKILLSTUDY,			// 技能心法的学习和升级
	GE_SKILLSTUDY_SUCCEED,			// 技能心法学习成功

	GE_TOGLE_ABILITY_STUDY,			// 生活技能学习界面
	GE_OPEN_AGNAME,					// 打开选择称号界面
	GE_CLOSE_AGNAME,				// 关闭选择称号界面
	
	GE_TOGLE_BANK,					// 打开银行界面
	GE_UPDATE_BANK,					// 更新银行界面
	GE_TOGLE_INPUT_MONEY,			// 打开输入钱的界面
	GE_CLOSE_INPUT_MONEY,			// 关闭输入金钱界面
	GE_OPEN_BANK_EVENT,				// 判断银行密码保护

	GE_RECEIVE_EXCHANGE_APPLY,		// 收到交易请求
	GE_STOP_EXCHANGE_TIP,			// 停止交易请求提示
	GE_OPEN_EXCHANGE_FRAME,			// 打开交易对话筐
	GE_UPDATE_EXCHANGE,				// 更新交易界面
	GE_CANCEL_EXCHANGE,				// 取消交易
	GE_SUCCEED_EXCHANGE_CLOSE,		// 交易成功，通知关闭交易界面（可能还会关闭相关界面）
	GE_EXCHANGE_INPUT_MONEY,		// 交易框输入的money
	GE_PACKET_OPEN_SALE,			// 密码保护摆摊交易

	GE_UPDATE_PET_PAGE,				// 刷新宠物界面
	GE_UPDATE_LIFESKILL_PAGE,		// 刷新生活技能界面
	GE_OPEN_COMPOSE_ITEM,			// 打开制作物品界面
	GE_UPDATE_COMPOSE_ITEM,			// 刷新制作物品界面
	GE_OPEN_COMPOSE_GEM,			// 打开宝石合成/镶嵌界面
	GE_UPDATE_COMPOSE_GEM,			// 刷新宝石合成/镶嵌界面
	GE_COMPOSE_GEM_SUCCESS,			// 刷新宝石合成/镶嵌界面
	GE_ENCHASE_CONFIRM_DLG,			// 镶嵌宝石替换确认界面

	GE_AFFIRM_SHOW,					// 打开放弃任务确认界面


	GE_OPEN_STALL_SALE,				// 摆摊(卖)
	GE_OPEN_STALL_BUY,				// 摆摊(买)
	GE_UPDATE_STALL_BUY,			// 更新(卖)
	GE_UPDATE_STALL_SALE,			// 更新(买)
	GE_OPEN_STALL_RENT_FRAME,		// 摆摊(租金税率提示)
	GE_STALL_SALE_DLG,				// 收摊确认 add by gh 2010/04/15
	GE_CLOSE_STALL_SALE,			// 收摊(关闭摊的对话框) 
	GE_STALL_SALE_SELECT,			// 摆摊(卖)时有选中摊位上的物品
	GE_STALL_BUY_SELECT,			// 摆摊(买)时有选中摊位上的物品
	GE_OPEN_STALL_MESSAGE,			// 摆摊(信息发布界面)
	GE_CLOSE_STALL_MESSAGE,			// 摆摊(关系消息界面)

	GE_STALL_OPEN_PUBLIC,			// 通知摆摊界面设置玩家开市状态		20100706BLL

	GE_OPEN_DISCARD_ITEM_FRAME,		// 销毁物品
	GE_OPEN_CANNT_DISCARD_ITEM,		// 不能销毁物品

	GE_SHOW_SPLIT_ITEM,				// 打开拆分武平对话框
	GE_HIDE_SPLIT_ITEM,				// 关闭拆分武平对话框

	GE_TOGLE_FRIEND_INFO,			// 打开关闭好友列表对话框
	GE_TOGLE_FRIEND,				// 打开关闭好友对话框
	GE_UPDATE_FRIEND,				// 跟新好友数据了
	GE_UPDATE_FRIEND_INFO,			// 好友数据界面打开
	GE_OPEN_EMAIL,					// 打开信件界面
	GE_OPEN_EMAIL_WRITE,			// 打开信件界面
	GE_HAVE_MAIL,					// 有邮件
	GE_SEND_MAIL,					// 发送邮件
	GE_UPDATE_EMAIL,				// 刷新当前邮件
	GE_FRIEND_SET,					// 设置
	GE_MOOD_CHANGE,					// 心情改变了
	GE_OPEN_HISTROY,				// 玩家历史消息
	GE_ADD_TEMPFRIEND,				// 加入到临时好友

	// 好友搜索
	GE_OPEN_FRIEND_SEARCH,			// 搜索好友界面

	//系统设置相关
	GE_TOGLE_SYSTEMFRAME,			// 打开系统主界面
	GE_TOGLE_VIEWSETUP,				// 打开视频设置界面
	GE_TOGLE_SOUNDSETUP,			// 打开声音设置界面
	GE_TOGLE_UISETUP,				// 打开界面设置
	GE_TOGLE_INPUTSETUP,			// 打开按键设置
	GE_TOGLE_GAMESETUP,				// 游戏性设置

	//玩家商店
	GE_PS_OPEN_OTHER_SHOP,			// 打开别人的商店
	GE_PS_OPEN_MY_SHOP,				// 打开自己的商店
	GE_PS_OPEN_CREATESHOP,			// 打开创建商店界面
	GE_PS_CLOSE_CREATESHOP,			// 关闭创建商店界面
	GE_PS_OPEN_SHOPLIST,			// 打开商店列表界面
	GE_PS_SELF_ITEM_CHANGED,		// 自己商店内物品发生改变
	GE_PS_OTHER_SELECT,				// 选中一个物品
	GE_PS_SELF_SELECT,				// 选中一个商品

	GE_PS_UPDATE_MY_SHOP,			// 更新自己商店的数据
	GE_PS_UPDATE_OTHER_SHOP,		// 更新商店的数据

	GE_PS_OPEN_OTHER_TRANS,			// 打开自己的盘出状态中的商店
	GE_PS_UPDATE_OTHER_TRANS,		// 更新
	GE_PS_OTHER_TRANS_SELECT,		// 选中

	GE_OPEN_PS_MESSAGE_FRAME,		// 玩家商店操作中需要弹出的确认对话框

	GE_PS_OPEN_MESSAGE,				// 打开玩家商店的消息框
	GE_PS_UPDATE_MESSAGE,			// 更新玩家商店的消息框

	GE_OPEN_PET_LIST,				// 交易等过程使用的宠物列表
	GE_VIEW_EXCHANGE_PET,			// 交易等过程中显示宠物
	GE_CLOSE_PET_LIST,				// 交易等过程结束的时候关闭宠物列表界面
	GE_UPDATE_PET_LIST,				// 更新宠物列表界面

	GE_OPEN_PRIVATE_INFO,			// 打开玩家信息对话框

	GE_OPEN_CALLOF_PLAYER,			// 打开有人要拉你的对话框
	GE_NET_CLOSE,					// 断开连接

	GE_OPEN_ITEM_COFFER,			// 打开锁定物品界面

	GE_PS_INPUT_MONEY,				// 弹出玩家商店的输入钱的框


	//--------------------------------------------------------------------------------------------------------------------
	// 组队相关事件

	GE_TEAM_OPEN_TEAMINFO_DLG,			// 打开队伍信息对话框.
										// 
										// arg0 - 正整数, 从0开始
										// 0 : 有人邀请你加入队伍
										// 1 : 有人申请加入队伍
										// 2 : 打开队伍信息
										// -1: 关闭对话框


	GE_TEAM_NOTIFY_APPLY,				// 通知队长, 有人申请加入队伍.
										// 
										// arg0 - 正整数, 从0开始
										// 0 : 有人邀请你加入队伍
										// 1 : 有人申请加入队伍
										// 申请的人的具体信息从申请的信息队列中获得.


	GE_TEAM_APPLY,						// 有人申请你加入队伍.
										// 
										// arg0 - 字符串
										// 申请的人.
	

	GE_TEAM_INVITE,						// 有人邀请你加入队伍.
										// 
										// arg0 - 字符串
										// 邀请你的人.


	GE_TEAM_CLEAR_UI,					// 清空界面
										//
										// 无参数


	GE_TEAM_REFRESH_UI,					// 刷新界面
										//
										// 无参数


	GE_TEAM_MEMBER_ENTER,				// 有新的队员进入
										//
										// arg0 - 正整数, 从1 开始
										// 在ui界面中的显示位置


	GE_TEAM_UPTEDATA_MEMBER_INFO,		// 更新队员信息
										// arg0 - 正整数, 从1 开始
										// 在ui界面中的显示位置


	GE_TEAM_MESSAGE,					// 队伍信息提示信息
										// arg0 - 字符串
										// 需要提示的信息.

										// 注意, 这条消息包含了提示给界面的信息
										// 如: xxx离开队伍
										//     xxx已经在一个队伍中.


	GE_TEAM_MEMBER_INVITE,				// 队员邀请某一个人加入队伍请求队长同意
										//
										// arg0 - 队员名字
										// arg1 - 被邀请人的名字

	GE_TEAM_FOLLOW_INVITE,				// 队长邀请队员进入组队跟随模式
										//
										// arg0 - 队长名字

	GE_TEAM_REFRESH_MEMBER,				// 刷新队员信息
										//
										// arg0 - 队员的位置索引

	/**********************************************************************************************************
	**
	** 以下以后需要删除
	**
	**
	**
	***********************************************************************************************************/

	GE_ON_TEAM_ENTER_MEMBER,		// 有新队员入队

	GE_SHOW_TEAM_YES_NO,			// 打开同意组队窗口.

	GE_SHOW_TEAM_MEMBER_INFO,		// 显示队员的详细信息.

	GE_SHOW_TEAM_MEMBER_NAME,		// 在左边的队友列表框中显示队友的名字

	GE_HIDE_ALL_PLAYER,				// 自己离开队伍后, 隐藏所有的队友界面

	// 队员的详细信息
	GE_SHOW_TEAM_MEMBER_NICK,		// 名字
		
	GE_SHOW_TEAM_MEMBER_FAMILY,		// 门派
	GE_SHOW_TEAM_MEMBER_LEVEL,		// 等级
	GE_SHOW_TEAM_MEMBER_POS,		// 位置
	GE_SHOW_TEAM_MEMBER_HP,			// hp
	GE_SHOW_TEAM_MEMBER_MP,			// mp
	GE_SHOW_TEAM_MEMBER_ANGER,		// 怒气
	GE_SHOW_TEAM_MEMBER_DEAD_LINK,  // 连线信息
	GE_SHOW_TEAM_MEMBER_DEAD,		// 死亡信息.

	GE_UPDATE_TEAM_MEMBER,			// 更新队员信息
									// arg0 - 队员序号（队伍中，自己不占用序号）
									// arg1 - 队员 guid（用于取得该队员信息）

	GE_SHOW_TEAM_FUNC_MENU_MEMBER,	// 显示队员的功能菜单
	GE_SHOW_TEAM_FUNC_MENU_LEADER,	// 显示队长的功能菜单
	
	//
	// 组队相关事件
	//------------------------------------------------------------------------------------------------------------------------

	
	//------------------------------------------------------------------------------------------------------------------------
	//
	// 登录流程相关
	//

	GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET,	// 显示系统信息
												//
												// arg0 - 字符串 : 需要提示的系统信息.
												//
												// 点击确认按钮后断开网络。
	
	GE_GAMELOGIN_SHOW_SYSTEM_INFO,				// 显示系统信息
												//
												// arg0 - 字符串 : 需要提示的系统信息.


	GE_GAMELOGIN_CLOSE_SYSTEM_INFO,				// 显示队长的功能菜单

	
	GE_GAMELOGIN_OPEN_SELECT_SERVER,			// 关闭选择服务器界面
																					

	GE_GAMELOGIN_CLOSE_SELECT_SERVER,			// 关闭选择服务器界面


	GE_GAMELOGIN_OPEN_COUNT_INPUT,				// 打开帐号输入界面
																					

	GE_GAMELOGIN_CLOSE_COUNT_INPUT,				// 关闭帐号输入界面

	GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON,	// 显示系统信息, 不显示按钮
												//
												// arg0 - 字符串 : 需要提示的系统信息.

	GE_GAMELOGIN_OPEN_SELECT_CHARACTOR,			// 显示人物选择界面

	GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR,		// 关闭人物选择界面

	GE_GAMELOGIN_OPEN_CREATE_CHARACTOR,			// 显示人物创建界面

	GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR,		// 关闭人物创建界面

	GE_GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR,	// 刷新人物信息

	GE_GAMELOGIN_CLOSE_BACK_GROUND,				// 关闭背景界面

	GE_GAMELOGIN_SYSTEM_INFO_YESNO,				// 系统信息提示 yes_no 界面.
												//
												// 参数0, 提示的字符串
												// 参数1,	对话框的类型
												//			0 -- 是否退出游戏
												//			1 -- 是否删除角色
												//			2 -- 是否更换帐号

	GE_GAMELOGIN_SELECT_LOGIN_SERVER,			// 选择一个login server
												//
												// 参数0, iAreaIndex   区域索引
												// 参数1, iLoginServer 区域索引

	GE_GAMELOGIN_CLEAR_ACCOUNT,					// 清空帐号显示.
												//

	GE_GAMELOGIN_SELECT_AREA,					// 清空帐号显示.
												//
	GE_GAMELOGIN_SELECT_LOGINSERVER,			// 清空帐号显示.
												//

	GE_GAMELOGIN_CREATE_CLEAR_NAME,				// 清空创建角色的名字。
												//

	GE_GAMELOGIN_CREATE_ROLE_OK,				// 创建角色成功。
												//
												
	

	//
	// 登录流程相关
	//
	//------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------------------------------
	//
	// 是否设置二级保护密码
	//

	GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG,			// 打开设置二级密码界面

	GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG,		// 打开unlock密码界面。

	GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG,		// 更改密码界面

	GE_OPEN_SYSTEM_TIP_INFO_DLG,					// 提示系统信息对话框。

	GE_INPUT_PASSWORD_DLG,							// 输入二级密码界面
	
	//
	// 是否设置二级保护密码
	//
	//------------------------------------------------------------------------------------------------------------------------
	
	GE_SUPERTOOLTIP,				//Tooltips
									/*
									| arg0 - 显示/隐藏 1,0
									| arg1 - 类型 "skill", "lifeability", "item", "xinfa", "pet_skill"
									| arg2, arg3 - 鼠标位置
									|
									*/
											 
	// 复活界面相关
	GE_RELIVE_SHOW,					// 显示复活界面
									// arg0 - 是否有复活的按钮
									// arg1 - 剩余时间

	GE_RELIVE_HIDE,					// 隐藏复活界面

	GE_RELIVE_REFESH_TIME,			// 刷新灵魂出窍的时间
									// 显示时间

	GE_OBJECT_CARED_EVENT,			//某逻辑对象的某些发生改变
									/*
									|  arg0 - Object的ID
									|  arg1 - 类型
									|			distance: 距离发生改变 (arg2距离)
									|			destroy:  被销毁
									|
									*/
	GE_UPDATE_MAP,					// 更新地图
	GE_UPDATE_MAP_LIST,				// 更新地图列表
	GE_UPDATE_LARGEMAP_LIST,		// 更新大地图列表
	GE_UPDATE_SCENEMAP_LIST,		// 更新场景地图列表

	GE_UPDATE_SUPERTOOLTIP,
	GE_UI_COMMAND,
	GE_OTHERPLAYER_UPDATE_EQUIP,

	GE_VARIABLE_CHANGED,			//某个全局变量发生改变， 
									/*
									| arg0 - 变量名
									| arg1 - 新的变量值
									*/

	GE_TIME_UPDATE,					//由时间系统定时触发的事件			
	GE_FRIEND_INFO,					// 打开好友详细信息

	GE_UPDATE_TARGETPET_PAGE,		//打开targetpet ui
	GE_UPDATE_PETSKILLSTUDY,		//更新宠物技能学习界面
	
	GE_UPDATE_PETINVITEFRIEND,		//更新宠物征友板界面
	GE_REPLY_MISSION_PET,			//宠物刷新

	GE_GUILD_CREATE,				//帮会创建事件
	GE_GUILD_CREATE_CONFIRM,		//帮会创建确认事件
	GE_GUILD_SHOW_LIST,				//显示帮会列表事件
	GE_GUILD_SHOW_MEMBERINFO,		//显示自己帮会成员管理界面
	GE_GUILD_UPDATE_MEMBERINFO,		//更新帮会成员的信息
	GE_GUILD_SHOW_DETAILINFO,		//显示自己帮会详细信息界面
	GE_GUILD_SHOW_APPOINTPOS,		//显示自己帮会可用职位界面
	GE_GUILD_DESTORY_CONFIRM,		//删除帮会确认事件
	GE_GUILD_QUIT_CONFIRM,			//退出帮会确认事件
	GE_GUILD_FORCE_CLOSE,			//强制帮会相关界面关闭事件
	GE_GUILD_INVITE_OBJECT,			//邀请对话框
	GE_GUILD_NOTIFY_INTIVE,			//通知收到帮会邀请
	GE_GUILD_INTIVE_CONFIRM,		//邀请确认
	GE_GUILD_MODIFY_TITLE,			//修改成员称号
	GE_GUILD_CREATE_FAMILY,			//创建家族
	GE_GUILD_CREATE_FAMILY_CONFIRM,	//创建家族确认
	GE_GUILD_DESTORY_FAMILY_CONFIRM,//删除家族确认
	GE_INPUT_PASSWD_COMMAND,		//通过输入密码打开帮会界面

	GE_CLOSE_MISSION_REPLY,			//关闭任务提交UI
	GE_TOGLE_TARGET_EQUIP_PAGE,		//开启查看对方角色装备界面
	GE_CLOSE_TARGET_EQUIP,			//关闭。。。。。。。。。。
	GE_TOGLE_TARGET_EQUIP_ALL,		//开启查看对方角色属性
	GE_CLOSE_TARGET_EQUIP_ALL,		//关闭。。。。。。。。
	GE_CHANGE_TARGET_EQUIP_ALL_PAGE,//切换。。。。。。。。
	GE_VIEW_RESOLUTION_CHANGED,		//屏幕分辨率变化时的通知消息
	GE_CLOSE_SYNTHESIZE_ENCHASE,	//关闭合成UI和镶嵌UI
	GE_TOGGLE_PETLIST,				//打开和关闭宠物列表
	GE_PET_FREE_CONFIRM,			//放生宠物确认


	// 坐骑相关
	GE_TOGLE_MOUNT_PAGE,			// 打开骑宠界面
	GE_CLOSE_MOUNT_PAGE,			// 关闭骑宠界面
	GE_MOUNT_ATTR_UPDATE,			// 更新骑宠属性

	GE_TOGLE_TARGET_MOUNT_PAGE,		// 打开或关闭目标坐骑界面
	GE_TARGET_MOUNT_ATTR_UPDATE,	// 更新目标坐骑属性

	GE_TOGLE_MOUNT_STUDY_PAGE,		// 打开或关闭骑宠学习界面
	GE_MOUNT_STUDY_ITEM_UPDATE,		// 更新骑宠学习物品

	GE_TOGLE_MOUNT_FEED_PAGE,		// 打开或关闭气冲喂养界面
	GE_MOUNT_FEED_ITEM_UPDATE,		// 更新骑宠喂养物品

	GE_TOGLE_MOUNT_TRAIN_PAGE,		// 打开或关闭骑宠驯养界面
	GE_MOUNT_TRAIN_ITEM_UPDATE,		// 更新驯养物品

	GE_TOGLE_MOUNT_IDENTIFY_PAGE,	// 打开或关闭骑宠鉴定界面

	GE_TOGLE_MOUNT_MATING_PAGE,		// 打开骑宠繁殖界面
	GE_CLOSE_MOUNT_MATING_PAGE,		// 关闭骑宠繁殖界面
	GE_MY_MOUNT_MATING_LOCKED,		// 自己的交配骑宠锁定
	GE_OTHER_MOUNT_MATING_LOCKED,	// 他人的交配骑宠锁定
	GE_REQUEST_TOGLE_MOUNT_MATING_PAGE,	// 申请开启繁殖界面
	GE_GAIN_MATING_MOUNT,			// 领取繁殖坐骑


	GE_OPEN_BUY_CONFIRM,			// 打开商店买入确认界面
	GE_OPEN_SALE_CONFIRM,			// 打开商店卖出确认界面

	// 用户创建分组输入名称的事件
	GE_FRIEND_CREATE_GROUP,
	// 好友搜索结果
	GE_OPEN_FRIEND_SEARCH_REASON,    

	// 添加好友新组
	GE_ADD_GROUP,
	GE_QUICK_CHANGE_NAME,			// 一键换装命名

	GE_ITEM_LINK,					// 物品链接 arg0：物品名称

	GE_TOGLE_MOUNT_CHILD_PAGE,		// 打开或关闭骑宠还童界面
	GE_MOUNT_CHILD_ITEM_UPDATE,		// 更新骑宠还童物品

	// 显示好友的详细信息 
	GE_SHOW_FRIEND_INFO,

	GE_EQUIP_SWAPCARD_OPEN,			//打开装备牌型交换界面
	GE_EQUIP_SWAPCARD_UPDATA,		//更新装备牌型交换界面
	GE_EQUIP_SWAPCARD_SUCESS,		//装备牌型交换成功

	GE_EQUIP_CHANGECARD_OPEN,		//打开装备牌型改造界面
	GE_EQUIP_CHANGECARD_UPDATA,		//更新装备牌型改造界面
	GE_EQUIP_CHANGECARD_SUCESS,		//装备牌型改造成功	 
	
	GE_EQUIP_ALL_OPEN,				// 打开个人装备 个人属性 个人称号  一键换装 骑乘属性  个人资料


	GE_SHOW_TITLE,					// 显示称号界面
	GE_UPDATE_TITLE,				// 更新称号界面

	GE_PERSONAL_MESSAGE,			// 人物消息
	GE_SYSTEM_MESSAGE,				// 系统消息

	GE_SHOW_SYSTEM_PROMPT,			// 系统信息提示图标
//	GE_SYSTEM_PROMPT_UPDATE,		// 更新系统信息提示图标

	GE_SHOW_SUPERTOOLTIP_TITLE,		// 显示称号提示

	GE_UNITE_GF_SHOW,				// 好友和帮会的联合

	GE_RICH_SAFE_GUARD,				// 保护范围界面
	GE_RICH_SAFE_TIME,				// 安全时限弹出界面
	GE_RICH_SET_PASSWORD,			// 设置密码、修改密码界面
	GE_RICH_MODIFY_PASSWORD,
	GE_RICH_SETUP,					// 财产保护功能设置界面
	GE_OPEN_RICHES_PROTECT_FRAME,	// 财产保护提示界面
	GE_RICH_TEMP_CANCEL_PASSWORD,	// 临时取消密码	
	GE_RICH_CANCEL_PASSWORD,		// 取消密码
	GE_RICH_CHECK_PASSWORD,			// 检测输入密码


	//temp  整和界面用， 等合到一个界面里就不用了
	GE_TEMP_CLOSE_FRIEND,				// 
	GE_TEMP_CLOSE_GUILD,				// 
	GE_EQUIP_ALL_CLOSE,		// 关闭个人装备 个人属性 个人称号  一键换装 骑乘属性  个人资料

	GE_TOGLE_PLAYER_ATTR_PAGE,		// 打开人物详细属性界面
	GE_CLOSE_PLAYER_ATTR_PAGE,		// 关闭人物详细属性界面

	GE_OPEN_DAY_TIPS,				// 打开每日提醒功能界面
	GE_UPDATE_DATE_TIME,			// 更新每日提醒日期和时间
	GE_UPDATE_DAY_TIPS,				// 更新每日提醒内容
	GE_UPDATE_CALENDAR,				// 更新月历内容

	GE_OPEN_TOP_LIST,				// 打开排行榜界面
	GE_UPDATE_TOP_LIST,				// 更新排行榜界面
									// arg0：为0表示填充排行榜名称为1表示填充排行榜数据
	GE_OPEN_MISSION_SCOUT,			// 打开任务跟踪界面
	GE_UPDATE_MISSION_SCOUT,		// 更新任务跟踪界面
	GE_UPDATA_EQUIP_ALL_POS,        //更新6个界面位置 〈个人装备 个人属性 个人称号  一键换装 骑乘属性  个人资料〉


	GE_COUNTRY_MANAGER_REQUEST_INFO,// 请求国家信息
	GE_COUNTRY_MANAGER_INFO,		// 国家信息
	GE_COUNTRY_MANAGER_ADJUST_AUTH,	// 国家官职调整
	GE_INPUT_COUNTRY_DLG,			// 输入国家官职
	GE_COUNTRY_MANAGER_CREATE_KING,	// 产生国王
	GE_COUNTRY_KING_CREATE_CONFIRM,	// 确认产生国王
	GE_SUPERTOOLTIP2,

	GE_TEAM_CHANGE_MODE,			//拾取模式更改信息
									// arg0 - 更改模式id

	GE_END_PACKUP_PACKET,			//背包整理结束
	GE_BEGIN_PACKUP_PACKET,			//背包整理开始

	GE_OPEN_WINDOW,					//打开一个窗口
	GE_CLOSE_WINDOW,				//关闭一个窗口
	GE_TOGGLE_WINDOW,				//切换一个窗口

	GE_VALIDATE_ADD_FRIEND,			//确认加为好友

	GE_FRIEND_DELETE_FRIEND,		//发出是否确认删除好友信息

	// 20100419 AddCodeBegin
	GE_SET_ROLE_OPENCLOSESIGN,		// 设置主菜单人物窗口打开关闭标记
	// 20100419 AddCodeEnd

	GE_SOUXIA_BANGDING_ENCHASE,		//确认绑定搜侠录
	GE_SOUXIA_STUDY,		//确定要进行学习
	GE_UPDATE_SOUXIA_STUDY,		//更新搜侠录已学技能列表框
	GE_CLEAR_SOUXIA_BAR,				//清除掉技能快捷栏中的搜侠技能
	GE_UPDATE_SOUXIA_SKILLNUM, //更新技能快捷栏中的搜侠技能的使用次数
	GE_UPDATE_SOUXIA_INDEXSKILLNUM,		//更新指定位置上的搜侠技能信息

	GE_SHOW_EQUIPLIANHUA,			//打开装备炼化界面
	GE_CLOSE_EQUIPLIANHUA,			//关闭装备炼化界面
	GE_ADD_LIANHUAEQUIP_ITEM,	//将炼化装备放置在按钮上
	GE_ADD_LIANHUAEQUIP_DIS,		//将炼化道具放置在按钮上

	GE_OPEN_EQUIP_FUMO,				//打开附魔界面
	GE_ADD_EQUIP_FUMO_ITEM,		//将要附魔的装备放置在按钮上
	GE_ADD_EQUIP_FUMO_DIS,			//将附魔道具放置在按钮上
	GE_CLOSE_EQUIP_FUMO,			//关闭附魔界面

	GE_OPEN_EQUIP_TUIMO,				//打开退魔界面
	GE_ADD_EQUIP_TUIMO_ITEM,		//将要退魔的装备放置在按钮上
	GE_ADD_EQUIP_TUIMO_DIS,			//将退魔道具放置在按钮上
	GE_CLOSE_EQUIP_TUIMO,			//关闭退魔界面

	GE_OPEN_ADD_NEW_FRIEND,		// 打开添加好友窗口
	
	GE_MAINTARGET_BUFFTEXT_REFRESH,	// 目标头像下方buff的tooltip更新

	GE_SET_SCROLL_INFO,				// 公告板滚动消息 20100624 BLL

	//add by gh RMB商城
	GE_OPEN_IB_SHOP,				// 打开RMB商城主界面
	GE_SHOW_ALL_IB_ITEM,			// 显示所有IB道具
	GE_UPDATE_IB_ITEM,				// 更新显示道具
};
