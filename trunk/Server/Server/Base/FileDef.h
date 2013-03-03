/********************************************************************************
 *	文件名：	FileDef.h
 *	全路径：	d:\Prj\Server\Server\Base\FileDef.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 6 日	18:26
 *
 *	功能说明：	所有的配置文件的宏定义
 *	修改记录：
*********************************************************************************/

#ifndef __FILEDEF_H__
#define __FILEDEF_H__

#define FILE_SERVER_VERSION					"K:/成吉思汗/Versions/sn.txt"

//Config 目录

#define FILE_CONFIG_INFO					"./Config/ConfigInfo.ini"
#define FILE_LOGIN_INFO						"./Config/LoginInfo.ini"
#define	FILE_WORLD_INFO						"./Config/WorldInfo.ini"
#define FILE_SHARE_MEM_INFO					"./Config/ShareMemInfo.ini"
#define FILE_MACHINE_INFO					"./Config/MachineInfo.ini"
#define FILE_SERVER_INFO					"./Config/ServerInfo.ini"
#define FILE_SCENE_INFO						"./Config/scenedefines.tab"
#define FILE_BASE_VALUE						"./Config/BaseValueTable.ini"
#define FILE_ITEM_SERIAL					"./Config/ItemSerial.ini"

#define FILE_PET_CONFIG						"./Config/PetConfigTable.ini"
#define FILE_GUILD_MEMBER_ACCESS			"./Config/GuildMemberAccess.ini"
#define FILE_IPRANGE						"./Config/IPRange.ini"
#define FILE_GMLIST							"./Config/GMList.ini"

#define FILE_NPC_CHAT						"./Config/npc_speak.tab"
#define FILE_NPCPAOPAOINDEXTBL				"./Config/npc_pop_index.tab"
#define FILE_PET_ATTRPOINTDISTRIBUTE		"./Config/pet_sttr_point.tab"
#define FILE_PET_SKILLDISTRIBUTE			"./Config/pet_skl.tab"
#define FILE_PET_SKILLINDEXTBL				"./Config/pet_skl_index.tab"
#define FILE_PET_STUDYSKILLTBL				"./Config/pet_skl_study.tab"
#define FILE_PET_APPERCEIVESKILLTBL			"./Config/pet_observe_skl.tab"
#define FILE_PET_DOMESTICATION_MONEY		"./Config/pet_tame_money.tab"
#define	FILE_PET_AISTRATEGYTBL				"./Config/pet_ai.tab"
#define FILE_PET_TRAINCHARGE				"./Config/PetTrainCharge.txt"
#define FILE_PLANTS							"./Config/Plants.txt"
#define FILE_ATTR_LEVEL_UP					"./Config/attr_lvup.tab"
#define FILE_MONSTER_DROP_CONTENT			"./Config/monsterdrop_content.tab"
#define FILE_DROP_ATTENUATION				"./Config/drop_fadeout.tab"
#define FILE_QUESTION						"./Config/quiz.tab"
#define FILE_DIE_PENALTY					"./Config/death_penalty.tab"
#define FILE_CHAR_DEFAULT					"./Config/PlayerTemplate.tab"
#define FILE_MONSTER_SKILL					"./Config/mon_wqskl.tab"
#define FILE_DROP_POS						"./Config/dorp_pos.tab"
#define FILE_DUR_SPOIL						"./Config/DurSpoil.tab"

// Public 目录
#define FILE_STANDARD_IMPACT_DATA			"../Public/Config/std_impact.tab"
#define FILE_MONSTER_AI						"../Public/Config/MonsterAITable.ini"
#define FILE_PLAYER_EXP_LEVEL				"../Public/Config/pl_explv.tab"
#define FILE_MONSTER_ATTREX					"../Public/Config/mon_attr_ex.tab"
#define FILE_HORSE_ATTR						"../Public/Config/qichengjichu.tab"
#define FILE_HORSE_SKILL					"../Server/Config/qichengSkill.tab"
#define FILE_HORSE_ITEM						"../Public/Config/qicheng_Daoju.tab"
#define FILE_HORSE_DROP						"../Server/Config/qichengdrop_content.tab"
#define FILE_PET_ATTR						"../Public/Config/pet_attr.tab"
#define FILE_PET_LEVELUP					"../Public/Config/pet_lvup.tab"
#define FILE_GROWPOINT						"../Public/Config/grow_point.tab"
#define FILE_XINFA_STUDY_SPEND				"../Public/Config/xf_learnspend.tab"
#define FILE_XINFA							"../Public/Config/xf.tab"
#define FILE_SKILL_TEMPLATE					"../Public/Config/skl_template.tab"
#define FILE_SKILL_DATA						"../Public/Config/skl_data.tab"
#define FILE_SKILL_STUDY					"../Public/Config/skl_learn.tab"
#define FILE_SPECIAL_OBJ_DATA				"../Public/Config/sp_obj.tab"
#define FILE_IMPACT_SEDATA					"../Public/Config/impact_buff_eff.tab"
#define FILE_CAMP_AND_STAND					"../Public/Config/camp_stand.tab"
#define FILE_ID_COLLECTION					"../Public/Config/id_gather.tab"
#define FILE_ABILITY						"../Public/Config/ability.tab"
#define FILE_HAIRSTYLE						"../Public/Config/char_hair.tab"
#define FILE_PRESCRIPTION					"../Public/Config/item_mix.tab"
#define	FILE_ABILITY_EXP					"../Public/Config/ability_exp.tab"
#define	FILE_TITLE_INFO 					"../Public/Config/title_info.tab"
#define	FILE_TITLE_COMBINATION  			"../Public/Config/title_combination.tab"
#define DIR_ABILITYLEVELUP_CONFIG			"../public/config/AbilityLevelUp"
//-----------------------------------------------------------------------------------
#define FILE_EQUIP_BASE						"../Public/Config/equip_algo.tab"
#define FILE_ATT_SET_BASE					"./Config/attribute_set.tab"
#define FILE_ATT_LEVEL_BASE					"./Config/attribute_level.tab"
#define FILE_EQUIPEXTRAATT_INFO				"./Config/EquipExtraAtt_info.tab"
//-----------------------------------------------------------------------------------
#define FILE_GEM_INFO						"../Public/Config/jew_info.tab"
#define FILE_EUIPHOLE_INFO					"../Public/Config/Equip_HoleReqInfo.tab"
#define FILE_EUIPLEVEUP_INFO				"../Public/Config/Equip_levelup.tab"
#define FILE_EUIPBIND_INFO					"../Public/Config/Equip_bind.tab"
#define FILE_SPECIALREPAIR_INFO				"../Public/Config/Equip_SpecialRepair.tab"
#define FILE_PRODUCT_INFO					"../Public/Config/Product_info.tab"
#define FILE_MATERIALLEVEL_INFO				"../Public/Config/MaterialLevel_info.tab"

#define FILE_EQUIPPOINT_INFO				"../Public/Config/EquipPoint_Value.tab"
#define FILE_EXTRABAG_INFO					"../Public/Config/Extra_Bag.tab"
#define FILE_CHAIN_INFO						"../Public/Config/chain_info.tab"
#define FILE_SOUL_INFO						"../Public/Config/Soul_info.tab"
#define FILE_CHANGEPAI_INFO					"../Public/Config/ChangePai_info.tab"
#define FILE_PAI_INFO						"../Public/Config/pai_info.tab"

#define FILE_COMMON_ITEM					"../Public/Config/common.tab"
#define FILE_TASK_ITEM						"../Public/Config/TaskItem.tab"
#define FILE_ITEM_RULE						"../Public/Config/item_formula.tab"
#define FILE_STORE_MAP_ITEM					"../Public/Config/store_map.tab"
#define FILE_EQUIP_SET_ATTR					"../Public/Config/equipset_attr.tab"

#define FILE_SHOP							"../Public/Config/shop_tab.tab"
#define FILE_DY_SHOP						"../Public/Config/Dyshop_tab.tab"
#define FILE_PLAYER_LEVEL_MONEY				"../Public/Config/lv_maxmoney.tab"
#define FILE_CITY_SCENE						"../Public/Config/city_info.tab"
#define FILE_CHAT_CONSUME					"../Public/Config/chat_cfg.tab"
#define FILE_CITY_BUILDING					"../Public/Config/city_build.tab"

#define FILE_NPC_QUEST						"../Public/Config/obj_quest.tab" 
#define FILE_MOUNT_ATTR			     		"../Public/Config/mount_attr.tab"  // [7/8/2010 陈军龙]

//PK相关
#define FILE_PK_POINT						"../Server/Config/PK_Point.tab"
#define FILE_PK_LEVEL						"../Server/Config/PK_Level.tab"
#define FILE_PK_PUNISH						"../Server/Config/PK_Punish.tab"

//开宝箱相关
#define FILE_OPENBOX_ITEM					"../Server/Config/OpenBoxItem.tab"

//循环任务
#define FILE_XUNHUAN_MISSION				"../Public/Config/mission/xunhuan_shimen.tab"

//每日提示
#define FILE_DAY_TIPS						"../Public/Config/DayTips.tab"

//排行榜
#define FILE_TOP_LIST						"../Public/Config/TopList.tab"

//可跳转的地图信息							
#define FILE_MAP_INFO_LIST					"../Public/Config/WorldPos.tab"

//系统定时活动						
#define FILE_TIMER_DOING_LIST				"../Public/Config/Timer.tab"

#define FILE_GUILD_LEVEL	    			"../Public/Config/guild_level_info.tab"

#define FILE_SOUXIA_ITEM					"../Public/Config/souxia.tab"  // add by gh 2010/05/10
#define FILE_SOUL_BEAD_ITEM				"../Public/Config/soul_bead.tab" // add by gh 2010/06/25

#define FILE_BUSINFOTABLE					"../Public/Config/BusInfo.tab"

// 其他

#define FILE_AI_SCRIPT						"../Public/Data/AIScript.tab"
#define FILE_SCRIPT							"../Public/Data/Script.tab"

#define PATH_AI_SCRIPT						"../Public/Data/AIScript/"
#define PATH_SCENE							"./scene/"



#define GET_FULL_PATH(fullpath, filepath, filename)	do { \
		sprintf(fullpath, "%s%s", (filepath), (filename)); \
	} while (0)

#define GET_AI_SCRIPT_FULL_PATH(fullpath, filename) GET_FULL_PATH(fullpath, PATH_AI_SCRIPT, (filename))
#define GET_SCENE_FULL_PATH(fullpath, filename) GET_FULL_PATH(fullpath, PATH_SCENE, (filename))
#endif // __FILEDEF_H__