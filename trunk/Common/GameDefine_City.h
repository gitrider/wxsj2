/********************************************************************************
 *	�ļ�����	GameDefine_City.h
 *	ȫ·����	d:\Prj\Common\GameDefine_City.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006-3-11
 *
 *	����˵����	���еĶ���
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMEDEFINE_CITY_H__
#define __GAMEDEFINE_CITY_H__


#define	MAX_CITY_PER_WORLD	200
#define MAX_CITY_PORT		5
#define MAX_CITY_BUILDING	1024
enum CITY_ERROR_TYPE
{
	CITY_ERROR_NOTHING				= 0,	// ûʲô���󡭡�
	CITY_ERROR_GUID_ON_WORKING,				//��ǰ�������ڽ��и�Σ���������ܽ��г��в���
};

//!!32λ����
enum ENUM_UPDATE_CITY_ATT
{
	CITY_UPDATE_CITY_LEVEL = 0,
	CITY_UPDATE_CITY_NAME,
	CITY_UPDATE_BUILDING,


	CITY_UPDATE_MAX,
};


#endif // __GAMEDEFINE_CITY_H__
