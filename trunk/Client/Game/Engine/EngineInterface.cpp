
/**	EngineInterface.cpp
 *	
 *	功能：	
 *	
 *	修改记录：
 *			
 *			
 */

#include "StdAfx.h"
#include "EngineInterface.h"
#include "Global.h"
#include "GIException.h"
#include "GIUtil.h"
#include "AxTrace.h"
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreRay.h>
#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreOverlayElement.h>

#include "Core/FairySystem.h"
#include "Core/TerrainData.h"
#include "Core/FrameStatsListener.h"
#include "Core/FairyObjectProxy.h"
#include "Camera/Camera_CharSel.h"
#include "Camera/Camera_Scene.h"
#include "Object/Entity/ObjEntityDummy.h"
#include "Object/Entity/ObjEntityStatic.h"
#include "Object/Entity/ObjEntityModel.h"
#include "Object/Entity/ObjEntityActor.h"
#include "EffectSystem/FairyBulletSystem.h"
#include "Debug/DebugPanel.h"
//#include "HelpPanel.h"
#include "OgreLogmanager.h"
#include "OgreCommon.h"
#include "Core/FairySceneInfo.h"
#include "Core/Terrain.h"
#include "Core/FairyResourceSolverHelper.h"
#include "Core/utf.h"
#include "Core/FairyResources.h"
#include "PostFilter/FairyPostFilters.h"
#include "PostFilter/FairyPostFilterManager.h"
#include "Resource/ResourceLoadListener.h"
#include "Core/FairyExpatParser.h"
#include "GIVariable.h"
//#include "TerrainGrid.h"
#include "GIKernel.h"
#include <OgreParticleSystemManager.h>
#include <OgreParticleSystem.h>
#include "GIProfile.h"
#include "GISound.h"
#include "ModelSystem/FairyLogicModel.h"
#include "Object/Manager/FakeObjectEntityManager.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyEnviromentObject.h"
#include <OgreMeshManager.h>
#include <OgreSkeletonManager.h>
#include <OgreMaterialManager.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreGpuProgramManager.h>
#include "GIWorldSystem.h"
#include "GIEventDefine.h"
#include "Core/FairyLogicModelObject.h"
#include "GITimeSystem.h"
#include "GIResourceProvider.h"
#include "ModelSystem/FairyLogicModelManager.h"
//#include "RakeSceneManager.h"
//#include <FairyScriptResGet.h>
#include "TerrainGrid.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "OgreControlSystemSetting.h"

#include "UIMapLight.h"


#include "Core/FairyStaticEntityObject.h"
#include "Core/FairyTerrainLiquidObject.h"
#include "Core/FairyEffectObject.h"
#include "Core/FairyTerrainLiquid.h"
#include "Core/FairyParticleSystemObject.h"
#include "Core/FairyLightObject.h"
#include "Core/FairyActorObject.h"
#include "Core/FairyModelObject.h"



#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>


struct G_AUTO_LOCALE_CONVERT
{
	G_AUTO_LOCALE_CONVERT()
	{
		std::locale langLocale("");
		old = std::locale::global(langLocale);
	}
	~G_AUTO_LOCALE_CONVERT()
	{
		std::locale::global(old);
	}

	std::locale old;
};


namespace Ogre
{

	extern "C"  void InstallD3D9Plugin(void);
	extern "C" void InstallParticleFXPlugin(void);
	extern "C" void InstallParticleFX2Plugin(void);
	extern "C"  void InstallOctreeScenePlugin(void);
	extern "C"  void InstallCgProgramManagerPlugin(void);
}


const CHAR CEngineInterface::sz32BitTexturesVar[]			= "View_ColorQuality";			// "yszl";	颜色质量，比特数(16/32位)
const CHAR CEngineInterface::szDefaultTextureFilteringVar[]	= "View_TextureFiltering";		// "cy";	采样(高/中/低)
const CHAR CEngineInterface::szLightmapQualityVar[]			= "View_LightmapQuality";		// "dxyy";	地形阴影(高/低/无)
const CHAR CEngineInterface::szAnimationQualityVar[]		= "View_ObjectMove";			// "wtdh";	静态物体动画(开/关)
const CHAR CEngineInterface::szPostFilterEnabledVar[]		= "View_FullScreenLight";		// "qpfg";	全屏泛光效果(开/关)
const CHAR CEngineInterface::szFSAAVar[]					= "View_Fanhunjiao";			// "fhj";	反混角(1/2/4)
const CHAR CEngineInterface::szShadowTechniqueVar[]			= "View_HumanLightmap";			// "rwyy";	人物阴影(开/关)
const CHAR CEngineInterface::szvsyncVar[]					= "View_Aplomb";				// "cztb";	垂直同步(开/关)



CEngineInterface* CEngineInterface::s_pMe = NULL;
KL_IMPLEMENT_DYNAMIC(CEngineInterface, GETCLASS(tGfxSystem));


CEngineInterface::CEngineInterface(VOID)
	: m_pFairySystem(NULL), 
	  m_pCamera_Current(NULL)
{
	s_pMe				=this;

	m_bDeathMode		=FALSE;
	m_nDeachModePercent =0;

	m_pCamera_Current	=0;
	m_pCamera_CharView	=0;
	m_pCamera_Scene		=0;
//	m_fParallelOffset	=0.0f;
//	m_fSaveParallelOffset=0.0f;
//	m_fOldParallelOffset=0.0f;
	m_fVerticalOffset	=0.0f;

	m_fFullClientWidth	= 0.0f;
	m_bFullWidth		= FALSE;
	m_fClientWidth		= 0.0f;
	m_fUnitGameToScene	= 0.0f;

	m_pRaySceneQuery	=0;
	m_pPostFilterManager=0;
	m_pEnviromentObject =0;
	m_pDebugPanel		=0;
	m_pFakeObjectManager=0;

	// 创建Fiary
	m_pFairySystem		=new Fairy::System;

	m_eOffsetTag		= OT_NONE;
	m_bLeftOffset		= FALSE;
	m_bRightOffset		= FALSE;

	m_pMapLight = NULL;

	m_pTerrainGrid = NULL;
}

CEngineInterface::~CEngineInterface(VOID)
{
}


VOID CEngineInterface::Release(VOID)
{
	if( m_pFakeObjectManager )
	{
		m_pFakeObjectManager->Release();
		delete m_pFakeObjectManager;
		m_pFakeObjectManager = NULL;
	}
	if(m_pCamera_CharView) 
	{
		delete m_pCamera_CharView;
		m_pCamera_CharView = NULL;
	}
	if(m_pCamera_Scene) 
	{
		delete m_pCamera_Scene;
		m_pCamera_Scene = NULL;
	}

	if(m_pDebugPanel)
	{
		delete m_pDebugPanel;
		m_pDebugPanel = NULL;
	}

	if(m_pTerrainGrid)
	{
		delete m_pTerrainGrid;
		m_pTerrainGrid = NULL;
	}

	if(m_pMapLight)
	{
		m_pMapLight->Release();
		delete m_pMapLight; m_pMapLight = 0;
	}

	if(m_pRaySceneQuery)
	{
		m_pFairySystem->getSceneManager()->destroyQuery(m_pRaySceneQuery);
		m_pRaySceneQuery = NULL;
	}

	if(m_pFairySystem)
	{
		delete m_pFairySystem;	
		m_pFairySystem = NULL;
	}
}

HWND CEngineInterface::GetWindow(VOID) const
{ 
	return g_hMainWnd; 
}

BOOL CEngineInterface::Axis_CheckValid(AXIS_TYPE typeSource, const fVector3& fvAxis)
{
	register Fairy::TerrainData* pTerrainData = m_pFairySystem->getTerrainData();
	//尚未加载
	if(pTerrainData && (pTerrainData->getXSize()==0 || pTerrainData->getZSize()==0))
	{
		pTerrainData = 0;
	}

	switch(typeSource)
	{
		//-----------------------------------------------
	case AX_GAME:
	case AX_PLAN:
		{
			if(pTerrainData)
				return pTerrainData->isValidGrid(std::make_pair((INT)fvAxis.x, (INT)fvAxis.z));
			else
				return TRUE;
		}
		break;

		//-----------------------------------------------
	case AX_GFX:
		{
			if(pTerrainData)
				return pTerrainData->isValidCoord(fvAxis.x, fvAxis.z);
			else
				return TRUE;
		}
		break;

		//-----------------------------------------------
	case AX_SCREEN:
		{
			RECT rect;
			GetClientRect(g_hMainWnd, &rect);

			POINT pt;
			pt.x = (INT)fvAxis.x;
			pt.y = (INT)fvAxis.y;

			return (PtInRect(&rect, pt)==TRUE);
		}
		break;

	default:
		return FALSE;
	}

	return FALSE;
}

BOOL CEngineInterface::Axis_Trans(AXIS_TYPE typeSource, const fVector3& fvSource, AXIS_TYPE typeTar, fVector3& fvTarget)
{
	register Fairy::TerrainData*	pTerrainData = m_pFairySystem->getTerrainData();
	
	// 尚未加载
	if(pTerrainData && (pTerrainData->getXSize()==0 || pTerrainData->getZSize()==0))
	{
		pTerrainData = 0;
	}

	fVector3 fvScale = GetSacle();

	//检查是否是合法的坐标
	fvTarget = fvSource;
	if(!Axis_CheckValid(typeSource, fvSource)) return FALSE;
	if(typeSource == typeTar) return TRUE;

	switch(typeSource)
	{
		//-----------------------------------------------
	case AX_GAME:
		{
			if(AX_PLAN==typeTar) return TRUE;
			else if(AX_GFX == typeTar)
			{
				if(pTerrainData)
				{
					fvTarget.x = pTerrainData->mPosition.x + (FLOAT)fvSource.x * pTerrainData->mScale.x;
					fvTarget.y = pTerrainData->mPosition.y + (FLOAT)fvSource.y * pTerrainData->mScale.y;
					fvTarget.z = pTerrainData->mPosition.z + (FLOAT)fvSource.z * pTerrainData->mScale.z;
				}
				else
				{
					fvTarget.x = (FLOAT)fvSource.x * fvScale.x;
					fvTarget.y = (FLOAT)fvSource.y * fvScale.y;
					fvTarget.z = (FLOAT)fvSource.z * fvScale.z;
				}
				return TRUE;
			}
			else if(AX_SCREEN==typeTar)
			{
				KLThrow("(AX_GAME->AX_SCREEN) Not support yet!");
			}
		}
		break;
		//-----------------------------------------------
	case AX_PLAN:
		{
			if(!pTerrainData) return FALSE;

			FLOAT fGfxX = pTerrainData->mPosition.x + (FLOAT)fvSource.x * pTerrainData->mScale.x;
			FLOAT fGfxZ = pTerrainData->mPosition.z + (FLOAT)fvSource.z * pTerrainData->mScale.z;

			if(AX_GAME==typeTar)
			{
				//取得相地形高度作为Y坐标(游戏坐标)
				fvTarget.y = (pTerrainData->getHeightAtGrid(fvSource.x, fvSource.z)-pTerrainData->mPosition.y)/ pTerrainData->mScale.y;
				return TRUE;
			}
			else if(AX_GFX == typeTar)
			{
				//取得相地形高度作为Y坐标
				fvTarget.x = fGfxX;
				fvTarget.z = fGfxZ;
				fvTarget.y = pTerrainData->getHeightAtGrid(fvSource.x, fvSource.z);
				return TRUE;
			}
			else if(AX_SCREEN==typeTar)
			{
				KLThrow("(AX_PLAN->AX_SCREEN) Not support yet!");
			}
		}
		break;

		//-----------------------------------------------
	case AX_GFX:
		{
			if(AX_GAME==typeTar || AX_PLAN==typeTar)
			{
				if(pTerrainData)
				{
					fvTarget.x = (fvSource.x-pTerrainData->mPosition.x)/pTerrainData->mScale.x;
					fvTarget.y = (fvSource.y-pTerrainData->mPosition.y)/pTerrainData->mScale.y;
					fvTarget.z = (fvSource.z-pTerrainData->mPosition.z)/pTerrainData->mScale.z;
				}
				else
				{
					fvTarget.x = (fvSource.x)/fvScale.x;
					fvTarget.y = (fvSource.y)/fvScale.y;
					fvTarget.z = (fvSource.z)/fvScale.z;
				}
				return TRUE;
			}
			else if(AX_SCREEN==typeTar)
			{
				//KLThrow("(AX_GFX->AX_SCREEN) Not support yet!");
				if(!pTerrainData) return FALSE;
				
				//得到Ogre相机
				Ogre::Camera* pOgreCamera = m_pCamera_Current->GetOgreCamera();
				if(!pOgreCamera) return FALSE;
				if(!(pOgreCamera->isVisible(Ogre::Vector3(fvSource.x, fvSource.y, fvSource.z)))) return FALSE;

				// x:[-1w, 1w]	z:[-1h, 1h]
				Ogre::Vector3 vRet = pOgreCamera->getProjectionMatrix()* pOgreCamera->getViewMatrix() * Ogre::Vector3(fvSource.x, fvSource.y, fvSource.z);

				int nWidth  = m_pFairySystem->getViewport()->getActualWidth();
				int nHeight = m_pFairySystem->getViewport()->getActualHeight();

				fvTarget.x = (( vRet.x + 1.0f)*nWidth/2.0f);
				fvTarget.y = ((-vRet.y + 1.0f)*nHeight/2.0f);
					
				return TRUE;
			}
		}
		break;

		//-----------------------------------------------
	case AX_SCREEN:
		{
			if(!pTerrainData) return FALSE;

			Ogre::Vector3 vRenderPos;
			BOOL bRet = m_pFairySystem->getTerrainIntersects(Fairy::Point((INT)fvSource.x, fvSource.y), vRenderPos);
			if(!bRet) return FALSE;

			if(AX_GAME==typeTar || AX_PLAN==typeTar)
			{
				return Axis_Trans(AX_GFX, fVector3(vRenderPos.x, vRenderPos.y, vRenderPos.z), AX_GAME, fvTarget);
			}
			else if(AX_GFX==typeTar)
			{
				fvTarget = fVector3(vRenderPos.x, vRenderPos.y, vRenderPos.z);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

fVector3 CEngineInterface::GetSacle(VOID) const
{
	register Fairy::TerrainData* pTerrainData = m_pFairySystem->getTerrainData();
	if(pTerrainData && pTerrainData->getXSize()>0 && pTerrainData->getZSize()>0)
		return fVector3(pTerrainData->mScale.x, pTerrainData->mScale.y, pTerrainData->mScale.z);
	else
		return fVector3(100.0f, 100.0f, 100.0f);
}

// 初始化
VOID CEngineInterface::Initial(VOID* pParam)
{
#if (_MSC_VER >= 1400)
	setlocale( LC_CTYPE, "" );
#endif

	//---------------------------------------------------------
	//取得渲染窗口句柄
	KLAssert(pParam);
	HWND hMainWnd = *((HWND*)pParam);
	g_hMainWnd = hMainWnd;

	g_pVaribleSys = (tVariableSystem*)g_theKernel.GetNode("bin\\var");
	KLAssert(g_pVaribleSys);

	g_pEventSys = (tEventSystem*)g_theKernel.GetNode("bin\\event");
	KLAssert(g_pEventSys);

	//---------------------------------------------------------

	// 改变加载方式
	// 屏掉下边这句了by czg 200911-30
	// CFairyScriptResGet::EnableResDynamicLoad(false);
	// 影响场景效果Z
	Ogre::SystemSetting::forcePixelShader(false);

	// 初始化KylinSystem
	m_pFairySystem->init("Resources.cfg", "", "", "Engine.log");	// Resources

	//---------------------------------------------------------
	// 加载OGRE插件
	g_pDebuger = (tDebuger*)g_theKernel.GetNode("bin\\debuger");

	g_pSoundSystem = (tSoundSystem*)g_theKernel.GetNode("bin\\snd");
	KLAssert(g_pSoundSystem);

	g_pWorldSystem = (tWorldSystem*)g_theKernel.GetNode("bin\\worldman");
	KLAssert(g_pWorldSystem);

	g_pTimeSystem = (tTimeSystem*)g_theKernel.GetNode("bin\\time");
	KLAssert(g_pTimeSystem);

	g_pResourceProvider = (tResourceProvider*)g_theKernel.GetNode("bin\\resprovider");
	KLAssert(g_pResourceProvider);
	g_pDataBase = (tDataBaseSystem*)g_theKernel.GetNode("bin\\dbc");
	KLAssert(g_pDataBase);

	STRING str = g_pVaribleSys->GetAs_String("Gfx_API");
	Ogre::Root* pTheRoot = &(Ogre::Root::getSingleton());
	// 选择渲染系统
	if(str == "Direct3D")
	{
#ifdef  USEOGRELIB
		Ogre::InstallD3D9Plugin();
#else
		pTheRoot->loadPlugin("RenderSystem_Direct3D9.dll");
		pTheRoot->setRenderSystem(pTheRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem"));
#endif
	
	}
	else if(str == "OpenGL")
	{
#ifdef  USEOGRELIB
	
#else
		pTheRoot->loadPlugin("RenderSystem_GL.dll");
		pTheRoot->setRenderSystem(pTheRoot->getRenderSystemByName("OpenGL Rendering Subsystem"));
#endif
	}
	else 
		KLThrow("UnKnown RenderSystem!");
	// 加载其他插件
#ifdef  USEOGRELIB
	Ogre::InstallParticleFXPlugin();
	Ogre::InstallParticleFX2Plugin();
	Ogre::InstallOctreeScenePlugin();
	Ogre::InstallCgProgramManagerPlugin();
#else
	pTheRoot->loadPlugin("Plugin_ParticleFX.dll");
	pTheRoot->loadPlugin("Plugin_ParticleFX2.dll");
	pTheRoot->loadPlugin("Plugin_OctreeSceneManager.dll");
	pTheRoot->loadPlugin("Plugin_CgProgramManager.dll");
#endif
	

	// 分析资源
	ScriptParsingListener listener;
	try 
	{
		// 传入渲染窗口句柄
		Ogre::NameValuePairList ViewSetting;
		INT nFSAA = g_pVaribleSys->GetAs_Int(szFSAAVar);
		INT nVsync = g_pVaribleSys->GetAs_Int(szvsyncVar);

		switch(nFSAA)
		{
		case 0:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(0);
			break;
		case 1:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(2);
			break;
		case 2:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(4);
			break;
		default:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(0);
			break;
		}

		if(nVsync == 1)
			ViewSetting["vsync"] = "true";
		else
			ViewSetting["vsync"] = "false";

		m_pFairySystem->setup((Fairy::ulong)(UINT_PTR)g_hMainWnd, &ViewSetting);

		// 关闭OGRE图标
		m_pFairySystem->getFrameStatsListener()->showLogoPanel(FALSE);
		
		// 状态板
		m_pFairySystem->getFrameStatsListener()->showStatPanel(FALSE);

		// 创建射线交集
		m_pRaySceneQuery = m_pFairySystem->getSceneManager()->createRayQuery(Ogre::Ray());
        m_pRaySceneQuery->setSortByDistance(TRUE);

		//---------------------------------------------------------
		// 设置DebugPanel
		m_pDebugPanel = new CDebugPanel(Ogre::OverlayManager::getSingleton().getByName("WXSJ/DebugOverlay"),		// CJSH
										Ogre::OverlayManager::getSingleton().getOverlayElement("WXSJ/StatPanel"), 
										Ogre::OverlayManager::getSingleton().getOverlayElement("WXSJ/HitObjPanel"), 
										Ogre::OverlayManager::getSingleton().getOverlayElement("WXSJ/HitObjText"), 
										m_pFairySystem->getRenderWindow());

		//---------------------------------------------------------
		// 创建摄像机
		{
			// 设置摄像机
			// 取得环境变量
		
			BOOL bHave;
			FLOAT fCamera_MinDistance = g_pVaribleSys->GetAs_Float("Camera_MinDistance", &bHave);
			if(bHave) 
				CCamera_Scene::MIN_DISTANCE = fCamera_MinDistance;

			FLOAT fCamera_MaxDistance = g_pVaribleSys->GetAs_Float("Camera_MaxDistance", &bHave);
			if(bHave)
				CCamera_Scene::MAX_DISTANCE = fCamera_MaxDistance;

			FLOAT fCamera_Pitch = g_pVaribleSys->GetAs_Float("Camera_Pitch", &bHave);
			if(bHave) 
				CCamera_Scene::STATIC_PITCH = -fCamera_Pitch * KLU_PI / 180.0f;

			FLOAT fCamera_PitchApproch = g_pVaribleSys->GetAs_Float("Camera_PitchApproch", &bHave);
			if(bHave) 
				CCamera_Scene::APPROCH_PITCH = -fCamera_PitchApproch * KLU_PI / 180.0f;

/*
			// 摄象机偏移量
			INT fCamera_Offset = g_pVaribleSys->GetAs_Float("Camera_Varible_Offset", &bHave);
			if( bHave )
//				Camera_SetOffset( 0.0f, m_fVerticalOffset );
				m_fVerticalOffset = fCamera_Offset;
*/

/*
			fCamera_Offset = g_pVaribleSys->GetAs_Float("Camera_Parallel_Offset", &bHave);
			if( bHave )
				m_fSaveParallelOffset = fCamera_Offset;
*/
		}

		m_pCamera_CharView = new CCamera_CharSel(m_pFairySystem->getCamera());
		m_pCamera_Scene = new CCamera_Scene(m_pFairySystem->getCamera());
		m_pCamera_Scene->SetDistance(CCamera_Scene::MAX_DISTANCE);

		m_pCamera_Current = m_pCamera_CharView;

		//--------------------------------------------
		// 初始化PostFilter系统
		m_pPostFilterManager = m_pFairySystem->getPostFilterManager();
		// 注册所有全局渲染器
		Fairy::registerAllPostFilterFactories(m_pPostFilterManager);
		m_pPostFilterManager->createPostFilter("Floodlighting", m_pFairySystem->getViewport());
		m_pPostFilterManager->createPostFilter("Bloom", m_pFairySystem->getViewport());
		m_pPostFilterManager->createPostFilter("HeatVision", m_pFairySystem->getViewport());
		m_pPostFilterManager->createPostFilter("Fade", m_pFairySystem->getViewport());
		m_pPostFilterManager->createPostFilter("MotionBlur", m_pFairySystem->getViewport());

		//--------------------------------------------
		// 设置特效系统的声音播放函数
		Fairy::LogicModel::SetPlaySoundFuncton((OnPlaySound)(g_pSoundSystem->GetPlaySoundFunc()));
		Fairy::LogicModel::SetStopSoundFunction((OnStopSound)(g_pSoundSystem->GetStopSoundFunc()));
		Fairy::BulletSystem::SetPlaySoundFuncton((OnPlaySound)(g_pSoundSystem->GetPlaySoundFunc()));
		Fairy::BulletSystem::SetStopSoundFunction((OnStopSound)(g_pSoundSystem->GetStopSoundFunc()));
		Fairy::LogicModel::SetGetHeightInWorldListener((GetHeightInWorld)(g_pWorldSystem->GetTerrainHeightFunc()));

		// 初始化 fake obeject manager
		m_pFakeObjectManager = new CFakeObjectEntityManager;
		m_pFakeObjectManager->Initial();

//		Fairy::Effect::setDefaultIterativeInterval(0.01f);
		Ogre::ParticleSystem::setDefaultIterationInterval(0.033f);

		// 系统设置接口挂接变量控制系统
		g_pEventSys->RegisterEventHandle("VARIABLE_CHANGED",_OnVariableChangedEvent);

		// 恢复上次颜色设置
		g_pEventSys->PushEvent(GE_VARIABLE_CHANGED, sz32BitTexturesVar, g_pVaribleSys->GetAs_String(sz32BitTexturesVar).c_str());


//		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps( 1 );


		// 加载人物阴影配置
		BOOL bHave;
		INT nHumanLightmap = g_pVaribleSys->GetAs_Float(szShadowTechniqueVar, &bHave );
		if( bHave )
			Scene_SetShadowTechnique( (BOOL)nHumanLightmap );

		// 加载全局泛光配置
		INT nFullScreenLight = g_pVaribleSys->GetAs_Float( szPostFilterEnabledVar, &bHave );
		if( bHave )
			Scene_SetPostFilterEnabled( (BOOL)nFullScreenLight );

		//初始化小地图探灯功能
		m_pMapLight = new CUIMapLight;
		m_pMapLight->Initial();

	} 
	catch(const Ogre::Exception& e)
	{
        BOOL rethrow = TRUE;

        if (e.getNumber() == Ogre::Exception::ERR_DUPLICATE_ITEM)
        {
            if (e.getSource() == "ResourceManager::add")
            {
                if (Ogre::StringUtil::endsWith(listener._scriptName, ".material", TRUE))
                {
                    char materialName[256];
                    if (sscanf(e.getDescription().c_str(), "Resource with the name %255s already exists.", materialName) == 1)
                    {
                        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
                        if (!material.isNull())
                        {
                            char message[1024];

                            Ogre::StringVectorPtr currentNames = Fairy::findResourceFilenames(listener._groupName, listener._scriptName);
                            const Ogre::String& currentName = currentNames.isNull() || currentNames->empty() ? listener._scriptName : currentNames->front();
                            if (material->getOrigin().empty())
                            {
								_snprintf(message, 1024,
										("Predefined material \"%s\" duplicated in file \"%s\", the application will exit."),
                                        material->getName().c_str(), currentName.c_str());
                            }
                            else
                            {
                                Ogre::StringVectorPtr originNames = Fairy::findResourceFilenames(material->getGroup(), material->getOrigin());
                                const Ogre::String& originName = originNames.isNull() || originNames->empty() ? material->getOrigin() : originNames->front();
                                if (currentName == originName)
                                {
									_snprintf(message, 1024,
												("The material \"%s\" occuring in the file \"%s\" more than once, the application will exit."),
			                                    material->getName().c_str(), currentName.c_str());
                                }
                                else
                                {
									_snprintf(message, 1024,
											("Duplicate material \"%s\" in files \"%s\" and \"%s\", the application will exit."),
                                            material->getName().c_str(), currentName.c_str(), originName.c_str());
                                }
                            }

							KLThrow("Ogre::%s", message);
                        }
                    }
                }
            }
		}
		
		throw;
	}
}

VOID CEngineInterface::Tick(VOID)
{
	const INT nStep = 5;
	CHAR szParam[32];
	if( m_bDeathMode && m_nDeachModePercent < 100 )
	{
		m_nDeachModePercent += nStep;

		_snprintf(szParam, 32, "%f", (100.0f-m_nDeachModePercent)/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "ColourAmount", szParam);

		_snprintf(szParam, 32, "%f", (FLOAT)m_nDeachModePercent/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "GrayAmount", szParam);
	}
	else if( !m_bDeathMode && m_nDeachModePercent > 0 )
	{
		m_nDeachModePercent-=nStep;

		_snprintf(szParam, 32, "%f", (100.0f-m_nDeachModePercent)/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "ColourAmount", szParam);

		_snprintf(szParam, 32, "%f", (FLOAT)m_nDeachModePercent/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "GrayAmount", szParam);

		if( m_nDeachModePercent <= 0 )
		{
			m_pPostFilterManager->setPostFilterEnabled("Fade", false);
		}
	}


	// 判断当前摄象机是否需要循环
	m_pCamera_Scene;
}

VOID CEngineInterface::Camera_SetCurrent(CAMERA_TYPE theType)
{
	switch(theType)
	{
	case CHAR_VIEW:
		m_pCamera_Current = m_pCamera_CharView;
		break;

	case SCENE:
		m_pCamera_Current = m_pCamera_Scene;
		break;
	}

	m_pCamera_Current->Update();
}

// 渲染Loading场景
VOID CEngineInterface::RenderLodingFrame(LPCTSTR szLoding)
{
	RenderFrame();
}

// 渲染OGRE场景
VOID CEngineInterface::RenderFrame(VOID)
{
	if(!Ogre::Root::getSingletonPtr())
	{
		KLThrow("Ogre::Root = 0");
	}

	const UINT MAX_RENDER_INTER_TIME = 2000;

	static UINT s_nLastRenderTime = 0;
	UINT s_nTimeNow = g_pTimeSystem->GetTimeNow();

	if(g_pTimeSystem->CalSubTime(s_nLastRenderTime, s_nTimeNow) > MAX_RENDER_INTER_TIME)
	{
		Ogre::Root::getSingleton().clearEventTimes();
		s_nLastRenderTime = s_nTimeNow;
		
		return;
	}

	s_nLastRenderTime = s_nTimeNow;

	//重置小地图探灯
	if(m_pMapLight)
	{
		m_pMapLight->Update();
	}
	
	Ogre::Root::getSingleton().renderOneFrame();
}

/// 响应WM_PAINT消息
VOID CEngineInterface::OnPaint(VOID)
{
	RenderFrame();
}

/// 窗口大小发生改变
VOID CEngineInterface::OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 用户点击最大化或者最小化
	if(WM_SIZE == message)
	{
		m_pFairySystem->resizeWindow(LOWORD(lParam), HIWORD(lParam));
	}
	// 用户拖动窗口边缘结束
	else if(WM_EXITSIZEMOVE == message)
	{
		RECT rect;
		GetClientRect(g_hMainWnd, &rect);

		m_pFairySystem->resizeWindow(rect.right-rect.left, rect.bottom-rect.top);
	}
}

// CGameProcedure::s_pGfxSystem->Camera_SetOffset()
VOID CEngineInterface::Camera_SetOffset( OFFSET_TAG eOT, FLOAT fParallelOffset, FLOAT fVerticalOffset )
{
/*
	KLAssert(m_pCamera_Current);

	m_eOffsetTag = eOT;


	if( OT_LEFT == eOT )
	{
		if( fParallelOffset > 0 )
		{
			if( !m_bLeftOffset )
			{
				m_fParallelOffset -= m_fSaveParallelOffset;	// 水平
				m_bLeftOffset = TRUE;
			}
		}
		else
		{
			if( m_bRightOffset )
				m_fParallelOffset += m_fSaveParallelOffset;
			else
				m_fParallelOffset = 0;

			m_bLeftOffset = FALSE;
		}
	}
	else if( OT_RIGHT == eOT )
	{
		if( fParallelOffset > 0 )
		{
			if( !m_bRightOffset )
			{
				m_fParallelOffset += m_fSaveParallelOffset;	// 水平
				m_bRightOffset = TRUE;
			}
		}
		else
		{
			if( m_bLeftOffset )
				m_fParallelOffset -= m_fSaveParallelOffset;
			else
				m_fParallelOffset = 0;

			m_bRightOffset = FALSE;
		}
	}

	m_fOldParallelOffset = m_fParallelOffset;

	((CCamera_Scene*)m_pCamera_Current)->SetOffset( m_fParallelOffset );
*/

	m_fVerticalOffset = fVerticalOffset;		// 垂直
}

VOID CEngineInterface::Camera_SetLookAt(const fVector3& fvPos)
{
	KLAssert(m_pCamera_Current);

	fVector3 pos = fvPos;

//	pos.x += m_fParallelOffset;
	//pos.z += m_fVerticalOffset;

	((CCamera_Scene*)m_pCamera_Current)->SetLookAt( pos );

	CheckObjectTransparent();
}

/// 缩放相机视口 [0.0f, 1.0f]  0-最近距离 1-最远距离
VOID CEngineInterface::Camera_SetZoom(FLOAT fZoom)
{
	// 超出范围
	if( fZoom < 0.0f || fZoom > 1.0f )
		return;

	FLOAT fDistance = CCamera_Scene::MIN_DISTANCE + (CCamera_Scene::MAX_DISTANCE - CCamera_Scene::MIN_DISTANCE) * fZoom;
	m_pCamera_Current->SetDistance(fDistance);
}

VOID CEngineInterface::Camera_SetZoom(BOOL bUp, INT multiple)
{
	const FLOAT ZOOMSPEED = 0.05f;
	// 
	FLOAT fZoom = (m_pCamera_Current->GetDistance() - CCamera_Scene::MIN_DISTANCE) / (CCamera_Scene::MAX_DISTANCE - CCamera_Scene::MIN_DISTANCE);
	
	// 获取当前的Pitch
	FLOAT fPitch = m_pCamera_Current->GetApprochPitch();
	// 计算Pitch步长值
	static FLOAT fProportion = (CCamera_Scene::STATIC_PITCH - CCamera_Scene::APPROCH_PITCH) * (ZOOMSPEED * multiple);

	// 接近人物
	if( bUp )
	{
		// 
		fZoom -= (ZOOMSPEED * multiple);
		if( fZoom < 0.0f )
			fZoom = 0.0f;

 //		if( (m_fParallelOffset > 0.0f) || (m_fParallelOffset < 0.0f) )
//			m_fParallelOffset = m_fOldParallelOffset + ( 1 - fZoom );

		// 数值由小到大
		fPitch -= fProportion;
		if( fPitch > CCamera_Scene::APPROCH_PITCH )
			fPitch = CCamera_Scene::APPROCH_PITCH;

	}
	// 远离人物
	else
	{
		// 
		fZoom += (ZOOMSPEED * multiple);
		if( fZoom > 1.0f )
			fZoom = 1.0f;

//		if( (m_fParallelOffset > 0.0f) || (m_fParallelOffset < 0.0f) )
//			m_fParallelOffset = m_fOldParallelOffset - ( 1 - fZoom );

		// 数值由大到小
		fPitch += fProportion;
		if( fPitch < CCamera_Scene::STATIC_PITCH )
			fPitch = CCamera_Scene::STATIC_PITCH;
	}


	//& 根据拉进或拉远来修正这个值， APPROCH_PITCH是最终趋近的判定值， 在Game.cfg中添写
	// 这里设置完值后最终通过camera的更新来修正pitch 
	m_pCamera_Scene->SetApprochPitch(fPitch);


	Camera_SetZoom(fZoom);


	// 根据距离来影响偏移量
//	Camera_SetLookAt( m_pCamera_Scene->GetGameLookAt() );
}

VOID CEngineInterface::Camera_Direction(FLOAT fDir)
{
	m_pCamera_Current->SetDirection(fDir);		
}

VOID CEngineInterface::Camera_AddDirection(FLOAT fMultiple)
{
	FLOAT fDir = m_pCamera_Current->GetDirection() + fMultiple;
	Camera_Direction(fDir);
}

VOID CEngineInterface::Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay)
{
	Ogre::Ray ray = m_pFairySystem->getWindowToViewportRay(nX, nY);

	fVector3 fGFX = fVector3(ray.getOrigin().x, ray.getOrigin().y, ray.getOrigin().z);
	Axis_Trans(AX_GFX, fGFX, AX_GAME, fRay.mOrigin);

	fRay.mDirection = fVector3(ray.getDirection().x, ray.getDirection().y, ray.getDirection().z);
}

// 震荡一次
VOID CEngineInterface::Camera_VibrateOneTime()
{
	KLAssert(m_pCamera_Current);

//	((CCamera_Scene*)m_pCamera_Current)->AddOneVib(10, 8, 2);
	Camera_VibrateOneTime(10, 8, 2);
}
VOID CEngineInterface::Camera_VibrateOneTime(FLOAT fAmplitude, FLOAT fDuration, UINT fFrequency)
{
	KLAssert(m_pCamera_Current);

	((CCamera_Scene*)m_pCamera_Current)->AddOneVib(fAmplitude, fDuration, fFrequency);
}


// 设置眼睛的位置, ogre 坐标
VOID CEngineInterface::Camera_SetEyePos(const fVector3& fvPos)
{
	if(m_pFairySystem)
	{
		Ogre::Camera* pOgreCamera = m_pFairySystem->getCamera();

		pOgreCamera->setPosition(Ogre::Vector3(fvPos.x, fvPos.y, fvPos.z));
		
	}
}

// 设置观察点的位置 ogre 坐标.
VOID CEngineInterface::Camera_SetLookAtPos(const fVector3& fvPos)
{
	if(m_pFairySystem)
	{
		Ogre::Camera* pOgreCamera = m_pFairySystem->getCamera();
		pOgreCamera->lookAt(Ogre::Vector3(fvPos.x, fvPos.y, fvPos.z));
	}
}

VOID CEngineInterface::Scene_PreLoad(const STRING& theScenePreload)
{
	char* lpAddress = NULL;
	DWORD dwSize = g_pResourceProvider->loadResource(theScenePreload.c_str(), lpAddress, "General");
	if(dwSize > 0)
	{
		register const char* pMemory = lpAddress;
		const char* pDeadEnd = lpAddress + dwSize + 1;

		char szLine[1024] = {0};
		do
		{
			//读取一行
			pMemory = KLU_GetLineFromMemory(szLine, 1024, pMemory, pDeadEnd);
			if(!pMemory) break;

			Fairy::LogicModelManager::getSingleton().loadModelResource(szLine);
		}while(true);
		g_pResourceProvider->unloadResource(lpAddress, dwSize);
	}
}

VOID CEngineInterface::Scene_UnLoad(void)
{
	// 卸载现有资源
	m_pFairySystem->resetScene();
}

VOID CEngineInterface::Scene_Load(INT nID, const STRING& theSceneDefine)
{
	Fairy::ExpatParser parserExpat(NULL);
	
	if(m_pTerrainGrid)
	{
		m_pTerrainGrid->destroyRenderable();
	}
	// 卸载现有资源
	m_pFairySystem->resetScene();

	// 加载场景文件
	m_pFairySystem->loadSceneFromResource(&parserExpat, theSceneDefine.c_str(), "Scene");

	// 释放未使用资源
	{
		Ogre::ResourceGroupManager& resManager = Ogre::ResourceGroupManager::getSingleton();

		const Ogre::StringVector& strGroupVec = resManager.getResourceGroups();

		for(int i=0; i<(int)strGroupVec.size(); i++)
		{
			if( strGroupVec[i] != Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME &&
				strGroupVec[i] != Fairy::BOOTSTRAP_RESOURCE_GROUP_NAME)
			{
				resManager.unloadUnreferencedResourcesInGroup(strGroupVec[i]);
			}
		}
	}
	
	// 设置LightMap
	BOOL bHave = FALSE;
	INT qu = g_pVaribleSys->GetAs_Uint(szLightmapQualityVar, &bHave);
	if ( !bHave )
	{
		qu = 3;
	}
	Scene_SetLightmapQuality(qu);
	
	// 取得环境特效
	Fairy::SceneInfo::ObjectsByTypeRange objEnviroment = m_pFairySystem->getSceneInfo()->findObjectsByType("Enviroment");

	if (objEnviroment.first != objEnviroment.second)
	{
		m_pEnviromentObject = static_cast< Fairy::EnviromentObject * >(objEnviroment.first->get());
		KLAssert(m_pEnviromentObject);
	}
	else
	{
		m_pEnviromentObject = NULL;
	}
}

VOID CEngineInterface::Scene_SetEnviromentInfo(const fVector3& fvPos)
{
	if( m_pEnviromentObject )
	{
		// 坐标转换
		fVector3 fvPosGfx;
		if( !Axis_Trans(AX_GAME, fvPos, AX_GFX, fvPosGfx) ) 
			return;

		// 设置新的位置
		m_pEnviromentObject->setWeatherParticleSystemPosition(Ogre::Vector3(fvPosGfx.x, fvPosGfx.y+300, fvPosGfx.z));
	}
}

//VOID CEngineInterface::Debug_Scene_SetFloodlighting(FLOAT fBlur, FLOAT fShine)
//{
//	Scene_SetPostFilter("Floodlighting");
//
//	CHAR szTemp[32];
//	
//	_snprintf(szTemp, 32, "%.2f", fBlur);
//	m_pPostFilterManager->setPostFilterParameter("BlurAmount", szTemp);
//
//	_snprintf(szTemp, 32, "%.2f", fShine);
//	m_pPostFilterManager->setPostFilterParameter("ShineAmount", szTemp);
//}

//设置地形阴影
VOID CEngineInterface::Scene_SetLightmapQuality(INT nQuality)
{
	switch(nQuality)
	{
	case 0:
		m_pFairySystem->getTerrain()->setLightmapQuality(Fairy::Terrain::LMQ_NONE);
		m_pFairySystem->getTerrain()->buildGeometry(m_pFairySystem->getBaseSceneNode());
		break;

	case 1:
		m_pFairySystem->getTerrain()->setLightmapQuality(Fairy::Terrain::LMQ_LOW);
		m_pFairySystem->getTerrain()->buildGeometry(m_pFairySystem->getBaseSceneNode());
		break;

	case 2:
		m_pFairySystem->getTerrain()->setLightmapQuality(Fairy::Terrain::LMQ_HIGH);
		m_pFairySystem->getTerrain()->buildGeometry(m_pFairySystem->getBaseSceneNode());
		break;
	
	default:
		m_pFairySystem->getTerrain()->setLightmapQuality(Fairy::Terrain::LMQ_HIGH);
		m_pFairySystem->getTerrain()->buildGeometry(m_pFairySystem->getBaseSceneNode());
		break;
	}
}


//显示设置全局渲染效果
VOID CEngineInterface::Scene_SetPostFilter(LPCTSTR szFilterName, BOOL state)
{
	// 设置全局渲染特效
	m_pPostFilterManager->setPostFilterEnabled(szFilterName, state);

	// 取得环境变量
	g_pVaribleSys->SetVariable("Gfx_PostFilter", szFilterName);
}

// 进入/退出死亡全局渲染效果
VOID CEngineInterface::Scene_SetPostFilter_Death(BOOL bEnter)
{
	if(bEnter == m_bDeathMode) 
		return;

	if(bEnter)
	{
		m_pPostFilterManager->setPostFilterEnabled("Fade", true);
		m_nDeachModePercent = 0;

		m_pPostFilterManager->setPostFilterParameter("Fade", "ColourAmount", "1.0");
		m_pPostFilterManager->setPostFilterParameter("Fade", "GrayAmount", "0.0");
	}
	else
	{
		m_nDeachModePercent = 100;
	}

	m_bDeathMode = bEnter;
}

tEntityNode* CEngineInterface::New_EntityObject(tEntityNode::EntityType type)
{
	switch(type)
	{
	case tEntityNode::ETYPE_DUMMY:
		return new CObjEntityDummy;

	case tEntityNode::ETYPE_MESH:
		return new CObjEntityStatic;

	case tEntityNode::ETYPE_MODEL:
		return new CObjEntityModel;

	case tEntityNode::ETYPE_ACTOR:
		return new CObjEntityActor;

	default:
		return NULL;
	}
}

VOID CEngineInterface::Destroy_EntityObject(tEntityNode* pNode)
{
	KLAssert(pNode);

	delete ((CObjectEntity*)pNode);

	// 之后不能调用和该node相关的任何代码
	return;
}

tEntityNode* CEngineInterface::Find_HitFairyObject(INT nX, INT nY)
{
	Ogre::Ray ray = m_pFairySystem->getWindowToViewportRay(Fairy::Point(nX, nY));
	m_pRaySceneQuery->setRay(ray);

	// 按照逻辑优先级的查询队列
	std::map< INT, tEntityNode* > mapRayQuery;

	const Ogre::RaySceneQueryResult& queryResult = m_pRaySceneQuery->execute();
	for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
	{
		Ogre::MovableObject* pMovable = it->movable;
		Fairy::ObjectPtr object = Fairy::getObjectFromMovable(pMovable);
		if (object&&object->getData())
		{
			tEntityNode* pEntityNode = (tEntityNode*)(object->getData());
			// 不是游戏逻辑中动态生成的
			if(!pEntityNode) 
				continue;

			// 不再被关心
			if(!(pEntityNode->GetRayQuery())) 
				continue;

			if(pEntityNode->GetType() == tEntityNode::ETYPE_ACTOR) // object->getType() == "LogicModel")
            {
                Fairy::LogicModelObject* logicModelObject = static_cast<Fairy::LogicModelObject*>(object.get());

                assert (logicModelObject);

                Fairy::LogicModel* model = logicModelObject->getLogicModel();

                assert (model);

				// 不再自定义BoundingBox中
				if (model->isUseExternalBoundingBox() && !(model->rayIntersect(ray)))
                {        
                    continue;
                }
            }

			// 如果已经有同级存在，按照摄像机远近排序
			if(mapRayQuery.find(pEntityNode->RayQuery_GetLevel()) != mapRayQuery.end()) 
				continue;

			// 放入查询队列
			mapRayQuery.insert(std::make_pair(pEntityNode->RayQuery_GetLevel(), pEntityNode));
		}
	}

	// 返回优先级最高的逻辑对象
	if(!mapRayQuery.empty()) 
	{
		return mapRayQuery.begin()->second;
	}

	return NULL;
}

VOID CEngineInterface::PushDebugString(LPCTSTR szMsg, ...)
{
}

VOID CEngineInterface::SetDebugString(LPCTSTR szName, LPCTSTR szValue)
{
	if(!m_pDebugPanel) return;
	m_pDebugPanel->SetDebugString(szName, szValue);
}

inline FLOAT CEngineInterface::Camera_GetDirection(VOID) const 
{
	return m_pCamera_Current->GetDirection();
}

inline FLOAT CEngineInterface::Camera_GetZoom(VOID) const 
{
	return m_pCamera_Current->GetDistance();
}

VOID CEngineInterface::SetHitObjText(LPCTSTR szHitObjText)
{
	m_pDebugPanel->SetHitObjString(szHitObjText);
}

VOID CEngineInterface::Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos)
{
	//static int name = 0;
	//char szTemp[MAX_PATH];
	//_snprintf(szTemp, MAX_PATH, "__test_%d", name++);

	//Ogre::ParticleSystem *system = m_pFairySystem->getSceneManager()->createParticleSystem(szTemp,	szParticleName);
	//assert (system);
	//Ogre::SceneNode *node = m_pFairySystem->getBaseSceneNode()->createChildSceneNode();

	//node->attachObject(system);

	//fVector3 fvGfx;
	//Axis_Trans(AX_GAME, fvPos, AX_GFX, fvGfx);

	//node->setPosition(Ogre::Vector3(fvGfx.x, fvGfx.y, fvGfx.z));//
}


//创建一个Fake渲染对象
VOID CEngineInterface::FakeObj_Create(LPCTSTR szObjectName, tEntityNode* pNode, tEntityNode* pParentNode, LPCTSTR szCameraName, 
									  int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName)
{
	m_pFakeObjectManager->CreateObject(szObjectName, (CObjEntityActor*)pNode, (CObjEntityActor*)pParentNode,szCameraName, nTexWidth, nTexHeight, szBackgroundName);
}

//销毁一个Fake渲染对象
VOID CEngineInterface::FakeObj_Destroy(LPCTSTR szObjName)
{
	m_pFakeObjectManager->DestroyObject(szObjName);
}

bool CEngineInterface::FakeObj_SetAttach(tEntityNode* pNode, tEntityNode* pAttachNode)
{
	if(pAttachNode && pAttachNode->GetType()!=tEntityNode::ETYPE_ACTOR) return false;

	return m_pFakeObjectManager->SetAttachObj((CObjEntityActor*)pNode, (CObjEntityActor*)pAttachNode);
}

//隐藏/显示Fake渲染对象
VOID CEngineInterface::FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio)
{
	m_pFakeObjectManager->SetVisible(szObjName, bVisible, fAspectRatio);
}

VOID CEngineInterface::Debug_SetFakeCamera(INT nType, FLOAT fValue)
{
	m_pFakeObjectManager->Debug_SetFakeCamera(nType, fValue);
}

VOID CEngineInterface::Debug_ToggleShowDebugPanel(VOID)
{
	m_pDebugPanel->ToggleShow();

	bool bShow = !(m_pFairySystem->getFrameStatsListener()->isStatPanelShown());
	m_pFairySystem->getFrameStatsListener()->showStatPanel(bShow);

}

VOID CEngineInterface::Debug_ToggleShowBoundingBox(VOID)
{
	Fairy::LogicModelManager::getSingleton().showModelBoundingBox(!Fairy::LogicModelManager::getSingleton().isShowBoundingBox());
}

VOID WINAPI CEngineInterface::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
	{
		const STRING& strVariable = pEvent->vArg[0];
		const STRING& strValue = pEvent->vArg[1];

		BOOL bEnable = (strValue=="1");

		// 静态物体动画
		if(strVariable == szAnimationQualityVar)
		{
			INT nValue = strValue[0] - '0';

			GetMe()->Scene_SetAnimationQuality( nValue*50 - 50 );
			//低的模型 -50
			//中		0
			//高		50
		}
		// 地形阴影
		else if(strVariable == szLightmapQualityVar)
		{
			INT nValue = strValue[0] - '0';

			GetMe()->Scene_SetLightmapQuality(nValue);
		}
		// 全局泛光
		else if(strVariable == szPostFilterEnabledVar)
		{
			if(bEnable)
				GetMe()->Scene_SetPostFilterEnabled(TRUE);
			else
				GetMe()->Scene_SetPostFilterEnabled(FALSE);			
		}
		// 颜色质量
		else if(strVariable == sz32BitTexturesVar)
		{
			INT nValue = strValue[0] - '0';

			if(nValue == 0)
			{	//16
				GetMe()->Scene_Enable32BitTextures(FALSE);
			}
			else if(nValue == 1)
			{
				//32
				GetMe()->Scene_Enable32BitTextures(TRUE);
			}
			else
			{
				//桌面
				int nDesktopBits = ::GetDeviceCaps(::GetDC(::GetDesktopWindow()), BITSPIXEL);
				if(16 == nDesktopBits)
				{
//					g_pVaribleSys->SetAs_Int(sz32BitTexturesVar, 0, FALSE);
					GetMe()->Scene_Enable32BitTextures(FALSE);
				}
				else
				{
//					g_pVaribleSys->SetAs_Int(sz32BitTexturesVar, 1, FALSE);
					GetMe()->Scene_Enable32BitTextures(TRUE);
				}
			}
		}
		// 纹理采样率
		else if(strVariable == szDefaultTextureFilteringVar)
		{
			INT nValue = strValue[0] - '0';

			GetMe()->Scene_SetTextureFiltering(nValue);
		}
		// 人物阴影
		else if( strVariable == szShadowTechniqueVar )
		{
			INT nValue = atoi(strValue.c_str());

			GetMe()->Scene_SetShadowTechnique(nValue);
		}
	}
}

//设置材质32位/16位转化
VOID CEngineInterface::Scene_Enable32BitTextures(BOOL bSetting)
{
	#if (_MSC_VER >= 1400)
	//	std::locale langLocale("");
	//	std::locale::global(langLocale);
		setlocale( LC_CTYPE, "" );
	#endif

//======
//	Ogre::TextureManager::getSingleton().enable32BitTextures(bSetting==TRUE);
}

//设置采样
VOID CEngineInterface::Scene_SetTextureFiltering(INT nOptions)
{
	switch(nOptions)
	{
	case 0:	
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_BILINEAR);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(1);
		break;
	case 1:
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(2);
		break;
	case 2:
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(8);
		break;
	default:
		break;
	}
}

//设置静态物体动画
VOID CEngineInterface::Scene_SetAnimationQuality(INT nOptions)
{
//	Fairy::System::setAnimationQuality( (bSetting == TRUE)?1:0);
	m_pFairySystem->setAnimationQuality( nOptions );
}

//设置全屏泛光效果
VOID CEngineInterface::Scene_SetPostFilterEnabled(BOOL bSetting)
{
//	Fairy::PostFilterManager::setPostFilterEnabled("Floodlighting",(bSetting==TRUE));
	m_pFairySystem->getPostFilterManager()->setPostFilterEnabled("Floodlighting",(bSetting==TRUE));
}

//设置人物实时阴影
VOID CEngineInterface::Scene_SetShadowTechnique(BOOL nOptions)
{
	////Ogre::SceneManager::setShadowTechnique
	//if(bSetting == TRUE)
	//	m_pFairySystem->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	//else
	//	m_pFairySystem->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

	//Ogre::SceneManager::setShadowTechnique
	if(nOptions == 2)
	{
		if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_1_1") || Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1") )
		{
			m_pFairySystem->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
			m_pFairySystem->getSceneManager()->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 0.5f));
			m_pFairySystem->getSceneManager()->setShadowDirectionalLightExtrusionDistance(1200);
			m_pFairySystem->getSceneManager()->setShadowUseInfiniteFarPlane(false);
			m_pFairySystem->getViewport()->setShadowsEnabled(true);
		}
		else
		{
			g_pVaribleSys->SetAs_Int("View_HumanLightmap",0,FALSE);
			//			STRING strTemp = "";
			//			strTemp = NOCOLORMSGFUNC("RenderSystem_Display_Adapter_Not_Support");
			//			g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE,"Not support mode!");

			return;
		}

		Fairy::LogicModelManager::getSingleton().setShadowCastable(true);
	}
	else if(nOptions == 1)
	{
		m_pFairySystem->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
		m_pFairySystem->getSceneManager()->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 0.5f));
		m_pFairySystem->getSceneManager()->setShadowDirectionalLightExtrusionDistance(1200);
		m_pFairySystem->getSceneManager()->setShadowUseInfiniteFarPlane(false);
		m_pFairySystem->getViewport()->setShadowsEnabled(true);

		Fairy::LogicModelManager::getSingleton().setShadowCastable(true);
	}
	else
	{
		m_pFairySystem->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
		m_pFairySystem->getViewport()->setShadowsEnabled(false);

		Fairy::LogicModelManager::getSingleton().setShadowCastable(false);
	}

}

//将当前屏幕截图到文件中
BOOL CEngineInterface::PrintScreen(LPTSTR szReturnBuf, INT nBufSize)
{
	const char* szScreenShortPath = "..\\ScreenShots";
	CHAR szFileName[MAX_PATH] = {0};
	try
	{
		//时间
		SYSTEMTIME timeNow;
		::GetLocalTime(&timeNow);

		_snprintf(szFileName, MAX_PATH, "%04d_%02d_%02d_%02d_%02d_%02d.jpg",
			timeNow.wYear, timeNow.wMonth, timeNow.wDay, 
			timeNow.wHour, timeNow.wMinute, timeNow.wSecond);

		//创建目录
		::CreateDirectory(szScreenShortPath, 0);

		//全路径
		char szPathFileName[MAX_PATH] = {0};
		_snprintf(szPathFileName, MAX_PATH, "%s\\%s", szScreenShortPath, szFileName);

		m_pFairySystem->getRenderWindow()->writeContentsToFile(szPathFileName);
	}
	catch(...)
	{
		return FALSE;	
	}

	strncpy(szReturnBuf, szFileName, nBufSize);
	return TRUE;
}

//// 重新设置旧的显示设置， 全屏显示等
//// 登录界面之后。 进入游戏中， 调用这个函数。
//void CEngineInterface::ResetOldShowSetting()
//{
//	//-------------------------------------------
//	//其他需要重置的显示设置
//	{
//		const char* szNeedReset[] = 
//		{
//			"View_FullScreen",		//全屏模式
//			"View_ColorQuality",	//颜色质量
//			"View_Resoution",		//分辨率
//		};
//
//		for(int i=0; i<sizeof(szNeedReset)/sizeof(const char*); i++)
//		{
//			g_pEventSys->PushEvent(GE_VARIABLE_CHANGED, szNeedReset[i], 
//				g_pVaribleSys->GetAs_String(szNeedReset[i]).c_str());
//		}
//	}
//}


//----laim
void CEngineInterface::SetShowObjectByType( LPCTSTR szName)
{
	
		
		static bool blogic=TRUE;
		static bool bstaticentity = TRUE;
		static bool bliquid = TRUE;
		static bool effect = TRUE;
		static bool pareticle = TRUE;
		static long lshowType = ENUM_OBJ_TYPE::ALL_TYPE;;

		 Ogre::String currentName = szName;

		if (currentName == "lm")
		{

		lshowType =lshowType ^ ( blogic << 5);

		}

		if (currentName == "se"){

		 lshowType =lshowType ^ bstaticentity;
		//  bstaticentity =!bstaticentity;
		}

		if (currentName == "sl"){
		lshowType = lshowType ^ (bliquid <<2);//ENUM_OBJ_TYPE::TERRAINLIQUIDOBJ_TYPE;
		//bliquid =!bliquid;
		}

		if (currentName == "ef"){
		lshowType = lshowType ^ (effect <<3);
		//effect =!effect;
		}

		if (currentName == "ps"){
		 lshowType =  lshowType ^ (effect <<1);
		//  pareticle =!pareticle;
		}

		if (currentName == "all"){
			lshowType = ENUM_OBJ_TYPE::ALL_TYPE;
		}



		//obj
		Fairy::LogicModelManager::LogicModelMap logicmap = Fairy::LogicModelManager::getSingleton().getModelMap();

		for ( Fairy::LogicModelManager::LogicModelMap::iterator i = logicmap.begin();
			i != logicmap.end(); ++i )
		{
			Fairy::LogicModel* model = i->second;
			model->setVisible(lshowType & ENUM_OBJ_TYPE::LOGICMODEL_TYPE);
		}

		Fairy::Scene::Objects objs = CEngineInterface::GetMe()->GetFairySystem()->getSceneInfo()->getObjects();
		Fairy::Scene::Objects::const_iterator it = objs.begin();
		for ( ; it != objs.end(); it++)
		{
			const Fairy::ObjectPtr& object = *it;
			if ( object->getType()== "StaticEntity" )
			{	
			
				Fairy::StaticEntityObject* entity = static_cast<Fairy::StaticEntityObject*>(object.get());
				Ogre::SceneNode* parentNode = entity->getSceneNode();
				if(parentNode)
					parentNode->setVisible( lshowType & ENUM_OBJ_TYPE::STATICENTITY_TYPE );

			}


			if( object->getType()==  "TerrainLiquid")
			{
				Fairy::TerrainLiquidObject* liquid = static_cast< Fairy::TerrainLiquidObject *>(object.get());
				Fairy::TerrainLiquid * parentNode = liquid->getTerrainLiquid();
				if(parentNode)
					parentNode->setVisible(lshowType & ENUM_OBJ_TYPE::TERRAINLIQUIDOBJ_TYPE);
			}


			if( object->getType()==  "Effect")  
			{
				Fairy::EffectObject* effect = static_cast<Fairy::EffectObject*>(object.get());
				Ogre::SceneNode* parentNode = effect->getSceneNode();
				if(parentNode)
					parentNode->setVisible(lshowType & ENUM_OBJ_TYPE::EFFECTOBJ_TYPE);
			}



			if(object->getType()==  "ParticleSystem")  
			{
				Fairy::ParticleSystemObject* particle = static_cast<Fairy::ParticleSystemObject*>(object.get());
				Ogre::SceneNode* parentNode = particle->getSceneNode();
				if(parentNode)
					parentNode->setVisible( lshowType & ENUM_OBJ_TYPE::PARTICLE_TYPE);
			}

		}
		
			return;
}

// 检测是否要透明
void CEngineInterface::CheckObjectTransparent()
{
	if(!m_pFairySystem)
		return ;

	CObjectManager* objMng = CObjectManager::GetMe();
	fVector3 fvPos;
	
	if( objMng->GetMySelf()->GetRenderInterface() )
	{
		objMng->GetMySelf()->GetRenderInterface()->Actor_GetLocator( GetCharaLocatorName(LOCATOR_CHAR_ATTACK), fvPos );		// 取人物绑定点的位置， 判断起来比较合理	"人物身体受击点"
	}

	Axis_Trans(tGfxSystem::AX_GAME, fvPos, tGfxSystem::AX_GFX, fvPos);

	Ogre::Camera* pOgreCamera = m_pFairySystem->getCamera();
	Ogre::Vector3 origin = pOgreCamera->getPosition();

	// 屏掉下边这句了by czg 200911-30
	m_pFairySystem->makeObjectTransparent(origin, Ogre::Vector3(fvPos.x, fvPos.y, fvPos.z), 0.8, 1, NULL);
}

VOID CEngineInterface::Scene_ShowGridSwitch(INT nZoneSize)
{
	if(m_pTerrainGrid == NULL)
	{
		m_pTerrainGrid = new Fairy::TerrainGrid(m_pFairySystem->getSceneManager(), nZoneSize);
	}

	//显示网格
	m_pTerrainGrid->setActive(!(m_pTerrainGrid->isActive()));
}


void StringConvert::mbcs_to_utf8(const std::string& strIn, std::string& strOut)
{
	//设置语言环境
	G_AUTO_LOCALE_CONVERT theAutoConvert;
	strOut = utf::mbcs_to_utf8(strIn);
}

void StringConvert::utf8_to_mbcs(const std::string& strIn, std::string& strOut)
{
	//设置语言环境
	G_AUTO_LOCALE_CONVERT theAutoConvert;
	strOut = utf::utf8_to_mbcs(strIn);
}


void MAP_POS_DEFINE::Set_Name(const char* strName)	
{
	_snprintf( name, 128, "%s", strName);
}
void MAP_POS_DEFINE::Set_Pos_Name(const char* strName)	
{
	_snprintf( name, 128, "%s", strName);
	std::string strmbcs = name;
	std::string strutf8;
	StringConvert::mbcs_to_utf8(strmbcs,strutf8);
	_snprintf(utf8name,128,strutf8.c_str());
}

void MAP_POS_DEFINE::Set_Pos_Utf8Name(const char* strName)	
{
	_snprintf( utf8name, 128, "%s", strName);
}


