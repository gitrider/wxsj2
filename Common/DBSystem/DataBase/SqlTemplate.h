#ifndef _SQL_TEMPLATE_H_
#define _SQL_TEMPLATE_H_
#include "Type.h"


#define CHAR_TABLE		"t_char"
#define CHAR_EXTRATABLE	"t_charextra"
#define ITEM_TABLE		"t_iteminfo"
#define SKILL_TABLE		"t_skill"
#define XINFA_TABLE		"t_xinfa"
#define ABILITY_TABLE	"t_ability"
#define TASK_TABLE		"t_mission"
#define RELATION_TABLE	"t_relation"
#define IMPACT_TABLE	"t_impact"
#define PET_TABLE		"t_pet"
#define ISERIAL_TABLE	"t_itemkey"
#define MAIL_TABLE		"t_mail"
#define GUILD_TABLE		"t_guild"
#define GUILDUSER_TABLE	"t_guilduser"
#define SHOP_TABLE		"t_pshop"
#define PETITEM_TABLE	"t_petcreate"
#define CITYINFO_TABLE	"t_city"
#define CRC32_TABLE		"t_crc32"
#define GLOBALDATA_TABLE	"t_global"
#define CSHOP_TABLE		"t_cshop"
#define CSHOPITEM_TABLE	"t_cshopitem"
#define CHAR_NAMETABLE	"t_charname"

#define CHAR_SOUXIATABLE	"t_souxia" //add by gh for souxia 2010/06/07

extern CHAR* LoadCharList;

extern CHAR* LoadCharEquipList;

extern CHAR* LoadCharFullData;
extern CHAR* LoadChardbVersion;
extern CHAR* LoadCharExtradbVersion;

extern CHAR* LoadCharItemList;
extern CHAR* LoadDupItemList;
extern CHAR* DeleteCharItemSingle;
extern CHAR* DeleteDupItem;
extern CHAR* NewCharItem;
extern CHAR* DeleteCharItemList;

extern CHAR* LoadCharSkillList;
extern CHAR* DeleteCharSkillList;
extern CHAR* NewCharSkill;

extern CHAR* LoadCharXinFaList;
extern CHAR* DeleteCharXinFaList;
extern CHAR* NewCharXinFa;

extern CHAR* LoadCharAbilityList;
extern CHAR* LoadCharPresList;
extern CHAR* DeleteCharAbilityList;
extern CHAR* NewCharAbility;
extern CHAR* UpdateCharPresList;


extern CHAR* LoadCharTaskDataAndFlag;
extern CHAR* LoadCharTaskList;
extern CHAR* DeleteCharTaskList;
extern CHAR* NewCharTask;
extern CHAR* UpdateCharTaskData;
extern CHAR* UpdateCharTaskFlag;


extern CHAR* LoadCharRelationList;
extern CHAR* LoadCharRelationData;
extern CHAR* DeleteCharRelationList;
extern CHAR* NewCharRelation;
extern CHAR* UpdateCharRelationData;


extern CHAR* LoadCharImpactList;
extern CHAR* DeleteCharImpactList;
extern CHAR* NewCharImpact;

extern CHAR* LoadCharPetList;
extern CHAR* DeleteCharPetList;
extern CHAR* NewCharPet;

extern CHAR* LoadCharPrivateInfo;
extern CHAR* UpdateCharPrivateInfo;

extern CHAR* LoadCharTitleInfo;
extern CHAR* UpdateCharTitleInfo;

extern CHAR* LoadCharCoolDownInfo;
extern CHAR* UpdateCharCoolDownInfo;

extern CHAR* LoadCharPvPInfo;
extern CHAR* UpdateCharPvPInfo;

extern CHAR* LoadCharFatigueInfo;
extern CHAR* UpdateCharFatigueInfo;

extern CHAR* LoadCharVisualGem;
extern CHAR* UpdateCharVisualGem;

extern CHAR* MigNewChar;
extern CHAR* MigLoadChar;
extern CHAR* MigFinishChar;
extern CHAR* MigCheckCharName;
extern CHAR* UpdateCharName;

extern CHAR* CheckCharName;
extern CHAR* CreateNewChar;
extern CHAR* CreateChar;
extern CHAR* FetchNewGuid;
extern CHAR* FetchSaveTime;
extern CHAR* UpdateSaveTime;

extern CHAR* DeleteBaseChar;
extern CHAR* UpdateCharFullData;
extern CHAR* DeleteCharRequest;
extern CHAR* ClearDeltime;

extern CHAR* LoadServerItemSerial;
extern CHAR* UpdateServerItemSerial;

extern CHAR* LoadWorldMailInfo;
extern CHAR* SaveWorldMailInfo;
extern CHAR* DeleteWorldMailInfo;



extern CHAR* LoadShopInfo;
extern CHAR* LoadShopRecord;
extern CHAR* LoadShopStall;
extern CHAR* LoadShopBuyInfo;

extern CHAR* SaveShopInfo;
extern CHAR* SaveShopDataVersion;
extern CHAR* SaveShopRecord;
extern CHAR* SaveShopStall;
extern CHAR* SaveShopBuyInfo;

extern CHAR* DeleteShopInfo;


extern CHAR* LoadWorldGuildInfo;
extern CHAR* SaveWorldGuildInfo;
extern CHAR* DeleteWorldGuildInfo;

extern CHAR* LoadWorldGuildExtraInfo;
extern CHAR* SaveWorldGuildExtraInfo;
extern CHAR* ClearWorldGuildExtraInfo;

extern CHAR* LoadPetItemInfo;
extern CHAR* SavePetItemInfo;
extern CHAR* SavePetItemDataVersion;

extern CHAR* LoadCityInfo;
extern CHAR* SaveCityInfo;

extern CHAR* LoadCrcInfo;
extern CHAR* SaveCrcInfo;
extern CHAR* DeleteCrcInfo;
extern CHAR* ChangeCrcInfo;

extern CHAR* LoadGlobalData;
extern CHAR* SaveGlobalData;

extern CHAR* LoadCommitShop;
extern CHAR* LoadCommitShopItem;
extern CHAR* SaveCommitShop;
extern CHAR* SaveCommitShopItem;
extern CHAR* DeleteCommitShopItem;

extern CHAR* SaveCharExtra;
extern CHAR* LoadCharExtra;
extern CHAR* DeleteCharExtra;

extern CHAR* NewCharSouXia; //add by gh 2010/06/07
extern CHAR* LoadCharSouXiaList; 
extern CHAR* UpdateCharSouXiaData; 
extern CHAR* DeleteCharSouXia; 

#endif