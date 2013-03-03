#include "stdafx.h"
#include "SqlTemplate.h"

// [8/1/2007]isvalid������1���������񹤾�ת��һ������ݱ�����ȷ����
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
*	��ɫ������Ϣ
*/
// [8/1/2007]isvalid������1���������񹤾�ת��һ������ݱ�����ȷ����
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
 *	��ɫ���ݿ⵱ǰ�汾
 */
//�������isvalid�ķ���ֵ�������Ƿ�����洢[2007/09/10 YangJun]
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
*	��ɫ��Ʒ�����
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
*	��ɫ��Ʒ��ɾ��
*/
CHAR*	   DeleteCharItemSingle  =
"update %s set isvalid=0,guid=0,world=0,server=0,dbversion=%d where charguid=%d and pos=%d and dbversion<=%d";

CHAR*		DeleteDupItem	   =
"update t_iteminfo set isvalid=0 where charguid=%d and pos=%d and dbversin<=%d";
/*
*	��ɫ�����б�
*/

CHAR*	   LoadCharSkillList	=	
"select \
charguid,skid, state, level \
from %s where charguid = %d and dbversion=%d";

/*
*	 ɾ����ɫ����
*/
CHAR*	   DeleteCharSkillList = 
"delete \
from %s where charguid = %d and dbversion<=%d";


/*
*	��ɫ�ķ��б�
*/

CHAR*	LoadCharXinFaList	=
"select \
charguid,xinfaid,xinfalvl \
from %s where charguid = %d and dbversion=%d";

/*
*	ɾ����ɫ�ķ�
*/
CHAR*  DeleteCharXinFaList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
*	������б�
*/

CHAR*	LoadCharAbilityList = 
"select \
charguid,abid,ablvl,abexp \
from %s where charguid = %d and dbversion=%d";

CHAR*	LoadCharPresList =
"select \
charguid,pres from %s where charguid = %d and dbversion=%d";



/*
*	ɾ��������б�
*/
CHAR* DeleteCharAbilityList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
*	�����б�
*/
CHAR*	LoadCharTaskList = 
"select \
charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8 \
from %s where charguid = %d and dbversion=%d";

/*
*	������������
*/
CHAR*	LoadCharTaskDataAndFlag = 
"select \
charguid,mdata,mflag \
from %s where charguid = %d and dbversion=%d";

/*
*	����˽����Ϣ
*/
CHAR*	LoadCharPrivateInfo =
"select \
charguid,pinfo from %s where charguid=%d and dbversion=%d";

/*
*	���سƺ���Ϣ
*/
CHAR*	LoadCharTitleInfo =
"select \
charguid,titleinfo from %s where charguid=%d and dbversion=%d";


/*
*	������ȴ��Ϣ
*/
CHAR*	LoadCharCoolDownInfo =
"select \
charguid,cooldown from %s where charguid=%d and dbversion=%d";


/*
*	����PvP��Ϣ
*/
CHAR*	LoadCharPvPInfo =
"select \
charguid,pvpinfo from %s where charguid=%d and dbversion=%d";

/*
*	����Fatigue��Ϣ
*/
CHAR*	LoadCharFatigueInfo =
"select \
charguid,fatigue from %s where charguid=%d and dbversion=%d";

/*
*	����װ�����κͱ�ʯ��Ϣ
*/
CHAR*	LoadCharVisualGem =
"select \
charguid,visualgem from %s where charguid=%d and dbversion=%d";

/*
*	ɾ�������б�
*/
CHAR* DeleteCharTaskList =
"delete from %s where charguid = %d and dbversion<=%d";

/*
*	��ϵ�б�
*/

CHAR*	LoadCharRelationList	=	
"select \
charguid,fguid,fname,fpoint,reflag,groupid,extdata \
from %s where charguid = %d and dbversion=%d";

// [8/1/2007]isvalid������1���������񹤾�ת��һ������ݱ�����ȷ����
CHAR*   LoadCharRelationData  =
"select \
charguid,relflag from %s where charguid = %d and isvalid =1 and dbversion=%d";

/*
*	��ϵ�б�ɾ��
*/

CHAR* DeleteCharRelationList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
*	���½�ɫ��ϵ��
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR* UpdateCharRelationData	=
"update %s set relflag = \'%s\' where charguid = %d and isvalid>0 and dbversion=%d";


/*
*	�����䷽��
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR* UpdateCharPresList	=
"update %s set pres = \'%s\' where charguid = %d and  isvalid>0 and dbversion = %d";

/*
*	Impact�б�
*/

CHAR*  LoadCharImpactList	=
"select \
charguid,imdata \
from %s where charguid = %d and dbversion = %d";
/*
*	Impact�б�ɾ��
*/
CHAR* DeleteCharImpactList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
*	�����б�
*/

CHAR*	LoadCharPetList	=
"select \
charguid ,hpetguid,lpetguid,dataxid,petname,petnick,level,needlevel,atttype,aitype,camp,hp,mp,\
life,pettype,genera,enjoy,strper,conper,dexper,sprper,iprper,gengu,growrate,\
repoint,exp,str,con,dex,spr,ipr,skill,pwflag,pclvl,hspetguid,lspetguid,savvy,title,curtitle,us_unlock_time,us_reserve \
from %s where charguid =%d and dbversion=%d";
/*
*	�����б�ɾ��
*/

CHAR*  DeleteCharPetList =
"delete \
from %s where charguid = %d and dbversion<=%d";

/*
*	�������
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

// [8/1/2007]isvalid������1���������񹤾�ת��һ������ݱ�����ȷ����
CHAR*	MigLoadChar	=
"select accname from %s where charguid=%d and isvalid=1";

// [8/1/2007]isvalid������1���������񹤾�ת��һ������ݱ�����ȷ����
CHAR*	MigFinishChar =
"update %s set isvalid=1 where charguid=%d";

CHAR*	MigCheckCharName =
"select charguid from %s where charname=\'%s\' limit 1";

//�޸���������[2007/08/22 YangJun]
CHAR*	UpdateCharName =
"update %s set charname=\'%s\' where charguid=%d";

/*
*	�����½�ɫ
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
*	��ȡ��guid
*/
CHAR*	FetchNewGuid =
"call fetch_guid()";

/*
*	ȡ��Savetime��Ϣ
*/
CHAR* FetchSaveTime =
"call fetch_savetime(%d)";

/*
*	�޸�SaveTime��Ϣ
*/
CHAR* UpdateSaveTime = 
"update t_char set savetime=0 where charguid=%d";

/*
*	ɾ����ɫ
*/
CHAR*	DeleteBaseChar =
"call delete_char_new(\'%s\',\'%s\',%d,%d,%u)";
/*
*	������Ʒ
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
*	��������
*/
CHAR*	NewCharSkill =
"insert into %s(charguid,skid,state,level,dbversion) \
values(%d,%d,%d,%d,%d) \n";


/*
*	�����ķ�
*/
CHAR*	NewCharXinFa =
"insert into %s(charguid,xinfaid,xinfalvl,dbversion) \
values(%d,%d,%d,%d)";


/*
*	���������
*/
CHAR*	NewCharAbility =
"insert into %s(charguid,abid,ablvl,abexp,dbversion) \
values(%d,%d,%d,%u,%d)";

/*
*	��������
*/
CHAR*	NewCharTask =
"insert into %s(charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8,dbversion) \
values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)";

/*
*	������������
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR*	UpdateCharTaskData =
"update \
%s  set mdata=\'%s\' \
where charguid = %d and isvalid>0 and dbversion=%d";

// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR*	UpdateCharTaskFlag =
"update \
%s set mflag=\'%s\' \
where charguid = %d and isvalid>0 and dbversion=%d";
/*
*	����˽����Ϣ
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR*	UpdateCharPrivateInfo = 
"update \
%s set pinfo=\'%s\' where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	���³ƺ���Ϣ
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR*	UpdateCharTitleInfo = 
"update \
%s set titleinfo=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";


/*
*	������ȴ��Ϣ
*/
CHAR*	UpdateCharCoolDownInfo = 
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
"update \
%s set cooldown=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	����PvP��Ϣ
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR*	UpdateCharPvPInfo = 
"update \
%s set pvpinfo=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	����Fatigue��Ϣ
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR*	UpdateCharFatigueInfo = 
"update \
%s set fatigue=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	����װ�����α�ʯ��Ϣ
*/
// [8/1/2007]isvalid����>0�����񹤾�(isvalid=31234)ҲҪʹ��
CHAR*	UpdateCharVisualGem = 
"update \
%s set visualgem=\'%s\'\
where charguid=%d and isvalid>0 and dbversion=%d";

/*
*	������ϵ��Ϣ
*/

CHAR*	NewCharRelation	 =
"insert into %s(charguid,fguid,fname,fpoint,reflag,groupid,extdata,dbversion) \
values(%d,%d,\'%s\',%d,%d,%d,\'%s\',%d)";

/*
*	����Impact��Ϣ
*/
CHAR*	NewCharImpact	=
"insert into %s(charguid,imdata,dbversion) \
values(%d,\'%s\',%d)";

/*
*	����������Ϣ
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
*	�����������Ʒ���к� 
*/

CHAR* LoadServerItemSerial =
"select sid,serial from %s where sid = %u";

/*
*	���·�������Ʒ���к�
*/

CHAR* UpdateServerItemSerial = 
"update %s set serial = %u where sid = %u";

/*
 *	��������Email��Ϣ
 */
CHAR* LoadWorldMailInfo	=
"select pindex,sender,recer,mailinfo,mailcont from %s where isvalid=1";
 /*	����Email��Ϣ
 */
CHAR*	SaveWorldMailInfo	=
"call save_mailinfo(\'%s\',\'%s\',\'%s\',\'%s\',%d,%d)";
/*
 *	ɾ������Email��Ϣ
 */
CHAR*	DeleteWorldMailInfo	=
"update t_mail set isvalid = 0 where pindex=%d";
/*
*	�������繤����Ϣ
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
*	�����̵���Ϣ
*/
CHAR* LoadShopInfo	=	
"select shopguid,type,stat,maxbmoney,basemoney,profit,createtime,shopname,\
shopdesc,ownername,ownerguid,isopen,sale,saleprice,partner,partnum,subtype,dataversion  \
from %s where sid=%d and isvalid=1";
/*
*	�����̵��¼
*/
CHAR* LoadShopRecord = 
"select recoder from %s where sid=%d and isvalid=1 and poolid=%d";

/*
*	�����̵��̨
*/
CHAR* LoadShopStall =
"select stallinfo from %s where sid=%d and isvalid=1 and poolid=%d";

CHAR* LoadShopBuyInfo =
"select buyinfo from %s where sid=%d and isvalid=1 and poolid=%d";


/*
*	�½����繤����Ϣ
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
*	�½��̵���Ϣ
*/
CHAR* SaveShopInfo		=	
"call save_shopinfo(%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,\'%s\',\
\'%s\',\'%s\',%d,%d,%d,%d,\'%s\',%d,%d,%d)";

CHAR* SaveShopDataVersion =
"update %s set dataversion=%d where sid=%d and poolid=%d";

/*
*	�����̵��¼
*/
CHAR*	SaveShopRecord	=
"update %s set recoder = \'%s\' where sid= %d and poolid=%d and isvalid=1 ";

/*
*	�����̵��̨
*/
CHAR*	SaveShopStall	=
"update %s set stallinfo = \'%s\' where sid= %d and poolid=%d and isvalid=1 ";

CHAR*	SaveShopBuyInfo	=
"update %s set buyinfo = \'%s\' where sid= %d and poolid=%d and isvalid=1 ";

/*
*	ɾ�����繤����Ϣ
*/
CHAR*	DeleteWorldGuildInfo	=
"update t_guild set isvalid = 0 where pindex=%d";

/*
*	�������޶���Ϣ
*/
CHAR* LoadPetItemInfo	=	
"select pindex,dataversion,pairdata  \
from %s where isvalid=1";


/*
*	�������޶���Ϣ
*/
CHAR*	SavePetItemInfo = 
"call save_petiteminfo(\'%s\',%d,%d)";

CHAR*	SavePetItemDataVersion = 
"update %s set dataversion=%d where pindex=%d";

/*
*	���������Ϣ
*/
CHAR*	LoadCityInfo = 
"select poolid,citydata \
from %s where isvalid = 1";

/*
*	���������Ϣ
*/
CHAR*	SaveCityInfo = 
"call save_cityinfo(\'%s\',%d,%d)";

/*
*	�������ﵵ��CRC32ֵ
*/
CHAR*	LoadCrcInfo =
"select charguid,logouttime,crc32,fulldata,isdelete,server from %s \
where aid=%u";

/*
*	�������ﵵ��CRC32ֵ
*/
CHAR*	SaveCrcInfo =
"insert into %s (charguid,logouttime,crc32,fulldata,isdelete,server) \
values(%d,%u,%d,\'%s\',%d,%d)";

/*
*	ɾ�����ﵵ��CRC32ֵ
*/
CHAR*	DeleteCrcInfo =
"delete from %s where charguid=%d and isdelete>0";

/*
*	�������ﵵ��CRC32ֵ
*/
CHAR*	ChangeCrcInfo =
"update %s set isdelete=0 where charguid=%d";

/*
*	����World��Global����ֵ
*/
CHAR*	LoadGlobalData =
"select poolid,data1 from %s";

/*
*	����World��Global����ֵ
*/
CHAR*	SaveGlobalData =
"call save_global(%d,%d)";

/*
*	�������ﵵ����չ��Ϣ
*/
CHAR*	SaveCharExtra =
"call save_charextra(%d,%d,%d,%d,%d,%u,%u,%d,%d,%d)";

/*
*	�������ﵵ����չ��Ϣ
*/
CHAR*	LoadCharExtra	=
"select charguid,buyyuanbao,kmcount,cmcount,sbmoney,sbunlock,sbstatus,ipregion,petnumextra from %s where charguid=%d and dbversion=%d";

/*
*	ɾ�����ﵵ����չ��Ϣ
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
*	 ɾ����ɫ����¼
*/
CHAR*	   DeleteCharSouXia = 
"delete \
from %s where charguid = %d";