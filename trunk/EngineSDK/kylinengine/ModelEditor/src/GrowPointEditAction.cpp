#include "GrowPointEditAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "CustomShape.h"
#include "WXOperatorManager.h"       // redo undo ������.
#include "GrowPointOperator.h"
#include "TriFunc.h"

#include "Core/TerrainData.h"
#include "ModelSystem/FairyLogicModelManager.h"
#include "ModelSystem/FairyLogicModel.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h> 
#include <OgreCamera.h>
#include <OgreMaterialManager.h>


namespace Fairy 
{
	using namespace Ogre;

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// growpoint info
	// ��������Ϣ
	//

	CGrowPointInfo::CGrowPointInfo()
		: m_pSceneNode(NULL)
		, m_pEntity(NULL)
		, m_pSelectedMaterial(NULL)
		, m_pOldMaterial(NULL)
		, m_bIsRecreateMaterial(true)
		, m_pParentSceneNode(NULL)
		, m_pModel(NULL)

	{
	}
	
	CGrowPointInfo::~CGrowPointInfo()
	{

	}

	
	// ����һ�������㡣
	bool CGrowPointInfo::CreateGrowPoint(const std::string& strMeshName)
	{
		// �ļ������Ȳ���<=4
		if( strMeshName.size() <= 4 ) return false;

		if(m_pParentSceneNode)
		{
			m_pSceneNode = m_pParentSceneNode->createChildSceneNode();
		}


		// �ͷ�ʵ����Դ.
		if(m_pSceneNode)
		{
			Ogre::SceneManager* creator = m_pSceneNode->getCreator();
			static unsigned long dwId = 0;
			char buf[128];

			if(creator)
			{
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "GrowPoint_%d", dwId);
				dwId++;

				if( strMeshName.substr( strMeshName.size() - 3 ) == "obj" )
				{
					m_pModel = Fairy::LogicModelManager::getSingleton().createLogicModel(strMeshName);

					m_pModel->setParentSceneNode(m_pSceneNode);

					return true;
				}
				else if( strMeshName.substr( strMeshName.size() - 4 ) == "mesh" )
				{
					m_pEntity = creator->createEntity(buf, strMeshName.c_str());

					if(m_pEntity)
					{
						m_pSceneNode->attachObject(m_pEntity);
						return true;
					}
					else
					{
						m_pParentSceneNode->removeAndDestroyChild(m_pSceneNode->getName());
					}
				}
				else
				{
					return false;
				}
			}	
			
		}
		
		return false;
	}

	// ����λ��
	void CGrowPointInfo::SetGrowPointPos(Ogre::Vector3 pos)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setPosition(pos);
			m_position = pos;
		}
	}

	// ���ø��ڵ�
	void CGrowPointInfo::SetParentSceneNode(Ogre::SceneNode* pParentSceneNode)
	{
		m_pParentSceneNode = pParentSceneNode;

		//////*********************************************************************************888/*/
		//////*********************************************************************************888/*/
		//////*********************************************************************************888/*/
		//////*********************************************************************************888/*/
		//////*********************************************************************************888/*/
		//for(int i = 0; i < 10; i++)
		//{
		//	if(m_pParentSceneNode)
		//	{
		//		Ogre::SceneNode*	pNode;	
		//		Ogre::Entity*		pEntity;
		//		pNode = m_pParentSceneNode->createChildSceneNode();
		//	
		//		Ogre::SceneManager* creator = pNode->getCreator();
		//		static unsigned long dwId = 0;
		//		char buf[128];
		//		memset(buf, 0, sizeof(buf));
		//		sprintf(buf, "GrowrrrPoint_%d", dwId);
		//		dwId++;
		//		pEntity = creator->createEntity(buf, "֩���.mesh");

		//		pNode->attachObject(pEntity);
		//		Ogre::Vector3 pos;
		//		pos.x = 100;
		//		pos.y = 0;
		//		pos.z = i * 300;
		//		pNode->setPosition(pos);
		//		
		//	}
		//}//
	}


	// �ͷ����������Դ
	void CGrowPointInfo::ReleaseGrowPoint()
	{

		// �ͷ�ʵ����Դ.
		if(m_pSceneNode)
		{
			if( m_pModel )
			{
				Fairy::LogicModelManager::getSingleton().destroyLogicModel( m_pModel );
				m_pModel = NULL;
			}

			if( m_pEntity )
			{
				Ogre::SceneManager* creator = m_pSceneNode->getCreator();
				m_pSceneNode->detachObject((Ogre::MovableObject *)m_pEntity);
				
				if(creator)
				{
					creator->destroyEntity(m_pEntity);
				}

				m_pEntity = NULL;
			}
		}

		if((m_pParentSceneNode)&&(m_pSceneNode))
		{
			m_pParentSceneNode->removeAndDestroyChild (m_pSceneNode->getName());

			m_pSceneNode = NULL;

		}

		m_bIsRecreateMaterial = true;

		// �ͷŽڵ���Դ.
	}

		// �Ƿ�ѡ��.
	bool CGrowPointInfo::IsSelected(Ogre::Ray& ray, float& fdis)
	{
		float fu = 0;
		float fv = 0;
		float fd = 0;
		bool  bRet = false;

		if(NULL == m_pEntity && NULL == m_pModel)
		{
			return false;
		}

		const AxisAlignedBox& BindBox = m_pEntity ? m_pEntity->getWorldBoundingBox() : m_pModel->getBoundingBox();
		
		Ogre::Vector3 v1; 
		Ogre::Vector3 v2; 
		Ogre::Vector3 v3;

		const Ogre::Vector3 dir  = ray.getDirection();
		const Ogre::Vector3 orig = ray.getOrigin();

		const Ogre::Vector3 vMax = BindBox.getMaximum(); 
		const Ogre::Vector3 vMin = BindBox.getMinimum() ;
		 
		float fDisTance = 100000.0f;

		////////////////////////////////////////////////////////////////////////////////////////
		// ȡmaxZ
		v1.x = vMin.x;
		v1.y = vMin.y; 
		v1.z = vMax.z; 

		v2.x = vMin.x; 
		v2.y = vMax.y; 
		v2.z = vMax.z; 

		v3.x = vMax.x; 
		v3.y = vMin.y; 
		v3.z = vMax.z; 

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		// ȡmaxZ
		v1.x = vMax.x;
		v1.y = vMax.y;
		v1.z = vMax.z;

		v2.x = vMin.x;
		v2.y = vMax.y;
		v2.z = vMax.z;

		v3.x = vMax.x;
		v3.y = vMin.y;
		v3.z = vMax.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡminZ
		v1.x = vMax.x;
		v1.y = vMax.y;
		v1.z = vMin.z;

		v2.x = vMin.x;
		v2.y = vMax.y;
		v2.z = vMin.z;

		v3.x = vMax.x;
		v3.y = vMin.y;
		v3.z = vMin.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		// ȡminZ
		v1.x = vMax.x;
		v1.y = vMax.y;
		v1.z = vMin.z;

		v2.x = vMin.x;
		v2.y = vMax.y;
		v2.z = vMin.z;

		v3.x = vMax.x;
		v3.y = vMin.y;
		v3.z = vMin.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMaxX
		v1.x = vMax.x;
		v1.y = vMin.y;
		v1.z = vMin.z;

		v2.x = vMax.x;
		v2.y = vMax.y;
		v2.z = vMin.z;

		v3.x = vMax.x;
		v3.y = vMin.y;
		v3.z = vMax.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMaxX
		v1.x = vMax.x;
		v1.y = vMax.y;
		v1.z = vMax.z;

		v2.x = vMax.x;
		v2.y = vMax.y;
		v2.z = vMin.z;

		v3.x = vMax.x;
		v3.y = vMin.y;
		v3.z = vMax.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMinX
		v1.x = vMin.x;
		v1.y = vMin.y;
		v1.z = vMin.z;

		v2.x = vMin.x;
		v2.y = vMax.y;
		v2.z = vMin.z;

		v3.x = vMin.x;
		v3.y = vMin.y;
		v3.z = vMax.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMinX
		v1.x = vMin.x;
		v1.y = vMax.y;
		v1.z = vMax.z;

		v2.x = vMin.x;
		v2.y = vMax.y;
		v2.z = vMin.z;

		v3.x = vMin.x;
		v3.y = vMin.y;
		v3.z = vMax.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMaxY
		v1.x = vMin.x;
		v1.y = vMax.y;
		v1.z = vMin.z;

		v2.x = vMin.x;
		v2.y = vMax.y;
		v2.z = vMax.z;

		v3.x = vMax.x;
		v3.y = vMax.y;
		v3.z = vMin.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMaxY
		v1.x = vMax.x;
		v1.y = vMax.y;
		v1.z = vMax.z;

		v2.x = vMin.x;
		v2.y = vMax.y;
		v2.z = vMax.z;

		v3.x = vMax.x;
		v3.y = vMax.y;
		v3.z = vMin.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMinY
		v1.x = vMin.x;
		v1.y = vMin.y;
		v1.z = vMin.z;

		v2.x = vMin.x;
		v2.y = vMin.y;
		v2.z = vMax.z;

		v3.x = vMax.x;
		v3.y = vMin.y;
		v3.z = vMin.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMinY
		v1.x = vMax.x;
		v1.y = vMin.y;
		v1.z = vMax.z;

		v2.x = vMin.x;
		v2.y = vMin.y;
		v2.z = vMax.z;

		v3.x = vMax.x;
		v3.y = vMin.y;
		v3.z = vMin.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		fdis = fDisTance;
		return bRet;
	}


	// ѡ��
	void CGrowPointInfo::SetSelMaterial()
	{
		if( m_pModel )
		{
			if( m_bIsRecreateMaterial )
			{
				m_pModel->setSelected( true );
				m_bIsRecreateMaterial = false;
			}
			else
			{
				m_pModel->setSelected( false );
				m_bIsRecreateMaterial = true;
			}
			return;
		}

		if(m_pSelectedMaterial.isNull()||(m_bIsRecreateMaterial))
		{
			// ѡ�в��ʵ�����.
			String strCloneName;
			if(m_pEntity)
			{
				Ogre::SubEntity* pSubEntiy = m_pEntity->getSubEntity(0);
				
				if(pSubEntiy)
				{
					
					Ogre::MaterialPtr pMaterial = pSubEntiy->getMaterial();
					if(pMaterial.isNull())
					{
						return;
					}//

					const String& strName = pMaterial->getName();
					strCloneName = strName;
					strCloneName += "_select";
						
					MaterialManager* pMaterialManager = (MaterialManager*)(pMaterial->getCreator());

					if(NULL == pMaterialManager)
					{
						return;
					}

					Ogre::MaterialPtr pMaterialClone = pMaterialManager->getByName(strCloneName); 
						
					if(pMaterialClone.isNull())
					{
						pMaterialClone = pMaterial->clone(strCloneName);
					}
					

					Ogre::Technique* pTechnique = pMaterialClone->getBestTechnique();
					Ogre::Pass* pPass = pTechnique->getPass(0);

					//pPass->setSceneBlending(SBT_ADD);
					//pPass->setSceneBlending(SBF_SOURCE_ALPHA , SBF_ONE_MINUS_SOURCE_ALPHA );
					//pTextureState->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1, Transparence, 1);//
				
					Ogre::ColourValue color(0.5, 0.2, 0.8, 1.0);
					TextureUnitState* pTextureState = pPass->getTextureUnitState(0);
					pTextureState->setColourOperationEx(LBX_ADD , LBS_TEXTURE , LBS_MANUAL, color, color );
					
					m_pSelectedMaterial = pMaterialClone;
					m_pOldMaterial      = pMaterial;
					
					pSubEntiy->setMaterialName(m_pSelectedMaterial->getName());

					m_bIsRecreateMaterial = false;

								
				}
			}
		}
		else
		{
		
			if(m_pEntity)
			{
				Ogre::SubEntity* pSubEntiy = m_pEntity->getSubEntity(0);
				
				if(pSubEntiy)
				{
					pSubEntiy->setMaterialName(m_pSelectedMaterial->getName());	
					
				}

			}
				
		}
	}


	// δѡ��.
	void CGrowPointInfo::SetUnSelMaterial()
	{
		m_bIsRecreateMaterial = true;

		if( m_pModel )
		{
			m_pModel->setSelected( false );
			return;
		}
		
		if((m_pEntity)&&(!m_pOldMaterial.isNull()))
		{
			Ogre::SubEntity* pSubEntiy = m_pEntity->getSubEntity(0);
			
			if(pSubEntiy)
			{
				pSubEntiy->setMaterialName(m_pOldMaterial->getName());	
				
			}

		}
			
	}
		
	//
	// growpoint info
	// ��������Ϣ
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////








	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// action
	//
	//

	CGrowPointEditAction::CGrowPointEditAction(SceneManipulator* sceneManipulator)
		 : Action(sceneManipulator)
		 , m_pSceneNode(NULL)
		 , m_FuncKeyState(FUNCTION_KEY)
		 , m_bIsShow(false)
		 , m_dwGrowPointInstanceId(0)
		 , m_dwGrowPointTypeId(0xffffffff)
		 , m_dwCurSelGrowPointInstanceId(0xffffffff)
		 , m_bIsShowTempObject(false)
	{
		CreateSceneNode();
			
		// ���ü�����
		ResetCounter();
	}

	CGrowPointEditAction::~CGrowPointEditAction(void)
	{
	}



	// ��������༭�ĳ����ڵ�
	void CGrowPointEditAction::CreateSceneNode()
	{
//*/
		m_pSceneNode = getSceneManipulator()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);

		}//
/*/

		//getSceneManipulator()->getBaseSceneNode
		//m_pSceneNode = getSceneManipulator()->getBaseSceneNode()->createChildSceneNode();
		m_pSceneNode = getSceneManipulator()->getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(true);

		}

		for(int i = 0; i < 10; i++)
		{
			if(m_pSceneNode)
			{
				Ogre::SceneNode*	pNode;	
				Ogre::Entity*		pEntity;
				Ogre::Vector3 pos;
				pos.x = i * 300;
				pos.y = 0;
				pos.z = 0;
				//pNode->setPosition(pos);
				pNode = m_pSceneNode->createChildSceneNode(pos);
			
				Ogre::SceneManager* creator = pNode->getCreator();
				static unsigned long dwId = 0;
				char buf[128];
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "GrowrrrPoint_%d", dwId);
				dwId++;
				pEntity = creator->createEntity(buf, "֩���.mesh");
				//pEntity = creator->createEntity(buf, "spotLight.mesh");

				pNode->attachObject(pEntity);
				
				
			}
		}//
//*/
		
	}


	// ���ؽڵ㡣
	void CGrowPointEditAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}


	const String& CGrowPointEditAction::getName(void) const
	{
		//����һ���ַ������֣� �����жϵ�ǰ����������.
		static const String name = "GrowPointEdit";
		return name;
	}


	// ����ƶ��Ĳ���
    void CGrowPointEditAction::_onMove(const Point& pt)
	{
		// ����ѡ�е���ʱģ�͵Ĳ���.
		Ogre::Vector3 position;
	    bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
		{
			m_curPoint = position;
			m_tempGrowPoint.SetGrowPointPos(m_curPoint);
			
		}

		m_screenPt = pt;
		// ��չ��ܼ�
		ClearFuncKey();
	
	}

	// ��갴�µĲ���.
	void CGrowPointEditAction::_onBegin(const Point& pt)
	{
		m_screenPt = pt;
	}



    void CGrowPointEditAction::_onDrag(const Point& pt)
	{
		m_screenPt = pt;
		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
		}
		else//
		{
			
		}

		// ÿ��ִ�н�����չ��ܼ�
		ClearFuncKey();
			
	}



	// ���̧��Ĳ���
    void CGrowPointEditAction::_onEnd(const Point& pt, bool canceled)
	{
		m_screenPt = pt;

		if(!getActive())
		{
			return;
		}

		
		if(m_bIsShowTempObject)
		{
				
			unsigned long dwId = AddNewGrowPoint(m_dwGrowPointTypeId, m_curPoint);

			// ����redo undo ����.
			CGrowPointAddOperator* pAddNewOperator = NULL;
			pAddNewOperator = new CGrowPointAddOperator;

			pAddNewOperator->SetAction(this);
			pAddNewOperator->SetName(String("add_growPoint"));
			pAddNewOperator->SetInstanceId(dwId);
			pAddNewOperator->SetTypeId(m_dwGrowPointTypeId);
			pAddNewOperator->SetPosition(m_curPoint, m_curPoint);
			getOperatorManager()->addOperator(pAddNewOperator);
		}
		else
		{
			unsigned long dwId;
			if(GetSelectedObject(dwId))
			{
				SetUnSelectObjectLook(m_dwCurSelGrowPointInstanceId);
				SetSelectObjectLook(dwId);
				m_dwCurSelGrowPointInstanceId = dwId;
			
			}
		}
		
		// ��չ��ܼ�
		ClearFuncKey();

	}



    void CGrowPointEditAction::_onActive(bool active)
	{
		
		if (!active)
        {
          	
        }
		else
		{
	
		}
		
		//HideScene(active);
	}

	//  ��չ��ܼ�.
	void CGrowPointEditAction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}



	void CGrowPointEditAction::setParameter(const String& name, const String& value)
	{

		 if (name == "FUNC_KEY")
        {
           if("SHIFT" == value)
		   {
			   SetFuncKey(FUNC_KEY_SHIFT);
		   }
		   else if("CTRL" == value)
		   {
			   SetFuncKey(FUNC_KEY_CTRL);
		   }
		   else if("ATL" == value)
		   {
			   SetFuncKey(FUNC_KEY_ALT);
		   }
		   else if("ESC" == value)
		   {
				m_bIsShowTempObject = false;
				m_tempGrowPoint.ReleaseGrowPoint();
		   }
		
        }
		else if("DELETE_GROWPOINT" == name)
		{
			
			CGrowPointInfo GrowPointInfo;
			GrowPointInfo = m_GrowPointMap[m_dwCurSelGrowPointInstanceId];

			// ����redo del ����.
			CGrowPointDelOperator* pAddNewOperator = NULL;
			pAddNewOperator = new CGrowPointDelOperator;

			pAddNewOperator->SetAction(this);
			pAddNewOperator->SetName(String("del_growpoint"));
			pAddNewOperator->SetInstanceId(GrowPointInfo.m_dwInstanceId);
			pAddNewOperator->SetTypeId(GrowPointInfo.m_dwTypeId);
			pAddNewOperator->SetPosition(GrowPointInfo.m_position, GrowPointInfo.m_position);
			getOperatorManager()->addOperator(pAddNewOperator);
			
			DelGrowPoint(m_dwCurSelGrowPointInstanceId);
		}
	}

	// ���ð��µĹ��ܼ�
	void CGrowPointEditAction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	// ���ü�����
	void CGrowPointEditAction::ResetCounter()
	{
		m_dwGrowPointInstanceId = 0;
	}


	unsigned long CGrowPointEditAction::AddNewGrowPoint(const std::string& strMeshName, Ogre::Vector3 pos)
	{
		CGrowPointInfo GrowPointInfo;

		
		GrowPointInfo.SetParentSceneNode(m_pSceneNode);
		if(GrowPointInfo.CreateGrowPoint(strMeshName))
		{
			GrowPointInfo.SetGrowPointPos(pos);
			GrowPointInfo.m_dwInstanceId = m_dwGrowPointInstanceId;
			GrowPointInfo.m_dwTypeId     = m_dwGrowPointTypeId;
			
			
			m_GrowPointMap[m_dwGrowPointInstanceId] = GrowPointInfo;
			getSceneManipulator()->_fireAddNewGrowPoint(m_dwGrowPointTypeId, m_dwGrowPointInstanceId);

			m_dwGrowPointInstanceId++;
			return (m_dwGrowPointInstanceId - 1);
		}
		else
		{
			return 0xffffffff;
		}
	}

	unsigned long CGrowPointEditAction::AddNewGrowPoint(unsigned long dwGrowPointTypeId, Ogre::Vector3 pos)
	{
		if(m_typeMeshMap.count(dwGrowPointTypeId))
		{
			m_dwGrowPointTypeId = dwGrowPointTypeId;
			std::string strTypeName = m_typeMeshMap[dwGrowPointTypeId];
			
			unsigned long dwId = AddNewGrowPoint(strTypeName, pos);

			if(0xffffffff != dwId)
			{
				SetGrowPointPos(dwId, pos);
				//// ����redo undo ����.
				//CGrowPointOperator* pAddNewOperator = NULL;
				//pAddNewOperator = new CGrowPointOperator;
				//getOperatorManager()->addOperator(pAddNewOperator);//
			}

			return dwId;
		}
		else
		{
			return 0xffffffff;
		}
	}

	// ������͡�
	void CGrowPointEditAction::AddGrowPointMeshType(unsigned long dwId, std::string strName)
	{
		m_typeMeshMap[dwId] = strName;
	}

	// �ı����͡�
	void CGrowPointEditAction::ChangeGrowPointMeshType(unsigned long dwId, std::string strName)
	{
		// ���õ�ǰ�༭��ģ�͡�
		m_dwGrowPointTypeId = dwId;

		AddGrowPointMeshType(dwId, strName);

		m_bIsShowTempObject = true;
		m_tempGrowPoint.ReleaseGrowPoint();
		m_tempGrowPoint.SetParentSceneNode(m_pSceneNode);
		m_tempGrowPoint.CreateGrowPoint(strName);
		m_tempGrowPoint.SetGrowPointPos(m_curPoint);
		m_tempGrowPoint.SetSelMaterial();
	
	}

	// �����������λ��
	void CGrowPointEditAction::SetGrowPointPos(unsigned long dwGrowPointInstanceId, Ogre::Vector3 pos)
	{
		if(m_GrowPointMap.count(dwGrowPointInstanceId))
		{
			CGrowPointInfo& GrowPointInfo = m_GrowPointMap[dwGrowPointInstanceId];
			GrowPointInfo.SetGrowPointPos(pos);

		}
	}

	// �õ��������λ��
	Ogre::Vector3 CGrowPointEditAction::GetGrowPos(unsigned long dwGrowPointInstanceId)
	{

		Ogre::Vector3 pos(0, 0, 0);

		if(m_GrowPointMap.count(dwGrowPointInstanceId))
		{
            CGrowPointInfo& GrowPointInfo = m_GrowPointMap[dwGrowPointInstanceId];
			
			pos = GrowPointInfo.m_position;
			return pos;

		}

		return pos;
	}

	// ��һ��������.
	void CGrowPointEditAction::ViewGrowPos(unsigned long dwGrowPointInstanceId)
	{
		Ogre::Vector3 pos(0, 0, 0);

		if(m_GrowPointMap.count(dwGrowPointInstanceId))
		{
            CGrowPointInfo& GrowPointInfo = m_GrowPointMap[dwGrowPointInstanceId];
			
			pos = GrowPointInfo.m_position;
		
		}
		
	
		// �õ��͵������λ��.
		Ogre::Vector3 oldCameraPos = mSceneManipulator->getCamera()->getPosition();

		// �õ�����
		Ogre::Vector3 CameraDir = mSceneManipulator->getCamera()->getDirection();

		float fx = 0;
		float fz = 0;
		float fy = 0;

		if( abs(CameraDir.y) < 0.0001 )
		{
			fx = (oldCameraPos.y - pos.y) * CameraDir.x + pos.x;
			fz = (oldCameraPos.y - pos.y) * CameraDir.z + pos.z;
		}
		else
		{
			fx = ((oldCameraPos.y - pos.y) / CameraDir.y) * CameraDir.x + pos.x;
			fz = ((oldCameraPos.y - pos.y) / CameraDir.y) * CameraDir.z + pos.z;
		}
	
		// �۾���λ��
		Ogre::Vector3 posEye(0, 0, 0);

		// �����������λ��λ��
		posEye.x = fx;
		posEye.y = oldCameraPos.y;
		posEye.z = fz;

		mSceneManipulator->getCamera()->setPosition(posEye);

	}

	// �õ��������߼�λ��
	Ogre::Vector3 CGrowPointEditAction::GetGrowLogicPos(unsigned long dwGrowPointInstanceId)
	{
		Ogre::Vector3 pos(0, 0, 0);

		float fx = 0;
		float fy = 0;
		float fz = 0;

		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		fTerrainDataPosY = getTerrainData()->mPosition.y;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		if(m_GrowPointMap.count(dwGrowPointInstanceId))
		{
            CGrowPointInfo& GrowPointInfo = m_GrowPointMap[dwGrowPointInstanceId];
			
			pos = GrowPointInfo.m_position;
		
			// ���г���������߼�����ת��
			fx = (pos.x - fTerrainDataPosX) / fScale;
			fy = (pos.y - fTerrainDataPosY) / fScale;
			fz = (pos.z - fTerrainDataPosZ) / fScale;

			pos.x = fx;
			pos.y = fy;
			pos.z = fz;

		}

		return pos;

	}


	// �Ƿ�ѡ��������.
	bool CGrowPointEditAction::IsSelectObject(Ogre::Ray& ray, CGrowPointInfo& GrowPointInfo, float& fdis)
	{
		return GrowPointInfo.IsSelected(ray, fdis);
	}

		// �Ƿ�ѡ��������.
	bool CGrowPointEditAction::IsSelectObject(Ogre::Ray& ray, unsigned long dwGrowPointInstanceId, float& fdis)
	{
		if(m_GrowPointMap.count(dwGrowPointInstanceId))
		{
			CGrowPointInfo GrowPointInfo = m_GrowPointMap[dwGrowPointInstanceId];
			return IsSelectObject(ray, GrowPointInfo, fdis);
		}
		else
		{
			return false;
		}
	}

		// �õ�ѡ�е�������
	bool CGrowPointEditAction::GetSelectedObject(unsigned long& dwGrowPointInstanceId)
	{
		GROWPOINT_MAP::iterator it;
		GROWPOINT_MAP::iterator itEnd;

		bool  bFind = false;
		float fDistance    = 10000.0f;
		float fSelDistance = 0;
		unsigned long dwInstanceId = 0;


		Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(m_screenPt.x, m_screenPt.y);

		itEnd = m_GrowPointMap.end();
		for(it = m_GrowPointMap.begin(); it != itEnd; it++)
		{
			dwInstanceId = it->first;
			if(!IsSelectObject(ray, dwInstanceId, fSelDistance))
			{
				continue;
			}

			if(fSelDistance < fDistance)
			{
				fDistance = fSelDistance;
				bFind = true;
				dwGrowPointInstanceId = dwInstanceId;
			}
		}

		return bFind;
	}



	// �Ƿ�ѡ��������.
	void CGrowPointEditAction::SetSelectObjectLook(unsigned long dwGrowPointInstanceId)
	{

		if(m_GrowPointMap.count(dwGrowPointInstanceId))
		{
			CGrowPointInfo& GrowPointInfo = m_GrowPointMap[dwGrowPointInstanceId];
			GrowPointInfo.SetSelMaterial();
			
		}
		

	}

	// �Ƿ�ѡ��������.
	void CGrowPointEditAction::SetUnSelectObjectLook(unsigned long dwGrowPointInstanceId)
	{

		if(m_GrowPointMap.count(dwGrowPointInstanceId))
		{
			CGrowPointInfo& GrowPointInfo = m_GrowPointMap[dwGrowPointInstanceId];
			GrowPointInfo.SetUnSelMaterial();
			
		}
		
	}


		// ɾ�������㡣
	void CGrowPointEditAction::DelGrowPoint(unsigned long dwGrowPointInstanceId)
	{
		unsigned long dwTypeId = 0;
	
		GROWPOINT_MAP::iterator it;
		it = m_GrowPointMap.find(dwGrowPointInstanceId);

		if(it != m_GrowPointMap.end())
		{
			CGrowPointInfo& GrowPointInfo = it->second;
			dwTypeId = GrowPointInfo.m_dwTypeId;

			GrowPointInfo.ReleaseGrowPoint();
			m_GrowPointMap.erase(it);
			getSceneManipulator()->_fireDelGrowPoint(dwTypeId, dwGrowPointInstanceId);

		}
		
	}



	////////////////////////////////////////////////////////////////////////////////////
	//
	// ���̺Ͷ��̲���
	//

	// �������ȡ�ļ�
	unsigned long CGrowPointEditAction::SaveGrowPointToFile(std::string strFileName)
	{

		SORT_GROWPOINT_MAP		tempMap;
		GROWPOINT_MAP::iterator it;
		GROWPOINT_MAP::iterator itEnd;
		GROWPOINT_PAIR			pairGrowPoint;

		itEnd = m_GrowPointMap.end();
		CGrowPointInfo tempInfo;

		// �������������������
		for(it = m_GrowPointMap.begin(); it != itEnd; it++)
		{
			tempInfo =  it->second;
			pairGrowPoint.second  = tempInfo;
			pairGrowPoint.first   = tempInfo.m_dwTypeId;
			tempMap.insert(pairGrowPoint);

		}

		char buf[1024];
		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.c_str(), "w");
		if(NULL == pFile)
		{
			return 1;
		}

		// д�������ļ�ͷ
		memset(buf, 0, sizeof(buf));
		//sprintf(buf, "INT\tINT\tFLOAT\tFLOAT\tFLOAT\n");
		sprintf(buf, "INT\tINT\tFLOAT\tFLOAT\n");
		::fwrite(buf, strlen(buf), 1, pFile);
		::fwrite(buf, strlen(buf), 1, pFile);

		float fx = 0;
		float fy = 0;
		float fz = 0;

		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		fTerrainDataPosY = getTerrainData()->mPosition.y;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		static unsigned long dwId = 0;
		memset(buf, 0, sizeof(buf));
		

		SORT_GROWPOINT_MAP::iterator sortIt;
		SORT_GROWPOINT_MAP::iterator sortItEnd;

		sortItEnd = tempMap.end();
		
		for(sortIt = tempMap.begin(); sortIt != sortItEnd; sortIt++)
		{
			dwId++;
			tempInfo = sortIt->second;

			// ���г���������߼�����ת��
			fx = (tempInfo.m_position.x - fTerrainDataPosX) / fScale;
			fy = (tempInfo.m_position.y - fTerrainDataPosY) / fScale;
			fz = (tempInfo.m_position.z - fTerrainDataPosZ) / fScale;

			//sprintf(buf, "%d\t%d\t%.4f\t%.4f\t%.4f\n", dwId, tempInfo.m_dwTypeId, fx, fy, fz);
			sprintf(buf, "%d\t%d\t%.4f\t%.4f\n", dwId, tempInfo.m_dwTypeId, fx, fz);
			::fwrite(buf, strlen(buf), 1, pFile);
		}
		
		// ���д��š�
		dwId = 0;
		::fclose(pFile);
		return 0;


	}
		
	// ������洢���ļ�
	unsigned long CGrowPointEditAction::ReadGrowPointFromFile(std::string strFileName)
	{


		GROWPOINT_MAP::iterator it;
		GROWPOINT_MAP::iterator itEnd;
	

		char buf[1024];
		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.c_str(), "r");
		if(NULL == pFile)
		{
			return 1;
		}

		m_GrowPointMap.clear();
		// �Թ��������ļ�ͷ
		memset(buf, 0, sizeof(buf));
		::fgets(buf, sizeof(buf), pFile);
		::fgets(buf, sizeof(buf), pFile);
		
		int iType;

		float fx = 0;
		float fy = 0;
		float fz = 0;

		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		fTerrainDataPosY = getTerrainData()->mPosition.y;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}


		Ogre::Vector3 pos;
		while(!feof(pFile))
		{
			memset(buf, 0, sizeof(buf));
			::fgets(buf, sizeof(buf), pFile);
			//::sscanf(buf, "%d %d %f %f %f", &iType, &iType, &fx, &fy, &fz);
			::sscanf(buf, "%d %d %f %f %f", &iType, &iType, &fx, &fz);
			if(0 == strcmp(buf, ""))
			{
				continue;
			}

			// �߼�����ͳ�������ת��.
			fx = fx * fScale + fTerrainDataPosX;
			//fy = fy * fScale + fTerrainDataPosY;
			fz = fz * fScale + fTerrainDataPosZ;

			pos.x = fx;
			//pos.y = fy;
			pos.z = fz;
			pos.y = getTerrainData()->getHeightAt(pos.x, pos.z);

			AddNewGrowPoint(iType, pos);
		
		}
	
		::fclose(pFile);

		HideScene(m_bIsShow);
		return 0;

	}

			
	//
	// ���̺Ͷ��̲���
	//
	////////////////////////////////////////////////////////////////////////////////////



	
	// �������������
	void CGrowPointEditAction::ClearGrowPointData()
	{
		// ��������Ϣ.
		GROWPOINT_MAP::iterator it;
		GROWPOINT_MAP::iterator itEnd;
		
		
		itEnd = m_GrowPointMap.end();
		for(it = m_GrowPointMap.begin(); it != itEnd; it++)
		{
			CGrowPointInfo& GrowPointInfo = it->second;
			GrowPointInfo.ReleaseGrowPoint();
		}

		m_GrowPointMap.clear();
		ResetCounter();

	}


}

