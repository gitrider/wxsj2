/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   10:16
	filename: 	MapServer\Server\Obj\Horse\HorseStruct.h
	file path:	MapServer\Server\Obj\Horse
	file base:	HorseStruct
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef HorseStruct_h__
#define HorseStruct_h__

#include "Obj_Character.h"

//////////////////////////////////////////////////////
// 初始化Obj_Horse的结构
//////////////////////////////////////////////////////
struct _OBJ_HORSE_INIT :
	public _OBJ_CHARACTER_INIT
{
	GUID_t			m_GUID;			// 角色在场景中的GUID，与PetGUID是两回事
	_PET_DB			m_PetDB;
	ObjID_t			m_CreatorID;	// 所有者的ID，即宠物的主人
	INT				m_nIndex;		// 在主人宠物槽中的索引
	INT				m_RespawnTime;

	_OBJ_HORSE_INIT( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;
	}

	virtual VOID CleanUp( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;

		_OBJ_CHARACTER_INIT::CleanUp();
	}
};

//////////////////////////////////////////////////////
// 属性刷新的上一次属性的保存结构
//////////////////////////////////////////////////////
struct _HORSE_ATTR_BACKUP
{
public:
	BOOL		m_bNameModified;
	BOOL		m_bTitleModified;

	FLOAT       m_MoveSpeed;
	_CAMP_DATA	m_CampData;						//阵营		
	ObjID_t		m_nOwnerID;
	PET_GUID_t	m_SpouseGUID;					// 配偶GUID
	INT			m_nAIType;						// 性格
	INT			m_Level;						// 等级
	INT			m_iSex;							// 性别

	INT			m_nDataID;						// 数据ID
	INT			m_nModelID;						// 外形
	INT			m_nMountID;						// 座骑ID

	PET_GUID_t	m_GUID;							// ID
	BYTE		m_byGeneration;					// 几代宠
	BYTE		m_byHappiness;					// 快乐度

	INT			m_nExp;							// 经验
	//资质
	INT			m_iStrengthPerception;			// 力量资质
	INT			m_iSmartnessPerception;			// 敏捷资质
	INT 		m_iMindPerception;				// 智力资质
	INT 		m_iConstitutionPerception;		// 体质资质
	//养成属性
	INT			m_iSavvy;						// 悟性
	INT			m_GenGu;						// 根骨
	INT			m_iPullulationRate;				// 成长率

	INT			m_nRemainPoint;					// 一级属性剩余点数

	//一级战斗属性
	INT			m_iStrength;					// 力量
	INT			m_iSmartness;					// 敏捷
	INT 		m_iMind;						// 智力
	INT 		m_iConstitution;				// 体质

	//二级战斗属性
	INT			m_nAtt_Near;           
	INT			m_nAtt_Far;            

	INT			m_nAtt_MagicNear;		
	INT			m_nAtt_MagicFar;		

	INT			m_nDef_Near;           
	INT			m_nDef_Far;      

	INT			m_nDef_MagicNear;
	INT			m_nDef_MagicFar;

	INT			m_nHit;				
	INT			m_nMiss;
	INT			m_nCritical;

public:
	_HORSE_ATTR_BACKUP( VOID );

	VOID CleanUp( VOID );
};

#endif