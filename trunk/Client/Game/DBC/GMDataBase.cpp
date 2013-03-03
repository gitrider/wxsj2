
/**	GMDataBase.cpp
 *
 *	���ܣ�	����ȫ�������ñ�
 *
 *	�޸ļ�¼��
 *			080324	���� DBC_EQUIP_LOC ������ǻ�װ������
 *			
 *			080331	���� DayTips.tab
 *			080410	���� TopList.tab
 *			080410	���� questlog.tab
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

// ����ĳ�е���ָ��ֵ�ĵ�һ��
const tDataBase::DBC_FIELD* CDataBase::Search_First_Column_Equ(INT nColumnNum, const DBC_FIELD& value) const
{
	return (DBC_FIELD*)m_pFileDBC->Search_First_Column_Equ(nColumnNum, value.iValue);
}

// ȡ��ID
INT CDataBase::GetID(VOID) const
{
	return m_pFileDBC->GetID(); 
}

// ȡ������
UINT CDataBase::GetFieldsNum(VOID) const
{ 
	return m_pFileDBC->GetFieldsNum(); 
}

// ȡ�ü�¼������
UINT CDataBase::GetRecordsNum(VOID) const
{ 
	return m_pFileDBC->GetRecordsNum(); 
}


// ֱ�Ӵ�һ��dbc�ļ�
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

	// ������ַ�����ǰһ����|��λ��
	int off1 = 0;
	if (1 < nCount)
	{
		off1 = GetCharOff(szData, '|', nCount-1);
		if (-1 == off1)
			off1 = 0;
		else
			++off1;
	}	

	// ������ַ����ĺ�һ����|��λ��
	int off2 = GetCharOff(szData, '|', nCount);
	if (-1 == off2)		
		off2 = (int)strlen(szData);

	// ��ȡ2��"|"֮����ַ���
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
	// ������Ҫ�򿪵����ݿⶨ��
	struct _DATABASE_DEFINE
	{
		INT				idDatabase;
		const CHAR*		pFileName;
	};

	_DATABASE_DEFINE  s_dbToLoad[] = 
	{
		
		// ֻ�ڿͻ�����
		{ DBC_CHARACTER_RACE,			"char_race.tab"			},	// ��������							CharRace.txt
		{ DBC_CHARACTER_HEAD_GEO,		"char_head.tab"			},	// ��������ģ��						CharHeadGeo.txt
//		{ DBC_CHARACTER_ACTION_SET,		"char_actset.tab"		},	// ���Ƕ�����						CharActionSet.txt
		{ DBC_CHARACTER_MODEL,			"char_model.tab"		},	// ����ID��Ӧģ������				CharModel.txt
		{ DBC_CHARACTER_MOUNT,			"char_mount.tab"		},	// �����							CharMount.txt
//		{ DBC_MISSION_DATA,				"mission_data.tab"		},	// ��������							MissionData.txt
//		{ DBC_CHARACTER_EFFECT,			"char_eff.tab"			},	// ������Ч							CharEffect.txt
		{ DBC_CHARACTER_FACE,			"char_face.tab"			},	// ͷ����Ϣ							CharacterFace.txt
		{ DBC_SYSTEM_COLOR,				"sys_color.tab"			},	// ������Ϣ��ɫ��					SystemColour.txt
		{ DBC_CREATURE_SOUND,			"animal_snd.tab"		},	// �������������					CreatureSound.txt
		{ DBC_ITEM_VISUAL_CHAR,			"item_char.tab"			},	// װ������Ʒ���������壬�ֱۣ��š���������	ItemVisualChar.txt
		{ DBC_EQUIP_LOC,				"equip_loc.tab"			},	// װ������Ʒ��ʾ���壨���壬�ֱۣ��š���������	ItemVisualChar.txt
		{ DBC_ITEM_VISUAL_LOCATOR,		"item_loc.tab"			},	// �ҽ�����Ʒ��ʾ���壨������		ItemVisualLocator.txt
		{ DBC_ITEM_MONSTER_VISUAL_LOCATOR,"item_mon_loc.tab"	},	// �ֹ�������
		{ DBC_SOUND_INFO,				"snd_info.tab"			},	// ���������ļ�						SoundInfo.txt
		{ DBC_EFFECT_DEFINE,			"eff_define.tab"		},	// ��Ч����							EffectDefine.txt
		{ DBC_SKILL_ADDIMPACT,			"skl_impact.tab"		},	// ���ܸ���Ч��						SkillAddImpact.txt
//$$	{ DBC_SKILL_DEPLETE,			"skl_deplete.tab"		},	// ��������							SkillDeplete.txt
		{ DBC_SCENE_DEFINE,				"scene_define.tab"		},	// ��������							SceneDefine.txt
		
		{ DBC_WORLDPOS_DEFINE,			"WorldPos.tab"			},	//����ת�ĵ�ͼ��Ϣ
		{ DBC_SCENENPC_DENFINE,			"scenemap_npc.tab"		},	//������ͼ�ϵ�����NPC����Ϣ
		
		{ DBC_SCENE_POS_DEFINE,			"scene_position.tab"	},	// �������͵㶨��					ScenePosDefine.txt
		{ DBC_UI_LAYOUTDEFINE,			"ui.tab"				},	// UI�鶨��							Interface.txt
		{ DBC_LIFEABILITY_PLATFORM,		"platform.tab"			},	// ����� ����ƽ̨				Platform.txt
		{ DBC_CODE_CONVERT,				"code_conv.tab"			},	// �������ɫת���ַ�				CodeConvert.txt
//		{ DBC_TITLE_NAME,				"char_title.tab"		},	// �ƺ��б�							CharTitle.txt
		{ DBC_MISSION_KIND,				"mission_type.tab"		},	// ��������							MissionKind.txt
		{ DBC_STRING_DICT,				"str_dic.tab"			},	// �������ɫת���ַ����ֵ䣩		StrDictionary.txt
		{ DBC_TALK_FILTER,				"str_filter.tab"		},	// ���˴ʻ��ļ�						StrFilter.txt
		{ DBC_TALK_ACT,					"chat_act.tab"			},	// ���춯�������ļ�					ChatMood.txt
		{ DBC_MONSTER_PAOPAO,			"mon_pop.tab"			},	// ������������						MonsterPaoPao.txt
		{ DBC_BUILDING_DEINFE,			"build_define.tab"		},	// ���н����ﶨ���					BuildingDefine.txt
		{ DBC_FULLCMP_FILTER,			"cmp_filter.tab"		},	// ��ȫƥ����˱����ڴ�����ᡢ������֮��ĵط�������ʹ��ϵͳ���飩	FullCompareFilter.txt

		// �ͷ���������
		//{ DBC_ITEM_COMMON_EQUIP_VISUAL,  "common_equipmodel.tab"},  // ��ͨװ�����α�
		//{ DBC_ITEM_ADVANCED_EQUIP_VISUAL,"senior_equipmodel.tab"},  // �߼�װ�����α�

		{ DBC_ITEM_EQUIP_VISUAL,		 "equipmodel.tab"		},  //װ��ģ�ͱ�
		{ DBC_ITEM_EQUIP,				 "equip_algo.tab"	    },  // ��Ʒ���� ����װ����
		{ DBC_ITEM_ATTR_GROUP,			 "attribute_set.tab"	},  // װ���������
		{ DBC_ITEM_ATTR_LEVEL,			 "attribute_level.tab"  },  // װ�����Եȼ���    //laim1108
		{ DBC_CHARACTER_HAIR_GEO,		"char_hair.tab"			},	// ����ͷ��ģ��						CharHairGeo.txt
		{ DBC_CHARACTER_EXPLEVEL,		"pl_explv.tab"			},	// ������������ֵ					PlayerExpLevel.txt
		{ DBC_CREATURE_ATT,				"mon_attr_ex.tab"		},	// ���ﶨ���ܱ�						MonsterAttrExTable.txt
		{ DBC_PET_EX_ATT,				"pet_attr.tab"			},	// ���︽������						PetAttrTable.txt
		{ DBC_PET_LEVELUP,				"pet_lvup.tab"			},	// �����������辭���				PetLevelUpTable.txt	
	
		{ DBC_ITEM_MEDIC,				"common.tab"			},	// ��Ʒ���� ҩƿ					CommonItem.txt
		{ DBC_ITEM_TASK,				"TaskItem.tab"			},  //������Ʒ
		{ DBC_ITEM_GEM,					"jew_info.tab"			},	// ��Ʒ���� ��ʯ					GemInfo.txt
		{ DBC_ITEM_STOREMAP,			"store_map.tab"			},	// ��Ʒ���� ��ͼ					StoreMapItem.txt
		{ DBC_ITEM_EXTRABAG,			"Extra_Bag.tab"			},	// ��Ʒ����  ��չ����
		{ DBC_ITEM_SOULBEAD,			"soul_bead.tab"			},	// ��Ʒ����  ����
	//	{ DBC_ITEM_SETATT,				"equipset_attr.tab"		},	// ��װ��Ϻ󸽼�����				EquipSetAttr.txt
		{ DBC_SPECIAL_OBJ_DATA,			"sp_obj.tab"			},	// ����OBJ����						SpecialObjData.txt
		{ DBC_LIFEABILITY_GROWPOINT,	"grow_point.tab"		},	// ����� ��Դ������				GrowPoint.txt
		{ DBC_LIFEABILITY_DEFINE,		"ability.tab"			},	// �����							Ability.txt
		{ DBC_LIFEABILITY_ITEMCOMPOSE,	"item_mix.tab"			},	// ����� ��Ʒ�ϳ�				ItemCompound.txt
		{ DBC_CAIKUANG_LEVELINFO,	"AbilityLevelUp\\caikuang.tab"			},	//	�ɿ���������Ϣ
		{ DBC_SHOUGE_LEVELINFO,	"AbilityLevelUp\\shouge.tab"			},		//�ո��������Ϣ
		{ DBC_BUZHUO_LEVELINFO,	"AbilityLevelUp\\buzhuo.tab"			},		//��׽����������Ϣ
		{ DBC_ZHUZAO_LEVELINFO,	"AbilityLevelUp\\zhuzao.tab"			},		//���켼��������Ϣ
		{ DBC_ZHIYI_LEVELINFO,	"AbilityLevelUp\\zhiyi.tab"			},					//���¼���������Ϣ
		{ DBC_GONGJIANG_LEVELINFO,	"AbilityLevelUp\\gongjiang.tab"		},	//��������������Ϣ
		{ DBC_PENGREN_LEVELINFO,	"AbilityLevelUp\\pengren.tab"			},	//��⿼���������Ϣ

//		{ DBC_SKILL_XINFA,				"xf.tab"				},	// �ķ�����							XinFa_V1.txt
//		{ DBC_XINFA_STUDYSPEND,			"xf_learnspend.tab"		},	// �ķ���������						XinFaStudySpend_V1.txt
//$$	{ DBC_SKILLDATA_V1_DEPLETE,		"skl_data.tab"			},	// ���ܣ���ȡskillData_V1.txt��		SkillData_V1.txt
		{ DBC_ITEM_RULE,				"item_formula.tab"		},	// ��Ʒ����							ItemRule.txt
		{ DBC_LV_MAXMONEY,				"lv_maxmoney.tab"		},	// �ȼ���Ǯ��Ӧ��					LvMaxMoney.txt
		{ DBC_TALK_CONFIG,				"chat_cfg.tab"			},	// ���������ļ�						ChatConfig.txt
		{ DBC_HAIR_STYLE,				"hair_sty.tab"			},	// ���͸ı����ı�					HairStyle.txt
		{ DBC_CITY_BUILDING,			"city_build.tab"		},	// ���н������						CityBuilding.txt

		{ DBC_SKILL_DATA,				"skl_template.tab"		},	// ����								SkillTemplate_V1.txt
		{ DBC_SKILL_STUDY_DATA,			"skl_learn.tab"			},	// ����ѧϰ
		{ DBC_BUFF_IMPACT,				"impact_buff_eff.tab"	},	// BUFF����Ч��						ImpactSEData_V1.txt
		{ DBC_DIRECTLY_IMPACT,			"impact_dir.tab"		},	// DIRECTLY����Ч��					ImpactDirectly.txt
		{ DBC_CAMP_AND_STAND,			"camp_stand.tab"		},	// ��Ӫ								CampAndStand.txt
		{ DBC_BULLET_DATA,				"blt.tab"				},	// �ӵ�								Bullet.txt
		{ DBC_MISSION_INFO,				"obj_quest.tab"			},	// ����NPC��Ϣ

		{ DBC_EQUIP_HOLE_INFO,			"Equip_HoleReqInfo.tab" },  // װ�������Ϣ��
		{ DBC_EQUIP_BINDING_INFO,		"Equip_bind.tab"		},	// װ������Ϣ��
		{ DBC_EQUIP_ADD_START,			"Equip_levelup.tab"		},	// װ��������Ϣ��
		{ DBC_EQUIP_SP_REPAIR,			"Equip_SpecialRepair.tab"}, // װ��������Ϣ��
		{ DBC_EQUIP_PRODUCT_INFO,		"Product_info.tab" },		// װ���ϳɸ����
		{ DBC_EQUIP_MATERIAL_INFO,		"MaterialLevel_info.tab"},  // ���ϵȼ���

		{ DBC_MOUNT_ITEM_DATA,			"qicheng_daoju.tab"		},

		{ DBC_EQUIP_CARD_INFO,			"pai_info.tab"			},	// ÿ������Ϣ��
		{ DBC_EQUIP_CARD_GROUP_INFO,	"chain_info.tab"		},	// װ�����С������Ϣ��
		{ DBC_EQUIP_CARD_ALL,			"soul_info.tab"			},	// װ�����󼤻���Ϣ��
		{ DBC_EQUIP_SET_INFO,			"equipset_attr.tab"		},	// ��װ��Ϣ��
		{ DBC_EQUIP_CARD_TARGET,		"ChangePai_info.tab"	},	// ���Ʊ��
		{ DBC_MOUNT_BASE_DATA,			"qichengjichu.tab"		},	// ���������

		{ DBC_TITLE,					"title_info.tab"		},	// �ƺ�

		{ DBC_DAY_TIPS,					"DayTips.tab"			},	// ÿ�����ѹ��ܱ�
		{ DBC_TOP_LIST,					"TopList.tab"			},	// ���а��ܱ�
		{ DBC_QUEST_LOG,				"questlog.tab"			},	// ��������ܱ�
		{ DBC_MODEL_PARAMETER,			"Model_Parameter.tab"	},	//ģ�͵ļ�������
		{ DBC_SOUXIA_DATA,				"souxia.tab"			},

		{ DBC_MAPPIC_DATA,				"Mappic.tab"			},	// ������תͼƬ��	  20100517 AddCode
		{ DBC_CHATHELP_DATA,			"Chathelp.tab"			},	// ������ת������ʾ�� 20100517 AddCode
		{ DBC_IMPACT_SOUXIAMODEL,   	"impact_SouxiaModel.tab"},
		{ DBC_BUS_DATA,                 "BusInfo.tab"           },
		{ DBC_ITEM_MOUNT_ATT,           "mount_attr.tab"        },  // �������Ա� [7/6/2010 �¾���] 
		{ DBC_IB_SHOP,								"ibshop.tab"},	//��ȡIBShop

	};

	
	// ���������ݿ�
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
		// ����ַ�����id��Ҫ����ת��
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
