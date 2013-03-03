#include "StdAfx.h"
#include "FakeObjectEntityManager.h"

#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreVector2.h>
#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreRenderSystem.h>
#include <OgreOverlayElement.h>
#include <OgreCommon.h>
#include <OgreRenderTargetListener.h>
#include <OgreColourValue.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreMaterialManager.h>
#include <OgreAxisAlignedBox.h>

#include "Core/FairyFlags.h"
#include "AxTrace.h"
#include "Core/FairySystem.h"
#include "GIException.h"
#include "GIGfxSystem.h"
#include "../../Engine/UIMapLight.h"
#include "../../Engine/EngineInterface.h"
#include "../Entity/ObjEntityActor.h"
#include "GIDataBase.h"
#include "GIDBC_Struct.h"
#include "Global.h"
#include "ModelSystem/FairyLogicModel.h"

//=====================================================================
//Ogre ViewPort��Ⱦ����
class CFakeRenderTargetListener : public Ogre::RenderTargetListener
{
public:
	virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		Ogre::SceneManager* pSceneManager = CEngineInterface::GetMe()->GetFairySystem()->getSceneManager();
		CFakeObjectEntityManager::FakeObject* pObj = CFakeObjectEntityManager::GetMe()->GetObjectAsViewPort(evt.source);
		if(!pObj) return;

		//----------------------------------------------------------
		//����VisibleFlag
		m_nRenderMask = pSceneManager->getVisibilityMask();
		pSceneManager->setVisibilityMask(Fairy::OVF_GUI_ELEMENTS);
		CFakeObjectEntityManager::GetMe()->SetActiveObject(pObj);
		//pObj->pEntityNode->SetGfxPosition(fVector3(0, 0, 0));
		//���ص�ͼ̽��
		CUIMapLight::GetMe()->Show(false);

		//----------------------------------------------------------
		//���û�����
		m_colAmbient = pSceneManager->getAmbientLight();
		pSceneManager->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));
		m_pLightModel->setPosition(pObj->pCamera->getPosition());
	}

	virtual void postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		Ogre::SceneManager* pSceneManager = CEngineInterface::GetMe()->GetFairySystem()->getSceneManager();

		CFakeObjectEntityManager::FakeObject* pObj = CFakeObjectEntityManager::GetMe()->GetObjectAsViewPort(evt.source);
		if(!pObj) return;

		//----------------------------------------------------------
		//�ָ�VisibleFlag
		pSceneManager->setVisibilityMask(m_nRenderMask);
		CFakeObjectEntityManager::GetMe()->SetActiveObject(0);

		//----------------------------------------------------------
		//�ָ�������
		pSceneManager->setAmbientLight(m_colAmbient);

#if 0
		{
			static int index = 0;
			static std::string strLast;
			if(index == 10)
			{
				CHAR szFileName[MAX_PATH];
				_snprintf(szFileName, MAX_PATH, "c:\\%s.tga", pObj->strName.c_str());
				pObj->pRenderTexture->writeContentsToFile(szFileName);
			}
			index++;//

			if(strLast != pObj->strName)
			{
				strLast = pObj->strName;
				index=0;
			}

		}
#endif		
	}

	//����ר�õƹ�
	void createSpecialLight(void)
	{
		m_pLightModel = 
			CEngineInterface::GetMe()->GetFairySystem()->getSceneManager()->createLight("Light_ForFakeSystem");
		KLAssert(m_pLightModel);

		//����VisibleFlag
		m_pLightModel->setVisibilityFlags(Fairy::OVF_GUI_ELEMENTS);
	}

	//���ٵƹ�
	void destroySpecialLight(void)
	{
		CEngineInterface::GetMe()->GetFairySystem()->getSceneManager()->destroyLight(m_pLightModel);
	}

	CFakeRenderTargetListener()
		:m_pLightModel(0)
	{
	}
protected:
	//������ʱ���泡����Ⱦ����

	//Visible Flag
	unsigned int			m_nRenderMask;
	//���������
	Ogre::ColourValue		m_colAmbient;
	//��������ģ�͵ĵƹ�
	Ogre::Light*			m_pLightModel;
};
CFakeRenderTargetListener	g_theListener;


//=====================================================================
CFakeObjectEntityManager*	CFakeObjectEntityManager::s_pMe = NULL;
CFakeObjectEntityManager::CFakeObjectEntityManager()
{
	s_pMe = this;
	m_fvPosition = Ogre::Vector3(0.f, 0.f, 0.f);
}

CFakeObjectEntityManager::~CFakeObjectEntityManager()
{
	m_FakeObjectPositionMap.clear();
}

// ��ʼ��
VOID  CFakeObjectEntityManager::Initial(VOID)
{
	//����ר�õƹ�
	g_theListener.createSpecialLight();
}

void  CFakeObjectEntityManager::Release()
{
	FakeObjectMap::iterator it = m_mapObject.begin();

	for(it=m_mapObject.begin(); it!=m_mapObject.end(); it++)
	{
		FakeObject& obj = it->second;
		//���ٶ���
		_DestroyFakeNode(obj);
	}
	m_mapObject.clear();

	//���ٵƹ�
	g_theListener.destroySpecialLight();
}

// ��һ������ŵ�ָ������Ӱ��λ��
void CFakeObjectEntityManager::CreateObject(LPCTSTR szObjName, CObjEntityActor* pNode, CObjEntityActor* pAttachNode, LPCTSTR szCameraName, int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName)
{
	// �ȸ��ݸ�fake object����������ȡ�ڷ�λ��
	m_fvPosition = Ogre::Vector3(_GetFakeObjPos(szObjName), 10000.f, 0.f);

	//���岻���ڣ������µ���Ⱦ����
	FakeObjectMap::iterator it = _GetFakeNode(szObjName, pNode, szCameraName, nTexWidth, nTexHeight, szBackgroundName);

	if(it == m_mapObject.end()) return;

	it->second.pEntityNode = pNode;
	it->second.pAttachEntityNode = pAttachNode;
	//����Ҫ��Ⱦ��ʵ�������Ⱦ������

	// ����������ϵģ�Ҫ�������λ�ã����˵�λ�ø�λ
	Fairy::LogicModel* modelImpl = pNode->GetActorImpl();

	if (modelImpl->isAttached())
	{
		Fairy::LogicModel* parentModel = modelImpl->getAttachModel();

		KLAssert (parentModel);

		modelImpl->setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));

		parentModel->setPosition(Ogre::Vector3(m_fvPosition.x, m_fvPosition.y, m_fvPosition.z));
		parentModel->setVisible(true);
	}
	else
		pNode->SetGfxPosition(fVector3(m_fvPosition.x, m_fvPosition.y, m_fvPosition.z));

	//��ʼ����ʾ�������й����л�һֱ����visibleΪtrue��״̬
	pNode->SetVisible(TRUE);
}

void CFakeObjectEntityManager::SetActiveObject(FakeObject* pObj)
{
	FakeObjectMap::iterator it;
	for(it=m_mapObject.begin(); it!=m_mapObject.end(); it++)
	{
		FakeObject& theObj = it->second;

		bool visible = &theObj == pObj;

		//theObj.pEntityNode->SetVisible(visible);
		//if(theObj.pAttachEntityNode)
		//{
		//	theObj.pAttachEntityNode->SetVisible(visible);
		//}

		//���ø�FakeObject��Ӧ��Rectangle2D����ʾ������
		if (theObj.pRectange)
			theObj.pRectange->setVisible(visible);
	}
}

bool CFakeObjectEntityManager::SetAttachObj(CObjEntityActor* pNode, CObjEntityActor* pAttachNode)
{
	FakeObjectMap::iterator it;
	for(it=m_mapObject.begin(); it!=m_mapObject.end(); it++)
	{
		FakeObject& theObj = it->second;

		if(theObj.pEntityNode == pNode)
		{
			theObj.pAttachEntityNode = pAttachNode;
			return true;
		}
	}

	return false;
}

VOID CFakeObjectEntityManager::Debug_SetFakeCamera(int nType, float fValue)
{
	FakeObjectMap::iterator it = m_mapObject.find("My_Pet");
	if(it == m_mapObject.end()) return;

	FakeObject& obj = it->second;

	Ogre::Camera* pCam = it->second.pCamera;

#if 1
	switch(nType)
	{
	case 2:
		{

			obj.fCameraHeight += fValue;
			_UpdateCamera(obj);

			AxTrace(0, 0, "Height:%f", obj.fCameraHeight);
		}
		break;

	case 3:
		{
			obj.fCameraDistance += fValue;
			_UpdateCamera(obj);

			AxTrace(0, 0, "Distance:%f", obj.fCameraDistance);
		}
		break;
	case 4:
		{
			obj.fCameraPitch += fValue;
			_UpdateCamera(obj);

			AxTrace(0, 0, "Pitch:%f", obj.fCameraPitch);
		}
		break;
	default:
		return;
	}
#endif
}

void CFakeObjectEntityManager::SetVisible(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio)
{
	FakeObjectMap::iterator it = m_mapObject.find(szObjName);
	if(it == m_mapObject.end()) return;
	FakeObject& obj = it->second;

	if(bVisible)
	{
		//���������AspectRatio
		obj.pCamera->setAspectRatio(fAspectRatio);
	}

	//�����Ƿ��Զ�ˢ��
	if(!obj.ptrRenderTexture.isNull() && obj.ptrRenderTexture->getBuffer()->getRenderTarget())
	{
		obj.ptrRenderTexture->getBuffer()->getRenderTarget()->setAutoUpdated(bVisible==TRUE);
	}
}

CFakeObjectEntityManager::FakeObjectMap::iterator CFakeObjectEntityManager::_GetFakeNode(LPCTSTR szNodeName, tEntityNode* pNode, LPCTSTR szCameraName, int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName)
{
	//ȱʡ�������λ��
	static const float s_fHeight	= 0.8f;
	static const float s_fDistance = 3.2f;
	static const float s_fPitch = 0.21f;

	FakeObjectMap::iterator it = m_mapObject.find(szNodeName);
	if(it != m_mapObject.end()) return it;

	//�����ڣ�����
	FakeObject newNode;
	newNode.strName = szNodeName;

	//--------------------------------------------------
	//����RenderTarget
	Ogre::TexturePtr ptrTex =
		Ogre::TextureManager::getSingleton().createManual(
		Ogre::String(szNodeName) + "_RenderTexture",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		nTexWidth, nTexHeight, 1, 0,
		Ogre::PF_R8G8B8A8,
		Ogre::TU_RENDERTARGET,
		0);

	//ptrTex->load();
	newNode.ptrRenderTexture = ptrTex;
	Ogre::RenderTexture* pTexture = ptrTex->getBuffer()->getRenderTarget();

	//ȱʡ��ˢ��
	pTexture->setAutoUpdated(false);
	pTexture->addListener(&g_theListener);

	//--------------------------------------------------
	//���������
	Ogre::SceneManager* pScnManager = CEngineInterface::GetMe()->GetFairySystem()->getSceneManager();
	newNode.pCamera = pScnManager->createCamera(Ogre::String(szNodeName) + "_Camera");

	//����ϵ��
	fVector3 fvScale = CEngineInterface::GetMe()->GetSacle();

	newNode.fCameraHeight=s_fHeight;
	newNode.fCameraDistance=s_fDistance;
	newNode.fCameraPitch=s_fPitch;

	STRING szUserCameraValue;
	pNode->Actor_GetObjectProperty(szCameraName, szUserCameraValue);
	//	if(szUserCameraValue.size() > 2 && szUserCameraValue.find(';') != STRING::npos)
	//	{
	//		sscanf(szUserCameraValue.c_str(), "%f;%f", &(newNode.fCameraHeight), &(newNode.fCameraDistance));
	//	}
	int Row_Index;
	Row_Index = atoi(szUserCameraValue.c_str());

	const tDataBase* pDBC = g_pDataBase->GetDataBase(DBC_MODEL_PARAMETER);
	KLAssert(pDBC);
	const _DBC_MODEL_PARAMETER* pParameter = NULL;

	pParameter = (const _DBC_MODEL_PARAMETER*)((tDataBase*)pDBC)->Search_Index_EQU(Row_Index);

	if(pParameter)
	{
		newNode.fCameraHeight = pParameter->nHeight;
		newNode.fCameraDistance = pParameter->nDistance;
	}

	//���������
	_UpdateCamera(newNode);

	newNode.pCamera->setNearClipDistance(10.f);
	newNode.pCamera->setAspectRatio((float)nTexWidth/nTexHeight);
	newNode.pCamera->setFOVy(Ogre::Degree(45.0f));	// ����ֵ
	newNode.pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);	//͸��ͶӰ (ƽ��ͶӰ Ogre::PT_ORTHOGRAPHIC)

	//--------------------------------------------------
	//����ViewPort
	newNode.pViewPort = pTexture->addViewport(newNode.pCamera, 1);
	newNode.pViewPort->setClearEveryFrame(true);	
	newNode.pViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
	newNode.pViewPort->setOverlaysEnabled(false);
	newNode.pViewPort->setSkiesEnabled(false);
	newNode.pViewPort->setShadowsEnabled(false);

	//--------------------------------------------------
	//����rectangle(����������Ʋ�Ϊ�ղ��������material template����)
	Ogre::String backgroundTexName(szBackgroundName);

	Ogre::MaterialPtr originMat = Ogre::MaterialManager::getSingleton().getByName("UIModelBackground");

	if (false == backgroundTexName.empty() && false == originMat.isNull())
	{
		newNode.pRectange = new Ogre::Rectangle2D(true);
		newNode.pRectange->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);

		Ogre::String cloneMatName = Ogre::String(szNodeName) + "_Rectangle";
		Ogre::MaterialPtr cloneMat = Ogre::MaterialManager::getSingleton().getByName(cloneMatName);

		if (cloneMat.isNull())
		{
			cloneMat = originMat->clone(cloneMatName);

			if (cloneMat->getNumTechniques())
			{
				Ogre::Technique* tech = cloneMat->getTechnique(0);

				if (tech->getNumPasses())
				{
					Ogre::Pass* pass = tech->getPass(0);

					if (pass->getNumTextureUnitStates())
					{
						Ogre::TextureUnitState* tex = pass->getTextureUnitState(0);

						tex->setTextureName(szBackgroundName);
					}
				}
			}

		}  

		newNode.pRectange->setMaterial(cloneMat->getName());

		newNode.pRectange->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
		newNode.pRectange->setVisibilityFlags(Fairy::OVF_GUI_ELEMENTS);

		// ����boundingboxΪ���޴��Է���camera�޳�����Ĭ�ϵİ�Χ�д�СΪ-1��1��
		newNode.pRectange->setBoundingBox( 
			Ogre::AxisAlignedBox( 
			Ogre::Vector3(Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY),
			Ogre::Vector3(Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY) 
			) );

		Ogre::SceneNode* parentNode = 
			CEngineInterface::GetMe()->GetFairySystem()->getBaseSceneNode()->createChildSceneNode();

		parentNode->attachObject(newNode.pRectange);
	}

	//--------------------------------------------------
	//����Map
	m_mapObject.insert(std::make_pair(newNode.strName, newNode));

	it = m_mapObject.find(newNode.strName);
	KLAssert(it != m_mapObject.end());

	//��������Map
	m_mapIndexOfViewPort.insert(std::make_pair(newNode.pViewPort, &(it->second)));
	return it;
}

//���������
void CFakeObjectEntityManager::_UpdateCamera(FakeObject& node)
{
	//����ϵ��
	fVector3 fvScale = CEngineInterface::GetMe()->GetSacle();

	//Lookat
	Ogre::Vector3 fvLookAtPos = m_fvPosition;
	fvLookAtPos += Ogre::Vector3(0.0f, node.fCameraHeight*fvScale.y, 0.0f);

	//EyePos
	Ogre::Vector3 fvEyePosPos = m_fvPosition;
	fvEyePosPos += Ogre::Vector3(0.0f, 
		(node.fCameraHeight+node.fCameraDistance*node.fCameraPitch)*fvScale.y, node.fCameraDistance*fvScale.z);

	node.pCamera->setPosition(fvEyePosPos);
	node.pCamera->lookAt(fvLookAtPos);
}

void CFakeObjectEntityManager::_DestroyFakeNode(FakeObject& obj)
{
	//���������
	CEngineInterface::GetMe()->GetFairySystem()->getSceneManager()->destroyCamera(obj.pCamera);

	//����ViewPort
	if(!obj.ptrRenderTexture.isNull() && obj.ptrRenderTexture->getBuffer()->getRenderTarget())
	{
		obj.ptrRenderTexture->getBuffer()->getRenderTarget()->removeAllViewports();
	}

	//����RenderTexture
	obj.ptrRenderTexture.setNull();
	Ogre::TextureManager::getSingleton().remove(
		Ogre::String(obj.strName) + "_RenderTexture");

	//����Rectangle2D
	if (obj.pRectange)
	{
		Ogre::SceneNode* parentNode = obj.pRectange->getParentSceneNode();

		if (parentNode)
		{
			parentNode->detachObject(obj.pRectange);

			// �ݻ�Rectangle2D���ҽӵ�scene node
			if (parentNode->getParentSceneNode())
				parentNode->getParentSceneNode()->removeAndDestroyChild(parentNode->getName());
		}

		delete obj.pRectange;
		obj.pRectange = NULL;
	}
}

float CFakeObjectEntityManager::_GetFakeObjPos(LPCTSTR szName)
{
	/**
	CFakeObjSystem::NewFakeObject()���Ѿ���֤�˴����szName��ȫ��Ψһ�ԡ�
	fake object�ڷŵ������-10000.0��object֮��ĵļ����1000.0���ڷŵ�x��λ�ó�+x�������
	*/
	FakeObjectPositionMap::iterator it = m_FakeObjectPositionMap.find(szName);

	if (it != m_FakeObjectPositionMap.end())
	{
		return it->second;
	}
	else
	{
		static int sObjectCount = 0;
		const float OBJECTDISTANCE = 1000.0f;

		const float OBJSTARTPOS = -10000.0f;

		float objPos = OBJSTARTPOS + sObjectCount * OBJECTDISTANCE;
		m_FakeObjectPositionMap.insert( FakeObjectPositionMap::value_type(szName, objPos) );

		++sObjectCount;

		return objPos;
	}
}

//����
void CFakeObjectEntityManager::DestroyObject(LPCTSTR szObjName)
{
	FakeObjectMap::iterator it = m_mapObject.find(szObjName);
	if(it == m_mapObject.end()) return;

	//�ȴ���������ɾ��
	std::map< Ogre::Viewport*, FakeObject* >::iterator itIndex;
	itIndex = m_mapIndexOfViewPort.find(it->second.pViewPort);
	if(itIndex != m_mapIndexOfViewPort.end())
	{
		m_mapIndexOfViewPort.erase(itIndex);
	}

	//ɾ���ڵ�����
	it->second.pEntityNode->SetVisible(FALSE);
	_DestroyFakeNode(it->second);

	m_mapObject.erase(it);
}

CFakeObjectEntityManager::FakeObject*	CFakeObjectEntityManager::GetObjectAsViewPort(Ogre::Viewport* pViewPort)
{
	std::map< Ogre::Viewport*, FakeObject* >::iterator it;
	it = m_mapIndexOfViewPort.find(pViewPort);

	if(it == m_mapIndexOfViewPort.end()) return NULL;

	return it->second;
}

bool CFakeObjectEntityManager::GetCamera(LPCTSTR szObjName, float& fHeight, float& fDistance, float& fPitch)
{
	FakeObjectMap::iterator it = m_mapObject.find(szObjName);
	if(it == m_mapObject.end()) return false;

	FakeObject& obj = it->second;
	fHeight = obj.fCameraHeight;
	fDistance = obj.fCameraDistance;
	fPitch = obj.fCameraPitch;

	return true;
}

bool CFakeObjectEntityManager::SetCamera(LPCTSTR szObjName, float fHeight, float fDistance, float fPitch)
{
	FakeObjectMap::iterator it = m_mapObject.find(szObjName);
	if(it == m_mapObject.end()) return false;

	FakeObject& obj = it->second;

	obj.fCameraHeight = fHeight;
	obj.fCameraDistance = fDistance;
	obj.fCameraPitch = fPitch;

	_UpdateCamera(obj);
	return true;
}
