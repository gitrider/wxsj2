/********************************************************************************
 *	文件名：	GameDefine_City.h
 *	全路径：	d:\Prj\Common\GameDefine_City.h
 *	创建人：	孙雨
 *	创建时间：	2006-3-11
 *
 *	功能说明：	城市的定义
 *	修改记录：
*********************************************************************************/

#ifndef __GAMEDEFINE_CITY_H__
#define __GAMEDEFINE_CITY_H__


#define	MAX_CITY_PER_WORLD	200
#define MAX_CITY_PORT		5
#define MAX_CITY_BUILDING	1024
enum CITY_ERROR_TYPE
{
	CITY_ERROR_NOTHING				= 0,	// 没什么错误……
	CITY_ERROR_GUID_ON_WORKING,				//当前工会正在进行高危操作，不能进行城市操作
};

//!!32位以内
enum ENUM_UPDATE_CITY_ATT
{
	CITY_UPDATE_CITY_LEVEL = 0,
	CITY_UPDATE_CITY_NAME,
	CITY_UPDATE_BUILDING,


	CITY_UPDATE_MAX,
};


#endif // __GAMEDEFINE_CITY_H__
