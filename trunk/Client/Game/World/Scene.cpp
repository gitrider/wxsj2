
#include "StdAfx.h"
#include "WorldManager.h"
#include "Scene.h"
#include "Global.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "GIException.h"
#include "GIDBC_Struct.h"
#include "GIDebuger.h"
#include "Sound\GMSoundSystem.h"
#include "dbc\GMDataBase.h"
#include "WXSJ_DBC.h"
#include "GIUtil.h"
#include "Variable\GMVariable.h"
#include "GIResourceProvider.h"
#include "Object\Logic\Obj_Building.h"
#include "Region.h"
#include "Path.h"
#include "DynamicLines.h"
#include "OgreSceneNode.h"
#include "Engine\EngineInterface.h"
#include "Core\FairySystem.h"
#include "OgreNode.h"
#include "OgreMaterial.h"
//#include "Ne.h"
//#include "NeSDK.h"
//#include "NeSpace.h"
//#include "NeRegion.h"

#undef min
#undef max
#include "PathLibInterface.h"

// 行走面文件版本
#define		REGION_EDIT_VER1	"REGION_EDIT_01"
//#define		DRAW_PATH_LINE


CScene::CScene(const _DBC_SCENE_DEFINE* pDefScene, bool bUserCity)
{
	KLAssert(pDefScene);

	m_pTheDefine = pDefScene;
	m_pBackSound = NULL;
	m_bUserCity	 = bUserCity;

	mPathData = 0;
	mPathFinder = 0;
	mPathLibRoot = 0;
	m_buildingCollsion = 0;

	m_pEnvSoundList.resize(0);
}

CScene::~CScene()
{
	for (int i = 0;i < (int)m_theRegion.size();i ++)
	{
		delete m_theRegion[i];
		m_theRegion[i] = NULL;
	}

	mPathLibRoot->destroyPathData(mPathData);
	mPathLibRoot->destroyPathFinder(mPathFinder);
	mPathLibRoot->destroyBuildingCollision(m_buildingCollsion);

	delete mPathLibRoot;
}


VOID CScene::Initial(VOID)
{
	// 创建网格数据
	const _DBC_SCENE_DEFINE* pSceneDefine = GetSceneDefine();

	// 预先分配好内存
	m_nZoneXSize = pSceneDefine->nXSize / SIZE_OF_ZONE;
	if(m_nZoneXSize*SIZE_OF_ZONE < pSceneDefine->nXSize) 
		m_nZoneXSize++;

	m_nZoneZSize = pSceneDefine->nZSize / SIZE_OF_ZONE;
	if(m_nZoneZSize*SIZE_OF_ZONE < pSceneDefine->nZSize) 
		m_nZoneZSize++;

	m_theZoneBuf.resize(m_nZoneXSize * m_nZoneZSize);

	// 设置网格数据
	for(INT i=0; i<(INT)m_theZoneBuf.size(); i++)
	{
		CZone& theZone = m_theZoneBuf[i];

		// 网格初始化
		theZone.Initial();
	}

	//m_neSDK = NeCreateSDK();

	//m_neSDK->createSpace();

	// 加载Region
	LoadRegionBinary(m_pTheDefine->szRegionFile, m_pTheDefine->szCollisionfile);

	//loadPOVBinary(m_pTheDefine->szPOVFile);

	// 加载环境音效
	LoadEnvSound();

	// 加载玩家建筑物数据
	if(m_bUserCity)
	{
		// 查找本场景中所有可能会出现的玩家建筑物
		DBC_DEFINEHANDLE(s_pCityBuilding, DBC_CITY_BUILDING);

		unsigned int nLineNum = s_pCityBuilding->GetRecordsNum();
		for(unsigned int i=0; i<nLineNum; i++)
		{
			const _DBC_CITY_BUILDING* pDefine = (const _DBC_CITY_BUILDING*)s_pCityBuilding->Search_LineNum_EQU((int)i);
			// 是否是属于该城市的建筑物
			if(pDefine->nCityID != m_pTheDefine->nServerID) 
				continue;

			// 加入
			BUILDING newBuilding;
			newBuilding.pDefine = pDefine;
			newBuilding.nLevel = -1;
			newBuilding.nObjID = -1;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
			m_allBuilding.insert(std::make_pair(pDefine->nID, newBuilding));
		}
	}
}

bool CScene::UpdateUserCity(int nNewLevel) //const _DBC_SCENE_DEFINE* pDefScene)
{
	// 不是玩家城市
	if(!m_bUserCity)
		return false;

	// 查找城市场景
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pSceneDef = 0;
	for(int i=0; i<(int)s_pSceneDBC->GetRecordsNum(); i++)
	{
		const _DBC_SCENE_DEFINE* pTempSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_LineNum_EQU(i);

		// Id和等级相同
		if(pTempSceneDef->nServerID==m_pTheDefine->nServerID && pTempSceneDef->nCityLevel==nNewLevel)
		{
			pSceneDef = pTempSceneDef;
			break;
		}
	}

	if(!pSceneDef) 
		return false;

	// 同一个场景
	if(m_pTheDefine->nLocalID == pSceneDef->nLocalID) 
		return false;

	// 大小必须相同
	if(	pSceneDef->nXSize != m_pTheDefine->nXSize || pSceneDef->nZSize != m_pTheDefine->nZSize) 
		return false;

	// 设置网格数据
	for(INT i=0; i<(INT)m_theZoneBuf.size(); i++)
	{
		CZone& theZone = m_theZoneBuf[i];
		// 网格初始化
		theZone.Initial();
	}

	// 是否是同一个阻挡区
	if(_stricmp(m_pTheDefine->szRegionFile, pSceneDef->szRegionFile))
	{
		LoadRegionBinary(m_pTheDefine->szRegionFile, m_pTheDefine->szCollisionfile);
	}

	//// 是否是同一个行走面
	//if(_stricmp(m_pTheDefine->szCollisionfile, pSceneDef->szCollisionfile))
	//{
	//	m_WalkCollisionMng.ClearData();
	//	m_WalkCollisionMng.LoadCollisionTriInfoFromFile(m_pTheDefine->szCollisionfile);
	//}

	// 是否是同一个背景音乐
	if(m_pTheDefine->nBackSound != pSceneDef->nBackSound)
	{
		// 销毁背景音乐资源
		ControlBkgSound(FALSE);
	}

	// 加载场景资源
	m_pTheDefine = pSceneDef;
	EnterScene();

	// 环境音效
	LoadEnvSound();

	return true;
}

bool CScene::UpdateCityBuilding(int nBuildingID, int nBuildingLevel)
{
	// 不是玩家城市
	if(!m_bUserCity) 
		return false;

	// 该建筑物是否存在
	BUILDING_REGISTER::iterator itFind = m_allBuilding.find(nBuildingID);
	if(itFind == m_allBuilding.end()) 
		return false;

	BUILDING& theBuilding = itFind->second;

	// 等级相同
	if(theBuilding.nLevel == nBuildingLevel) 
		return true;

	// 如果尚未创建
	CObject_Building* pBuilding = (CObject_Building*)CObjectManager::GetMe()->FindObject(theBuilding.nObjID);
	if(!pBuilding)
	{
		pBuilding = (CObject_Building*)CObjectManager::GetMe()->NewObject("CObject_Building");
		if(!pBuilding) 
			return false;

		// 设置资源
		pBuilding->SetLogicData_Define(theBuilding.pDefine);

		// 保存
		theBuilding.nObjID = pBuilding->GetID();
	}

	// 设置等级
	pBuilding->SetLogicData_Level(nBuildingLevel);

	return true;
}

VOID CScene::EnterScene(VOID)
{
	//----------------
	// 在渲染层创建地形

	// 加载场景文件
	CGameProcedure::s_pGfxSystem->Scene_Load(m_pTheDefine->nLocalID, m_pTheDefine->szSceneName);

	// 预加载所有Npc资源
	char szPreLoadFile[MAX_PATH];
	strncpy(szPreLoadFile, m_pTheDefine->szSceneName, MAX_PATH);
	char* pSzExt = ::PathFindExtension(szPreLoadFile);
	if(pSzExt)
	{
		*pSzExt = 0;
		//***_monster.preload
		strncat(szPreLoadFile, "_monster.preload", MAX_PATH);

		CGameProcedure::s_pGfxSystem->Scene_PreLoad(szPreLoadFile);
	}

	// 播放背景音乐  , 使用系统保存的变量来控制是否播放	
	STRING strVar;
	CVariableSystem::GetMe()->GetVariable(CSoundSystemFMod::szBKSoundVar, strVar);
	if(strVar == "1")
	{
		ControlBkgSound(TRUE);
	}
}

VOID CScene::LeaveScene(VOID)
{
	// 销毁背景音乐资源
	ControlBkgSound(FALSE);
	// 销毁环境音效
	DestroyEnvSound();

	//-------------------
	// 销毁网格上的物体
	for(INT i=0; i<(INT)m_theZoneBuf.size(); i++)
	{
		CZone& theZone = m_theZoneBuf[i];

		register std::set< INT >::iterator it = theZone.m_setRegisterObject.begin();
		while(it != theZone.m_setRegisterObject.end())
		{
			// 销毁物体
			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(*it);
			if(pObj && !(pObj->GetFakeObjectFlag()))
			{
				pObj->Release();
				CObjectManager::GetMe()->DestroyObject( pObj );
			}

			it++;
		}
	}

	m_theZoneBuf.clear();
}

void CScene::DestroyEnvSound(void)
{
	for(int i=0; i<(int)m_pEnvSoundList.size(); i++)
	{
		EnvSound_t& snd = m_pEnvSoundList[i];

		if(snd.pSoundSource)
		{
			CSoundSystemFMod::GetMe()->Source_Destroy(snd.pSoundSource);
			snd.pSoundSource = 0;
		}
	}

	m_pEnvSoundList.clear();
}

// 场景位置是否合法
BOOL CScene::IsValidPosition(const fVector2& fvPos)
{
	if(fvPos.x < 0.0f || fvPos.x > (FLOAT)(m_pTheDefine->nXSize)) 
		return FALSE;
	if(fvPos.y < 0.0f || fvPos.y > (FLOAT)(m_pTheDefine->nZSize)) 
		return FALSE;

	return true;
}

VOID CScene::Tick(VOID)
{
	// 是否允许播放环境音
	if(CVariableSystem::GetMe()->GetAs_Int(CSoundSystemFMod::sz3DSoundVar) == 1)
	{
		if(!CObjectManager::GetMe()->GetMySelf()) return;

		// 环境音效的距离计算
		fVector2 vMyPos(CObjectManager::GetMe()->GetMySelf()->GetPosition().x, CObjectManager::GetMe()->GetMySelf()->GetPosition().z);
		FLOAT fDistance = 0.0;
		for(UINT i = 0; i<m_pEnvSoundList.size(); i++)
		{
			// 计算距离
			fVector2 vSoundPos((FLOAT)m_pEnvSoundList[i].nPosx, (FLOAT)m_pEnvSoundList[i].nPosz);
			fDistance = KLU_GetDist(vMyPos,vSoundPos);

			if(fDistance<=m_pEnvSoundList[i].nDistance)
			{
				// 添加音效
				if(m_pEnvSoundList[i].pSoundSource == NULL)
				{
					m_pEnvSoundList[i].pSoundSource =  CSoundSystemFMod::GetMe()->Source_Create(CSoundSourceFMod::type_circumstance, TRUE, TRUE, &m_pEnvSoundList[i].pSoundSource );
					if(m_pEnvSoundList[i].pSoundSource == 0)
						continue; 

					tSoundBuffer* pSndBuffer = CSoundSystemFMod::GetMe()->Buffer_Create(m_pEnvSoundList[i].nSoundID); 
					if(pSndBuffer)
					{
						m_pEnvSoundList[i].pSoundSource->SetBuffer(pSndBuffer);
						m_pEnvSoundList[i].pSoundSource->SetPos(fVector3(vSoundPos.x, 0, vSoundPos.y));
						m_pEnvSoundList[i].pSoundSource->SetLooping(TRUE);
						m_pEnvSoundList[i].pSoundSource->Play();
					}
				}
			}
			else if(m_pEnvSoundList[i].pSoundSource != NULL)
			{
				// 删除音效
				CSoundSystemFMod::GetMe()->Source_Destroy(m_pEnvSoundList[i].pSoundSource);
				m_pEnvSoundList[i].pSoundSource	= NULL;
			}
		}
	}
}

//控制背景音乐
VOID CScene::ControlBkgSound(BOOL bEnable)
{
	if(bEnable && !m_pBackSound)
	{
		// 生成背景音乐 
		m_pBackSound = CSoundSystemFMod::GetMe()->Source_Create( CSoundSourceFMod::type_background, FALSE, FALSE, &m_pBackSound );

		tSoundBuffer* pSndBuffer = CSoundSystemFMod::GetMe()->Buffer_Create(m_pTheDefine->nBackSound);
		if(pSndBuffer)
		{
			m_pBackSound->SetBuffer(pSndBuffer);
			m_pBackSound->SetLooping(TRUE);
			m_pBackSound->Play();
		}
	}
	else if(!bEnable && m_pBackSound)
	{
		CSoundSystemFMod::GetMe()->Source_Destroy(m_pBackSound);
		m_pBackSound = 0;
	}
}

//控制背景音乐音量
VOID CScene::ControlBkgSoundVol()
{
	if(m_pBackSound)
	{
		m_pBackSound->UpdateVolume();
	}
}

//控制环境音音量
VOID CScene::ControlEnvSoundVol()
{
	for(UINT i=0; i<m_pEnvSoundList.size(); i++)
	{
		if(m_pEnvSoundList[i].pSoundSource)
		{
			m_pEnvSoundList[i].pSoundSource->UpdateVolume();
		}
	}
}

//控制环境音
VOID CScene::ControlEnvSound(BOOL bEnable)
{
	if(bEnable)
	{
		LoadEnvSound();
	}
	else
	{
		for(UINT i=0; i<m_pEnvSoundList.size(); i++)
		{
			if(m_pEnvSoundList[i].pSoundSource)
			{
				//删除音效
				CSoundSystemFMod::GetMe()->Source_Destroy(m_pEnvSoundList[i].pSoundSource);
				m_pEnvSoundList[i].pSoundSource	= 0;
			}
		}
	}
}

VOID CScene::LoadRegionBinary(LPCTSTR szRegionFile, LPCTSTR szBuildingFile)
{
	if(!szRegionFile || szRegionFile[0] == '\0') return;

	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szRegionFile, lpAddress, "Scene");
	if( dwSize == 0 || lpAddress == 0 ) 
	{
		KLThrow("Can't load path file: %s", szRegionFile);
		return;
	}

	if (!mPathLibRoot)
		mPathLibRoot = new PathLib::PathLibRoot;

	if (!mPathData)
		mPathData = mPathLibRoot->getPathData();

	mPathData->importPathFile(lpAddress, dwSize);

	if (!mPathFinder)
		mPathFinder = mPathLibRoot->getPathFinder();

	mPathFinder->init(mPathData);

	CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );

	//加载行走面
	if(!m_buildingCollsion)
		m_buildingCollsion = mPathLibRoot->getBuildingCollsion();
	dwSize = CGameProcedure::s_pResourceProvider->loadResource(szBuildingFile, lpAddress, "Scene");
	if(dwSize == 0 || lpAddress == 0) 
	{
		return;
	}
	m_buildingCollsion->openSceneFile(lpAddress, dwSize);
	CGameProcedure::s_pResourceProvider->unloadResource(lpAddress, dwSize);

}

const CRegion& CScene::GetRegion(INT uid)
{
	const static CRegion pcr;
	std::vector<CRegion*>::iterator it;
	for(it = m_theRegion.begin();it!=m_theRegion.end();it++)
	{		
		if((*it)->m_ID == uid)
		{
			return *(*it);		
		}
	}

	return pcr;	
}

CZone* CScene::GetZone(INT nXPos, INT nZPos)
{
	// 物体位置非法
	if(	nXPos<0 || nXPos >= m_nZoneXSize || nZPos<0 || nZPos >= m_nZoneZSize )
	{
		//Warning.... 该物体定义的位置不在地图之内
		return NULL;
	}

	return &(m_theZoneBuf[ nZPos*m_nZoneXSize + nXPos ]);
}


// 环境音效
BOOL CScene::LoadEnvSound()
{
	// 只读一次
	if(m_pEnvSoundList.size() != 0)
	{
		// 销毁环境音效
		DestroyEnvSound();
	}

	// 计算名字
	STRING	strFileName = m_pTheDefine->szRegionFile;
	STRING::size_type nValidEnd = strFileName.find(".region", 0);
	if(nValidEnd == STRING::npos) 
	{
		nValidEnd = strFileName.find(".Region", 0);
		if(nValidEnd == STRING::npos)
			return FALSE;
	}
	STRING::size_type nFinalEnd	= strFileName.size();
	strFileName.replace(nValidEnd, nFinalEnd, ".esd");

	// 读入音效列表
	char* lpAddress = NULL;
	DBC::DBCFile* m_pEnvSoundDefine = new DBC::DBCFile(DBC_ENV_SOUND);
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(strFileName.c_str(), lpAddress, "Scene");
	if( dwSize > 0 )
	{
		BOOL bRet = m_pEnvSoundDefine->OpenFromMemory( lpAddress, lpAddress + dwSize + 1);
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
		if(!bRet)
		{
			delete m_pEnvSoundDefine;
			return TRUE;
		}
	}
	else
	{
		delete m_pEnvSoundDefine;
		return TRUE;
	}



	// 建立环境音效列表
 	m_pEnvSoundList.resize((INT)m_pEnvSoundDefine->GetRecordsNum());
	for(INT i=0; i<(INT)m_pEnvSoundDefine->GetRecordsNum(); i++)
	{
		_DBC_ENV_SOUND* pLine			= (_DBC_ENV_SOUND*)(m_pEnvSoundDefine)->Search_Posistion(i,0);
		m_pEnvSoundList[i].nID			= pLine->nID;
		m_pEnvSoundList[i].nSoundID		= pLine->nSoundID;	
		m_pEnvSoundList[i].nPosx		= pLine->nXPos;
		m_pEnvSoundList[i].nPosz		= pLine->nZPos;
		m_pEnvSoundList[i].nDistance	= pLine->nDistance;
		m_pEnvSoundList[i].pSoundSource	= 0;
	}

	delete m_pEnvSoundDefine;

	return TRUE;

}

BOOL CScene::GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight)
{
	// 检测坐标是否非法
	if(fvMapPos.x < 0 || fvMapPos.x > GetSceneDefine()->nXSize) return FALSE;
	if(fvMapPos.y < 0 || fvMapPos.y > GetSceneDefine()->nZSize) return FALSE;

	fVector3 fvTerrain;
	if(!CGameProcedure::s_pGfxSystem->Axis_Trans(tGfxSystem::AX_PLAN, 
			fVector3(fvMapPos.x, 0.0f, fvMapPos.y), tGfxSystem::AX_GAME, fvTerrain)) 
			return FALSE;

	FLOAT fBuildHeight;
	BOOL bOnBuild = m_buildingCollsion->getHeight(fvMapPos.x, fBuildHeight, fvMapPos.y);
	if(bOnBuild && fBuildHeight>=fvTerrain.y)
	{
		fHeight = fBuildHeight;
	}
	else
	{
		fHeight = fvTerrain.y;
	}
	return TRUE;

	return TRUE;
}



void CScene::drawAllLinesBetweenPOV()
{
#ifdef DRAW_PATH_LINE
	SceneNode* tmpLineNode = (SceneNode*)CEngineInterface::GetMe()->GetFairySystem()->getSceneManager()->
		getRootSceneNode()->getChild("LineNode");

	NeSpace* tmpSpace = m_neSDK->getSpace(0);
	for (int i = 0;i < tmpSpace->getNumRegions();i ++)
	{
		NeRegion* tmpRegion = tmpSpace->getRegion(i);

		//CRegion* tmpRegion = m_theRegion[i];

		for (int j = 0;j < tmpRegion->getNumPointsOfVisibility();j ++)
		{
			//CRegion::VisibilityPoints* tmpCurrentVP = &tmpRegion->m_vfPoints[j];// 获取场景所有区域的点 [12/4/2007 JiangWei]
			NeVisibilityPoints* tmpCurrentVP = tmpRegion->getPointOfVisibility(j);

			for (int m = 0;m < (int)tmpCurrentVP->m_visibilityPointsList.size();m ++)
			{
				const NeVisibilityPoints* tmpTargetPoint = tmpCurrentVP->m_visibilityPointsList[m];

				Line3D* tmpLine = new Line3D();
				tmpLine->addPoint(Ogre::Vector3(tmpCurrentVP->m_position.getX() * 100.0f,0.0f,tmpCurrentVP->m_position.getZ() * 100.0f));
				tmpLine->addPoint(Ogre::Vector3(tmpTargetPoint->m_position.getX() * 100.0f,0.0f,tmpTargetPoint->m_position.getZ() * 100.0f));
				tmpLine->drawLines();
				//if(1 == tmpRegion->getID())
					tmpLineNode->attachObject(tmpLine);
			}
		}
	}

	tmpLineNode->translate(-3200.0f,0.0f,-3200.0f,Ogre::Node::TS_WORLD);

#endif // DRAW_PATH_LINE
}

//NeSDK* CScene::getNeSDK() const
//{
//	return m_neSDK;
//}

//void CScene::loadPOVBinary( LPCTSTR szPOVFile )
//{
//	if(!szPOVFile || szPOVFile[0] == '\0') 
//		return;
//
//	NeSpace* tmpSpace = m_neSDK->getSpace(0);
//
//	char* lpAddress = NULL;
//	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szPOVFile, lpAddress, "Scene");
//	if(dwSize > 0)
//	{
//		char* pPoint = lpAddress;
//
//		for (int i = 0;i < tmpSpace->getNumRegions();i ++)
//		{
//			NeRegion* tmpRegion = tmpSpace->getRegion(i);
//
//			for (int j = 0;j < tmpRegion->getNumPointsOfVisibility();j ++)
//			{
//				NeVisibilityPoints* tmpCurrentPOV = tmpRegion->getPointOfVisibility(j);
//
//				int tmpNumLinkedPOV;
//				memcpy(&tmpNumLinkedPOV, pPoint, sizeof(int));
//				pPoint += sizeof(int);
//
//				for (int m = 0;m < tmpNumLinkedPOV;m ++)
//				{
//					int tmpBelongRegionID;
//					memcpy(&tmpBelongRegionID, pPoint, sizeof(int));
//					pPoint += sizeof(int);
//
//					int tmpLinkingPOVID;
//					memcpy(&tmpLinkingPOVID, pPoint, sizeof(int));
//					pPoint += sizeof(int);
//
//					NeRegion* tmpTargetRegion = tmpSpace->getRegionByID(tmpBelongRegionID);
//
//					const NeVisibilityPoints* tmpLinkingPOV = tmpTargetRegion->
//						getPointOfVisibility(tmpLinkingPOVID);
//
//					tmpCurrentPOV->m_visibilityPointsList.push_back(tmpLinkingPOV);
//				}
//
//			}
//		}
//
//		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
//	}
//
//	drawAllLinesBetweenPOV();
//
//}

bool CScene::collisionRay(const fRay& ray, fVector3& fvHitPosition)
{
	PathLib::ResultPoint3 ptHit(0, 0, 0);
	bool hit = m_buildingCollsion->collsionRay(
		PathLib::ResultPoint3(ray.mOrigin.x, ray.mOrigin.y, ray.mOrigin.z), 
		PathLib::ResultPoint3(ray.mDirection.x, ray.mDirection.y, ray.mDirection.z),
		ptHit);
	if(hit)
	{
		fvHitPosition = fVector3(ptHit.x, ptHit.y, ptHit.z);
	}

	return hit;
}