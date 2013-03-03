
/**	GMEventSystem.cpp
 *
 *	功能：	事件处理
 *	
 *	修改记录：
 *			080305	GE_GUILD_INVITE_OBJECT
 *			080306	GUILD_INTIVE_CONFIRM
 *					GUILD_NOTIFY_INTIVE
 *					GE_GUILD_MODIFY_TITLE
 *			080308	GUILD_CREATE_FAMILY
 *					GUILD_CREATE_FAMILY_CONFIRM		//创建家族确认
 *					GUILD_DESTORY_FAMILY_CONFIRM	//删除家族确认
 *			080409  GE_EQUIP_ALL_OPEN     //
 */

#include "StdAfx.h"
#include "GMEventSystem.h"
#include "GIException.h"
#include "../Procedure/GameProcedure.h"
#include "GITimeSystem.h"

//==============================================================
EVENT_DEFINE g_GlobalEvent[] = 
{
	{ GE_APPLICATION_INITED,		"APPLICATION_INITED",		},
	{ GE_ON_SCENE_TRANS,			"ON_SCENE_TRANS",			},
	{ GE_ON_SERVER_TRANS,			"ON_SERVER_TRANS",			},
	{ GE_ON_SCENE_UPDATED,			"ON_SCENE_UPDATED",			},
	{ GE_SCENE_TRANSED,				"SCENE_TRANSED",			},
	{ GE_PLAYER_ENTER_WORLD,		"PLAYER_ENTERING_WORLD",	},
	{ GE_PLAYER_LEAVE_WORLD,		"PLAYER_LEAVE_WORLD",		},
	{ GE_TEAM_CHANG_WORLD,			"TEAM_CHANGE_WORLD",		},		// 队友改变场景了
	{ GE_SKILL_CHANGED,				"SKILL_CHANGED",			},
	{ GE_MAINTARGET_CHANGED,		"MAINTARGET_CHANGED",		},
	{ GE_MAINTARGET_OPEND,			"MAINTARGET_OPEN",			},		// 打开主窗口
	{ GE_PACKAGE_ITEM_CHANGED,		"PACKAGE_ITEM_CHANGED",		},
	{ GE_TOGLE_SKILL_BOOK,			"TOGLE_SKILL_BOOK",			},
	{ GE_CLOSE_SKILL_BOOK,			"CLOSE_SKILL_BOOK",			},
	{ GE_SKILL_UPDATE,				"SKILL_UPDATE",				},
	{ GE_TOGLE_PET_PAGE,			"TOGLE_PET_PAGE",			},
	{ GE_TOGLE_PETATTRIBUTE_PAGE,			"TOGLE_PETATTRIBUTE_PAGE",			},
	{ GE_UPDATE_PETATTRIBUTE_PAGE,			"UPDATE_PETATTRIBUTE_PAGE",			},
	{ GE_PETPAGE_CLOSE,			"PETPAGE_CLOSE",			},
	{ GE_TOGLE_LIFE_PAGE,			"TOGLE_LIFE_PAGE",			},
	{ GE_TOGLE_COMMONSKILL_PAGE,	"TOGLE_COMMONSKILL_PAGE"	},
	{ GE_TOGLE_CONTAINER,			"TOGLE_CONTAINER",			},

	// 20100419 AddCodeBegin
	{ GE_SET_ROLE_OPENCLOSESIGN,	"SET_ROLE_OPENCLOSESIGN"	}, // 设置主菜单人物窗口打开关闭标记
	// 20100419 AddCodeEnd

//---------------------------------------------------------------------------
	// 骑宠相关
	{ GE_TOGLE_MOUNT_PAGE,			"TOGLE_MOUNT_PAGE"			},
	{ GE_CLOSE_MOUNT_PAGE,			"CLOSE_MOUNT_PAGE"			},
	{ GE_MOUNT_ATTR_UPDATE,			"MOUNT_ATTR_UPDATE"			},
	
	{ GE_TOGLE_TARGET_MOUNT_PAGE,   "TOGLE_TARGET_MOUNT_PAGE"   },
	{ GE_TARGET_MOUNT_ATTR_UPDATE,	"TARGET_MOUNT_ATTR_UPDATE"  },

	{ GE_TOGLE_MOUNT_STUDY_PAGE,	"TOGLE_MOUNT_STUDY_PAGE"	},
	{ GE_MOUNT_STUDY_ITEM_UPDATE,	"MOUNT_STUDY_ITEM_UPDATE"   },

	{ GE_TOGLE_MOUNT_FEED_PAGE,		"TOGLE_MOUNT_FEED_PAGE"		},
	{ GE_MOUNT_FEED_ITEM_UPDATE,	"MOUNT_FEED_ITEM_UPDATE"	},
	
	{ GE_TOGLE_MOUNT_TRAIN_PAGE,	"TOGLE_MOUNT_TRAIN_PAGE"	},
	{ GE_MOUNT_TRAIN_ITEM_UPDATE,   "MOUNT_TRAIN_ITEM_UPDATE"  	},

	{ GE_TOGLE_MOUNT_CHILD_PAGE,	"TOGLE_MOUNT_CHILD_PAGE"	},
	{ GE_MOUNT_CHILD_ITEM_UPDATE,	"MOUNT_CHILD_ITEM_UPDATE"	},

	{ GE_TOGLE_MOUNT_IDENTIFY_PAGE,	"TOGLE_MOUNT_IDENTIFY_PAGE"	},

	{ GE_TOGLE_MOUNT_MATING_PAGE,	"TOGLE_MOUNT_MATING_PAGE"	},
	{ GE_CLOSE_MOUNT_MATING_PAGE,	"CLOSE_MOUNT_MATING_PAGE"   },
	{ GE_MY_MOUNT_MATING_LOCKED,	"MY_MOUNT_MATING_LOCKED"	},
	{ GE_OTHER_MOUNT_MATING_LOCKED,	"OTHER_MOUNT_MATING_LOCKED"	},
	{ GE_REQUEST_TOGLE_MOUNT_MATING_PAGE, "REQUEST_TOGLE_MOUNT_MATING_PAGE" },
	{ GE_GAIN_MATING_MOUNT,			"GAIN_MATING_MOUNT"			},
	
//---------------------------------------------------------------------------	
	//------------------------------
	//角色基本属性
	{ GE_UNIT_HP,					"UNIT_HP",					  FALSE},
	{ GE_UNIT_MP,					"UNIT_MP",					  FALSE},
	{ GE_UNIT_PENUMA,				"UNIT_PENUMA",				  FALSE},
	{ GE_UNIT_RAGE,					"UNIT_RAGE",				  TRUE},
//	{ GE_UNIT_XINFA,				"UNIT_XINFA",				  TRUE},
	{ GE_UNIT_EXP,					"UNIT_EXP",					  FALSE},
	{ GE_UNIT_MONEY,				"UNIT_MONEY",				  TRUE},
	{ GE_UNIT_RACE_ID,				"UNIT_RACE_ID",				  TRUE},
	{ GE_UNIT_NAME,					"UNIT_NAME",				  TRUE},
	{ GE_UNIT_CAMP_ID,				"UNIT_CAMP_ID",				  TRUE},
	{ GE_UNIT_LEVEL,				"UNIT_LEVEL",				  FALSE},
	{ GE_UNIT_MOVE_SPEED,			"UNIT_MOVE_SPEED",			  TRUE},
	{ GE_UNIT_FIGHT_STATE,			"UNIT_FIGHT_STATE",			  TRUE},
	{ GE_UNIT_MAX_EXP,				"UNIT_MAX_EXP",				  FALSE},
	//------------------------------"
	//一级战斗属性					"
	{ GE_UNIT_STR,					"UNIT_STR",					  TRUE},
	//{ GE_UNIT_SPR,					"UNIT_SPR",					  TRUE},
	{ GE_UNIT_CON,					"UNIT_CON",					  TRUE},
	{ GE_UNIT_INT,					"UNIT_INT",					  TRUE},
	{ GE_UNIT_DEX,					"UNIT_DEX",					  TRUE},
	{ GE_UNIT_POINT_REMAIN,			"UNIT_POINT_REMAIN",		  TRUE},
	//------------------------------"
	//二级战斗属性					"
	{ GE_UNIT_ATT_NEAR,				"UNIT_ATT_NEAR",			  TRUE},
	{ GE_UNIT_DEF_NEAR,				"UNIT_DEF_NEAR",			  TRUE},
	{ GE_UNIT_ATT_FAR,				"UNIT_ATT_FAR",				  TRUE},
	{ GE_UNIT_DEF_FAR,				"UNIT_DEF_FAR",				  TRUE},

//	{ GE_UNIT_ATT_PHYSICS,			"UNIT_ATT_PHYSICS",			  TRUE},
	{ GE_UNIT_ATT_MAGIC_NEAR,			"UNIT_ATT_MAGIC_NEAR",			  TRUE},
	{ GE_UNIT_ATT_MAGIC_FAR,			"UNIT_ATT_MAGIC_FAR",			  TRUE},
//	{ GE_UNIT_DEF_PHYSICS,			"UNIT_DEF_PHYSICS",			  TRUE},
	{ GE_UNIT_DEF_MAGIC_NEAR,			"UNIT_DEF_MAGIC_NEAR",			  TRUE},
	{ GE_UNIT_DEF_MAGIC_FAR,			"UNIT_DEF_MAGIC_FAR",			  TRUE},
	
	{ GE_UNIT_MAX_HP,				"UNIT_MAX_HP",				  TRUE},
	{ GE_UNIT_MAX_MP,				"UNIT_MAX_MP",				  TRUE},
	{ GE_UNIT_MAX_PNEUMA,			"UNIT_MAX_PNEUMA",			  TRUE},
	{ GE_UNIT_MAX_RAGE,				"UNIT_MAX_RAGE",			  TRUE},
	
	{ GE_UNIT_HP_RE_SPEED,			"UNIT_HP_RE_SPEED",			  TRUE},
	{ GE_UNIT_MP_RE_SPEED,			"UNIT_MP_RE_SPEED",			  TRUE},
	{ GE_UNIT_HIT,					"UNIT_HIT",					  TRUE},
	{ GE_UNIT_MISS,					"UNIT_MISS",				  TRUE},
	{ GE_UNIT_CRIT_RATE,			"UNIT_CRIT_RATE",			  TRUE},
	{ GE_UNIT_TOUGHNESS,			"UNIT_TOUGHNESS",			  TRUE},
	{ GE_UNIT_ATT_SPEED,			"UNIT_ATT_SPEED",			  TRUE},
	{ GE_UNIT_ATT_GOLD,				"UNIT_ATT_GOLD",			  TRUE},
	{ GE_UNIT_DEF_GOLD,				"UNIT_DEF_GOLD",			  TRUE},
	{ GE_UNIT_ATT_WOOD,				"UNIT_ATT_WOOD",			  TRUE},
	{ GE_UNIT_DEF_WOOD,				"UNIT_DEF_WOOD",			  TRUE},
	{ GE_UNIT_ATT_WATER,			"UNIT_ATT_WATER",			  TRUE},
	{ GE_UNIT_DEF_WATER,			"UNIT_DEF_WATER",			  TRUE},
	{ GE_UNIT_ATT_FIRE,				"UNIT_ATT_FIRE",			  TRUE},
	{ GE_UNIT_DEF_FIRE,				"UNIT_DEF_FIRE",			  TRUE},
	{ GE_UNIT_ATT_SOIL,				"UNIT_ATT_SOIL",			  TRUE},
	{ GE_UNIT_DEF_SOIL,				"UNIT_DEF_SOIL",			  TRUE},
	{ GE_UNIT_MENPAI,				"UNIT_MENPAI",				  TRUE},
	{ GE_UNIT_HAIR_MESH,			"UNIT_HAIR_MESH",			  TRUE},
	{ GE_UNIT_HAIR_COLOR,			"UNIT_HAIR_COLOR",			  TRUE},
	{ GE_UNIT_FACE_IMAGE,			"UNIT_FACE_IMAGE",			  TRUE},	// 头像信息改变
	{ GE_UNIT_EQUIP_VER,			"UNIT_EQUIP_VER",			  TRUE},
	{ GE_UNIT_EQUIP,				"UNIT_EQUIP",				  TRUE},
	{ GE_UNIT_EQUIP_WEAPON,			"UNIT_EQUIP_WEAPON",		  TRUE},
	{ GE_UNIT_TITLE,				"UNIT_TITLE",				  TRUE},
	{ GE_UNIT_STRIKEPOINT,			"UNIT_STRIKEPOINT",			  TRUE},
	{ GE_UNIT_RELATIVE,				"UNIT_RELATIVE",			  TRUE},
	{ GE_UNIT_VIGOR,				"UNIT_VIGOR",				  TRUE},
	{ GE_UNIT_MAX_VIGOR,			"UNIT_MAX_VIGOR",			  TRUE},
	{ GE_UNIT_ENERGY,				"UNIT_ENERGY",				  TRUE},
	{ GE_UNIT_MAX_ENERGY,			"UNIT_MAX_ENERGY",			  TRUE},
//
//---------------------------------------------------------------------------
	{ GE_SHOW_CONTEXMENU,			"SHOW_CONTEXMENU",			},
	{ GE_TOGLE_COMPOSE_WINDOW,		"TOGLE_COMPOSE_WINDOW",		},
	{ GE_TOGLE_CONSOLE,				"TOGLE_CONSOLE",			},
	{ GE_ON_SKILL_ACTIVE,			"ON_SKILL_ACTIVE",			},
	{ GE_POSITION_MODIFY,			"POSITION_MODIFY",			},
	{ GE_CHAT_MESSAGE,				"CHAT_MESSAGE",				},
	{ GE_CHAT_CHANGE_PRIVATENAME,	"CHAT_CHANGE_PRIVATENAME",	},
	{ GE_CHAT_CHANGE_COPY, "CHAT_CHANGE_COPY", },
	{ GE_CHAT_CHANNEL_CHANGED,		"CHAT_CHANNEL_CHANGED",		},

	
	{ GE_MAINTARGET_BUFF_REFRESH, "MAINTARGET_BUFF_REFRESH" },// 20100423 AddCode 目标头像下方buff图标更新

	{ GE_MAINTARGET_BUFFTEXT_REFRESH, "MAINTARGET_BUFFTEXT_REFRESH" },// 目标头像下方buff的tooltip更新 20100528 BLL 

	{ GE_CHAT_TAB_CREATE,			"CHAT_TAB_CREATE",			},
	{ GE_CHAT_TAB_CONFIG,			"CHAT_TAB_CONFIG",			},
	{ GE_CHAT_TAB_CREATE_FINISH,	"CHAT_TAB_CREATE_FINISH",	},
	{ GE_CHAT_TAB_CONFIG_FINISH,	"CHAT_TAB_CONFIG_FINISH",	},
	{ GE_CHAT_FACEMOTION_SELECT,	"CHAT_FACEMOTION_SELECT",	},
	{ GE_CHAT_TEXTCOLOR_SELECT,		"CHAT_TEXTCOLOR_SELECT",	},
	{ GE_CHAT_CONTEX_MENU,			"CHAT_CONTEX_MENU",			},
	{ GE_CHAT_ACTSET,				"CHAT_ACTSET",				},
	{ GE_CHAT_ACT_SELECT,			"CHAT_ACT_SELECT",			},
	{ GE_CHAT_ACT_HIDE,				"CHAT_ACT_HIDE",			},
	{ GE_CHAT_PINGBI_LIST,			"CHAT_PINGBI_LIST",			},
	{ GE_CHAT_PINGBI_UPDATE,		"CHAT_PINGBI_UPDATE",		},
	{ GE_ACCELERATE_KEYSEND,		"ACCELERATE_KEYSEND",		},
	{ GE_CHAT_ADJUST_MOVE_CTL,		"CHAT_ADJUST_MOVE_CTL",		},
	{ GE_CHAT_INPUTLANGUAGE_CHANGE,	"CHAT_INPUTLANGUAGE_CHANGE",	TRUE},
	{ GE_CHAT_SHOWUSERINFO,			"CHAT_SHOWUSERINFO",		},
	{ GE_CHAT_LOAD_TAB_CONFIG,		"CHAT_LOAD_TAB_CONFIG",		},

	{ GE_LOOT_OPENED,				"LOOT_OPENED",				},
	{ GE_LOOT_SLOT_CLEARED,			"LOOT_SLOT_CLEARED",		},
	{ GE_LOOT_CLOSED,				"LOOT_CLOSED",				},
	{ GE_PROGRESSBAR_SHOW,			"PROGRESSBAR_SHOW",			},
	{ GE_PROGRESSBAR_HIDE,			"PROGRESSBAR_HIDE",			},
	{ GE_PROGRESSBAR_WIDTH,			"PROGRESSBAR_WIDTH",		},
	{ GE_CHANGE_BAR,				"CHANGE_BAR",				},
	{ GE_COLSE_SECOND_MENU,			"COLSE_SECOND_MENU",		},
	{ GE_TOGLE_MISSION,				"TOGLE_MISSION",			},
	{ GE_UPDATE_MISSION,			"UPDATE_MISSION",			},
	{ GE_REPLY_MISSION,				"REPLY_MISSION",			},
	{ GE_UPDATE_REPLY_MISSION,		"UPDATE_REPLY_MISSION",		},
	{ GE_TOGLE_LARGEMAP,			"TOGLE_LARGEMAP",			},
	{ GE_TOGLE_CLOSELARGEMAP,		"TOGLE_CLOSELARGEMAP",		},
	{ GE_TOGLE_SCENEMAP,			"TOGLE_SCENE_MAP",			},
	{ GE_TOGLE_CLOSESCENEMAP,		"TOGLE_CLOSESCENEMAP",		},
	{ GE_OPEN_MINIMAP,				"OPEN_MINIMAP",				},
	{ GE_OPEN_MINIMAPEXP,			"OPEN_MINIMAPEXP",			},

	{ GE_QUEST_EVENTLIST,			"QUEST_EVENTLIST",			},
	{ GE_QUEST_INFO,				"QUEST_INFO",				},
	{ GE_QUEST_REGIE,				"QUEST_REGIE",				},
	{ GE_QUEST_CONTINUE_DONE,		"QUEST_CONTINUE_DONE",		},
	{ GE_QUEST_CONTINUE_NOTDONE,	"QUEST_CONTINUE_NOTDONE",	},
	{ GE_QUEST_AFTER_CONTINUE,		"QUEST_AFTER_CONTINUE",		},
	{ GE_QUEST_TIPS,				"QUEST_TIPS",				},
	{ GE_TOGLE_COMPOSEITEM,			"TOGLE_COMPOSEITEM",		},
//	{ GE_TOGLE_COMPOSEGEM,			"TOGLE_COMPOSEGEM",			},
	{ GE_ON_LIFEABILITY_ACTIVE,		"ON_LIFEABILITY_ACTIVE",	},
	{ GE_NEW_DEBUGMESSAGE,			"NEW_DEBUGMESSAGE",			},
	{ GE_OPEN_BOOTH,				"OPEN_BOOTH",				},
	{ GE_CLOSE_BOOTH,				"CLOSE_BOOTH",				},
	{ GE_QUEST_INPUT_PASSWD_COMMAND,"QUEST_INPUT_PASSWD_COMMAND", },

	//------------------------------------------------------------------------------------------------------------
	// 一键换装
	{ GE_QUICK_CHANGE_EQUIP,		"QUICK_CHANGE_EQUIP",		},
	{ GE_UPDATE_QUICK_CHANGE,		"UPDATE_QUICK_CHANGE",		},
	{ GE_QUICK_CHANGE_SUCCESS,		"QUICK_CHANGE_SUCCESS",		},
	{ GE_UPDATE_QUICK_CHANGE_EQUIPNAME,		"UPDATE_QUICK_CHANGE_EQUIPNAME",		},

	//------------------------------------------------------------------------------------------------------------
	// 装备属性界面消息
	{ GE_UPDATE_EQUIP,				"REFRESH_EQUIP",				},	// 更新装备, 无参数.
	{ GE_MANUAL_ATTR_SUCCESS_EQUIP,	"MANUAL_ATTR_SUCCESS_EQUIP",	},	// 手动调节属性成功.
	{ GE_CUR_TITLE_CHANGEED,		"CUR_TITLE_CHANGED",			},	// 当前标题改变了.

	{ GE_TOGLE_TARGET_EQUIP_PAGE,		"TOGLE_TARGET_EQUIP_PAGE",	},
	{ GE_CLOSE_TARGET_EQUIP,			"CLOSE_TARGET_EQUIP",		},
	{ GE_TOGLE_TARGET_EQUIP_ALL,		"TOGLE_TARGET_EQUIP_ALL",	},
	{ GE_CLOSE_TARGET_EQUIP_ALL,		"CLOSE_TARGET_EQUIP_ALL",	},
	{ GE_CHANGE_TARGET_EQUIP_ALL_PAGE,  "CHANGE_TARGET_EQUIP_ALL_PAGE", },


	{ GE_UPDATE_BOOTH,				"UPDATE_BOOTH",				},
	{ GE_UPDATE_TEAM_MEMBER,		"ON_UPDATE_TEAM_MEMBER",	},

	{ GE_OPEN_CHARACTOR,			"OPEN_CHARACTOR",			},
	{ GE_OPEN_EQUIP,				"OPEN_EQUIP",				},
	{ GE_TOGLE_JOINSCHOOL,			"TOGLE_JOINSCHOOL"			},
	{ GE_UPDATE_CONTAINER,			"UPDATE_CONTAINER",			},

	{ GE_OPEN_SOUXIA,			"OPEN_SOUXIA",			},
	{ GE_CLOSE_SOUXIA,			"CLOSE_SOUXIA",			},
	{ GE_UPDATE_SOUXIA_TAB,		"UPDATE_SOUXIA_TAB"	},

	{ GE_OPEN_SOUXIASEW,		"OPEN_SOUXIASEW"	},
	{ GE_OPEN_SOUXIACOMPLEX,		"OPEN_SOUXIACOMPLEX"	},
	{ GE_OPEN_SOUXIADESTORY,		"OPEN_SOUXIADESTORY"	},
	{ GE_OPEN_SOUXIADISMANTLE,		"OPEN_SOUXIADISMANTLE"	},
	{ GE_ADD_SOUXIADISMANTLE_ITEM,		"ADD_SOUXIADISMANTLE_ITEM"	},
	{ GE_ADD_SOUXIADISMANTLE_DIS,		"ADD_SOUXIADISMANTLE_DIS"	},
	{ GE_SHOW_SOUXIADISMANTLE_BUILD, "SHOW_SOUXIADISMANTLE_BUILD" },
	{ GE_ADD_SOUXIADESTORY_ITEM,		"ADD_SOUXIADESTORY_ITEM"	},
	{ GE_ADD_SOUXIACOMPLEX_ITEM1,		"ADD_SOUXIACOMPLEX_ITEM1"	},
	{ GE_ADD_SOUXIACOMPLEX_ITEM2,		"ADD_SOUXIACOMPLEX_ITEM2"	},
	{ GE_ADD_SOUXIACOMPLEX_DIS,		"ADD_SOUXIACOMPLEX_DIS"	},
	{ GE_SHOW_SOUXIACOMPLEX_BUILD, "SHOW_SOUXIACOMPLEX_BUILD" },
	{ GE_ADD_SOUXIASEW_ITEM1,			"ADD_SOUXIASEW_ITEM1"	},
	{ GE_ADD_SOUXIASEW_DISITEM,			"ADD_SOUXIASEW_DISITEM"	},
	{ GE_ADD_SOUXIASEW_DIS,				"ADD_SOUXIASEW_DIS"	},
	{ GE_SHOW_SOUXIASEW_BUILD,		"SHOW_SOUXIASEW_BUILD" },

	// Nick 2007.11.21
	{ GE_OPEN_MAIN_FUNC_BTN,		"OPEN_MAIN_FUNC_BTN"		},		// 主功能按钮，用于打开8个主功能按钮

	{ GE_OPEN_ADD_HOLE,				"OPEN_ADD_HOLE",			},	
	{ GE_UPDATE_ADD_HOLE,			"UPDATE_ADD_HOLE",			},      //------

	{ GE_ADD_HOLE_SUCCESS,			"EQUIP_ADD_HOLE_SUCCESS",	},


	{ GE_OPEN_BANG_DING,			"OPEN_BANG_DING",		    },	
	{ GE_UPDATE_BANG_DING,			"UPDATE_BANG_DING",		    },      
	{ GE_ADD_BANG_DING_SUCCESS,		"EQUIP_BANG_DING_SUCCESS",	},

	{ GE_OPEN_DA_XING,				"OPEN_DA_XING",			    },
	{ GE_UPDATE_DA_XING,		    "UPDATE_DA_XING",		    },
	{ GE_OPEN_DA_XING_SUCCESS,		"EQUIP_DA_XING_SUCCESS"		},

	{ GE_OPEN_SP_REPAIR,			"OPEN_SP_REPAIR",			},
	{ GE_UPDATE_SP_REPAIR,		    "UPDATE_SP_REPAIR",		    }, 
	{ GE_OPEN_SP_REPAIR_SUCCESS,	"EQUIP_SP_REPAIR_SUCCESS"	},
	{ GE_OPEN_REPAIR_SURE,			"EQUIP_REPAIR_SURE_OPEN"	},

	{ GE_OPEN_SYNTHESIZE,			"EQUIP_OPEN_SYNTHESIZE"		},
	{ GE_CLOSE_SYNTHESIZE,			"EQUIP_CLOSE_SYNTHESIZE"	},

	{ GE_OPEN_EQUIP_REWORK,			"EQUIP_OPEN_REWORK"			},
	{ GE_CLOSE_EQUIP_REWORK,		"EQUIP_CLOSE_REWORK"	    },

	{ GE_OPEN_EQUIP_DISASS,			"EQUIP_OPEN_DISASS"			},
	{ GE_UPDATE_EQUIP_DISASS,		"EQUIP_UPDATE_DISASS"	    },		
	{ GE_CLOSE_EQUIP_DISASS,		"EQUIP_CLOSE_DISASS"	    },
 
	{ GE_IMPACT_SELF_UPDATE,		"IMPACT_SELF_UPDATE",		},
	{ GE_IMPACT_SELF_UPDATE_TIME,	"IMPACT_SELF_UPDATE_TIME",	},
	{ GE_OPEN_AGNAME,				"OPEN_AGNAME",				},
	{ GE_CLOSE_AGNAME,				"CLOSE_AGNAME",				},
	{ GE_TOGLE_BANK,				"TOGLE_BANK",				},
	{ GE_UPDATE_BANK,				"UPDATE_BANK",				},
	{ GE_TOGLE_INPUT_MONEY,			"TOGLE_INPUT_MONEY",		},
	{ GE_CLOSE_INPUT_MONEY,			"CLOSE_INPUT_MONEY",		},

	{ GE_OPEN_BANK_EVENT,			"OPEN_BANK_EVENT",			},

	{ GE_RECEIVE_EXCHANGE_APPLY,	"RECEIVE_EXCHANGE_APPLY",	},
	{ GE_STOP_EXCHANGE_TIP,			"STOP_EXCHANGE_TIP",		},
	{ GE_OPEN_EXCHANGE_FRAME,		"OPEN_EXCHANGE_FRAME",		},
	{ GE_UPDATE_EXCHANGE,			"UPDATE_EXCHANGE",			},

	{ GE_CANCEL_EXCHANGE,			"CANCEL_EXCHANGE",			},
	{ GE_SUCCEED_EXCHANGE_CLOSE,	"SUCCEED_EXCHANGE_CLOSE",	},
	{ GE_EXCHANGE_INPUT_MONEY,		"EXCHANGE_INPUT_MONEY",		},

	{ GE_PACKET_OPEN_SALE,			"PACKET_OPEN_SALE",			},		// 通过密码打开摆摊

	{ GE_UPDATE_PET_PAGE,			"UPDATE_PET_PAGE",			},
	{ GE_UPDATE_LIFESKILL_PAGE,		"UPDATE_LIFESKILL_PAGE",	},
	{ GE_OPEN_COMPOSE_ITEM,			"OPEN_COMPOSE_ITEM",		},
	{ GE_UPDATE_COMPOSE_ITEM,		"UPDATE_COMPOSE_ITEM",		},
	{ GE_OPEN_COMPOSE_GEM,			"OPEN_COMPOSE_GEM",			},
	{ GE_UPDATE_COMPOSE_GEM,		"UPDATE_COMPOSE_GEM",		},
	{ GE_COMPOSE_GEM_SUCCESS,		"COMPOSE_GEM_SUCCESS",		},
	{ GE_ENCHASE_CONFIRM_DLG,		"ENCHASE_CONFIRM_DLG",		},
	{ GE_AFFIRM_SHOW,				"AFFIRM_SHOW",				},

	// 摆摊相关
	{ GE_OPEN_STALL_SALE,			"OPEN_STALL_SALE",			},
	{ GE_OPEN_STALL_BUY,			"OPEN_STALL_BUY",			},
	{ GE_UPDATE_STALL_BUY,			"UPDATE_STALL_BUY",			},
	{ GE_UPDATE_STALL_SALE,			"UPDATE_STALL_SALE",		},
	{ GE_OPEN_STALL_RENT_FRAME,		"OPEN_STALL_RENT_FRAME",	},
	{ GE_STALL_SALE_SELECT,			"STALL_SALE_SELECT",		},
	{ GE_STALL_BUY_SELECT,			"STALL_BUY_SELECT",			},
	{ GE_OPEN_STALL_MESSAGE,		"OPEN_STALL_MESSAGE",		},
	{ GE_CLOSE_STALL_MESSAGE,		"CLOSE_STALL_MESSAGE",		},
	{ GE_STALL_SALE_DLG,			"STALL_SAVE_DLG",			},	// add by gh for close_stall 2010/04/15
	{ GE_CLOSE_STALL_SALE,			"CLOSE_STALL_SALE",			},	// add by gh for close_stall 2010/04/15

	{ GE_STALL_OPEN_PUBLIC,			"STALL_OPEN_PUBLIC",		},	// 通知摆摊界面设置玩家开市状态 20100706
	// 销毁物品
	{ GE_OPEN_DISCARD_ITEM_FRAME,	"OPEN_DISCARD_ITEM_FRAME",	},
	// 不能销毁物品
	{ GE_OPEN_CANNT_DISCARD_ITEM,	"OPEN_CANNT_DISCARD_ITEM",	},

	{ GE_SHOW_SPLIT_ITEM,			"SHOW_SPLIT_ITEM",			},
	{ GE_HIDE_SPLIT_ITEM,			"HIDE_SPLIT_ITEM",			},
	{ GE_TOGLE_FRIEND_INFO,			"TOGLE_FRIEND_INFO",		},
	{ GE_TOGLE_FRIEND,				"TOGLE_FRIEND",				},
	{ GE_OPEN_EMAIL,				"OPEN_EMAIL",				},
	{ GE_OPEN_EMAIL_WRITE,			"OPEN_EMAIL_WRITE",			},
	{ GE_HAVE_MAIL,					"HAVE_MAIL",				},
	{ GE_SEND_MAIL,					"SEND_MAIL",				},
	{ GE_UPDATE_EMAIL,				"UPDATE_EMAIL",				},
	{ GE_FRIEND_SET,				"FRIEND_SET",				},
	{ GE_MOOD_CHANGE,				"MOOD_CHANGE",				},
	{ GE_OPEN_HISTROY,				"OPEN_HISTROY",				},
	{ GE_ADD_TEMPFRIEND,			"ADD_TEMPFRIEND",			},

	//系统设置相关
	{ GE_TOGLE_SYSTEMFRAME,			"TOGLE_SYSTEMFRAME",		},
	{ GE_TOGLE_VIEWSETUP,			"TOGLE_VIEWSETUP",			},
	{ GE_TOGLE_SOUNDSETUP,			"TOGLE_SOUNDSETUP",			},
	{ GE_TOGLE_UISETUP,				"TOGLE_UISETUP",			},
	{ GE_TOGLE_INPUTSETUP,			"TOGLE_INPUTSETUP",			},
	{ GE_TOGLE_GAMESETUP,			"TOGLE_GAMESETUP",			},

	//玩家商店
	{ GE_PS_OPEN_OTHER_SHOP,		"PS_OPEN_OTHER_SHOP",		},
	{ GE_PS_OPEN_MY_SHOP,			"PS_OPEN_MY_SHOP",			},
	{ GE_PS_OPEN_CREATESHOP,		"PS_OPEN_CREATESHOP",		},
	{ GE_PS_CLOSE_CREATESHOP,		"PS_CLOSE_CREATESHOP",		},
	{ GE_PS_OPEN_SHOPLIST,			"PS_OPEN_SHOPLIST",			},
	{ GE_PS_SELF_ITEM_CHANGED,		"PS_SELF_ITEM_CHANGED",		},
	{ GE_PS_OTHER_SELECT,			"PS_OTHER_SELECT",			},
	{ GE_PS_SELF_SELECT,			"PS_SELF_SELECT",			},

	{ GE_PS_UPDATE_MY_SHOP,			"PS_UPDATE_MY_SHOP",		},
	{ GE_PS_UPDATE_OTHER_SHOP,		"PS_UPDATE_OTHER_SHOP",		},
									
	{ GE_PS_OPEN_OTHER_TRANS,		"PS_OPEN_OTHER_TRANS",		},
	{ GE_PS_UPDATE_OTHER_TRANS,		"PS_UPDATE_OTHER_TRANS",	},
	{ GE_PS_OTHER_TRANS_SELECT,		"PS_OTHER_TRANS_SELECT",	},

	{ GE_OPEN_PS_MESSAGE_FRAME,		"OPEN_PS_MESSAGE_FRAME",	},

	{ GE_PS_OPEN_MESSAGE,			"PS_OPEN_MESSAGE",			},
	{ GE_PS_UPDATE_MESSAGE,			"PS_UPDATE_MESSAGE",		},

	{ GE_OPEN_PET_LIST,				"OPEN_PET_LIST",			},
	{ GE_VIEW_EXCHANGE_PET,			"VIEW_EXCHANGE_PET",		},
	{ GE_CLOSE_PET_LIST,			"CLOSE_PET_LIST",			},
	{ GE_UPDATE_PET_LIST,			"UPDATE_PET_LIST",			},

	{ GE_OPEN_PRIVATE_INFO,			"OPEN_PRIVATE_INFO",		},

	{ GE_OPEN_CALLOF_PLAYER,		"OPEN_CALLOF_PLAYER",		},
	{ GE_NET_CLOSE,					"NET_CLOSE",				},

	{ GE_OPEN_ITEM_COFFER,			"OPEN_ITEM_COFFER",			},
	{ GE_PS_INPUT_MONEY,			"PS_INPUT_MONEY",			},

	//------------------------------------------------------------------------------------------------------------
	//
	// 组队相关事件
	
	{ GE_TEAM_OPEN_TEAMINFO_DLG,	"TEAM_OPEN_TEAMINFO_DLG",	},		// 打开队伍信息对话框.
																		// 
																		// arg0 - 正整数, 从0开始
																		// 0 : 打开队伍信息
																		// 1 : 有人申请加入队伍
																		// 2 : 有人邀请你加入队伍

	{ GE_TEAM_NOTIFY_APPLY,			"TEAM_NOTIFY_APPLY",		},		// mainmenubar, 闪烁按钮. 
																		// arg0 - 正整数, 从0开始
																		// 0 : 表示已经加入队伍, 不需要闪烁按钮, 打开界面为队伍信息
																		// 1 : 有人申请加入队伍
																		// 2 : 有人邀请你加入队伍
																		// 申请的人的具体信息从申请的信息队列中获得.

	{ GE_TEAM_APPLY,				"TEAM_APPLY",				},		// 有人申请你加入队伍.
																		// 
																		// arg0 - 字符串
																		// 申请的人.

	{ GE_TEAM_INVITE,				"TEAM_INVITE",				},		// 有人邀请你加入队伍.
																		// 
																		// arg0 - 字符串
																		// 邀请你的人.
	
	{ GE_TEAM_CLEAR_UI,				"TEAM_HIDE_ALL_PLAYER",		},		// 清空界面
										
	
	{ GE_TEAM_REFRESH_UI,			"TEAM_REFRESH_DATA",		},		// 刷新界面
	
	
	{ GE_TEAM_MEMBER_ENTER,			"TEAM_ENTER_MEMBER",		},		// 有新的队员进入
																		//
																		// arg0 - 正整数, 从1 开始
																		// 在ui界面中的显示位置
	
	{ GE_TEAM_UPTEDATA_MEMBER_INFO,	"TEAM_UPDATE_MEMBER",		},		// 更新队员信息
																		// arg0 - 正整数, 从1 开始
																		// 在ui界面中的显示位置
	
	{ GE_TEAM_MESSAGE,				"TEAM_TEAM_MESSAGE",		},		// 队伍信息提示信息
																		// arg0 - 字符串
																		// 需要提示的信息.
	
	{ GE_TEAM_MEMBER_INVITE,		"TEAM_MEMBER_INVITE",      },		// 队员邀请某一个人加入队伍请求队长同意
																		//
																		// arg0 - 队员名字
																		// arg1 - 被邀请人的名字

	{ GE_TEAM_FOLLOW_INVITE,		"TEAM_FOLLOW_INVITE",		},		// 队长邀请队员进入组队跟随模式
																		//
																		// arg0 - 队长名字

	{ GE_TEAM_REFRESH_MEMBER,		"TEAM_REFRESH_MEMBER",		},		// 刷新队员信息
																		//
																		// arg0 - 队员的位置索引

	{ GE_TEAM_CHANGE_MODE,			"TEAM_CHANGE_MODE",		},			// 刷新拾取模式
																		// arg0 - 更改的拾取模式id
    /*****************************************************************************************************************************
	**
	** 以下以后需要删除.
	**
	**
	******************************************************************************************************************************/
	//{ GE_ON_TEAM_ENTER_MEMBER,		"TEAM_ENTER_MEMBER",		},// 有新的队员进入
	{ GE_UPDATE_TEAM_MEMBER,		"ON_UPDATE_TEAM_MEMBER",	},// 更新队员信息
	{ GE_SHOW_TEAM_YES_NO,				"SHOW_TEAM_YES_NO",		},// 有人邀请你加入队伍.
	{ GE_SHOW_TEAM_MEMBER_INFO,			"SHOW_PLAYER_INFO",		},// 显示队员的详细信息
	{ GE_SHOW_TEAM_MEMBER_NAME,			"SHOW_PLAYER_NAME",		},// 在左边的队友列表框中显示队友的名字
	{ GE_HIDE_ALL_PLAYER,				"HIDE_ALL_PLAYER",		},// 自己离开队伍后, 清空左边的队伍列表.
	{ GE_SHOW_TEAM_MEMBER_NICK,			"SHOW_TEAM_NICK",		},// 昵称
	{ GE_SHOW_TEAM_MEMBER_FAMILY,		"SHOW_TEAM_FAMILY",		},// 门派
	{ GE_SHOW_TEAM_MEMBER_LEVEL,		"SHOW_TEAM_LEVEL",		},// 等级
	{ GE_SHOW_TEAM_MEMBER_POS,			"SHOW_TEAM_POS",		},// 位置
	{ GE_SHOW_TEAM_MEMBER_HP,			"SHOW_TEAM_HP",			},// hp
	{ GE_SHOW_TEAM_MEMBER_MP,			"SHOW_TEAM_MP",			},// mp
	{ GE_SHOW_TEAM_MEMBER_ANGER,		"SHOW_TEAM_ANGER",		},// 怒气
	{ GE_SHOW_TEAM_MEMBER_DEAD_LINK,	"SHOW_TEAM_DEAD_LINK",	},// 连线信息
	{ GE_SHOW_TEAM_MEMBER_DEAD,			"SHOW_TEAM_DEAD",		},// 死亡信息.
	{ GE_SHOW_TEAM_FUNC_MENU_MEMBER,	"SHOW_TEAM_CONTEXMENU_MEMBER", },// 显示队员的功能菜单
	{ GE_SHOW_TEAM_FUNC_MENU_LEADER,	"SHOW_TEAM_CONTEXMENU_LEADER", },// 显示队长的功能菜单

	// 组队相关事件
	//---------------------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------------------------------
	//
	// 登录流程相关
	//
	
	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO,		"GAMELOGIN_SHOW_SYSTEM_INFO",	},		// 显示系统信息
																					//
																					// arg0 - 字符串 : 需要提示的系统信息.


	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET,		"GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET",	},		// 显示系统信息
																									//
																									// arg0 - 字符串 : 需要提示的系统信息.
																									//
																									// 点击确认按钮后断开网络。


	{ GE_GAMELOGIN_CLOSE_SYSTEM_INFO,		"GAMELOGIN_CLOSE_SYSTEM_INFO",	},		// 显示队长的功能菜单

	
	{ GE_GAMELOGIN_OPEN_SELECT_SERVER,		"GAMELOGIN_OPEN_SELECT_SERVER",	},		// 关闭选择服务器界面
																					

	{ GE_GAMELOGIN_CLOSE_SELECT_SERVER,		"GAMELOGIN_CLOSE_SELECT_SERVER",},		// 关闭选择服务器界面


	{ GE_GAMELOGIN_OPEN_COUNT_INPUT,		"GAMELOGIN_OPEN_COUNT_INPUT",	},		// 打开帐号输入界面
																					

	{ GE_GAMELOGIN_CLOSE_COUNT_INPUT,		"GAMELOGIN_CLOSE_COUNT_INPUT",},		// 关闭帐号输入界面

	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON,		"GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON",	},	// 显示系统信息, 不显示按钮
																								//
																								// arg0 - 字符串 : 需要提示的系统信息.

	{ GE_GAMELOGIN_OPEN_SELECT_CHARACTOR,		"GAMELOGIN_OPEN_SELECT_CHARACTOR",	},			// 显示人物选择界面

	{ GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR,		"GAMELOGIN_CLOSE_SELECT_CHARACTOR",	},			// 关闭人物选择界面


	{ GE_GAMELOGIN_OPEN_CREATE_CHARACTOR,		"GAMELOGIN_OPEN_CREATE_CHARACTOR", },			// 显示人物创建界面

	{ GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR,		"GAMELOGIN_CLOSE_CREATE_CHARACTOR",},			// 关闭人物关闭界面

	{ GE_GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR,"GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR",},	// 刷新人物信息


	{ GE_GAMELOGIN_CLOSE_BACK_GROUND,			"GAMELOGIN_CLOSE_BACK_GROUND",},				// 关闭背景界面


	{ GE_GAMELOGIN_SYSTEM_INFO_YESNO,			"GAMELOGIN_SYSTEM_INFO_YESNO",},				// 系统信息提示 yes_no 界面.
																								//
																								// 参数0,	提示的字符串
																								// 参数1,	对话框的类型
																								//			0 -- 是否退出游戏
																								//			1 -- 是否删除角色
																								//			2 -- 是否更换帐号



	{ GE_GAMELOGIN_SELECT_LOGIN_SERVER,			"GAMELOGIN_SELECT_LOGIN_SERVER",},				// 选择一个login server
																								//
																								// 参数0, iAreaIndex   区域索引
																								// 参数1, iLoginServer 区域索引

	{ GE_GAMELOGIN_CLEAR_ACCOUNT,				"GAMELOGIN_CLEAR_ACCOUNT"},						// 清空帐号显示.

	{ GE_GAMELOGIN_SELECT_AREA,					"GAMELOGIN_SELECT_AREA"},						// 选择区域
												
	{ GE_GAMELOGIN_SELECT_LOGINSERVER,			"GAMELOGIN_SELECT_LOGINSERVER"},				// 选择Login Server

	{ GE_GAMELOGIN_CREATE_CLEAR_NAME,			"GAMELOGIN_CREATE_CLEAR_NAME"},				// 清空创建角色名字

	{ GE_GAMELOGIN_CREATE_ROLE_OK,				"GAMELOGIN_CREATE_ROLE_OK"},				// 创建角色成功

	
												

	
	//
	// 登录流程相关
	//
	//------------------------------------------------------------------------------------------------------------------------


	{ GE_TOGLE_SKILLSTUDY,				"TOGLE_SKILLSTUDY",		},		// 门派技能学习界面
	{ GE_SKILLSTUDY_SUCCEED,			"SKILLSTUDY_SUCCEED",	},		// 门派技能学习界面
	{ GE_TOGLE_ABILITY_STUDY,			"TOGLE_ABILITY_STUDY"	},		// 生活技能学习界面


	{ GE_SUPERTOOLTIP,					"SHOW_SUPERTOOLTIP" }, //显示超级tooltip
	{ GE_UPDATE_SUPERTOOLTIP,			"UPDATE_SUPERTOOLTIP" }, //刷新超级tooltip
	{ GE_SHOW_SUPERTOOLTIP_TITLE,		"SHOW_SUPERTOOLTIP_TITLE" }, //刷新超级tooltip	//temp


	//
	// 复活界面相关
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ GE_RELIVE_SHOW,					"RELIVE_SHOW",			},		// 显示复活界面
	{ GE_RELIVE_HIDE,					"RELIVE_HIDE",			},		// 隐藏复活界面
	{ GE_RELIVE_REFESH_TIME,			"RELIVE_REFESH_TIME",	},		// 刷新灵魂出窍的时间

	{ GE_OBJECT_CARED_EVENT,			"OBJECT_CARED_EVENT",	},

	{ GE_UPDATE_MAP,					"UPDATE_MAP",	},
	{ GE_UPDATE_MAP_LIST,				"UPDATE_MAP_LIST",	},
	{ GE_UPDATE_LARGEMAP_LIST,			"UPDATE_LARGEMAP_LIST", },
	{ GE_UPDATE_SCENEMAP_LIST,			"UPDATE_SCENEMAP_LIST", },

	{ GE_UI_COMMAND,					"UI_COMMAND",	},
	{ GE_OTHERPLAYER_UPDATE_EQUIP,		"OTHERPLAYER_UPDATE_EQUIP", },
	{ GE_VARIABLE_CHANGED,				"VARIABLE_CHANGED" , TRUE },	//全局变量发生改变
	{ GE_TIME_UPDATE,					"TIME_UPDATE", },

	{ GE_UPDATE_FRIEND,					"UPDATE_FRIEND",	},			// 好友数据更新了
	{ GE_UPDATE_FRIEND_INFO,			"UPDATE_FRIEND_INFO", },
	
	{ GE_OPEN_FRIEND_SEARCH,			"OPEN_FRIEND_SEARCH", },		// 好友搜索
	{ GE_OPEN_FRIEND_SEARCH_REASON,		"OPEN_FRIEND_SEARCH_REASON", },	// 好友搜索结果
	{ GE_VALIDATE_ADD_FRIEND,			"VALIDATE_ADD_FRIEND", },		//有人请求加为好友
	{ GE_FRIEND_DELETE_FRIEND,			"FRIEND_DELETE_FRIEND",},		//确认删除好友

	
	{ GE_UPDATE_TARGETPET_PAGE,			"UPDATE_TARGETPET_PAGE", },
	{ GE_UPDATE_PETSKILLSTUDY,			"UPDATE_PETSKILLSTUDY", },
	{ GE_UPDATE_PETINVITEFRIEND,		"UPDATE_PETINVITEFRIEND", },
	{ GE_REPLY_MISSION_PET,				"REPLY_MISSION_PET", },
	
	//------------------------------------------------------------------------------------------------------------------------
	//
	// 是否设置二级保护密码
	//

	{ GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG,		"MINORPASSWORD_OPEN_SET_PASSWORD_DLG", },		// 打开设置二级密码界面
	{ GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG,	"MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG", },	// 打开unlock密码界面。
	{ GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG,	"MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG", },	// 打开更改密码界面。

	{ GE_OPEN_SYSTEM_TIP_INFO_DLG,	"OPEN_SYSTEM_TIP_INFO_DLG", },									// 提示系统信息对话框。
	{ GE_INPUT_PASSWORD_DLG,		"INPUT_PASSWORD_DLG", },										// 输入二级密码界面。

	// 帮会相关
	{ GE_GUILD_CREATE,					"GUILD_CREATE", },				// 帮会创建事件
	{ GE_GUILD_CREATE_CONFIRM,			"GUILD_CREATE_CONFIRM", },		// 帮会创建确认事件
	{ GE_GUILD_SHOW_LIST,				"GUILD_SHOW_LIST", },			// 显示帮会列表事件
	{ GE_GUILD_SHOW_MEMBERINFO,			"GUILD_SHOW_MEMBERINFO", },		// 显示自己帮会成员管理界面
	{ GE_GUILD_SHOW_DETAILINFO,			"GUILD_SHOW_DETAILINFO", },		// 显示自己帮会详细信息界面
	{ GE_GUILD_SHOW_APPOINTPOS,			"GUILD_SHOW_APPOINTPOS", },		// 显示自己帮会可用职位界面
	{ GE_GUILD_DESTORY_CONFIRM,			"GUILD_DESTORY_CONFIRM", },		// 删除帮会确认事件
	{ GE_GUILD_QUIT_CONFIRM,			"GUILD_QUIT_CONFIRM", },		// 退出帮会确认事件
	{ GE_GUILD_UPDATE_MEMBERINFO,		"GUILD_UPDATE_MEMBERINFO", },	// 更新帮会成员的信息
	{ GE_GUILD_FORCE_CLOSE,				"GUILD_FORCE_CLOSE", },			// 强制帮会相关界面关闭事件
	{ GE_GUILD_INVITE_OBJECT,			"GUILD_INVITE_OBJECT", },		// 显示邀请对话框
	{ GE_GUILD_NOTIFY_INTIVE,			"GUILD_NOTIFY_INTIVE", },		// 闪烁帮会按钮， 提示收到邀请
	{ GE_GUILD_INTIVE_CONFIRM,			"GUILD_INTIVE_CONFIRM", },		// 邀请确认
	{ GE_GUILD_MODIFY_TITLE,			"GUILD_MODIFY_TITLE", },		// 修改成员称号
	{ GE_GUILD_CREATE_FAMILY,			"GUILD_CREATE_FAMILY", },			//创建家族确认
	{ GE_GUILD_CREATE_FAMILY_CONFIRM,	"GUILD_CREATE_FAMILY_CONFIRM", },	//创建家族确认
	{ GE_GUILD_DESTORY_FAMILY_CONFIRM,	"GUILD_DESTORY_FAMILY_CONFIRM", },	//删除家族确认
	{ GE_INPUT_PASSWD_COMMAND,			"INPUT_PASSWD_COMMAND", },	// 通过输入密码触发

	// 任务相关
	{ GE_CLOSE_MISSION_REPLY,			"CLOSE_MISSION_REPLY",		},	
	{ GE_VIEW_RESOLUTION_CHANGED,		"VIEW_RESOLUTION_CHANGED",	},
	{ GE_CLOSE_SYNTHESIZE_ENCHASE,		"CLOSE_SYNTHESIZE_ENCHASE", },
	{ GE_TOGGLE_PETLIST,				"TOGGLE_PETLIST",			},
	{ GE_PET_FREE_CONFIRM,				"PET_FREE_CONFIRM",			},

   
	//------------------------------------------------------------------------------------------------------------------------
	{ GE_OPEN_BUY_CONFIRM,				"OPEN_BUY_CONFIRM", },
	{ GE_OPEN_SALE_CONFIRM,				"OPEN_SALE_CONFIRM", },


	// 
	{ GE_FRIEND_CREATE_GROUP,			"FRIEND_ADD_GROUP", },

	// 打开添加好友输入框
	{GE_OPEN_ADD_NEW_FRIEND,			"OPEN_ADD_NEW_FRIEND", },

	// 添加好友新组
	{ GE_ADD_GROUP,						"ADD_GROUP", },
	// 显示详细信息
	{ GE_SHOW_FRIEND_INFO,				"SHOW_FRIEND_INFO", },

	{ GE_QUICK_CHANGE_NAME,				"QUICK_CHANGE_NAME", },
	
	// 物品链接
	{ GE_ITEM_LINK,						"ITEM_LINK",		},

	
	 //打开装备牌型交换界面
	{ GE_EQUIP_SWAPCARD_OPEN,			"EQUIP_SWAPCARD_OPEN",},

	//更新装备牌型交换界面
	{ GE_EQUIP_SWAPCARD_UPDATA,			"EQUIP_SWAPCARD_UPDATA",},
	//装备牌型交换成功
	{ GE_EQUIP_SWAPCARD_SUCESS,			"EQUIP_SWAPCARD_SUCESS",},

	//打开装备牌型改造界面
	{ GE_EQUIP_CHANGECARD_OPEN,			"EQUIP_CHANGECARD_OPEN",},


	//更新装备牌型改造界面
	{ GE_EQUIP_CHANGECARD_UPDATA,		"EQUIP_CHANGECARD_UPDATA",},
	//装备牌型改造成功
	{ GE_EQUIP_CHANGECARD_SUCESS,		"EQUIP_CHANGECARD_SUCESS",},


	// 称号表
	{ GE_SHOW_TITLE,					"SHOW_TITLE", },
	{ GE_UPDATE_TITLE,					"UPDATE_TITLE", },

	// 系统和人物的信息
	{ GE_PERSONAL_MESSAGE,				"PERSONAL_MESSAGE", },
	{ GE_SYSTEM_MESSAGE,				"SYSTEM_MESSAGE", },

	// 系统信息提示图标
	{ GE_SHOW_SYSTEM_PROMPT,			"SHOW_SYSTEM_PROMPT", },

	{ GE_UNITE_GF_SHOW,					"UNITE_GF_SHOW", },

	{ GE_RICH_SAFE_GUARD,				"RICH_SAFE_GUARD",			 },	// 保护范围界面
	{ GE_RICH_SAFE_TIME,				"RICH_SAFE_TIME",			 },	// 安全时限弹出界面
	{ GE_RICH_SET_PASSWORD,				"RICH_SET_PASSWORD",		 },	// 设置密码、修改密码界面
	{ GE_RICH_MODIFY_PASSWORD,			"RICH_MODIFY_PASSWORD",		 },
	{ GE_RICH_SETUP,					"RICH_SETUP",				 },	// 财产保护功能设置界面
	{ GE_OPEN_RICHES_PROTECT_FRAME,		"OPEN_RICHES_PROTECT_FRAME", },	// 财产保护提示界面
	{ GE_RICH_TEMP_CANCEL_PASSWORD,		"RICH_TEMP_CANCEL_PASSWORD", },	// 临时取消密码
	{ GE_RICH_CANCEL_PASSWORD,			"RICH_CANCEL_PASSWORD",		 },	// 取消密码
	{ GE_RICH_CHECK_PASSWORD,			"RICH_CHECK_PASSWORD",		 },	// 检测输入密码

	//temp
	{ GE_TEMP_CLOSE_FRIEND,				"TEMP_CLOSE_FRIEND", },		// 
	{ GE_TEMP_CLOSE_GUILD,				"TEMP_CLOSE_GUILD", },		//


	//打开装备 个人资料  宠物等6个界面
	{ GE_EQUIP_ALL_OPEN,		"EquipAll_OPEN",},
	//打开装备 个人资料  宠物等6个界面
	{ GE_EQUIP_ALL_CLOSE,		"EquipAll_ClOSE",},

	// 人物详细属性界面
	{ GE_TOGLE_PLAYER_ATTR_PAGE, "TOGLE_PLAYER_ATTR_PAGE", },
	{ GE_CLOSE_PLAYER_ATTR_PAGE, "CLOSE_PLAYER_ATTR_PAGE", },
	
	// 打开每日提醒功能界面
	{ GE_OPEN_DAY_TIPS,					"OPEN_DAY_TIPS",	},

	// 更新每日提醒日期
	{ GE_UPDATE_DATE_TIME,				"UPDATE_DATE_TIME",	},
	// 更新每日提醒内容
	{ GE_UPDATE_DAY_TIPS,				"UPDATE_DAY_TIPS",	},
	// 更新月历内容
	{ GE_UPDATE_CALENDAR,				"UPDATE_CALENDAR",	},
	// 打开排行榜界面
	{ GE_OPEN_TOP_LIST,					"OPEN_TOP_LIST",	},
	// 更新排行榜界面
	{ GE_UPDATE_TOP_LIST,				"UPDATE_TOP_LIST",	},
	// 打开任务跟踪界面
	{ GE_OPEN_MISSION_SCOUT,			"OPEN_MISSION_SCOUT",},
	// 更新任务跟踪界面
	{ GE_UPDATE_MISSION_SCOUT,			"UPDATE_MISSION_SCOUT",},

	{ GE_UPDATA_EQUIP_ALL_POS,			"UPDATA_EQUIP_ALL_POS",},

	{ GE_COUNTRY_MANAGER_CREATE_KING,	"COUNTRY_MANAGER_CREATE_KING", },	// 产生国王
	{ GE_COUNTRY_KING_CREATE_CONFIRM,	"COUNTRY_KING_CREATE_CONFIRM", },	// 确认产生国王
	{ GE_COUNTRY_MANAGER_REQUEST_INFO,	"COUNTRY_MANAGER_REQUEST_INFO", },	// 请求国家信息
	{ GE_COUNTRY_MANAGER_INFO,			"COUNTRY_MANAGER_INFO",},			// 国家信息
	{ GE_COUNTRY_MANAGER_ADJUST_AUTH,	"COUNTRY_MANAGER_ADJUST_AUTH",},	// 国家官职调整
	{ GE_INPUT_COUNTRY_DLG,				"INPUT_COUNTRY_DLG", },				// 输入国家官职

	{ GE_SUPERTOOLTIP2,				"SHOW_SUPERTOOLTIP2", },

	{ GE_END_PACKUP_PACKET, "END_PACKUP_PACKET"},
	{ GE_BEGIN_PACKUP_PACKET, "BEGIN_PACKUP_PACKET"},

	{ GE_OPEN_WINDOW,				"OPEN_WINDOW", },//打开一个窗口

	{ GE_CLOSE_WINDOW, "CLOSE_WINDOW"},//关闭一个窗口
	{ GE_TOGGLE_WINDOW, "TOGGLE_WINDOW"},//切换一个窗口

	{ GE_SOUXIA_BANGDING_ENCHASE, "SOUXIA_BANGDING_ENCHASE" },	//确认是否绑定搜侠录	
	{ GE_SOUXIA_STUDY, "SOUXIA_STUDY" },		//绑定搜侠录
	{ GE_UPDATE_SOUXIA_STUDY, "UPDATE_SOUXIA_STUDY" },		//更新搜侠录已学技能列表框
	{ GE_CLEAR_SOUXIA_BAR, "CLEAR_SOUXIA_BAR" },		//清除掉技能快捷栏中的搜侠技能
	{ GE_UPDATE_SOUXIA_SKILLNUM, "UPDATE_SOUXIA_SKILLNUM" },		//更新技能快捷栏中的搜侠技能的使用次数
	{ GE_UPDATE_SOUXIA_INDEXSKILLNUM, "UPDATE_SOUXIA_INDEXSKILLNUM" },		//更新指定位置上的搜侠技能信息

	{ GE_SHOW_EQUIPLIANHUA, "SHOW_EQUIPLIANHUA" },
	{ GE_CLOSE_EQUIPLIANHUA, "CLOSE_EQUIPLIANHUA" },
	{ GE_ADD_LIANHUAEQUIP_ITEM, "ADD_LIANHUAEQUIP_ITEM" },
	{ GE_ADD_LIANHUAEQUIP_DIS, "ADD_LIANHUAEQUIP_DIS" },

	{ GE_OPEN_EQUIP_FUMO, "OPEN_EQUIP_FUMO" },
	{ GE_ADD_EQUIP_FUMO_ITEM, "ADD_EQUIP_FUMO_ITEM" },
	{ GE_ADD_EQUIP_FUMO_DIS, "ADD_EQUIP_FUMO_DIS" },
	{ GE_CLOSE_EQUIP_FUMO, "CLOSE_EQUIP_FUMO" },

	{ GE_OPEN_EQUIP_TUIMO, "OPEN_EQUIP_TUIMO" },
	{ GE_ADD_EQUIP_TUIMO_ITEM, "ADD_EQUIP_TUIMO_ITEM" },
	{ GE_ADD_EQUIP_TUIMO_DIS, "ADD_EQUIP_TUIMO_DIS" },
	{ GE_CLOSE_EQUIP_TUIMO, "CLOSE_EQUIP_TUIMO" },


	{ GE_SET_SCROLL_INFO,				"SET_SCROLLINFO"},	// 公告板滚动消息 20100624 BLL
	//RMB商城相关
	{GE_OPEN_IB_SHOP,					"OPEN_IB_SHOP"},// 打开RMB商城主界面
	{GE_SHOW_ALL_IB_ITEM,				"SHOW_ALL_IB_ITEM"},// 显示所有RMB道具
	{GE_UPDATE_IB_ITEM,					"UPDATE_IB_ITEM"},	// 更新RMB道具
	
};

//==============================================================
KL_IMPLEMENT_DYNAMIC(CEventSystem, GETCLASS(tEventSystem));
CEventSystem* CEventSystem::s_pMe = NULL;
CEventSystem::CEventSystem()
{
	s_pMe = this;
	m_dwLastTickCount = 0;
}

CEventSystem::~CEventSystem()
{
}

VOID CEventSystem::Initial(VOID*)
{
	INT nEventNum = sizeof(g_GlobalEvent) / sizeof(EVENT_DEFINE);

	for(INT i=0; i<nEventNum; i++)
	{
		// 生成名称检索表
		m_mapEventIndex_AsName[g_GlobalEvent[i].szEvent] = &(g_GlobalEvent[i]);
		// 生成id检索表
		m_mapEventIndex_AsID[g_GlobalEvent[i].idEvent] = &(g_GlobalEvent[i]);
	}
}

VOID CEventSystem::Release(VOID)
{
}

VOID CEventSystem::Tick(VOID)
{
}

void CEventSystem::_PushEvent(const EVENT& event)
{
	if(!event.pEventDef)
		return;

	// 如果是慢速处理的事件
	if(event.pEventDef->delayProcess)
	{
		m_delayQueueEvent.push_back(event);
	}
	else
	{
		m_queueEvent.push_back(event);
	}
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, const std::vector< STRING >& vParam)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id];

	event.vArg = vParam;

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, FLOAT fArg0)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%f", fArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, INT iArg0)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) 
		return; 

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id]; 

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 )
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", nArg2);
	event.vArg.push_back( szTemp );

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	CHAR szTemp[32];

	_snprintf(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg1);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1, INT iArg2)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	CHAR szTemp[32];

	_snprintf(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg1);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg2);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", iArg2);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg3);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, const std::vector< STRING >& vParam)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsName[eventName];

	event.vArg = vParam;

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsName[eventName];

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, FLOAT fArg0)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsName[eventName]; 

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%f", fArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, LPCTSTR szArg0, LPCTSTR szArg1)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsName[eventName]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, INT iArg0)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) 
		return; 

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsName[eventName]; 

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, LPCTSTR szArg0)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsName[eventName]; 

	event.vArg.push_back(szArg0);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 )
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsName[eventName]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", nArg2);
	event.vArg.push_back( szTemp );

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, INT iArg0, INT iArg1)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsName[eventName]; 

	CHAR szTemp[32];

	_snprintf(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg1);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(STRING& eventName, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3)
{
	if(m_mapEventIndex_AsName.find(eventName) == m_mapEventIndex_AsName.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsName[eventName]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", iArg2);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg3);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

// 注册事件处理函数
VOID CEventSystem::RegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT uOwnerData)
{
	if(!funHandle) 
		return;

	EVENT_DEFINE* pEvent = m_mapEventIndex_AsName[nameEvent];
	if(!pEvent) 
		return;

	pEvent->listFuncNotify.push_back( std::make_pair(funHandle, uOwnerData) );
}

VOID CEventSystem::ProcessAllEvent(VOID)
{
	// 处理慢速队列
	if(!(m_delayQueueEvent.empty()))
	{
		const UINT WORK_STEP = 2;
		UINT nTickCountNow = CGameProcedure::s_pTimeSystem->GetTickCount();
		UINT nTickCountStep = CGameProcedure::s_pTimeSystem->CalSubTime(m_dwLastTickCount, nTickCountNow);
		if(nTickCountStep >= WORK_STEP) 
		{
			m_dwLastTickCount = nTickCountNow;

			const EVENT& event = *(m_delayQueueEvent.begin());

			_ProcessEvent(event);

			m_delayQueueEvent.erase(m_delayQueueEvent.begin());
		}
	}

	register std::list< EVENT >::iterator it;
	for(it=m_queueEvent.begin(); it!=m_queueEvent.end(); it++)
	{
		const EVENT& event = *it;

		// 检测是否有同样的Event已经被处理
		bool bMultiPushed = false;
		for(register std::list< EVENT >::iterator itPrev=m_queueEvent.begin(); itPrev !=it; itPrev++)
		{
			if(*itPrev == *it) 
			{
				bMultiPushed = true;
				break;
			}
		}

		if(bMultiPushed) 
			continue;

		_ProcessEvent(event);
	}

	m_queueEvent.clear();
}

void CEventSystem::_ProcessEvent(const EVENT& event)
{
	// 查找事件定义
	EVENT_DEFINE* pEventDef = event.pEventDef;
	if(!pEventDef) 
		return;

	// 调用处理函数
	if(!(pEventDef->listFuncNotify.empty()))
	{
		EVENT_DEFINE::REGISTER_STRUCT::iterator it;
		for(it=pEventDef->listFuncNotify.begin(); it!=pEventDef->listFuncNotify.end(); it++)
		{
			if((*it).first)
				((*it).first)(&event, (*it).second);
		}
	}
}

//去掉事件处理函数
VOID CEventSystem::UNRegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT uOwnerData)
{
	if(!funHandle) return;

	EVENT_DEFINE* pEvent = m_mapEventIndex_AsName[nameEvent];
	if(!pEvent) return;

	pEvent->listFuncNotify.remove( std::make_pair(funHandle, uOwnerData) );

}

// 20100517 AddCodeBegin
VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, LPCTSTR szArg2 )
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);
	event.vArg.push_back(szArg2);

	_PushEvent(event);
}
// 20100517 AddCodeEnd
