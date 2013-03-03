
#pragma once


#include "GIWorldSystem.h"
#include "Zone.h"
#include "Region.h"
#include "GameDefine.h"
#include ".\buildingcollisionmng.h"


struct _DBC_ENV_SOUND;//������Ч
class tSoundSource;
struct _DBC_CITY_DEFINE;
struct _DBC_CITY_BUILDING;
//class NeSDK;

namespace PathLib
{
	class PathFinder;
	class PathData;
	class PathLibRoot;
	class BuildingCollision;
}

class CScene : public tScene
{
public:

	enum  { SIZE_OF_ZONE = 10  };	//Zone�Ĵ�С


	//ȡ�ó�������
	virtual const _DBC_SCENE_DEFINE*	GetSceneDefine(VOID)const	{ return m_pTheDefine; }
	//����λ���Ƿ�Ϸ�
	virtual BOOL	IsValidPosition(const fVector2& fvPos);
	//�����߼�����ȡ�õ��θ߶ȣ����������棬�����������Ƿ�������FALSE
	virtual BOOL	GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight);
	//
	//���ز�����������Region����
	//  1. �Ӷ������ļ�szRegionFile�ж�ȡ����Region���壬�����浽m_theRegion�ṹ��
	//  2. ����ÿ��Region��λ�ã�����ע�ᵽ��Ӧ��CZone��
	//  3. �������������ֱ���׳��쳣
	//  ��һ�������Ķ����ư汾.
	VOID			LoadRegionBinary(LPCTSTR szRegionFile, LPCTSTR szBuildingFile);

	//void			loadPOVBinary(LPCTSTR szPOVFile);

	//�õ�ָ���������λ�÷Ƿ�������NULL
	CZone*			GetZone(INT nXPos, INT nZPos);
	//��������X����õ����ڵ�Zone��x����
	INT				GetZoneX(FLOAT fx)const { return (INT)(fx/SIZE_OF_ZONE); }
	//��������Z����õ����ڵ�Zone��z����
	INT				GetZoneZ(FLOAT fz)const { return (INT)(fz/SIZE_OF_ZONE); }

	//�õ���С
	INT				GetSizeX(VOID) const { return m_nZoneXSize; }
	INT				GetSizeZ(VOID) const { return m_nZoneZSize; }


	//���Ʊ�������
	VOID			ControlBkgSound(BOOL bEnable);
	VOID			ControlBkgSoundVol();
	//���ƻ�����
	VOID			ControlEnvSound(BOOL bEnable);
	VOID			ControlEnvSoundVol();

	bool			collisionRay(const fRay& ray, fVector3& fvHitPosition);

	/***************************************************
		��ҳ������
	****************************************************/
	bool IsUserCity(void) const { return m_bUserCity; }
	//��ҳ�������
	bool UpdateUserCity(int nNewLevel);
	//���½�����
	bool UpdateCityBuilding(int nBuildingID, int nBuildingLevel);

public:
	//������ʼ�������ص��Σ���̬��Ʒ��
	virtual VOID	Initial(VOID);
	virtual VOID	EnterScene(VOID);
	virtual VOID	LeaveScene(VOID);
	virtual VOID	Tick(VOID);

public:
	CScene(const _DBC_SCENE_DEFINE* pSceneDef, bool bUserCity);
	virtual ~CScene();

protected:
	//�����е�Regionע�ᵽZone
	VOID						RegisteAllRegion(VOID);
	BOOL						GlobalCheckCross(fVector2 fvStart,fVector2 fvEnd);
	//������Ч
	BOOL						LoadEnvSound(VOID);
	//�������л�����Ч
	void						DestroyEnvSound(void);

protected:
	//��������ṹ, ���ļ��ж�ȡ
	const _DBC_SCENE_DEFINE* 	m_pTheDefine;

	//Zone����
	INT							m_nZoneXSize;
	INT							m_nZoneZSize;
	std::vector< CZone >		m_theZoneBuf;	// ��СΪ m_nZoneXSize*m_nZoneZSize

	//��������
	tSoundSource*				m_pBackSound;

	//������Ч�б�
	struct EnvSound_t
	{
		INT				nID;
		INT				nSoundID;
		UINT			nPosx;
		UINT			nPosz;
		UINT			nDistance;
		tSoundSource*	pSoundSource;
	};

	std::vector<EnvSound_t>		m_pEnvSoundList;	

	/***************************************************
		��ҳ������
	****************************************************/
	bool m_bUserCity;	//�Ƿ�����ҳ���

	//������
	struct BUILDING
	{
		const _DBC_CITY_BUILDING*	pDefine;	//��Դ����
		INT							nLevel;		//��ǰ�ȼ�
		INT							nObjID;		//����ʱʵ��ID
	};

	//��ǰ�����н�����(��Դ����idΪ����)
	typedef std::map< INT, BUILDING >	BUILDING_REGISTER;
	BUILDING_REGISTER				m_allBuilding;	//���н�����

public:
	// �˺������Ż�
	const CRegion&	GetRegion(INT uid);

	//Region����	
	std::vector< CRegion* >		m_theRegion;
	//// ���������������
	//CBuildingCollisionMng		m_WalkCollisionMng;
	
	//NeSDK*						getNeSDK() const;

	//NeSDK*						m_neSDK;


protected:
	void	drawAllLinesBetweenPOV();

public:
	PathLib::PathLibRoot* mPathLibRoot;
	PathLib::PathData* mPathData;
	PathLib::PathFinder* mPathFinder;
	// ���������������
	PathLib::BuildingCollision*	m_buildingCollsion;
};