
/**	GMDataBase.cpp
 *
 *	功能：	管理全部的配置表
 *
 *	修改记录：
 *			080324	增加 DBC_EQUIP_LOC 针对主角换装表格改造
 *			
 *			080331	增加 DayTips.tab
 *			080410	增加 TopList.tab
 *			080410	增加 questlog.tab
 */

#include "StdAfx.h"
#include "GMDataBase.h"
#include "GIException.h"
#include "GIUtil.h"
#include "WXSJ_DBC.h"
#include "../Global.h"
#include "../Procedure/GameProcedure.h"
#include "GIResourceProvider.h"


//============================================================================

CDataBase::CDataBase(INT id)
{
	m_pFileDBC = new DBC::DBCFile(id);
}

CDataBase::~CDataBase()
{
	delete m_pFileDBC;
}


const tDataBase::DBC_FIELD* CDataBase::Search_Index_EQU(INT iIndexValue) const 
{
	return (DBC_FIELD*)m_pFileDBC->Search_Index_EQU(iIndexValue);
}

const tDataBase::DBC_FIELD* CDataBase::Search_LineNum_EQU(INT iLineNum) const
{
	return  (DBC_FIELD*)m_pFileDBC->Search_Posistion(iLineNum, 0); 
}

// 查找某列等于指定值的第一行
const tDataBase::DBC_FIELD* CDataBase::Search_First_Column_Equ(INT nColumnNum, const DBC_FIELD& value) const
{
	return (DBC_FIELD*)m_pFileDBC->Search_First_Column_Equ(nColumnNum, value.iValue);
}

// 取得ID
INT CDataBase::GetID(VOID) const
{
	return m_pFileDBC->GetID(); 
}

// 取得列数
UINT CDataBase::GetFieldsNum(VOID) const
{ 
	return m_pFileDBC->GetFieldsNum(); 
}

// 取得记录的条数
UINT CDataBase::GetRecordsNum(VOID) const
{ 
	return m_pFileDBC->GetRecordsNum(); 
}


// 直接打开一个dbc文件
BOOL CDataBase::Util_OpenDBCFromTxt(LPCTSTR szFileName, DBC::DBCFile* pDBCFile)
{
	KLAssert(szFileName && pDBCFile);

	STRING strFileName = "../../Public/Config/";
	strFileName += szFileName;
	if (::PathFileExists(strFileName.c_str()))
	{
		return pDBCFile->OpenFromTXT(strFileName.c_str()); 
	}
	strFileName = "../resource/config/";
	strFileName += szFileName;
	if (::PathFileExists(strFileName.c_str()))
	{
		return pDBCFile->OpenFromTXT(strFileName.c_str()); 
	}
	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szFileName, lpAddress, "General" );
	if( dwSize > 0 )
	{
		BOOL bRet = pDBCFile->OpenFromMemory( lpAddress, lpAddress + dwSize + 1);
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
		return bRet;
	}

	return FALSE;
}

BOOL CDataBase::OpenFromTXT(LPCTSTR szFileName)
{
	return Util_OpenDBCFromTxt(szFileName, m_pFileDBC);
}


BOOL CDataBase::GetSplitData(const CHAR* szData, UINT nCount, CHAR* szOutStr)
{
	if (NULL == szData || NULL == szOutStr)
		return false;

	// 找输出字符串的前一个“|”位置
	int off1 = 0;
	if (1 < nCount)
	{
		off1 = GetCharOff(szData, '|', nCount-1);
		if (-1 == off1)
			off1 = 0;
		else
			++off1;
	}	

	// 找输出字符串的后一个“|”位置
	int off2 = GetCharOff(szData, '|', nCount);
	if (-1 == off2)		
		off2 = (int)strlen(szData);

	// 获取2个"|"之间的字符串
	int copySize = off2 - off1;
	if (0 < copySize)
	{
		std::string temp( szData );
		temp.copy(szOutStr, copySize, off1 );

		return true;
	}

	return false;
}

INT CDataBase::GetCharOff(const CHAR* szStr, const CHAR ch, UINT nCount)
{
	if (NULL == szStr)
		return -1;

	int nNum = 0;
	int nSize = (int)strlen(szStr);

	for (int i=0; i<nSize; ++i)
	{
		if (ch == szStr[i])
		{
			++nNum;
			if (nNum >= nCount)
				return i;
		}
	}

	return -1;
}


//============================================================================


KL_IMPLEMENT_DYNAMIC(CDataBaseSystem, GETCLASS(tDataBaseSystem));
CDataBaseSystem* CDataBaseSystem::s_pMe = NULL;



CDataBaseSystem::CDataBaseSystem()
{
	s_pMe = this;
}

CDataBaseSystem::~CDataBaseSystem()
{
}


VOID CDataBaseSystem::Initial(VOID*)
{
	OpenAllDataBase();
}

VOID CDataBaseSystem::Release(VOID)
{
	CloseAllDataBase();
}

VOID CDataBaseSystem::OpenAllDataBase(VOID)
{
	// 所有需要打开的数据库定义
	struct _DATABASE_DEFINE
	{
		INT				idDatabase;
		const CHAR*		pFileName;
	};

	_DATABASE_DEFINE  s_dbToLoad[] = 
	{
		
		// 只在客户端用
		{ DBC_CHARACTER_RACE,			"char_race.tab"			},	// 主角种族							CharRace.txt
		{ DBC_CHARACTER_HEAD_GEO,		"char_head.tab"			},	// 主角脸部模型						CharHeadGeo.txt
//		{ DBC_CHARACTER_ACTION_SET,		"char_actset.tab"		},	// 主角动作组						CharActionSet.txt
		{ DBC_CHARACTER_MODEL,			"char_model.tab"		},	// 外形ID对应模型名称				CharModel.txt
		{ DBC_CHARACTER_MOUNT,			"char_mount.tab"		},	// 坐骑表							CharMount.txt
//		{ DBC_MISSION_DATA,				"mission_data.tab"		},	// 任务数据							MissionData.txt
//		{ DBC_CHARACTER_EFFECT,			"char_eff.tab"			},	// 主角特效							CharEffect.txt
		{ DBC_CHARACTER_FACE,			"char_face.tab"			},	// 头像信息							CharacterFace.txt
		{ DBC_SYSTEM_COLOR,				"sys_color.tab"			},	// 字体信息颜色表					SystemColour.txt
		{ DBC_CREATURE_SOUND,			"animal_snd.tab"		},	// 生物声音定义表					CreatureSound.txt
		{ DBC_ITEM_VISUAL_CHAR,			"item_char.tab"			},	// 装备类物品索引（身体，手臂，脚。。。。）	ItemVisualChar.txt
		{ DBC_EQUIP_LOC,				"equip_loc.tab"			},	// 装备类物品显示定义（身体，手臂，脚。。。。）	ItemVisualChar.txt
		{ DBC_ITEM_VISUAL_LOCATOR,		"item_loc.tab"			},	// 挂接类物品显示定义（武器）		ItemVisualLocator.txt
		{ DBC_ITEM_MONSTER_VISUAL_LOCATOR,"item_mon_loc.tab"	},	// 怪挂武器表
		{ DBC_SOUND_INFO,				"snd_info.tab"			},	// 声音定义文件						SoundInfo.txt
		{ DBC_EFFECT_DEFINE,			"eff_define.tab"		},	// 特效定义							EffectDefine.txt
		{ DBC_SKILL_ADDIMPACT,			"skl_impact.tab"		},	// 技能附加效果						SkillAddImpact.txt
//$$	{ DBC_SKILL_DEPLETE,			"skl_deplete.tab"		},	// 技能消耗							SkillDeplete.txt
		{ DBC_SCENE_DEFINE,				"scene_define.tab"		},	// 场景定义							SceneDefine.txt
		
		{ DBC_WORLDPOS_DEFINE,			"WorldPos.tab"			},	//可跳转的地图信息
		{ DBC_SCENENPC_DENFINE,			"scenemap_npc.tab"		},	//场景地图上的任务NPC的信息
		
		{ DBC_SCENE_POS_DEFINE,			"scene_position.tab"	},	// 场景传送点定义					ScenePosDefine.txt
		{ DBC_UI_LAYOUTDEFINE,			"ui.tab"				},	// UI组定义							Interface.txt
		{ DBC_LIFEABILITY_PLATFORM,		"platform.tab"			},	// 生活技能 操作平台				Platform.txt
		{ DBC_CODE_CONVERT,				"code_conv.tab"			},	// 表情和颜色转换字符				CodeConvert.txt
//		{ DBC_TITLE_NAME,				"char_title.tab"		},	// 称号列表							CharTitle.txt
		{ DBC_MISSION_KIND,				"mission_type.tab"		},	// 任务种类							MissionKind.txt
		{ DBC_STRING_DICT,				"str_dic.tab"			},	// 表情和颜色转换字符（字典）		StrDictionary.txt
		{ DBC_TALK_FILTER,				"str_filter.tab"		},	// 过滤词汇文件						StrFilter.txt
		{ DBC_TALK_ACT,					"chat_act.tab"			},	// 聊天动作命令文件					ChatMood.txt
		{ DBC_MONSTER_PAOPAO,			"mon_pop.tab"			},	// 怪物聊天泡泡						MonsterPaoPao.txt
		{ DBC_BUILDING_DEINFE,			"build_define.tab"		},	// 城市建筑物定义表					BuildingDefine.txt
		{ DBC_FULLCMP_FILTER,			"cmp_filter.tab"		},	// 完全匹配过滤表（用在创建帮会、起名字之类的地方，避免使用系统词组）	FullCompareFilter.txt

		// 和服务器共用
		//{ DBC_ITEM_COMMON_EQUIP_VISUAL,  "common_equipmodel.tab"},  // 普通装备外形表
		//{ DBC_ITEM_ADVANCED_EQUIP_VISUAL,"senior_equipmodel.tab"},  // 高级装备外形表

		{ DBC_ITEM_EQUIP_VISUAL,		 "equipmodel.tab"		},  //装备模型表
		{ DBC_ITEM_EQUIP,				 "equip_algo.tab"	    },  // 物品定义 人物装备表
		{ DBC_ITEM_ATTR_GROUP,			 "attribute_set.tab"	},  // 装备属性组表
		{ DBC_ITEM_ATTR_LEVEL,			 "attribute_level.tab"  },  // 装备属性等级表    //laim1108
		{ DBC_CHARACTER_HAIR_GEO,		"char_hair.tab"			},	// 主角头发模型						CharHairGeo.txt
		{ DBC_CHARACTER_EXPLEVEL,		"pl_explv.tab"			},	// 主角升级经验值					PlayerExpLevel.txt
		{ DBC_CREATURE_ATT,				"mon_attr_ex.tab"		},	// 生物定义总表						MonsterAttrExTable.txt
		{ DBC_PET_EX_ATT,				"pet_attr.tab"			},	// 宠物附加属性						PetAttrTable.txt
		{ DBC_PET_LEVELUP,				"pet_lvup.tab"			},	// 宠物升级所需经验表				PetLevelUpTable.txt	
	
		{ DBC_ITEM_MEDIC,				"common.tab"			},	// 物品定义 药瓶					CommonItem.txt
		{ DBC_ITEM_TASK,				"TaskItem.tab"			},  //任务物品
		{ DBC_ITEM_GEM,					"jew_info.tab"			},	// 物品定义 宝石					GemInfo.txt
		{ DBC_ITEM_STOREMAP,			"store_map.tab"			},	// 物品定义 地图					StoreMapItem.txt
		{ DBC_ITEM_EXTRABAG,			"Extra_Bag.tab"			},	// 物品定义  扩展背包
		{ DBC_ITEM_SOULBEAD,			"soul_bead.tab"			},	// 物品定义  魂珠
	//	{ DBC_ITEM_SETATT,				"equipset_attr.tab"		},	// 套装组合后附加属性				EquipSetAttr.txt
		{ DBC_SPECIAL_OBJ_DATA,			"sp_obj.tab"			},	// 法术OBJ数据						SpecialObjData.txt
		{ DBC_LIFEABILITY_GROWPOINT,	"grow_point.tab"		},	// 生活技能 资源生长点				GrowPoint.txt
		{ DBC_LIFEABILITY_DEFINE,		"ability.tab"			},	// 生活技能							Ability.txt
		{ DBC_LIFEABILITY_ITEMCOMPOSE,	"item_mix.tab"			},	// 生活技能 物品合成				ItemCompound.txt
		{ DBC_CAIKUANG_LEVELINFO,	"AbilityLevelUp\\caikuang.tab"			},	//	采矿技能升级信息
		{ DBC_SHOUGE_LEVELINFO,	"AbilityLevelUp\\shouge.tab"			},		//收割技能升级信息
		{ DBC_BUZHUO_LEVELINFO,	"AbilityLevelUp\\buzhuo.tab"			},		//捕捉技能升级信息
		{ DBC_ZHUZAO_LEVELINFO,	"AbilityLevelUp\\zhuzao.tab"			},		//铸造技能升级信息
		{ DBC_ZHIYI_LEVELINFO,	"AbilityLevelUp\\zhiyi.tab"			},					//制衣技能升级信息
		{ DBC_GONGJIANG_LEVELINFO,	"AbilityLevelUp\\gongjiang.tab"		},	//工匠技能升级信息
		{ DBC_PENGREN_LEVELINFO,	"AbilityLevelUp\\pengren.tab"			},	//烹饪技能升级信息

//		{ DBC_SKILL_XINFA,				"xf.tab"				},	// 心法定义							XinFa_V1.txt
//		{ DBC_XINFA_STUDYSPEND,			"xf_learnspend.tab"		},	// 心法升级花费						XinFaStudySpend_V1.txt
//$$	{ DBC_SKILLDATA_V1_DEPLETE,		"skl_data.tab"			},	// 技能（读取skillData_V1.txt）		SkillData_V1.txt
		{ DBC_ITEM_RULE,				"item_formula.tab"		},	// 物品规则							ItemRule.txt
		{ DBC_LV_MAXMONEY,				"lv_maxmoney.tab"		},	// 等级金钱对应表					LvMaxMoney.txt
		{ DBC_TALK_CONFIG,				"chat_cfg.tab"			},	// 聊天配置文件						ChatConfig.txt
		{ DBC_HAIR_STYLE,				"hair_sty.tab"			},	// 发型改变消耗表					HairStyle.txt
		{ DBC_CITY_BUILDING,			"city_build.tab"		},	// 城市建筑物表						CityBuilding.txt

		{ DBC_SKILL_DATA,				"skl_template.tab"		},	// 技能								SkillTemplate_V1.txt
		{ DBC_SKILL_STUDY_DATA,			"skl_learn.tab"			},	// 技能学习
		{ DBC_BUFF_IMPACT,				"impact_buff_eff.tab"	},	// BUFF附加效果						ImpactSEData_V1.txt
		{ DBC_DIRECTLY_IMPACT,			"impact_dir.tab"		},	// DIRECTLY附加效果					ImpactDirectly.txt
		{ DBC_CAMP_AND_STAND,			"camp_stand.tab"		},	// 阵营								CampAndStand.txt
		{ DBC_BULLET_DATA,				"blt.tab"				},	// 子弹								Bullet.txt
		{ DBC_MISSION_INFO,				"obj_quest.tab"			},	// 任务NPC信息

		{ DBC_EQUIP_HOLE_INFO,			"Equip_HoleReqInfo.tab" },  // 装备打孔信息表
		{ DBC_EQUIP_BINDING_INFO,		"Equip_bind.tab"		},	// 装备绑定信息表
		{ DBC_EQUIP_ADD_START,			"Equip_levelup.tab"		},	// 装备打星信息表
		{ DBC_EQUIP_SP_REPAIR,			"Equip_SpecialRepair.tab"}, // 装备打星信息表
		{ DBC_EQUIP_PRODUCT_INFO,		"Product_info.tab" },		// 装备合成改造表
		{ DBC_EQUIP_MATERIAL_INFO,		"MaterialLevel_info.tab"},  // 材料等级表

		{ DBC_MOUNT_ITEM_DATA,			"qicheng_daoju.tab"		},

		{ DBC_EQUIP_CARD_INFO,			"pai_info.tab"			},	// 每张牌信息表
		{ DBC_EQUIP_CARD_GROUP_INFO,	"chain_info.tab"		},	// 装备灵魂小激活信息表
		{ DBC_EQUIP_CARD_ALL,			"soul_info.tab"			},	// 装备灵魂大激活信息表
		{ DBC_EQUIP_SET_INFO,			"equipset_attr.tab"		},	// 套装信息表
		{ DBC_EQUIP_CARD_TARGET,		"ChangePai_info.tab"	},	// 换牌表格
		{ DBC_MOUNT_BASE_DATA,			"qichengjichu.tab"		},	// 坐骑基础表

		{ DBC_TITLE,					"title_info.tab"		},	// 称号

		{ DBC_DAY_TIPS,					"DayTips.tab"			},	// 每日提醒功能表
		{ DBC_TOP_LIST,					"TopList.tab"			},	// 排行榜功能表
		{ DBC_QUEST_LOG,				"questlog.tab"			},	// 任务管理功能表
		{ DBC_MODEL_PARAMETER,			"Model_Parameter.tab"	},	//模型的技术参数
		{ DBC_SOUXIA_DATA,				"souxia.tab"			},

		{ DBC_MAPPIC_DATA,				"Mappic.tab"			},	// 场景跳转图片表	  20100517 AddCode
		{ DBC_CHATHELP_DATA,			"Chathelp.tab"			},	// 场景跳转文字提示表 20100517 AddCode
		{ DBC_IMPACT_SOUXIAMODEL,   	"impact_SouxiaModel.tab"},
		{ DBC_BUS_DATA,                 "BusInfo.tab"           },
		{ DBC_ITEM_MOUNT_ATT,           "mount_attr.tab"        },  // 坐骑属性表 [7/6/2010 陈军龙] 
		{ DBC_IB_SHOP,								"ibshop.tab"},	//读取IBShop

	};

	
	// 打开所有数据库
	INT nNum = sizeof(s_dbToLoad)/sizeof(_DATABASE_DEFINE);
	for(INT i=0; i<nNum; i++)
	{
		CDataBase* pDataBase = new CDataBase(s_dbToLoad[i].idDatabase);
		if( !pDataBase )
			KLThrow( "New DataBase %d Error!", nNum );

		if(!(pDataBase->OpenFromTXT (s_dbToLoad[i].pFileName)))
		{
			KLThrow("Open %s Error!", s_dbToLoad[i].pFileName);
		}

		if(m_mapDataBaseBuf.find(s_dbToLoad[i].idDatabase) != m_mapDataBaseBuf.end())
		{
			KLThrow("DBC ID Multi define(%s)", s_dbToLoad[i].pFileName);
		}

		m_mapDataBaseBuf.insert(std::make_pair(s_dbToLoad[i].idDatabase, pDataBase));
	}
}

VOID CDataBaseSystem::CloseAllDataBase(VOID)
{
	std::map< INT, CDataBase* >::iterator it;
	for(it = m_mapDataBaseBuf.begin(); it!=m_mapDataBaseBuf.end(); it++)
	{
		delete (CDataBase*)(it->second);
		it->second = NULL;
	}

	m_mapDataBaseBuf.clear();
}

const tDataBase* CDataBaseSystem::GetDataBase(INT idDataBase) const
{
	std::map< INT, CDataBase* >::const_iterator it = m_mapDataBaseBuf.find(idDataBase);
	return (const tDataBase*)it->second;
}

LPCTSTR TransTitleStr( LPCTSTR szTitle )
{
	if( szTitle[0] == '#' )
	{
		// 这个字符串是id需要读表转换
		STRINGTOID( szTitle, TBIndex );
		if( TBIndex > 0 )
		{
//			DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);	// char_title.tab
//			const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TBIndex);
//			
//			return pTitle->szTitle;
		}

		return "";
	}

	return szTitle;	
}
