// GameDefine_Result.cpp

#include "stdafx.h"
#include "Type.h"
#include <map>
#include "GameDefine_Result.h"

struct _RESULT_TEXT
{
	OPERATE_RESULT	nCode;
	char			*pszText;
};

static _RESULT_TEXT s_aResultText[] =
{
	{ OR_OK,							"�ɹ�"								},
	{ OR_ERROR,							""									},
	{ OR_DIE,							"��������"							},
	{ OR_INVALID_SKILL,					"��Ч����"							},
	{ OR_TARGET_DIE,					"Ŀ��������"						},
	{ OR_LACK_MANA,						"��������"							},
	{ OR_COOL_DOWNING,					"��ȴʱ��δ��"						},
	{ OR_NO_TARGET,						"û��Ŀ��"							},
	{ OR_INVALID_TARGET,				"��ЧĿ��"							},
	{ OR_OUT_RANGE,						"������Χ"							},
	{ OR_IN_RANGE,						"����̫��"							},
	{ OR_NO_PLATFORM,					"û�в���ƽ̨"						},
	{ OR_OUT_PLATFORM,					"����Ŀ��̫Զ"						},
	{ OR_NO_TOOL,						"û�в�������"						},
	{ OR_STUFF_LACK,					"ȱ�ٲ���"							},
	{ OR_BAG_OUT_OF_SPACE,				"����ȱ�ٿռ�"						},
	{ OR_WARNING,						"δ֪����"							},
	{ OR_BUSY,							"��������������"					},
	{ OR_MISSION_HAVE,					"�Ѿ�ӵ�и�����"					},
	{ OR_MISSION_LIST_FULL,				"�����¼����"						},
	{ OR_MISSION_NOT_FIND,				"û�ҵ�������"						},
	{ OR_EXP_LACK,						"�����Ȳ���"						},
	{ OR_CHAR_DO_NOT_KNOW_THIS_SKILL,	"��ɫ���������ּ���"				},
	{ OR_NO_SCRIPT,						"Ŀ���ɫ�޽ű�"					},
	{ OR_NOT_ENOUGH_HP,					"û���㹻������ֵ"					},
	{ OR_NOT_ENOUGH_RAGE,				"û���㹻��ŭ��ֵ"					},
	{ OR_NOT_ENOUGH_STRIKE_POINT,		"û���㹻��������"					},
	{ OR_NOT_ENOUGH_ITEM,				"û���㹻�ĵ���"					},
	{ OR_NOT_ENOUGH_VIGOR,				"û���㹻�Ļ���"					},
	{ OR_NO_LEVEL,						"���ܵȼ�����"						},
	{ OR_CANNOT_UPGRADE,				"��ʯ�޷�������"					},
	{ OR_FAILURE,						"����ʧ��"							},
	{ OR_GEM_CONFLICT,					"һ��װ���ϲ�����װ��ͬ���͵ı�ʯ"	},
	{ OR_NEED_IN_FURY_STANCE,			"��Ҫ�ڿ�״̬��ʹ��"				},
	{ OR_INVALID_TARGET_POS,			"��ЧĿ���"						},
	{ OR_GEM_SLOT_LACK,					"�޷���Ƕ���౦ʯ"					},
	{ OR_LIMIT_MOVE,					"�޷��ƶ�"							},
	{ OR_LIMIT_USE_SKILL,				"�޷�ʹ�ü���"						},
	{ OR_INACTIVE_SKILL,				"ʹ����δ����ļ���"				},
	{ OR_TOO_MUCH_HP,					"����ֵ����С���޶���ֵ"			},
	{ OR_NEED_A_WEAPON,					"��������Ҫʹ��һ������"			},
	{ OR_NEED_HIGH_LEVEL_XINFA,			"����ķ��ȼ�����"					},
	{ OR_NEED_LEARN_SKILL_FIRST,		"��δѧ���������"					},
	{ OR_NEED_MENPAI_FOR_LEVELUP,		"��������һ�����ɲ��ܼ�������"	},
	{ OR_U_CANNT_DO_THIS_RIGHT_NOW,		"�������޷�������"					},
	{ OR_ROBOT_TIMEOUT,					"�һ�ʱ�䳬ʱ"						},
	{ OR_NEED_HAPPINESS_60,				"��ĳ�����ֶȲ���60�����ܳ�ս����Ҫѱ��"},
	{ OR_NEED_HIGH_LEVEL,				"��ĵȼ�����"						},
	{ OR_CANNOT_GETEXP,					"��ĳ������޷��õ�����"			},
	{ OR_NEED_LIFE,						"��ĳ�������Ϊ0���޷����ٻ�����Ϊ����������"},
	{ OR_EXP_FULL,						"���ľ����Ѿ���������"				},
	{ OR_TOO_MANY_TRAPS,				"�޷����ø���Ĵ�������"			},
	
	{ OR_PET_PLACARD_ISSUE_FAILED,		"����ʧ��"							},
	{ OR_PET_PLACARD_NOT_APPOINT_PET,	"δָ������"						},
	{ OR_PET_PLACARD_ONLY_CAN_ISSUE_ONE,"�Բ���ͬʱֻ�ܷ���һֻ��"		},
	{ OR_PET_PLACARD_NEED_PET_TYPE,		"�Բ�����ĳ�����Ǳ���Ŷ"		},
	{ OR_PET_PLACARD_NEED_PET_LEVEL,	"�Բ�����ĳ輶�𻹲���Ŷ"		},
	{ OR_PET_PLACARD_NEED_PET_HAPPINESS,"�Բ�����ĳ����ֵ������"		},
	{ OR_PET_PLACARD_NEED_PET_LIFE,		"�Բ�����ĳ���������"			},
	{ OR_NEED_IN_STEALTH_MODE,   		"��Ҫ������״̬��ʹ��"				},
	{ OR_NOT_ENOUGH_ENERGY,				"û���㹻�ľ���"					},
	{ OR_NEED_IN_SHIELD_MODE,			"��Ҫ�ڻ��屣����"					},
	{ OR_PET_DIE,						"��ĳ����Ѿ�����"					},
	{ OR_PET_HADRECALL_NEEDHAPPINESS,	"��ĳ�����ֶȲ���60���ѱ��ջ�"	},
	{ OR_PET_HADRECALL_NEEDLIFE,		"��ĳ�������Ϊ0���ѱ��ջ�"			},

	{ OR_CAN_NOT_MOVE_STALL_OPEN,   	"�޷��ڰ�̯���ƶ�"					},

	{ OR_GEM_NOT_FIT_EQUIP,				"���ֱ�ʯ������Ƕ������װ����"		},
	{ OR_CANNOT_ASK_PETDETIAL,			"���޷��쿴�ó������Ϣ"			},
	{ OR_VARIANCEPET_CANNOT_RETURNTOCHILD, "����費�ܽ��л�ͯ"				},
	{ OR_COMBAT_CANNOT_RETURNTOCHILD,	"��ս�費�ܽ��л�ͯ"				},
	{ OR_IMPASSABLE_ZONE,				"����������"						},
	{ OR_NEED_SETMINORPASSWORD,			"�������ö����������ظ��ղŵĲ���"	},
	{ OR_NEED_UNLOCKMINORPASSWORD,		"���ȶԶ���������н��������ظ��ղŵĲ���" },
	{ OR_PETINEXCHANGE_CANNOT_GOFIGHT,	"���ڽ��׵ĳ����޷���ս"			},
	{ OR_HASSPOUSE_CANNOT_RETURNTOCHILD,"�Ѿ�����ż�ĳ��ﲻ�ܻ�ͯ"			},
	{ OR_CAN_NOT_FIND_SPECIFIC_ITEM,	"ȱ��ָ����Ʒ"						},
	{ OR_ISSUEHORSE_MUSTBE_ZHANMA,		"��������˱�����ս��"},
	{ OR_ISSUEHORSE_MUSTNOT_HAVEMATE,	"��������˲������ѻ��"},
	{ OR_ISSUEHORSE_HAVE_ONLYONE,		"���ֻ�ܷ���һֻ���"},
	{ OR_ISSUEHORSE_NOTENOUGH_LEVEL,	"��������˵ȼ�����"},
	{ OR_ISSUEHORSE_NOTENOUGH_HAPPINESS,"��������˿��ֶȲ���"},
	{ OR_ISSUEHORSE_NOTENOUGH_MONEY,	"�����������Ҫ����Ľ�Ǯ"},
	{ OR_ISSUEHORSE_SUCC,				"��������˳ɹ�"},
	{ OR_ISSUEHORSE_FAIL,				"���������ʧ��"},
	{ OR_CANCELHORSEFINDMATE_SUCC,		"ȡ��������ѳɹ�"},
	{ OR_CANCELHORSEFINDMATE_FAIL,		"ȡ���������ʧ��"},
	{ OR_THIS_PLAYER_IS_OFFLINE,		"��������ڲ�����"},
	{ OR_DOUBLEMOODACT_REFUSED,			"˫����ж������ܾ�"},
	{ OR_NOT_ENOUGH_MP,					"û���㹻������ֵ"},
	{ OR_NOT_ENOUGH_PNEUMA,				"û���㹻��Ԫ��ֵ"},

	{ OR_CALLUP_HORSE_CAN_NOT_FREE,		"��ս�����ܷ���"					},
	{ OR_HORSE_HAPPINESS_ENOUGH,	    "�����˺ܿ���,����Ҫѱ��"			},
	{ OR_NOT_ENOUGH_MONEY,				"�����ϵĽ�Ǯ����"					},
	{ OR_HORSE_CALLUP_CAN_NOT_CHECKUP,	"��ս�����ܼ���"					},
	{ OR_HORSE_CHECKUP_NEED_10LEVEL,	"�����ϲ��ܼ���"					},	
	{ OR_HORSE_NO_ENOUGH_SKILL_SOLT,	"������û���㹻����ͨ��,���ȶ���ʹ�ü��ܲ۵��߰�"		},
	{ OR_HORSE_HAVE_TOO_MUCH_SKILL,		"������û�п���ͨ�鵽����,���ȶ���ʹ�ü����������߰�"	},

	{OR_HP_IS_FULL, "����ֵ������"},
	{OR_MP_IS_FULL, "����ֵ������"},

	{OR_PETZHAOHUAN_NO_TIMES, "�������ٻ�����ʹ�ô�������"},
	{OR_ZUOJIZHAOHUAN_NO_TIMES, "�������ٻ�����ʹ�ô�������"},

	{OR_BUS_HASPASSENGER,	"�������ڴ��״̬" },
	{OR_BUS_NOTINBUS		,	"�㲢δ���ڴ��״̬" },
	{OR_BUS_RUNNING			, "Ŀ���Ѿ�Զ��" },
	{OR_BUS_HASMOUNT	,  "�����������״̬"} ,
	{OR_BUS_HASPET		, "������������ڳ�ս״̬"},
	{OR_BUS_CANNOT_TEAM_FOLLOW , "����������Ӹ���״̬" },
	{OR_BUS_CANNOT_DRIDE	, "��������˫�����״̬"},
	{OR_BUS_CANNOT_CHANGE_MODEL ,"�������ڱ���״̬"},
	{OR_BUS_PASSENGERFULL	,"Ŀ�����޿ռ�"},
};

class COResultTextManager
{
protected:
	typedef std::map< INT, const char* >	COResultTextMap;
	COResultTextMap			m_mapOResultText;

public:
	COResultTextManager()
	{
		INT i, nSize;
		nSize = sizeof( s_aResultText ) / sizeof( _RESULT_TEXT );
		for ( i = 0; i < nSize; i++ )
		{
			m_mapOResultText.insert( COResultTextMap::value_type( s_aResultText[i].nCode, s_aResultText[i].pszText ) );
		}
	}

	~COResultTextManager()
	{
		m_mapOResultText.erase(m_mapOResultText.begin(), m_mapOResultText.end());
	}

	LPCSTR GetOperateResultText( OPERATE_RESULT oResult )
	{
		static const char *szOR_ERROR			= "unkown error";

		COResultTextMap::iterator itFind;
		itFind = m_mapOResultText.find(oResult);
		if ( itFind != m_mapOResultText.end() )
		{
			return itFind->second;
		}
		else
		{
			return szOR_ERROR;
		}
	}

};

static COResultTextManager g_OResultTextManager;

const char *GetOResultText( OPERATE_RESULT code )
{
	return g_OResultTextManager.GetOperateResultText( code );
}
