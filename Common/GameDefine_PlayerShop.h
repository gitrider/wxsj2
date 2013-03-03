/********************************************************************************
 *	文件名：	GameDefine_PlayerShop.h
 *	全路径：	d:\Prj\Common\GameDefine_PlayerShop.h
 *	创建人：	孙雨
 *	创建时间：	2006-5-8
 *
 *	功能说明：	玩家商店客户端服务器公用的定义
 *	修改记录：
*********************************************************************************/

#ifndef __GAMEDEFINE_PLAYERSHOP_H__
#define __GAMEDEFINE_PLAYERSHOP_H__

namespace PLAYER_SHOP
{
	enum STATUS_STALL
	{
		STALL_INVALID = 0,					//此柜台还没有开放给玩家使用
		STALL_CLOSE,						//此柜台已经打烊
		STALL_OPEN,							//此柜台已经开张
	};
}

#define MAX_DY_GROUP_PRICE_NUM            10      //动态商店组物品价格种类上限
#define MAX_DY_PER_GROUP_GOODS_NUM        10      //动态商店每组贩卖物品上限

enum    DY_PRICE_TYPE                       //动态商店价格类型
{
    PRICE_GOLD = 0,                         //游戏币
    PRICE_CREDIT,                           //声望
    PRICE_HONOR,                            //荣誉
    PRICE_GP,                               //帮贡

    PRICE_ATTRIB_STR,					    //力量 力量
    PRICE_ATTRIB_CON,					    //体制 体制
    PRICE_ATTRIB_INT,					    //定力 智力
    PRICE_ATTRIB_DEX,					    //身法 敏捷
    PRICE_ATTRIB_POINT_REMAIN,			    //剩余点数
    PRICE_ATTRIB_SKILLPOINT_REMAIN,         //剩余技能点

    PRICE_ATTRIB_ATT_NEAR,                  //近程攻击
    PRICE_ATTRIB_ATT_FAR,                   //远程攻击
    PRICE_ATTRIB_DEF_NEAR,                  //近程防御
    PRICE_ATTRIB_DEF_FAR,                   //远程防御
    PRICE_ATTRIB_ATT_MAGIC_NEAR,			//近程内功攻击力
    PRICE_ATTRIB_DEF_MAGIC_NEAR,			//近程内功防御力
    PRICE_ATTRIB_MAXHP,				        //最大生命值
    PRICE_ATTRIB_MAXMP,				        //最大魔法值

    PRICE_RMB,                              //人民币
    PRICE_TYPE_NUM,
};

#endif // __GAMEDEFINE_PLAYERSHOP_H__
