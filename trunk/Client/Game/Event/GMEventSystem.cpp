
/**	GMEventSystem.cpp
 *
 *	���ܣ�	�¼�����
 *	
 *	�޸ļ�¼��
 *			080305	GE_GUILD_INVITE_OBJECT
 *			080306	GUILD_INTIVE_CONFIRM
 *					GUILD_NOTIFY_INTIVE
 *					GE_GUILD_MODIFY_TITLE
 *			080308	GUILD_CREATE_FAMILY
 *					GUILD_CREATE_FAMILY_CONFIRM		//��������ȷ��
 *					GUILD_DESTORY_FAMILY_CONFIRM	//ɾ������ȷ��
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
	{ GE_TEAM_CHANG_WORLD,			"TEAM_CHANGE_WORLD",		},		// ���Ѹı䳡����
	{ GE_SKILL_CHANGED,				"SKILL_CHANGED",			},
	{ GE_MAINTARGET_CHANGED,		"MAINTARGET_CHANGED",		},
	{ GE_MAINTARGET_OPEND,			"MAINTARGET_OPEN",			},		// ��������
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
	{ GE_SET_ROLE_OPENCLOSESIGN,	"SET_ROLE_OPENCLOSESIGN"	}, // �������˵����ﴰ�ڴ򿪹رձ��
	// 20100419 AddCodeEnd

//---------------------------------------------------------------------------
	// ������
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
	//��ɫ��������
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
	//һ��ս������					"
	{ GE_UNIT_STR,					"UNIT_STR",					  TRUE},
	//{ GE_UNIT_SPR,					"UNIT_SPR",					  TRUE},
	{ GE_UNIT_CON,					"UNIT_CON",					  TRUE},
	{ GE_UNIT_INT,					"UNIT_INT",					  TRUE},
	{ GE_UNIT_DEX,					"UNIT_DEX",					  TRUE},
	{ GE_UNIT_POINT_REMAIN,			"UNIT_POINT_REMAIN",		  TRUE},
	//------------------------------"
	//����ս������					"
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
	{ GE_UNIT_FACE_IMAGE,			"UNIT_FACE_IMAGE",			  TRUE},	// ͷ����Ϣ�ı�
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

	
	{ GE_MAINTARGET_BUFF_REFRESH, "MAINTARGET_BUFF_REFRESH" },// 20100423 AddCode Ŀ��ͷ���·�buffͼ�����

	{ GE_MAINTARGET_BUFFTEXT_REFRESH, "MAINTARGET_BUFFTEXT_REFRESH" },// Ŀ��ͷ���·�buff��tooltip���� 20100528 BLL 

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
	// һ����װ
	{ GE_QUICK_CHANGE_EQUIP,		"QUICK_CHANGE_EQUIP",		},
	{ GE_UPDATE_QUICK_CHANGE,		"UPDATE_QUICK_CHANGE",		},
	{ GE_QUICK_CHANGE_SUCCESS,		"QUICK_CHANGE_SUCCESS",		},
	{ GE_UPDATE_QUICK_CHANGE_EQUIPNAME,		"UPDATE_QUICK_CHANGE_EQUIPNAME",		},

	//------------------------------------------------------------------------------------------------------------
	// װ�����Խ�����Ϣ
	{ GE_UPDATE_EQUIP,				"REFRESH_EQUIP",				},	// ����װ��, �޲���.
	{ GE_MANUAL_ATTR_SUCCESS_EQUIP,	"MANUAL_ATTR_SUCCESS_EQUIP",	},	// �ֶ��������Գɹ�.
	{ GE_CUR_TITLE_CHANGEED,		"CUR_TITLE_CHANGED",			},	// ��ǰ����ı���.

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
	{ GE_OPEN_MAIN_FUNC_BTN,		"OPEN_MAIN_FUNC_BTN"		},		// �����ܰ�ť�����ڴ�8�������ܰ�ť

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

	{ GE_PACKET_OPEN_SALE,			"PACKET_OPEN_SALE",			},		// ͨ������򿪰�̯

	{ GE_UPDATE_PET_PAGE,			"UPDATE_PET_PAGE",			},
	{ GE_UPDATE_LIFESKILL_PAGE,		"UPDATE_LIFESKILL_PAGE",	},
	{ GE_OPEN_COMPOSE_ITEM,			"OPEN_COMPOSE_ITEM",		},
	{ GE_UPDATE_COMPOSE_ITEM,		"UPDATE_COMPOSE_ITEM",		},
	{ GE_OPEN_COMPOSE_GEM,			"OPEN_COMPOSE_GEM",			},
	{ GE_UPDATE_COMPOSE_GEM,		"UPDATE_COMPOSE_GEM",		},
	{ GE_COMPOSE_GEM_SUCCESS,		"COMPOSE_GEM_SUCCESS",		},
	{ GE_ENCHASE_CONFIRM_DLG,		"ENCHASE_CONFIRM_DLG",		},
	{ GE_AFFIRM_SHOW,				"AFFIRM_SHOW",				},

	// ��̯���
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

	{ GE_STALL_OPEN_PUBLIC,			"STALL_OPEN_PUBLIC",		},	// ֪ͨ��̯����������ҿ���״̬ 20100706
	// ������Ʒ
	{ GE_OPEN_DISCARD_ITEM_FRAME,	"OPEN_DISCARD_ITEM_FRAME",	},
	// ����������Ʒ
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

	//ϵͳ�������
	{ GE_TOGLE_SYSTEMFRAME,			"TOGLE_SYSTEMFRAME",		},
	{ GE_TOGLE_VIEWSETUP,			"TOGLE_VIEWSETUP",			},
	{ GE_TOGLE_SOUNDSETUP,			"TOGLE_SOUNDSETUP",			},
	{ GE_TOGLE_UISETUP,				"TOGLE_UISETUP",			},
	{ GE_TOGLE_INPUTSETUP,			"TOGLE_INPUTSETUP",			},
	{ GE_TOGLE_GAMESETUP,			"TOGLE_GAMESETUP",			},

	//����̵�
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
	// �������¼�
	
	{ GE_TEAM_OPEN_TEAMINFO_DLG,	"TEAM_OPEN_TEAMINFO_DLG",	},		// �򿪶�����Ϣ�Ի���.
																		// 
																		// arg0 - ������, ��0��ʼ
																		// 0 : �򿪶�����Ϣ
																		// 1 : ��������������
																		// 2 : ����������������

	{ GE_TEAM_NOTIFY_APPLY,			"TEAM_NOTIFY_APPLY",		},		// mainmenubar, ��˸��ť. 
																		// arg0 - ������, ��0��ʼ
																		// 0 : ��ʾ�Ѿ��������, ����Ҫ��˸��ť, �򿪽���Ϊ������Ϣ
																		// 1 : ��������������
																		// 2 : ����������������
																		// ������˵ľ�����Ϣ���������Ϣ�����л��.

	{ GE_TEAM_APPLY,				"TEAM_APPLY",				},		// ����������������.
																		// 
																		// arg0 - �ַ���
																		// �������.

	{ GE_TEAM_INVITE,				"TEAM_INVITE",				},		// ����������������.
																		// 
																		// arg0 - �ַ���
																		// ���������.
	
	{ GE_TEAM_CLEAR_UI,				"TEAM_HIDE_ALL_PLAYER",		},		// ��ս���
										
	
	{ GE_TEAM_REFRESH_UI,			"TEAM_REFRESH_DATA",		},		// ˢ�½���
	
	
	{ GE_TEAM_MEMBER_ENTER,			"TEAM_ENTER_MEMBER",		},		// ���µĶ�Ա����
																		//
																		// arg0 - ������, ��1 ��ʼ
																		// ��ui�����е���ʾλ��
	
	{ GE_TEAM_UPTEDATA_MEMBER_INFO,	"TEAM_UPDATE_MEMBER",		},		// ���¶�Ա��Ϣ
																		// arg0 - ������, ��1 ��ʼ
																		// ��ui�����е���ʾλ��
	
	{ GE_TEAM_MESSAGE,				"TEAM_TEAM_MESSAGE",		},		// ������Ϣ��ʾ��Ϣ
																		// arg0 - �ַ���
																		// ��Ҫ��ʾ����Ϣ.
	
	{ GE_TEAM_MEMBER_INVITE,		"TEAM_MEMBER_INVITE",      },		// ��Ա����ĳһ���˼����������ӳ�ͬ��
																		//
																		// arg0 - ��Ա����
																		// arg1 - �������˵�����

	{ GE_TEAM_FOLLOW_INVITE,		"TEAM_FOLLOW_INVITE",		},		// �ӳ������Ա������Ӹ���ģʽ
																		//
																		// arg0 - �ӳ�����

	{ GE_TEAM_REFRESH_MEMBER,		"TEAM_REFRESH_MEMBER",		},		// ˢ�¶�Ա��Ϣ
																		//
																		// arg0 - ��Ա��λ������

	{ GE_TEAM_CHANGE_MODE,			"TEAM_CHANGE_MODE",		},			// ˢ��ʰȡģʽ
																		// arg0 - ���ĵ�ʰȡģʽid
    /*****************************************************************************************************************************
	**
	** �����Ժ���Ҫɾ��.
	**
	**
	******************************************************************************************************************************/
	//{ GE_ON_TEAM_ENTER_MEMBER,		"TEAM_ENTER_MEMBER",		},// ���µĶ�Ա����
	{ GE_UPDATE_TEAM_MEMBER,		"ON_UPDATE_TEAM_MEMBER",	},// ���¶�Ա��Ϣ
	{ GE_SHOW_TEAM_YES_NO,				"SHOW_TEAM_YES_NO",		},// ����������������.
	{ GE_SHOW_TEAM_MEMBER_INFO,			"SHOW_PLAYER_INFO",		},// ��ʾ��Ա����ϸ��Ϣ
	{ GE_SHOW_TEAM_MEMBER_NAME,			"SHOW_PLAYER_NAME",		},// ����ߵĶ����б������ʾ���ѵ�����
	{ GE_HIDE_ALL_PLAYER,				"HIDE_ALL_PLAYER",		},// �Լ��뿪�����, �����ߵĶ����б�.
	{ GE_SHOW_TEAM_MEMBER_NICK,			"SHOW_TEAM_NICK",		},// �ǳ�
	{ GE_SHOW_TEAM_MEMBER_FAMILY,		"SHOW_TEAM_FAMILY",		},// ����
	{ GE_SHOW_TEAM_MEMBER_LEVEL,		"SHOW_TEAM_LEVEL",		},// �ȼ�
	{ GE_SHOW_TEAM_MEMBER_POS,			"SHOW_TEAM_POS",		},// λ��
	{ GE_SHOW_TEAM_MEMBER_HP,			"SHOW_TEAM_HP",			},// hp
	{ GE_SHOW_TEAM_MEMBER_MP,			"SHOW_TEAM_MP",			},// mp
	{ GE_SHOW_TEAM_MEMBER_ANGER,		"SHOW_TEAM_ANGER",		},// ŭ��
	{ GE_SHOW_TEAM_MEMBER_DEAD_LINK,	"SHOW_TEAM_DEAD_LINK",	},// ������Ϣ
	{ GE_SHOW_TEAM_MEMBER_DEAD,			"SHOW_TEAM_DEAD",		},// ������Ϣ.
	{ GE_SHOW_TEAM_FUNC_MENU_MEMBER,	"SHOW_TEAM_CONTEXMENU_MEMBER", },// ��ʾ��Ա�Ĺ��ܲ˵�
	{ GE_SHOW_TEAM_FUNC_MENU_LEADER,	"SHOW_TEAM_CONTEXMENU_LEADER", },// ��ʾ�ӳ��Ĺ��ܲ˵�

	// �������¼�
	//---------------------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------------------------------
	//
	// ��¼�������
	//
	
	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO,		"GAMELOGIN_SHOW_SYSTEM_INFO",	},		// ��ʾϵͳ��Ϣ
																					//
																					// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.


	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET,		"GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET",	},		// ��ʾϵͳ��Ϣ
																									//
																									// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.
																									//
																									// ���ȷ�ϰ�ť��Ͽ����硣


	{ GE_GAMELOGIN_CLOSE_SYSTEM_INFO,		"GAMELOGIN_CLOSE_SYSTEM_INFO",	},		// ��ʾ�ӳ��Ĺ��ܲ˵�

	
	{ GE_GAMELOGIN_OPEN_SELECT_SERVER,		"GAMELOGIN_OPEN_SELECT_SERVER",	},		// �ر�ѡ�����������
																					

	{ GE_GAMELOGIN_CLOSE_SELECT_SERVER,		"GAMELOGIN_CLOSE_SELECT_SERVER",},		// �ر�ѡ�����������


	{ GE_GAMELOGIN_OPEN_COUNT_INPUT,		"GAMELOGIN_OPEN_COUNT_INPUT",	},		// ���ʺ��������
																					

	{ GE_GAMELOGIN_CLOSE_COUNT_INPUT,		"GAMELOGIN_CLOSE_COUNT_INPUT",},		// �ر��ʺ��������

	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON,		"GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON",	},	// ��ʾϵͳ��Ϣ, ����ʾ��ť
																								//
																								// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.

	{ GE_GAMELOGIN_OPEN_SELECT_CHARACTOR,		"GAMELOGIN_OPEN_SELECT_CHARACTOR",	},			// ��ʾ����ѡ�����

	{ GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR,		"GAMELOGIN_CLOSE_SELECT_CHARACTOR",	},			// �ر�����ѡ�����


	{ GE_GAMELOGIN_OPEN_CREATE_CHARACTOR,		"GAMELOGIN_OPEN_CREATE_CHARACTOR", },			// ��ʾ���ﴴ������

	{ GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR,		"GAMELOGIN_CLOSE_CREATE_CHARACTOR",},			// �ر�����رս���

	{ GE_GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR,"GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR",},	// ˢ��������Ϣ


	{ GE_GAMELOGIN_CLOSE_BACK_GROUND,			"GAMELOGIN_CLOSE_BACK_GROUND",},				// �رձ�������


	{ GE_GAMELOGIN_SYSTEM_INFO_YESNO,			"GAMELOGIN_SYSTEM_INFO_YESNO",},				// ϵͳ��Ϣ��ʾ yes_no ����.
																								//
																								// ����0,	��ʾ���ַ���
																								// ����1,	�Ի��������
																								//			0 -- �Ƿ��˳���Ϸ
																								//			1 -- �Ƿ�ɾ����ɫ
																								//			2 -- �Ƿ�����ʺ�



	{ GE_GAMELOGIN_SELECT_LOGIN_SERVER,			"GAMELOGIN_SELECT_LOGIN_SERVER",},				// ѡ��һ��login server
																								//
																								// ����0, iAreaIndex   ��������
																								// ����1, iLoginServer ��������

	{ GE_GAMELOGIN_CLEAR_ACCOUNT,				"GAMELOGIN_CLEAR_ACCOUNT"},						// ����ʺ���ʾ.

	{ GE_GAMELOGIN_SELECT_AREA,					"GAMELOGIN_SELECT_AREA"},						// ѡ������
												
	{ GE_GAMELOGIN_SELECT_LOGINSERVER,			"GAMELOGIN_SELECT_LOGINSERVER"},				// ѡ��Login Server

	{ GE_GAMELOGIN_CREATE_CLEAR_NAME,			"GAMELOGIN_CREATE_CLEAR_NAME"},				// ��մ�����ɫ����

	{ GE_GAMELOGIN_CREATE_ROLE_OK,				"GAMELOGIN_CREATE_ROLE_OK"},				// ������ɫ�ɹ�

	
												

	
	//
	// ��¼�������
	//
	//------------------------------------------------------------------------------------------------------------------------


	{ GE_TOGLE_SKILLSTUDY,				"TOGLE_SKILLSTUDY",		},		// ���ɼ���ѧϰ����
	{ GE_SKILLSTUDY_SUCCEED,			"SKILLSTUDY_SUCCEED",	},		// ���ɼ���ѧϰ����
	{ GE_TOGLE_ABILITY_STUDY,			"TOGLE_ABILITY_STUDY"	},		// �����ѧϰ����


	{ GE_SUPERTOOLTIP,					"SHOW_SUPERTOOLTIP" }, //��ʾ����tooltip
	{ GE_UPDATE_SUPERTOOLTIP,			"UPDATE_SUPERTOOLTIP" }, //ˢ�³���tooltip
	{ GE_SHOW_SUPERTOOLTIP_TITLE,		"SHOW_SUPERTOOLTIP_TITLE" }, //ˢ�³���tooltip	//temp


	//
	// ����������
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ GE_RELIVE_SHOW,					"RELIVE_SHOW",			},		// ��ʾ�������
	{ GE_RELIVE_HIDE,					"RELIVE_HIDE",			},		// ���ظ������
	{ GE_RELIVE_REFESH_TIME,			"RELIVE_REFESH_TIME",	},		// ˢ�������ϵ�ʱ��

	{ GE_OBJECT_CARED_EVENT,			"OBJECT_CARED_EVENT",	},

	{ GE_UPDATE_MAP,					"UPDATE_MAP",	},
	{ GE_UPDATE_MAP_LIST,				"UPDATE_MAP_LIST",	},
	{ GE_UPDATE_LARGEMAP_LIST,			"UPDATE_LARGEMAP_LIST", },
	{ GE_UPDATE_SCENEMAP_LIST,			"UPDATE_SCENEMAP_LIST", },

	{ GE_UI_COMMAND,					"UI_COMMAND",	},
	{ GE_OTHERPLAYER_UPDATE_EQUIP,		"OTHERPLAYER_UPDATE_EQUIP", },
	{ GE_VARIABLE_CHANGED,				"VARIABLE_CHANGED" , TRUE },	//ȫ�ֱ��������ı�
	{ GE_TIME_UPDATE,					"TIME_UPDATE", },

	{ GE_UPDATE_FRIEND,					"UPDATE_FRIEND",	},			// �������ݸ�����
	{ GE_UPDATE_FRIEND_INFO,			"UPDATE_FRIEND_INFO", },
	
	{ GE_OPEN_FRIEND_SEARCH,			"OPEN_FRIEND_SEARCH", },		// ��������
	{ GE_OPEN_FRIEND_SEARCH_REASON,		"OPEN_FRIEND_SEARCH_REASON", },	// �����������
	{ GE_VALIDATE_ADD_FRIEND,			"VALIDATE_ADD_FRIEND", },		//���������Ϊ����
	{ GE_FRIEND_DELETE_FRIEND,			"FRIEND_DELETE_FRIEND",},		//ȷ��ɾ������

	
	{ GE_UPDATE_TARGETPET_PAGE,			"UPDATE_TARGETPET_PAGE", },
	{ GE_UPDATE_PETSKILLSTUDY,			"UPDATE_PETSKILLSTUDY", },
	{ GE_UPDATE_PETINVITEFRIEND,		"UPDATE_PETINVITEFRIEND", },
	{ GE_REPLY_MISSION_PET,				"REPLY_MISSION_PET", },
	
	//------------------------------------------------------------------------------------------------------------------------
	//
	// �Ƿ����ö�����������
	//

	{ GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG,		"MINORPASSWORD_OPEN_SET_PASSWORD_DLG", },		// �����ö����������
	{ GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG,	"MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG", },	// ��unlock������档
	{ GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG,	"MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG", },	// �򿪸���������档

	{ GE_OPEN_SYSTEM_TIP_INFO_DLG,	"OPEN_SYSTEM_TIP_INFO_DLG", },									// ��ʾϵͳ��Ϣ�Ի���
	{ GE_INPUT_PASSWORD_DLG,		"INPUT_PASSWORD_DLG", },										// �������������档

	// ������
	{ GE_GUILD_CREATE,					"GUILD_CREATE", },				// ��ᴴ���¼�
	{ GE_GUILD_CREATE_CONFIRM,			"GUILD_CREATE_CONFIRM", },		// ��ᴴ��ȷ���¼�
	{ GE_GUILD_SHOW_LIST,				"GUILD_SHOW_LIST", },			// ��ʾ����б��¼�
	{ GE_GUILD_SHOW_MEMBERINFO,			"GUILD_SHOW_MEMBERINFO", },		// ��ʾ�Լ�����Ա�������
	{ GE_GUILD_SHOW_DETAILINFO,			"GUILD_SHOW_DETAILINFO", },		// ��ʾ�Լ������ϸ��Ϣ����
	{ GE_GUILD_SHOW_APPOINTPOS,			"GUILD_SHOW_APPOINTPOS", },		// ��ʾ�Լ�������ְλ����
	{ GE_GUILD_DESTORY_CONFIRM,			"GUILD_DESTORY_CONFIRM", },		// ɾ�����ȷ���¼�
	{ GE_GUILD_QUIT_CONFIRM,			"GUILD_QUIT_CONFIRM", },		// �˳����ȷ���¼�
	{ GE_GUILD_UPDATE_MEMBERINFO,		"GUILD_UPDATE_MEMBERINFO", },	// ���°���Ա����Ϣ
	{ GE_GUILD_FORCE_CLOSE,				"GUILD_FORCE_CLOSE", },			// ǿ�ư����ؽ���ر��¼�
	{ GE_GUILD_INVITE_OBJECT,			"GUILD_INVITE_OBJECT", },		// ��ʾ����Ի���
	{ GE_GUILD_NOTIFY_INTIVE,			"GUILD_NOTIFY_INTIVE", },		// ��˸��ᰴť�� ��ʾ�յ�����
	{ GE_GUILD_INTIVE_CONFIRM,			"GUILD_INTIVE_CONFIRM", },		// ����ȷ��
	{ GE_GUILD_MODIFY_TITLE,			"GUILD_MODIFY_TITLE", },		// �޸ĳ�Ա�ƺ�
	{ GE_GUILD_CREATE_FAMILY,			"GUILD_CREATE_FAMILY", },			//��������ȷ��
	{ GE_GUILD_CREATE_FAMILY_CONFIRM,	"GUILD_CREATE_FAMILY_CONFIRM", },	//��������ȷ��
	{ GE_GUILD_DESTORY_FAMILY_CONFIRM,	"GUILD_DESTORY_FAMILY_CONFIRM", },	//ɾ������ȷ��
	{ GE_INPUT_PASSWD_COMMAND,			"INPUT_PASSWD_COMMAND", },	// ͨ���������봥��

	// �������
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

	// ����Ӻ��������
	{GE_OPEN_ADD_NEW_FRIEND,			"OPEN_ADD_NEW_FRIEND", },

	// ��Ӻ�������
	{ GE_ADD_GROUP,						"ADD_GROUP", },
	// ��ʾ��ϸ��Ϣ
	{ GE_SHOW_FRIEND_INFO,				"SHOW_FRIEND_INFO", },

	{ GE_QUICK_CHANGE_NAME,				"QUICK_CHANGE_NAME", },
	
	// ��Ʒ����
	{ GE_ITEM_LINK,						"ITEM_LINK",		},

	
	 //��װ�����ͽ�������
	{ GE_EQUIP_SWAPCARD_OPEN,			"EQUIP_SWAPCARD_OPEN",},

	//����װ�����ͽ�������
	{ GE_EQUIP_SWAPCARD_UPDATA,			"EQUIP_SWAPCARD_UPDATA",},
	//װ�����ͽ����ɹ�
	{ GE_EQUIP_SWAPCARD_SUCESS,			"EQUIP_SWAPCARD_SUCESS",},

	//��װ�����͸������
	{ GE_EQUIP_CHANGECARD_OPEN,			"EQUIP_CHANGECARD_OPEN",},


	//����װ�����͸������
	{ GE_EQUIP_CHANGECARD_UPDATA,		"EQUIP_CHANGECARD_UPDATA",},
	//װ�����͸���ɹ�
	{ GE_EQUIP_CHANGECARD_SUCESS,		"EQUIP_CHANGECARD_SUCESS",},


	// �ƺű�
	{ GE_SHOW_TITLE,					"SHOW_TITLE", },
	{ GE_UPDATE_TITLE,					"UPDATE_TITLE", },

	// ϵͳ���������Ϣ
	{ GE_PERSONAL_MESSAGE,				"PERSONAL_MESSAGE", },
	{ GE_SYSTEM_MESSAGE,				"SYSTEM_MESSAGE", },

	// ϵͳ��Ϣ��ʾͼ��
	{ GE_SHOW_SYSTEM_PROMPT,			"SHOW_SYSTEM_PROMPT", },

	{ GE_UNITE_GF_SHOW,					"UNITE_GF_SHOW", },

	{ GE_RICH_SAFE_GUARD,				"RICH_SAFE_GUARD",			 },	// ������Χ����
	{ GE_RICH_SAFE_TIME,				"RICH_SAFE_TIME",			 },	// ��ȫʱ�޵�������
	{ GE_RICH_SET_PASSWORD,				"RICH_SET_PASSWORD",		 },	// �������롢�޸��������
	{ GE_RICH_MODIFY_PASSWORD,			"RICH_MODIFY_PASSWORD",		 },
	{ GE_RICH_SETUP,					"RICH_SETUP",				 },	// �Ʋ������������ý���
	{ GE_OPEN_RICHES_PROTECT_FRAME,		"OPEN_RICHES_PROTECT_FRAME", },	// �Ʋ�������ʾ����
	{ GE_RICH_TEMP_CANCEL_PASSWORD,		"RICH_TEMP_CANCEL_PASSWORD", },	// ��ʱȡ������
	{ GE_RICH_CANCEL_PASSWORD,			"RICH_CANCEL_PASSWORD",		 },	// ȡ������
	{ GE_RICH_CHECK_PASSWORD,			"RICH_CHECK_PASSWORD",		 },	// �����������

	//temp
	{ GE_TEMP_CLOSE_FRIEND,				"TEMP_CLOSE_FRIEND", },		// 
	{ GE_TEMP_CLOSE_GUILD,				"TEMP_CLOSE_GUILD", },		//


	//��װ�� ��������  �����6������
	{ GE_EQUIP_ALL_OPEN,		"EquipAll_OPEN",},
	//��װ�� ��������  �����6������
	{ GE_EQUIP_ALL_CLOSE,		"EquipAll_ClOSE",},

	// ������ϸ���Խ���
	{ GE_TOGLE_PLAYER_ATTR_PAGE, "TOGLE_PLAYER_ATTR_PAGE", },
	{ GE_CLOSE_PLAYER_ATTR_PAGE, "CLOSE_PLAYER_ATTR_PAGE", },
	
	// ��ÿ�����ѹ��ܽ���
	{ GE_OPEN_DAY_TIPS,					"OPEN_DAY_TIPS",	},

	// ����ÿ����������
	{ GE_UPDATE_DATE_TIME,				"UPDATE_DATE_TIME",	},
	// ����ÿ����������
	{ GE_UPDATE_DAY_TIPS,				"UPDATE_DAY_TIPS",	},
	// ������������
	{ GE_UPDATE_CALENDAR,				"UPDATE_CALENDAR",	},
	// �����а����
	{ GE_OPEN_TOP_LIST,					"OPEN_TOP_LIST",	},
	// �������а����
	{ GE_UPDATE_TOP_LIST,				"UPDATE_TOP_LIST",	},
	// ��������ٽ���
	{ GE_OPEN_MISSION_SCOUT,			"OPEN_MISSION_SCOUT",},
	// ����������ٽ���
	{ GE_UPDATE_MISSION_SCOUT,			"UPDATE_MISSION_SCOUT",},

	{ GE_UPDATA_EQUIP_ALL_POS,			"UPDATA_EQUIP_ALL_POS",},

	{ GE_COUNTRY_MANAGER_CREATE_KING,	"COUNTRY_MANAGER_CREATE_KING", },	// ��������
	{ GE_COUNTRY_KING_CREATE_CONFIRM,	"COUNTRY_KING_CREATE_CONFIRM", },	// ȷ�ϲ�������
	{ GE_COUNTRY_MANAGER_REQUEST_INFO,	"COUNTRY_MANAGER_REQUEST_INFO", },	// ���������Ϣ
	{ GE_COUNTRY_MANAGER_INFO,			"COUNTRY_MANAGER_INFO",},			// ������Ϣ
	{ GE_COUNTRY_MANAGER_ADJUST_AUTH,	"COUNTRY_MANAGER_ADJUST_AUTH",},	// ���ҹ�ְ����
	{ GE_INPUT_COUNTRY_DLG,				"INPUT_COUNTRY_DLG", },				// ������ҹ�ְ

	{ GE_SUPERTOOLTIP2,				"SHOW_SUPERTOOLTIP2", },

	{ GE_END_PACKUP_PACKET, "END_PACKUP_PACKET"},
	{ GE_BEGIN_PACKUP_PACKET, "BEGIN_PACKUP_PACKET"},

	{ GE_OPEN_WINDOW,				"OPEN_WINDOW", },//��һ������

	{ GE_CLOSE_WINDOW, "CLOSE_WINDOW"},//�ر�һ������
	{ GE_TOGGLE_WINDOW, "TOGGLE_WINDOW"},//�л�һ������

	{ GE_SOUXIA_BANGDING_ENCHASE, "SOUXIA_BANGDING_ENCHASE" },	//ȷ���Ƿ������¼	
	{ GE_SOUXIA_STUDY, "SOUXIA_STUDY" },		//������¼
	{ GE_UPDATE_SOUXIA_STUDY, "UPDATE_SOUXIA_STUDY" },		//��������¼��ѧ�����б��
	{ GE_CLEAR_SOUXIA_BAR, "CLEAR_SOUXIA_BAR" },		//��������ܿ�����е���������
	{ GE_UPDATE_SOUXIA_SKILLNUM, "UPDATE_SOUXIA_SKILLNUM" },		//���¼��ܿ�����е��������ܵ�ʹ�ô���
	{ GE_UPDATE_SOUXIA_INDEXSKILLNUM, "UPDATE_SOUXIA_INDEXSKILLNUM" },		//����ָ��λ���ϵ�����������Ϣ

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


	{ GE_SET_SCROLL_INFO,				"SET_SCROLLINFO"},	// ����������Ϣ 20100624 BLL
	//RMB�̳����
	{GE_OPEN_IB_SHOP,					"OPEN_IB_SHOP"},// ��RMB�̳�������
	{GE_SHOW_ALL_IB_ITEM,				"SHOW_ALL_IB_ITEM"},// ��ʾ����RMB����
	{GE_UPDATE_IB_ITEM,					"UPDATE_IB_ITEM"},	// ����RMB����
	
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
		// �������Ƽ�����
		m_mapEventIndex_AsName[g_GlobalEvent[i].szEvent] = &(g_GlobalEvent[i]);
		// ����id������
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

	// ��������ٴ�����¼�
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

// ע���¼�������
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
	// �������ٶ���
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

		// ����Ƿ���ͬ����Event�Ѿ�������
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
	// �����¼�����
	EVENT_DEFINE* pEventDef = event.pEventDef;
	if(!pEventDef) 
		return;

	// ���ô�����
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

//ȥ���¼�������
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
