#include "stdafx.h"
#include "SqlTemplate.h"

// [8/1/2007]isvalid必须是1，避免移民工具转移一半的数据被不正确加载
CHAR*		LoadCharList		= 
"select \
charguid,sex,charname,level,haircolor,\
facecolor,hairmodel,facemodel,scene,menpai, \
headid, country \
from %s where accname =\'%s\' and isvalid =1";
/*
UINT		ItemType;
SHORT		Pos
*/
CHAR*		LoadCharEquipList	=
"select \
itemtype,pos,visualid,maxgemid \
from %s where charguid =%d and pos>=%d and pos<%d and isvalid =1";


/*
*	角色基本信息
*/
// [8/1/2007]isvalid必须是1，避免移民工具转移一半的数据被不正确加载
CHAR*		LoadCharFullData	=
"select \
firstenterscence,charguid,charname,title,sex,\
createtime,level,enegry,exp,vmoney,\
pw,haircolor,facecolor,hairmodel,facemodel,\
scene,xpos,zpos,logintime,logouttime,\
dbversion,camp,menpai,country,hp,\
mp,strikepoint,str,con,ipr,\
dex,points,settings,shopinfo,carrypet,\
guldid,teamid,headid,erecover,vigor,\
maxvigor,vrecover,energymax,pwdeltime,dietime,\
bankmoney,bankend,bkscene,bkxpos,bkzpos,\
rage \
from %s where charguid = %d and isvalid = 1";

/*
 *	角色数据库当前版本
 */
//必须根据isvalid的返回值来决定是否放弃存储[2007/09/10 YangJun]
CHAR*		LoadChardbVersion	=
"select dbversion,isvalid from %s where charguid = %d";

CHAR*		LoadCharExtradbVersion =
"select dbversion from %s where charguid = %d";


CHAR*		UpdateCharFullData	=	
"update %s  set \
firstenterscence=%d,  title='%s',     sex=%d,         level=%d,         enegry=%d, \
exp=%d,               vmoney=%d,      pw='%s',        haircolor=%d,     facecolor=%d, \
hairmodel=%d,         facemodel=%d,   scene=%d,       xpos=%d,          zpos=%d,	\
logintime=%d,         logouttime=%d,  dbversion=%d,   camp='%s',        menpai=%d, \
country=%d,           hp=%d,          mp=%d,          strikepoint=%d,   str=%d,	\
con=%d,               ipr=%d,         dex=%d,         points=%d,        settings='%s',\
shopinfo='%s',        carrypet='%s' , guldid=%d,      teamid =%d,       headid=%d,	\
erecover=%d,          vigor=%d,       maxvigor=%d,    vrecover=%d,      energymax=%d,	\
pwdeltime=%d,         dietime=%d,     bankmoney=%d,   bankend=%d,       bkscene=%d,	\
bkxpos=%d,            bkzpos=%d,      rage=%d	\
where charguid = %d and isvalid >0 and dbversion<=%d";

CHAR*		DeleteCharRequest = 
"update %s set isvalid = 0,deltime=0,charname = CONCAT(charname,'@DELETE_',%d) \
where charguid = %d";

CHAR*		ClearDeltime = 
"update %s set deltime=0,crc32=crc32-%u where charguid=%d";
/*
*	角色物品表加载
*/

CHAR*		LoadCharItemList	=
"select \
charguid,world,server,guid,itemtype,pos,fixattr,\
p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,\
p21,p22,p23,p24,p25,p26,p27,p28,p29,p30,p31,p32,p33,p34,p35,p36,p37,p38,p39,p40,\
creator,var \
from %s where charguid = %d and isvalid = 1 and dbversion=%d";

CHAR*		LoadDupItemList		=
"select charguid,pos,itemtype,dbversion from %s where guid=%d and world=%d and server=%d and isvalid>0";
/*
*	角色物品表删除
*/
CHAR*	   DeleteCharItemSingle  =
"update %s set isvalid=0,guid=0,world=0,server=0,dbversion=%d where charguid=%d and pos=%d and dbversion<=%d";

CHAR*		DeleteDupItem	   =
"update t_iteminfo set isvalid=0 where charguid=%d and pos=%d and dbversin<=%d";
/*
*	角色技能列表
*/

CHAR*	   LoadCharSkillList	=	
"select \
charguid,skid, state, level \
from %s where charguid = %d and dbversion=%d";

/*
*	 删除角色技能
*/
CHAR*	   DeleteCharSkillList = 
"delete \
from %s where charguid = %d and dbversion<=%d";


/*
*	角色心法列表
*/

CHAR*	LoadCharXinFaList	=
"select \
charguid,xinfaid,xinfalvl \
from %s where charguid = %d and dbversion=%d";

/*
*	删除角色心法
*/
CHAR*  DeleteCharXinFaList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
*	生活技能列表
*/

CHAR*	LoadCharAbilityList = 
"select \
charguid,abid,ablvl,abexp \
from %s where charguid = %d and dbversion=%d";

CHAR*	LoadCharPresList =
"select \
charguid,pres from %s where charguid = %d and dbversion=%d";



/*
*	删除生活技能列表
*/
CHAR* DeleteCharAbilityList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
*	任务列表
*/
CHAR*	LoadCharTaskList = 
"select \
charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8 \
from %s where charguid = %d and dbversion=%d";

/*
*	加载任务数据
*/
CHAR*	LoadCharTaskDataAndFlag = 
"select \
charguid,mdata,mflag \
from %s where charguid = %d and dbversion=%d";

/*
*	加载私人信息
*/
CHAR*	LoadCharPrivateInfo =
"select \
charguid,pinfo from %s where charguid=%d and dbversion=%d";

/*
*	加载称号信息
*/
CHAR*	LoadCharTitleInfo =
"select \
charguid,titleinfo from %s where charguid=%d and dbversion=%d";


/*
*	加载冷却信息
*/
CHAR*	LoadCharCoolDownInfo =
"select \
charguid,cooldown from %s where charguid=%d and dbversion=%d";


/*
*	加载PvP信息
*/
CHAR*	LoadCharPvPInfo =
"select \
charguid,pvpinfo from %s where charguid=%d and dbversion=%d";

/*
*	加载Fatigue信息
*/
CHAR*	LoadCharFatigueInfo =
"select \
charguid,fatigue from %s where charguid=%d and dbversion=%d";

/*
*	加载装备外形和宝石信息
*/
CHAR*	LoadCharVisualGem =
"select \
charguid,visualgem from %s where charguid=%d and dbversion=%d";

/*
*	删除任务列表
*/
CHAR* DeleteCharTaskList =
"delete from %s where charguid = %d and dbversion<=%d";

/*
*	关系列表
*/

CHAR*	LoadCharRelationList	=	
"select \
charguid,fguid,fname,fpoint,reflag,groupid,extdata \
from %s where charguid = %d and dbversion=%d";

// [8/1/2007]isvalid必须是1，避免移民工具转移一半的数据被不正确加载
CHAR*   LoadCharRelationData  =
"select \
charguid,relflag from %s where charguid = %d and isvalid =1 and dbversion=%d";

/*
*	关系列表删除
*/

CHAR* DeleteCharRelationList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
*	更新角色关系表
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR* UpdateCharRelationData	=
"update %s set relflag = \'%s\' where charguid = %d and isvalid>0 and dbversion=%d";


/*
*	更新配方表
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR* UpdateCharPresList	=
"update %s set pres = \'%s\' where charguid = %d and  isvalid>0 and dbversion = %d";

/*
*	Impact列表
*/

CHAR*  LoadCharImpactList	=
"select \
charguid,imdata \
from %s where charguid = %d and dbversion = %d";
/*
*	Impact列表删除
*/
CHAR* DeleteCharImpactList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
*	珍兽列表
*/

CHAR*	LoadCharPetList	=
"select \
charguid ,hpetguid,lpetguid,dataxid,petname,petnick,level,needlevel,atttype,aitype,camp,hp,mp,\
life,pettype,genera,enjoy,strper,conper,dexper,sprper,iprper,gengu,growrate,\
repoint,exp,str,con,dex,spr,ipr,skill,pwflag,pclvl,hspetguid,lspetguid,savvy,title,curtitle,us_unlock_time,us_reserve \
from %s where charguid =%d and dbversion=%d";
/*
*	珍兽列表删除
*/

CHAR*  DeleteCharPetList =
"delete \
from %s where charguid = %d and dbversion<=%d";

/*
*	移民相关
*/
CHAR*	MigNewChar =
"insert into %s(accname,charguid,charname,title,pw,sex,level,enegry,outlook,scene,xpos,zpos,menpai,\
hp,mp,strikepoint,str,con,dex,spr,ipr,points,logouttime,logintime,createtime,dbversion,haircolor,\
hairmodel,facecolor,facemodel,vmoney,settings,isvalid,exp,pres,\
shopinfo,carrypet,guldid,teamid,headid,erecover,vigor,maxvigor,vrecover,energymax,pwdeltime,\
pinfo,bkscene,bkxpos,bkzpos,titleinfo,dietime,bankmoney,bankend,cooldown,defeq) \
values(\'%s\',%d,\'%s\',\'%s\',\'%s\',\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,\'%s\',\
\'%s\',\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,\
\'%s\',%d,%d,%d,\'%s\',%d,%d,%d,\'%s\',%d)";

// [8/1/2007]isvalid必须是1，避免移民工具转移一半的数据被不正确加载
CHAR*	MigLoadChar	=
"select accname from %s where charguid=%d and isvalid=1";

// [8/1/2007]isvalid必须是1，避免移民工具转移一半的数据被不正确加载
CHAR*	MigFinishChar =
"update %s set isvalid=1 where charguid=%d";

CHAR*	MigCheckCharName =
"select charguid from %s where charname=\'%s\' limit 1";

//修改人物名称[2007/08/22 YangJun]
CHAR*	UpdateCharName =
"update %s set charname=\'%s\' where charguid=%d";

/*
*	创建新角色
*/
CHAR* CreateNewChar	=
"insert into %s(accname,charguid,charname,title,pw,sex,level,enegry,outlook,scene,xpos,zpos,menpai,\
hp,mp,strikepoint,str,con,dex,ipr,points,logouttime,logintime,createtime,dbversion,haircolor,\
hairmodel,facecolor,facemodel,vmoney,settings,isvalid,exp,pres,\
shopinfo,carrypet,guldid,teamid,headid,erecover,vigor,maxvigor,vrecover,energymax,pwdeltime,\
pinfo,bkscene,bkxpos,bkzpos,titleinfo,dietime,bankmoney,bankend,cooldown,defeq) \
values(\'%s\',%d,\'%s\',\'%s\',\'%s\',\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,\'%s\',\
\'%s\',\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,\
\'%s\',%d,%d,%d,\'%s\',%d,%d,%d,\'%s\',%d)";

CHAR* CreateChar =
"call create_newchar(\'%s\',\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d)";

CHAR* CheckCharName =
"call is_charname_ok(\'%s\')";

/*
*	获取新guid
*/
CHAR*	FetchNewGuid =
"call fetch_guid()";

/*
*	取得Savetime信息
*/
CHAR* FetchSaveTime =
"call fetch_savetime(%d)";

/*
*	修改SaveTime信息
*/
CHAR* UpdateSaveTime = 
"update t_char set savetime=0 where charguid=%d";

/*
*	删除角色
*/
CHAR*	DeleteBaseChar =
"call delete_char_new(\'%s\',\'%s\',%d,%d,%u)";
/*
*	创建物品
*/

CHAR* NewCharItem	=
"call save_iteminfo(%d,%d,%d,%d,%u,%d,%u,%d,\'%s\',\
%d,%d,%d,%d,%d,  %d,%d,%d,%d,%d,  %d,%d,%d,%d,%d,  %d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,  %d,%d,%d,%d,%d,  %d,%d,%d,%d,%d,  %d,%d,%d,%d,%d,\
%d,%d,\'%s\',\'%s\')";
//"insert into %s(charguid,world,server,guid,itemtype,pos,fixattr,\
//p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,isvalid,dbversion,creator,var) \
//values(%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',\'%s\')\n";

/*
*	创建技能
*/
CHAR*	NewCharSkill =
"insert into %s(charguid,skid,state,level,dbversion) \
values(%d,%d,%d,%d,%d) \n";


/*
*	创建心法
*/
CHAR*	NewCharXinFa =
"insert into %s(charguid,xinfaid,xinfalvl,dbversion) \
values(%d,%d,%d,%d)";


/*
*	创建生活技能
*/
CHAR*	NewCharAbility =
"insert into %s(charguid,abid,ablvl,abexp,dbversion) \
values(%d,%d,%d,%u,%d)";

/*
*	创建任务
*/
CHAR*	NewCharTask =
"insert into %s(charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8,dbversion) \
values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)";

/*
*	更新任务数据
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR*	UpdateCharTaskData =
"update \
%s  set mdata=\'%s\' \
where charguid = %d and isvalid>0 and dbversion=%d";

// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR*	UpdateCharTaskFlag =
"update \
%s set mflag=\'%s\' \
where charguid = %d and isvalid>0 and dbversion=%d";
/*
*	更新私人信息
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR*	UpdateCharPrivateInfo = 
"update \
%s set pinfo=\'%s\' where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	更新称号信息
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR*	UpdateCharTitleInfo = 
"update \
%s set titleinfo=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";


/*
*	更新冷却信息
*/
CHAR*	UpdateCharCoolDownInfo = 
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
"update \
%s set cooldown=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	更新PvP信息
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR*	UpdateCharPvPInfo = 
"update \
%s set pvpinfo=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	更新Fatigue信息
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR*	UpdateCharFatigueInfo = 
"update \
%s set fatigue=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	更新装备外形宝石信息
*/
// [8/1/2007]isvalid必须>0，移民工具(isvalid=31234)也要使用
CHAR*	UpdateCharVisualGem = 
"update \
%s set visualgem=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	创建关系信息
*/

CHAR*	NewCharRelation	 =
"insert into %s(charguid,fguid,fname,fpoint,reflag,groupid,extdata,dbversion) \
values(%d,%d,\'%s\',%d,%d,%d,\'%s\',%d)";

/*
*	创建Impact信息
*/
CHAR*	NewCharImpact	=
"insert into %s(charguid,imdata,dbversion) \
values(%d,\'%s\',%d)";

/*
*	创建珍兽信息
*/
CHAR* NewCharPet		=
"insert into %s	\
(charguid ,         hpetguid,             lpetguid,               dataxid,                petname, \
 petnick,           level,                needlevel,              atttype,                aitype, \
 camp,              hp,                   mp,                     life, \
 genera,            enjoy,                strper,                 conper,                 dexper, \
 iprper,            gengu,                growrate,               repoint, \
 exp,               str,                  con,                    dex, \
 ipr,               skill,                pwflag,                 pclvl,                  dbversion, \
 hspetguid,         lspetguid,            savvy) \
 values  \
 (%d,                %d,                   %d,                     %d,                     \'%s\', \
 \'%s\',             %d,                   %d,                     %d,                     %d, \
 \'%s\',             %d,                   %d,                     %d, \
 %d,                 %d,                   %d,                     %d, \
 %d,                 %d,                   %d,                     %d, \
 %d,                 %d,                   %d,                     %d,                     %d, \
 %d,                 \'%s\',               %d,                     %d,                     %d, \
 %d,                 %d,                   %d)";

/*
*	载入服务器物品序列号 
*/

CHAR* LoadServerItemSerial =
"select sid,serial from %s where sid = %u";

/*
*	更新服务器物品序列号
*/

CHAR* UpdateServerItemSerial = 
"update %s set serial = %u where sid = %u";

/*
 *	载入世界Email信息
 */
CHAR* LoadWorldMailInfo	=
"select pindex,sender,recer,mailinfo,mailcont from %s where isvalid=1";
 /*	保存Email信息
 */
CHAR*	SaveWorldMailInfo	=
"call save_mailinfo(\'%s\',\'%s\',\'%s\',\'%s\',%d,%d)";
/*
 *	删除世界Email信息
 */
CHAR*	DeleteWorldMailInfo	=
"update t_mail set isvalid = 0 where pindex=%d";
/*
*	载入世界工会信息
*/
CHAR*	LoadWorldGuildInfo	=
"select  guildid,guildname,guildstat,chiefguid,pcount,ucount,mucount,gpoint,guildmoney,cityid,\
time,logevity,contribu,honor,indlvl,agrlvl,comlvl,deflvl,techlvl,ambilvl,admin,guilddesc, \
chiefname,cname,glvl,guildmsg, guilduser \
from %s where isvalid=1";

CHAR*	LoadWorldGuildExtraInfo	=
"select  guildid,guildextra \
from %s where guildid=%d and isvalid=1";
/*
*	载入商店信息
*/
CHAR* LoadShopInfo	=	
"select shopguid,type,stat,maxbmoney,basemoney,profit,createtime,shopname,\
shopdesc,ownername,ownerguid,isopen,sale,saleprice,partner,partnum,subtype,dataversion  \
from %s where sid=%d and isvalid=1";
/*
*	载入商店纪录
*/
CHAR* LoadShopRecord = 
"select recoder from %s where sid=%d and isvalid=1 and poolid=%d";

/*
*	载入商店柜台
*/
CHAR* LoadShopStall =
"select stallinfo from %s where sid=%d and isvalid=1 and poolid=%d";

CHAR* LoadShopBuyInfo =
"select buyinfo from %s where sid=%d and isvalid=1 and poolid=%d";


/*
*	新建世界工会信息
*/
CHAR*	SaveWorldGuildInfo	=
"call save_guildinfo(%d,\'%s\',%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d,\
\'%s\',\'%s\',\'%s\',\'%s\',%d,\'%s\',%d,\'%s\')";

CHAR*	SaveWorldGuildExtraInfo	=
"update %s set guildextra = \'%s\' where guildid=%d and isvalid=1";

CHAR*	ClearWorldGuildExtraInfo	=
"update %s set guildextra = \'\' where guildid=%d";

/*
*	新建商店信息
*/
CHAR* SaveShopInfo		=	
"call save_shopinfo(%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,\'%s\',\
\'%s\',\'%s\',%d,%d,%d,%d,\'%s\',%d,%d,%d)";

CHAR* SaveShopDataVersion =
"update %s set dataversion=%d where sid=%d and poolid=%d";

/*
*	更新商店纪录
*/
CHAR*	SaveShopRecord	=
"update %s set recoder = \'%s\' where sid= %d and poolid=%d and isvalid=1 ";

/*
*	更新商店柜台
*/
CHAR*	SaveShopStall	=
"update %s set stallinfo = \'%s\' where sid= %d and poolid=%d and isvalid=1 ";

CHAR*	SaveShopBuyInfo	=
"update %s set buyinfo = \'%s\' where sid= %d and poolid=%d and isvalid=1 ";

/*
*	删除世界工会信息
*/
CHAR*	DeleteWorldGuildInfo	=
"update t_guild set isvalid = 0 where pindex=%d";

/*
*	载入珍兽对信息
*/
CHAR* LoadPetItemInfo	=	
"select pindex,dataversion,pairdata  \
from %s where isvalid=1";


/*
*	保存珍兽对信息
*/
CHAR*	SavePetItemInfo = 
"call save_petiteminfo(\'%s\',%d,%d)";

CHAR*	SavePetItemDataVersion = 
"update %s set dataversion=%d where pindex=%d";

/*
*	载入城市信息
*/
CHAR*	LoadCityInfo = 
"select poolid,citydata \
from %s where isvalid = 1";

/*
*	保存城市信息
*/
CHAR*	SaveCityInfo = 
"call save_cityinfo(\'%s\',%d,%d)";

/*
*	载入人物档案CRC32值
*/
CHAR*	LoadCrcInfo =
"select charguid,logouttime,crc32,fulldata,isdelete,server from %s \
where aid=%u";

/*
*	保存人物档案CRC32值
*/
CHAR*	SaveCrcInfo =
"insert into %s (charguid,logouttime,crc32,fulldata,isdelete,server) \
values(%d,%u,%d,\'%s\',%d,%d)";

/*
*	删除人物档案CRC32值
*/
CHAR*	DeleteCrcInfo =
"delete from %s where charguid=%d and isdelete>0";

/*
*	保留人物档案CRC32值
*/
CHAR*	ChangeCrcInfo =
"update %s set isdelete=0 where charguid=%d";

/*
*	载入World的Global数据值
*/
CHAR*	LoadGlobalData =
"select poolid,data1 from %s";

/*
*	保留World的Global数据值
*/
CHAR*	SaveGlobalData =
"call save_global(%d,%d)";

/*
*	保存人物档案扩展信息
*/
CHAR*	SaveCharExtra =
"call save_charextra(%d,%d,%d,%d,%d,%u,%u,%d,%d,%d)";

/*
*	加载人物档案扩展信息
*/
CHAR*	LoadCharExtra	=
"select charguid,buyyuanbao,kmcount,cmcount,sbmoney,sbunlock,sbstatus,ipregion,petnumextra from %s where charguid=%d and dbversion=%d";

/*
*	删除人物档案扩展信息
*/
CHAR*	DeleteCharExtra	=
"delete from %s where charguid=%d";

CHAR*	LoadCommitShop	=
"select poolid,cshopid from %s where worldid=%d and serverid=%d and isvalid>0";

CHAR*	LoadCommitShopItem	=
"select cshoppos,serial,charguid,charname,gtype,gvalue,svalue,ctime,costctime from %s \
where cshopid=%d and isvalid>0";

CHAR*	SaveCommitShop	=
"call save_cshop(%d,%d,%d,%d)";

CHAR*	SaveCommitShopItem	=
"call save_cshopitem(%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d)";

CHAR*	DeleteCommitShopItem =
"update %s set isvalid=0 where cshopid=%d and cshoppos=%d";



//add by gh for souxia 2010/06/07

CHAR* NewCharSouXia	=
"call save_souxiainfo(%d,%d,\'%s\',%d)";
// "insert into %s (charguid,curPos,data) \
// values(%d,%d,\'%s\')";

CHAR*		LoadCharSouXiaList	=
"select data \
from %s where charguid =%d and curPos>=%d and curPos<%d";

CHAR*		UpdateCharSouXiaData =
"update  %s \
set data=\'%s\' \
where charguid =%d and curPos=%d";

/*
*	 删除角色捜侠录
*/
CHAR*	   DeleteCharSouXia = 
"delete \
from %s where charguid = %d";