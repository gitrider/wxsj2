/********************************************************************
filename:   WXLogicModelManager.cpp

purpose:    ���ڽ���������������LogicModel��
*********************************************************************/

#include "FairyLogicModelManager.h"

#include "ModelSystem/FairyLogicModel.h"

#include "Core/FairySystem.h"
#include "Core/TerrainData.h"
#include "Core/FairyUtils.h"

#include <OgreSceneManager.h>
#include <OgreException.h>
#include <OgreLogManager.h>
#include <OgreStringConverter.h>
#include <OgreControllerManager.h>
#include <OgreController.h>
#include <OgrePredefinedControllers.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreEntity.h>

#include <mbstring.h>

#include <OgreDynLibManager.h>
#include <OgreDynlib.h>
#include <ScorpioCollision.h>
#include "Scorpio.h"

#include <OgreWireBoundingBox.h>
#include "Core/FairyFlags.h"
//#include <OgreSkeletonInstance.h>
//#include <OgreBone.h>

#define TIXML_USE_STL
#include <tinyxml.h>

template<> Fairy::LogicModelManager * Ogre::Singleton<Fairy::LogicModelManager>::ms_Singleton = 0;

namespace
{
	class Fairy::LogicModelManager;

	class LogicModelControllerValue : public Ogre::ControllerValue<float>
	{
	protected:
		Fairy::LogicModelManager* mLogicModelMgr;

	public:
		LogicModelControllerValue(Fairy::LogicModelManager* mgr)
			: mLogicModelMgr(mgr)
		{
		}

		float getValue(void) const
		{
			return 0.0f;
		}

		void setValue(float value)
		{
			mLogicModelMgr->updateAllLogicModel(value);
			mLogicModelMgr->updateAllModelBoundingBox();
		}
	};
}

namespace Fairy	{

    //bool LogicModelManager::TerrainCollider::_support(const Ogre::Vector3& point, Ogre::Vector3& position, Ogre::Vector3& normal)
    //{
    //    if (!mTerrainData)
    //        return false;

    //    mTerrainData->getPositionAndNormalAt(point.x * mScale, point.z * mScale, position, normal);
    //    // Add margin avoid co-plane
    //    position.y += mTerrainData->mScale.y * (0.05f / normal.y);
    //    if (position.y <= point.y * mScale)
    //        return false;

    //    position *= mInverseScale;
    //    return true;
    //}

    /// ������ײ��
    class TerrainCollider : public Ogre::UserCollideTester
    {
    protected:
        TerrainData* mTerrainData;
        Ogre::Real mScale;
        Ogre::Real mInverseScale;
    public:
        TerrainCollider(Fairy::TerrainData* terrainData, Ogre::Real scale)
            : mTerrainData(terrainData)
            , mScale(scale)
            , mInverseScale(1 / scale)
        {
        }

        bool _support(const Ogre::Vector3& point, Ogre::Vector3& position, Ogre::Vector3& normal)
        {
            if (!mTerrainData)
                return false;

            mTerrainData->getPositionAndNormalAt(point.x * mScale, point.z * mScale, position, normal);
            // Add margin avoid co-plane
            position.y += mTerrainData->mScale.y * (0.05f / normal.y);
            if (position.y <= point.y * mScale)
                return false;

            position *= mInverseScale;
            return true;
        }
    };

	void LogicModelManager::updateAllModelBoundingBox()
	{
		if (mShowModelBoundingBox)
		{
			// Check the base node first
			if (!mBoundingBoxBaseNode)
			{
				mBoundingBoxBaseNode = mSystem->getSceneManager()->getRootSceneNode()->createChildSceneNode();
			}

			for (ModelBoundingBoxMap::iterator it = mBoundingBoxes.begin(); it != mBoundingBoxes.end();)
			{
				Ogre::String modelName = it->first;
				Ogre::WireBoundingBox* boundingBox = it->second;				

				if (mLogicModelMap.find(modelName) == mLogicModelMap.end())
				{	
					if (boundingBox->getParentSceneNode())
					boundingBox->getParentSceneNode()->detachObject(boundingBox->getName());
					mFreeBoundingBoxes.push_back(boundingBox);
					
					ModelBoundingBoxMap::iterator itErase = it;
					++it;					
					mBoundingBoxes.erase(itErase);
				}
				else
				{
					++it;	
				}
			}

			// Create the bounding Box list and show
			for ( LogicModelMap::iterator i = mLogicModelMap.begin();i != mLogicModelMap.end(); ++i )
			{
				LogicModel* model = i->second;

				assert (model);
				if ( model->getVisibleFlag() != (unsigned int)Fairy::OVF_GUI_ELEMENTS && model->isActive() && false == model->isChildModel() && false == model->isAttached() )					
				{
					Ogre::WireBoundingBox* boundingBox;
					Ogre::String modelName = i->first;

					if (mBoundingBoxes.find(modelName) == mBoundingBoxes.end())
					{
						if (mFreeBoundingBoxes.empty())
						{
							boundingBox = new Ogre::WireBoundingBox();
							boundingBox->setCastShadows(false);
							boundingBox->setQueryFlags(0);
							mBoundingBoxes.insert(ModelBoundingBoxMap::value_type(modelName,boundingBox));
						}
						else
						{
							boundingBox = mFreeBoundingBoxes.back();
							mBoundingBoxes.insert(ModelBoundingBoxMap::value_type(modelName,boundingBox));
							mFreeBoundingBoxes.pop_back();
						}
						assert(!boundingBox->getParentSceneNode());
						mBoundingBoxBaseNode->attachObject(boundingBox);
					}
					else
					{
						ModelBoundingBoxMap::iterator target = mBoundingBoxes.find(modelName);
						boundingBox = target->second;
					}

					// Setup boundingBox
					Ogre::Vector3 additionPos = Ogre::Vector3::ZERO;
					Ogre::Matrix4 addTrans = Ogre::Matrix4::IDENTITY;
					/*if (model->mSkeletonEntity)
					{
						Ogre::Bone* rootBone = model->mSkeleton->getRootBone();
						if (rootBone)
						{
							additionPos = rootBone->getPosition()-rootBone->getInitialPosition();
							addTrans.setTrans(additionPos);
						}
					}*/
					Ogre::AxisAlignedBox aabb = model->getBoundingBox();
					aabb.transformAffine(model->getSceneNode()->_getFullTransform()*addTrans);
					boundingBox->setupBoundingBox(aabb);
				}
			}
		}
	}

	void LogicModelManager::showModelBoundingBox(bool bShow)
	{
		if (mShowModelBoundingBox == bShow)
		{
			return;
		}
		mShowModelBoundingBox = bShow;
		if (mShowModelBoundingBox)
		{
			updateAllModelBoundingBox();
		}
		else
		{
			// Detach the bounding Box from the base node
			for (ModelBoundingBoxMap::const_iterator it = mBoundingBoxes.begin(); it != mBoundingBoxes.end(); ++it)
			{
				Ogre::WireBoundingBox* boundingBox = it->second;
				if (boundingBox->getParentSceneNode())
					boundingBox->getParentSceneNode()->detachObject(boundingBox->getName());
				mFreeBoundingBoxes.push_back(boundingBox);
			}

			mBoundingBoxes.clear();
		}
	}

	LogicModelManager::LogicModelManager( Fairy::System *system ) :
	mSystem(system),
	mAutoNamedModel(0),
    mShadowCastable(false),
    mCollisionManager(NULL),
    mTerrainCollider(NULL),
    mEnableWalkingEffect(true),
    mInterpolationMode(Ogre::Animation::IM_LINEAR),
	mShowModelBoundingBox(false),
	mBoundingBoxBaseNode(NULL)
	{
		mLogicModelTemplateMap.clear();

		mScriptPatterns.push_back("*.obj");

		Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

		_createController();
		mBoundingBoxes.clear();
		mFreeBoundingBoxes.clear();
	}
	//-----------------------------------------------------------------------
	LogicModelManager::~LogicModelManager()
	{
		if (mBoundingBoxBaseNode)
		{
			mSystem->getSceneManager()->destroySceneNode(mBoundingBoxBaseNode);
			mBoundingBoxBaseNode = NULL;
		}

		for (ModelBoundingBoxMap::const_iterator it = mBoundingBoxes.begin(); it != mBoundingBoxes.end(); ++it)
		{
			Ogre::WireBoundingBox* box = it->second;
			delete box;
		}

		mBoundingBoxes.clear();

		for (BoundingBoxList::const_iterator it = mFreeBoundingBoxes.begin(); it != mFreeBoundingBoxes.end(); ++it)
		{
			Ogre::WireBoundingBox* box = *it;
			delete box;
		}

		mFreeBoundingBoxes.clear();

		_destroyController();

		Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);

		destroyAllLogicModelTemplates();

        destroyAllLogicModels();

        _clearAnimationSkeletonLinks();

        _removeMaterialPool();

        _cleanupSkeletalEntityCache();

        _destroyTerrainCollider();

        _destroyColliderManager();
	}
	//-----------------------------------------------------------------------
	LogicModelManager *LogicModelManager::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	//---------------------------------------------------------------------
	LogicModelManager & LogicModelManager::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}
	//---------------------------------------------------------------------
	const Ogre::StringVector& LogicModelManager::getScriptPatterns(void) const
	{
		return mScriptPatterns;
	}
    //---------------------------------------------------------------------
    void LogicModelManager::_parseSingleObject(TiXmlNode* rootElement, const Ogre::String& objectName)
    {
        const char* pTempValue = NULL;

        const TiXmlElement *pObjectElement = rootElement->ToElement();

        if (!pObjectElement)
        {
            Ogre::LogManager::getSingleton().logMessage( "Object Element is NULL : _parseSingleObject " + objectName);

            return;
        }

        const TiXmlElement *pAttribElement = NULL;
        const TiXmlElement *pEntityListElement = NULL;
        const TiXmlElement *pMaterialListElement = NULL;
        const TiXmlElement *pEffectElement = NULL;
        const TiXmlElement *pShadowUncastableElement = NULL;
        const TiXmlElement *pLocatorsElement = NULL;
        const TiXmlElement *pAnimationSkeletonElement = NULL;
        const TiXmlElement *pFabricColliderElement = NULL;

        // ���objectΪ���ַ�����˵�����obj�Ķ������ںϲ�����ļ���
        Ogre::String realObjectName;

        if (objectName.empty())
        {
            realObjectName = pObjectElement->Attribute("name");
        }
        else
            realObjectName = objectName;

        LogicModel *pModel = new LogicModel(realObjectName, mSystem);

        // load object attributes
        pAttribElement = pObjectElement->FirstChildElement("Attributes");
        if(pAttribElement)
        {
            const TiXmlElement *pAttrib = NULL;

            pAttrib = pAttribElement->FirstChildElement("Attribute");
            while(pAttrib)
            {
               Ogre::String			name, value, hints;
                //load attribute name
                pTempValue = pAttrib->Attribute("name");
                if(!pTempValue)
                {
                    pAttrib = pAttrib->NextSiblingElement("Attribute");
                    continue;
                }

                name = pTempValue;
                //load attribute value
                pTempValue = pAttrib->Attribute("value");

                if(!pTempValue) 
                    value = "";
                else 
                    value = pTempValue;

                //load attribute hints
                pTempValue = pAttrib->Attribute("hints");

                if(!pTempValue) 
                    hints = "";
                else 
                    hints = pTempValue;

                pModel->addAttribute(name, value, hints);

                pAttrib = pAttrib->NextSiblingElement("Attribute");
            }
        }

        //load object mesh list
        pEntityListElement = pObjectElement->FirstChildElement("EntityList");
        if(pEntityListElement)
        {
            const TiXmlElement *pEntity = NULL;

            pEntity = pEntityListElement->FirstChildElement("Entity");

            while(pEntity)
            {
               Ogre::String name, meshName, mat;

                pTempValue = pEntity->Attribute("name");

                if(pTempValue) 
                    name = pTempValue;
                else 
                    name = "";

                pTempValue = pEntity->Attribute("file");

                if(pTempValue) 
                    meshName = pTempValue;
                else 
                    meshName = "";

                pTempValue = pEntity->Attribute("material");

                if(pTempValue) 
                    mat = pTempValue;
                else 
                    mat = "";

                pModel->addEntity(name, meshName, mat);
				// If the attribute with the name "name" doesn't exist ,add it to attribute list
				if (pModel->mAttribMap.find(name) == pModel->mAttribMap.end())
				{
					pModel->addAttribute(name, "", "Mesh");
				}

                pEntity = pEntity->NextSiblingElement("Entity");
            }
        }

        //load object material list
        pMaterialListElement = pObjectElement->FirstChildElement("MaterialList");
        if(pMaterialListElement)
        {
            const TiXmlElement *pMaterial = NULL;

            pMaterial = pMaterialListElement->FirstChildElement("Material");

            while(pMaterial)
            {
               Ogre::String name, entityEntry, matName;

                pTempValue = pMaterial->Attribute("name");

                if(pTempValue) 
                    name = pTempValue;
                else 
                    name = "";

                pTempValue = pMaterial->Attribute("entity");

                if(pTempValue) 
                    entityEntry = pTempValue;
                else 
                    entityEntry = "";

                pTempValue = pMaterial->Attribute("material");

                if(pTempValue) 
                    matName = pTempValue;
                else 
                    matName = "";

                pModel->addMaterial(name, entityEntry, matName);
				// If the attribute with the name "name" doesn't exist , add it to attribute list
				if (pModel->mAttribMap.find(name) == pModel->mAttribMap.end())
				{
					pModel->addAttribute(name, "", "Material");
				}

                pMaterial = pMaterial->NextSiblingElement("Material");
            }
        }

        //load locator info list
        pLocatorsElement = pObjectElement->FirstChildElement("Locators");

        if(pLocatorsElement)
        {
            const TiXmlElement *pLocator = NULL;

            pLocator = pLocatorsElement->FirstChildElement("Locator");

            while(pLocator)
            {
               Ogre::String LocatorName, BoneName;
                Ogre::Vector3 LocatorOffPos = Ogre::Vector3::ZERO;
                Ogre::Quaternion LocatorOffRot = Ogre::Quaternion::IDENTITY;
                Ogre::Real qx = 0.0f, qy = 0.0f, qz = 0.0f, qw = 0.0f;

                bool transferable = false;
                bool translateFirst = false;

                pTempValue = pLocator->Attribute("name");

                if(pTempValue) 
                    LocatorName = pTempValue;
                else
                {
                    pLocator = pLocator->NextSiblingElement("Locator");
                    continue;
                }

                pTempValue = pLocator->Attribute("bonename");

                if(pTempValue) 
                    BoneName = pTempValue;
                else 
                    BoneName = "";

                LocatorOffPos.x = Ogre::StringConverter::parseReal( pLocator->Attribute("x") );
                LocatorOffPos.y = Ogre::StringConverter::parseReal( pLocator->Attribute("y") );
                LocatorOffPos.z = Ogre::StringConverter::parseReal( pLocator->Attribute("z") );

                qx = Ogre::StringConverter::parseReal( pLocator->Attribute("qx") );
                qy = Ogre::StringConverter::parseReal( pLocator->Attribute("qy") );
                qz = Ogre::StringConverter::parseReal( pLocator->Attribute("qz") );
                qw = Ogre::StringConverter::parseReal( pLocator->Attribute("qw") );
                LocatorOffRot.FromAngleAxis(Ogre::Radian(qw), Ogre::Vector3(qx, qy, qz));

                pTempValue = pLocator->Attribute("transferable");

                if (pTempValue)
                    transferable = Ogre::StringConverter::parseBool(pTempValue);

                pTempValue = pLocator->Attribute("translateFirst");

                if (pTempValue)
                    translateFirst = Ogre::StringConverter::parseBool(pTempValue);

                if( false == pModel->addLocator(LocatorName, BoneName, LocatorOffPos, LocatorOffRot, transferable, translateFirst) )
                {
                    pLocator = pLocator->NextSiblingElement("Locator");
                    continue;
                }

                const TiXmlElement *pSlotElement = NULL;
                pSlotElement = pLocator->FirstChildElement("Slot");
                while(pSlotElement)
                {
                   Ogre::String SlotName, SlotObj, slotAttrib;

                    pTempValue = pSlotElement->Attribute("name");
                    if(pTempValue) 
                        SlotName = pTempValue;
                    else
                    {
                        pSlotElement = pSlotElement->NextSiblingElement("Slot");
                        continue;
                    }

                    pTempValue = pSlotElement->Attribute("object");
                    if(pTempValue) 
                        SlotObj = pTempValue;
                    else 
                        SlotObj = "";

                    pTempValue = pSlotElement->Attribute("attribs");
                    if(pTempValue)
                        slotAttrib = pTempValue;
                    else
                        slotAttrib = "";

                    pModel->addSlot(LocatorName, SlotName, SlotObj, slotAttrib);
					// If the attribute with the name "SlotName" doesn't exist ,add it to attribute list
					if (pModel->mAttribMap.find(SlotName) == pModel->mAttribMap.end())
					{
						pModel->addAttribute(SlotName, "", "SlotCreate");
					}

                    pSlotElement = pSlotElement->NextSiblingElement("Slot");
                }
                pLocator = pLocator->NextSiblingElement("Locator");
            }

        }

        //load object effect list
        pEffectElement = pObjectElement->FirstChildElement("Effects");
        if(pEffectElement)
        {
            const TiXmlElement *pEffect = NULL;

            pEffect = pEffectElement->FirstChildElement("Effect");

            while(pEffect)
            {
               Ogre::String name, effectName, locatorName;
                bool translateAll = false;
                Ogre::ColourValue colour(Ogre::ColourValue::White);

                pTempValue = pEffect->Attribute("name");

                if(pTempValue) 
                    name = pTempValue;
                else 
                    name = "";

                pTempValue = pEffect->Attribute("effect");

                if(pTempValue) 
                    effectName = pTempValue;
                else 
                    effectName = "";

                pTempValue = pEffect->Attribute("locator");

                if(pTempValue) 
                    locatorName = pTempValue;
                else 
                    locatorName = "";

                pTempValue = pEffect->Attribute("translateAll");

                if (pTempValue)
                    translateAll = Ogre::StringConverter::parseBool(pTempValue);

                pTempValue = pEffect->Attribute("colour");

                if (pTempValue)
                    colour = Ogre::StringConverter::parseColourValue(pTempValue);

                pModel->addEffectInfo(name, effectName, locatorName, translateAll, colour);

                pEffect = pEffect->NextSiblingElement("Effect");
            }
        }

        //load object shadow uncastable list
        pShadowUncastableElement = pObjectElement->FirstChildElement("ShadowUncasts");
        if(pShadowUncastableElement)
        {
            const TiXmlElement *pMesh = NULL;

            pMesh = pShadowUncastableElement->FirstChildElement("Mesh");

            while(pMesh)
            {
               Ogre::String meshName;

                pTempValue = pMesh->Attribute("meshname");

                if(pTempValue) 
                    meshName = pTempValue;
                else 
                    meshName = "";                

                pModel->addShadowUncastable(meshName);

                pMesh = pMesh->NextSiblingElement("Mesh");
            }
        }

        // load animation skeleton link
        pAnimationSkeletonElement = pObjectElement->FirstChildElement("Skeletons");

		// �½�һ��link map
		// Insure there is a link for this obj
		AnimationSkeletonLinks* links = _createAnimationSkeletonLinksForModel(realObjectName);

		if (pAnimationSkeletonElement)
        {
            const TiXmlElement* pSkeleton = NULL;         

            pSkeleton = pAnimationSkeletonElement->FirstChildElement("Skeleton");

            while (pSkeleton)
            {
               Ogre::String skeletonName;

                pTempValue = pSkeleton->Attribute("skeleton");

                if (pTempValue)
                    skeletonName = pTempValue;
                else
                    skeletonName = "";

               Ogre::String animations;

                pTempValue = pSkeleton->Attribute("animations");

                if (pTempValue)
                    animations = pTempValue;
                else
                    animations = "";

                if (skeletonName.empty() || animations.empty())
                {
                    Ogre::LogManager::getSingleton().logMessage( "skeleton name is empty or animations is empty "
                        "when parse animation skeleton link!"
                        "LogicModelManager::parseScript " + realObjectName );

                    pSkeleton = pSkeleton->NextSiblingElement("Skeleton");
                    continue;
                }

                if (links)
                {
                    Ogre::StringVector animationArray = Ogre::StringUtil::split(animations);

                    assert (animationArray.size());

                    for (size_t animationIndex = 0; animationIndex < animationArray.size(); ++animationIndex)
                    {
                        // ��link map�в��붯����������ƵĶ�Ӧ��ϵ
						String szTemp = animationArray[animationIndex];
                        links->insert( AnimationSkeletonLinks::value_type( szTemp, skeletonName ) );
                        // pModel->addAnimationSkeletonLink( animationArray[animationIndex], skeletonName );
	
                    }
                }

                pSkeleton = pSkeleton->NextSiblingElement("Skeleton");
            }

			if (links)
			{
				pModel->mlinks = links;
			}
        }

        // load fabric collider
        pFabricColliderElement = pObjectElement->FirstChildElement("FabricColliders");

        if (pFabricColliderElement)
        {
            const TiXmlElement* pFabricCollider = NULL;

            pFabricCollider = pFabricColliderElement->FirstChildElement("FabricCollider");

            while (pFabricCollider)
            {
               Ogre::String colliderName;

                pTempValue = pFabricCollider->Attribute("name");

                if (pTempValue)
                    colliderName = pTempValue;
                else
                    colliderName = "";

               Ogre::String locatorName;

                pTempValue = pFabricCollider->Attribute("locator");

                if (pTempValue)
                    locatorName = pTempValue;
                else
                    locatorName = "";

               Ogre::String colliderInfo;

                pTempValue = pFabricCollider->Attribute("info");

                if (pTempValue)
                    colliderInfo = pTempValue;
                else
                    colliderInfo = "";

                if (colliderName.empty() || colliderInfo.empty() || locatorName.empty())
                {
                    Ogre::LogManager::getSingleton().logMessage( "one of attribute is empty "
                        "when parse fabric collider info!"
                        "LogicModelManager::parseScript " + realObjectName );

                    pFabricCollider = pFabricCollider->NextSiblingElement("FabricCollider");
                    continue;
                }

                Ogre::StringVector infos = Ogre::StringUtil::split(colliderInfo,",");

                LogicModel::FabricColliderType colliderType;

                if (infos[0] == "box")
                    colliderType = LogicModel::FCT_BOX;
                else if (infos[0] == "sphere")
                    colliderType = LogicModel::FCT_SPHERE;
                else if (infos[0] == "plane")
                    colliderType = LogicModel::FCT_PLANE;
                else
                {
                    Ogre::LogManager::getSingleton().logMessage("fabric collider type '" + infos[0] + "' is wrong!"
                        " collider name : " + colliderName + " LogicModelManager::parseScript " + realObjectName );

                    pFabricCollider = pFabricCollider->NextSiblingElement("FabricCollider");
                    continue;
                }

                if (infos.size() > 1)
                    pModel->addFabricCollider(colliderName, locatorName, colliderType, infos[1]);
                else
                    pModel->addFabricCollider(colliderName, locatorName, colliderType);

                pFabricCollider = pFabricCollider->NextSiblingElement("FabricCollider");
            }
        }

        // ���ļ���ͳһת����Сд
        char* tempName = new char[realObjectName.length()+1];
        memcpy( tempName, realObjectName.c_str(), realObjectName.length() );
        tempName[realObjectName.length()]=0;

        _mbslwr((uchar*)tempName);

       Ogre::String lowerFileName(tempName);

        std::pair<LogicModelMap::iterator, bool> inserted = 
            mLogicModelTemplateMap.insert( LogicModelMap::value_type(lowerFileName, pModel) );

        if (false == inserted.second)
        {
            delete pModel;
            pModel = NULL;

            Ogre::LogManager::getSingleton().logMessage( "Logic Model with name '" + lowerFileName + "' inserted failed! " +
                "LogicModelManager::parseScript " + realObjectName );
        }

        delete [] tempName;
    }
	//---------------------------------------------------------------------
	void LogicModelManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
	{
		TiXmlDocument xmldoc;
        TiXmlElement *pRootElement = NULL;
	
		xmldoc.Parse(stream->getAsString().c_str());
		if(xmldoc.Error())
		{
            Ogre::LogManager::getSingleton().logMessage( "parse Logic Model Failed : "
                +Ogre::String(xmldoc.ErrorDesc()) );

			return;
		}

        // ��ȡxml�ļ���root element��ȡ������value�������жϵ�ǰ�������Ǻϲ�����ļ����Ǻϲ�ǰ���ļ�
        pRootElement = xmldoc.RootElement();

        if (pRootElement->ValueStr() == "AllObjects")
        {
            // ����Ǻϲ�����ļ�������������ļ��е�object
            for( TiXmlNode* child = pRootElement->FirstChild(); child; child = child->NextSibling() )
            {
                _parseSingleObject(child, "");
            }
        }
        else
        {
            _parseSingleObject(pRootElement, stream->getName());
        }
	}
	//---------------------------------------------------------------------
	Ogre::Real LogicModelManager::getLoadingOrder(void) const
	{
		return 1000.0f;
	}
	//---------------------------------------------------------------------
	void LogicModelManager::_parseFailed( LogicModel *model, const char *errorDesc )
	{
		delete model;
		model = NULL;

		Ogre::LogManager::getSingleton().logMessage( "parse Logic Model Failed : "
			+Ogre::String(errorDesc) );
	}
	//-----------------------------------------------------------------------
	LogicModel* LogicModelManager::createLogicModel( const Ogre::String& modelName )
	{
		String autoName = "AutoNameModel_" + Ogre::StringConverter::toString(mAutoNamedModel++);

		return createLogicModel( autoName, modelName );
	}
	//-----------------------------------------------------------------------
	LogicModel* LogicModelManager::createLogicModel(const Ogre::String& name, const Ogre::String& modelName)
	{
		if (mLogicModelMap.find(name) != mLogicModelMap.end())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
				"Logic Model with name '" + name + "' already exists.", 
				"LogicModelManager::createLogicModel");
		}

		const LogicModel* modelTemplate = getTemplateByName(modelName);

        if (!modelTemplate)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Cannot find required Logic Model template '" + modelName + "'",
                "LogicModelManager::createLogicModel");
        }
		
		LogicModel* newModel = new LogicModel(modelName, mSystem);

        // �Ȱ�ָ���¼������������κβ�������׳����쳣�������new������ָ����ܱ�
        // ��ȷɾ������Ϊ���Ѿ����浽map����
        std::pair<LogicModelMap::iterator, bool> inserted = 
            mLogicModelMap.insert( LogicModelMap::value_type(name, newModel) );

        assert (inserted.second);

		*newModel = *modelTemplate;

	//	newModel->setName(objName);

		newModel->initModel();

        // ����Ĭ�ϵ�ͶӰѡ��
        newModel->setShadowCastable(mShadowCastable);

        // ����������Ч�Ƿ��
        newModel->setEnableWalkingEffect(mEnableWalkingEffect);

		return newModel;	
	}
	//-----------------------------------------------------------------------
	void LogicModelManager::destroyLogicModel(const Ogre::String& name)
	{
		LogicModelMap::iterator i = mLogicModelMap.find(name);

		if ( i != mLogicModelMap.end() )
		{
			delete i->second;
			i->second = NULL;

			mLogicModelMap.erase(i);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModelManager::destroyLogicModel(LogicModel* model)
	{
		for ( LogicModelMap::iterator i = mLogicModelMap.begin();
			i != mLogicModelMap.end(); ++i )
		{
			if (i->second == model)
			{
				delete i->second;
				i->second = NULL;

				mLogicModelMap.erase(i);

				break;
			}
		}
	}
    //-----------------------------------------------------------------------
    void LogicModelManager::destroyAllLogicModels(void)
    {
        for ( LogicModelMap::iterator i = mLogicModelMap.begin();
            i != mLogicModelMap.end(); ++i )
        {
            if (i->second)
            {
                delete i->second;
                i->second = NULL;
            }
        }
        mLogicModelMap.clear();
    }
    //-----------------------------------------------------------------------
    void LogicModelManager::destroyAllLogicModelTemplates(void)
    {
        for ( LogicModelMap::iterator i = mLogicModelTemplateMap.begin();
            i != mLogicModelTemplateMap.end(); ++i )
        {
            if (i->second)
            {
                delete i->second;
                i->second = NULL;
            }
        }
        mLogicModelTemplateMap.clear();
    }
	//-----------------------------------------------------------------------
	const LogicModel * LogicModelManager::getTemplateByName(const Ogre::String& modelName) const
	{
        // ���ļ���ͳһת����Сд
        char* tempName = new char[modelName.length()+1];
        memcpy( tempName, modelName.c_str(), modelName.length() );
        tempName[modelName.length()]=0;

        _mbslwr((uchar*)tempName);

       Ogre::String objName( tempName );

        delete [] tempName;

		LogicModelMap::const_iterator i = mLogicModelTemplateMap.find(objName);
		if (i != mLogicModelTemplateMap.end())
		{
			return i->second;
		}
		else
		{
			return NULL;
		}
	}
	//-----------------------------------------------------------------------
	LogicModel * LogicModelManager::getModelByName(const Ogre::String& modelName)
	{
		LogicModelMap::iterator i = mLogicModelMap.find(modelName);
		if (i != mLogicModelMap.end())
		{
			return i->second;
		}
		else
		{
			return NULL;
		}
	}
	//---------------------------------------------------------------------
	void LogicModelManager::_createController(void)
	{
		static Ogre::ControllerFunctionRealPtr gIdentityControllerFunction(new Ogre::ScaleControllerFunction(1.0f, false));
		Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
		mController = controllerManager.createController(
			controllerManager.getFrameTimeSource(),
			Ogre::ControllerValueRealPtr(new LogicModelControllerValue(this)),
			gIdentityControllerFunction);
	}
	//---------------------------------------------------------------------
	void LogicModelManager::_destroyController(void)
	{
		if (mController)
		{
			Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
			controllerManager.destroyController(mController);
			mController = NULL;
		}
	}
	//---------------------------------------------------------------------
	void LogicModelManager::updateAllLogicModel(Ogre::Real time)
	{
        for ( LogicModelMap::iterator i = mLogicModelMap.begin();
            i != mLogicModelMap.end(); ++i )
        {
            LogicModel* model = i->second;

            assert (model);

            // �����ǰ���model����model��������������ڱ��model�ϣ�����ϵ������
            // �Ͳ���execute����Ϊ���ǻ��ڸ�model�е�execute�б�execute
            // !( model->isChildModel() || model->isAttached() ) == !(model->isChildModel()) && !(model->isAttached())
            if ( model->isActive() && false == model->isChildModel() && false == model->isAttached() )
                model->execute(time);
	
        }
	}
    //---------------------------------------------------------------------
    void LogicModelManager::setShadowCastable(bool castable)
    {
        if (mShadowCastable != castable)
        {
            mShadowCastable = castable;

            // �Ե�ǰ���л��ŵ�LogicModel����������
            for ( LogicModelMap::iterator i = mLogicModelMap.begin();
                i != mLogicModelMap.end(); ++i )
            {
                LogicModel* model = i->second;

                assert (model);

                model->setShadowCastable(castable);
            }
        }        
    }
    //---------------------------------------------------------------------
    void LogicModelManager::setEnableWalkingEffect(bool enable)
    {
        if (mEnableWalkingEffect != enable)
        {
            mEnableWalkingEffect = enable;

            // �Ե�ǰ���л��ŵ�LogicModel����������
            for ( LogicModelMap::iterator i = mLogicModelMap.begin();
                i != mLogicModelMap.end(); ++i )
            {
                LogicModel* model = i->second;

                assert (model);

                model->setEnableWalkingEffect(enable);
            }
        }        
    }
    //---------------------------------------------------------------------
    void LogicModelManager::loadModelResource(const Ogre::String& modelName, const Ogre::String& groupName)
    {
        // ֻ����һ��Ϊ�վͲ����д���
        if ( modelName.empty() || groupName.empty() )
            return;
        else
        {
            // �Ȼ�ȡmodelģ��
            const LogicModel* modelTemplate = getTemplateByName(modelName);

            if (!modelTemplate)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Cannot find required Logic Model template '" + modelName + "'",
                    "LogicModelManager::loadModelResource");
            }

            // ����entity�б���������Ӧ����Դ
            for ( LogicModel::EntityMap::const_iterator i = modelTemplate->mEntityMap.begin();
                i != modelTemplate->mEntityMap.end(); ++i )
            {
                const LogicModel::EntityValue& tempValue = i->second;

               Ogre::String meshName = tempValue.mMeshName;
               Ogre::String materialName = tempValue.mMaterialName;

                // ��load��obj�ļ��ж����material
                if (false == materialName.empty())
                {
                    if (materialName.find(",") !=Ogre::String::npos)
                    {
                        Ogre::StringVector matNames = Ogre::StringUtil::split(materialName, ",");

                        assert (matNames.size() > 1);

                        for (size_t matNamesIndex = 0; matNamesIndex < matNames.size(); ++matNamesIndex)
                        {
                            const Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(matNames[matNamesIndex]);

                            if (false == material.isNull())
                                material->load();
                        }
                    }
                    else
                    {
                        // �ȼ���Ƿ����������
                        const Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);

                        if (false == material.isNull())
                            material->load();
                    }
                }

                // ��ȡmesh��skeleton������еĻ���
                if (false == meshName.empty())
                {
                    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load(meshName, groupName);

                    if (false == mesh.isNull())
                    {
                        // �����obj�ļ���û����material�����������ȡmesh�Դ���material
                        if (materialName.empty())
                        {
                            unsigned short subMeshNum = mesh->getNumSubMeshes();

                            for (unsigned short subMeshIndex = 0;
                                subMeshIndex < subMeshNum; ++subMeshIndex)
                            {
                                Ogre::SubMesh* subMesh = mesh->getSubMesh(subMeshIndex);

                                assert (subMesh);

                                const Ogre::String& subMatName = subMesh->getMaterialName();

                                const Ogre::MaterialPtr material = 
                                    Ogre::MaterialManager::getSingleton().getByName(subMatName);

                                if (false == material.isNull())
                                    material->load();
                            }
                        }
                    }                
                }			
            } // for each entity value

            // ��ȡlocator�ϵ�model
            for ( LogicModel::LocatorMap::const_iterator itLocator = modelTemplate->mLocatorMap.begin();
                itLocator != modelTemplate->mLocatorMap.end(); ++itLocator )
            {
                const LogicModel::LocatorValue& tempValue = itLocator->second;

                for ( LogicModel::SlotMap::const_iterator itSlot = tempValue.mSlotMap.begin();
                    itSlot != tempValue.mSlotMap.end(); ++itSlot )
                {
                    const LogicModel::SlotValue& tempSlot = itSlot->second;

                    loadModelResource(tempSlot.mModelName, groupName);
                }
            }
        }
    }
    //---------------------------------------------------------------------
    Ogre::CollisionManager* LogicModelManager::getCollisionManager(void)
    {
        if (NULL == mCollisionManager)
        {
            //Ogre::DynLib* dynLib = 0;

            //dynLib = Ogre::DynLibManager::getSingleton().load("Plugin_Scorpio");
            //if (dynLib)
            //{
            //    Ogre::CreateCollisionManagerProc createCollisionManager = static_cast<Ogre::CreateCollisionManagerProc>(
            //        dynLib->getSymbol(SCORPIO_CREATECOLLISIONMANAGERPROC_NAME));
            //    if (createCollisionManager)
            //    {
            //        mCollisionManager = (*createCollisionManager)();
            //    }
            //}

			mCollisionManager = Ogre::createCollisionManager();
        }

        return mCollisionManager;        
    }
    //---------------------------------------------------------------------
    Ogre::UserCollider* LogicModelManager::getTerrainCollider(void)
    {
        if (NULL == mTerrainCollider)
        {
            // ˳�㴴��һ��terrain collider
            static TerrainCollider terrainCollider(mSystem->getTerrainData(), 100);
            mTerrainCollider = getCollisionManager()->createUserCollider(&terrainCollider);
        }

        return mTerrainCollider;        
    }
    //---------------------------------------------------------------------
    void LogicModelManager::_destroyTerrainCollider(void)
    {
        if (mTerrainCollider)
        {
            getCollisionManager()->destroyCollider(mTerrainCollider);
            mTerrainCollider = NULL;
        }
    }
    //---------------------------------------------------------------------
    void LogicModelManager::_destroyColliderManager(void)
    {
        if (mCollisionManager)
        {
            //Ogre::DynLib* dynLib = 0;

            //dynLib = Ogre::DynLibManager::getSingleton().load("Plugin_Scorpio");
            //if (dynLib)
            //{
            //    Ogre::DestroyCollisionManagerProc destroyCollisionManager = static_cast<Ogre::DestroyCollisionManagerProc>(
            //        dynLib->getSymbol(SCORPIO_DESTROYCOLLISIONMANAGERPROC_NAME));
            //    if (destroyCollisionManager)
            //    {
            //        (*destroyCollisionManager)(mCollisionManager);
            //        mCollisionManager = NULL;
            //    }
            //}

			destroyCollisionManager(mCollisionManager);
			mCollisionManager = NULL;
        }       
    }
    //---------------------------------------------------------------------
    LogicModelManager::AnimationSkeletonLinks* 
        LogicModelManager::_createAnimationSkeletonLinksForModel(const Ogre::String& modelName)
    {
        AnimationSkeletonLinksMap::iterator it =
            mAnimationSkeletonLinksMap.find(modelName);

        if (it != mAnimationSkeletonLinksMap.end())
        {
            Ogre::LogManager::getSingleton().logMessage( "AnimationSkeletonLinks with name '"
                + modelName + "' already existed! Maybe something is wrong!" + "LogicModelManager::parseScript" );

            return it->second;
        }
        else
        {
            AnimationSkeletonLinks* links = new AnimationSkeletonLinks;
            mAnimationSkeletonLinksMap.insert(AnimationSkeletonLinksMap::value_type(modelName, links));

            return links;
        }
    }
    //---------------------------------------------------------------------
    void LogicModelManager::_clearAnimationSkeletonLinks(void)
    {
        AnimationSkeletonLinksMap::iterator it =
            mAnimationSkeletonLinksMap.begin();

        while (it != mAnimationSkeletonLinksMap.end())
        {
            delete it->second;

            ++it;
        }

        mAnimationSkeletonLinksMap.clear();
    }
    //---------------------------------------------------------------------
    const Ogre::String& LogicModelManager::getSkeletonFromAnimationName(const Ogre::String& modelName, const Ogre::String& animName)
    {
        /// �Ȼ�ȡ����ָ����ģ�͵�link map
        AnimationSkeletonLinksMap::iterator it =
            mAnimationSkeletonLinksMap.find(modelName);

        if (it == mAnimationSkeletonLinksMap.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "can't find the skeleton link map of the model '" + modelName + 
                "', get animation '" + animName + "' failed!",
                "LogicModelManager::_getSkeletonFromAnimationName");
        }

        AnimationSkeletonLinks* links = it->second;

        /// ���ҳ������������Ӧ�Ĺ�������
        AnimationSkeletonLinks::const_iterator linksIt = 
            links->find(animName);

        if (linksIt == links->end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "can't find the skeleton link to the animation '" + 
                animName + ", get animation '" + animName + "' failed!",
                "LogicModelManager::_getSkeletonFromAnimationName"); 
        }

        return linksIt->second;
    }
    //---------------------------------------------------------------------
    LogicModelManager::AnimationSkeletonLinksIterator 
        LogicModelManager::getAnimationSkeletonLinksIterator(const Ogre::String& modelName)
    {
        AnimationSkeletonLinksMap::iterator it =
            mAnimationSkeletonLinksMap.find(modelName);

        if ( it != mAnimationSkeletonLinksMap.end() )
        {
            AnimationSkeletonLinks* links = it->second;
            return AnimationSkeletonLinksIterator(links->begin(), links->end());
        }       
		else
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
				"can't find the animation skeleton link of the model '" + 
				modelName,
				"LogicModelManager::getAnimationSkeletonLinksIterator"); 
		}
	}
    //---------------------------------------------------------------------
    void LogicModelManager::addToMaterialPool(MaterialHandle handle)
    {
        // ��list�е�������λ,��֤����ʹ�õ���mat����ǰ��
        MaterialCaches::iterator cacheIt = mMaterialCaches.find(handle);

        if (cacheIt != mMaterialCaches.end())
        {
            MaterialPool::iterator poolIt = cacheIt->second.mPoolIterator;

            mMaterialPool.erase(poolIt);
            mMaterialPool.push_back(handle);

            cacheIt->second.mPoolIterator = --mMaterialPool.end();

            return;
        }

        // �ܵ������˵��list��û�����Ԫ��
        const size_t MATERIAL_POOL_SIZE = 512;
        const size_t REMOVE_SIZE = MATERIAL_POOL_SIZE * 0.2;

        // �����ǰpool�е�Ԫ�س���������
        if (mMaterialPool.size() >= MATERIAL_POOL_SIZE)
        {
            // ����Ҫɾ����Ԫ�ظ���
            size_t realRemoveSize = REMOVE_SIZE;

            // ��pool�Ŀ�ͷ��ʼ����
            MaterialPool::iterator it = mMaterialPool.begin();

            // ֻҪɾ���ĸ�����Ϊ0,���һ�û��pool��β��
            while (realRemoveSize && it != mMaterialPool.end())
            {
                MaterialHandle matHandle = *it;
                MaterialCaches::iterator it2 = mMaterialCaches.find(matHandle);
                assert(it2 != mMaterialCaches.end());

                // ���жϵ�ǰҪɾ���Ĳ����Ƿ񻹴�������,���û��,�Ϳ���ɾ��
                if (it2 == mMaterialCaches.end() ||     // Just in case
                    it2->second.mMaterial.useCount() <= Ogre::ResourceGroupManager::RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS + 1)
                {
                    // ɾ��������������
                    if (it2 != mMaterialCaches.end())   // Just in case
                    {
                        mMaterialCaches.erase( it2 );
                    }

                    --realRemoveSize;

                    mMaterialPool.erase(it++);

                    Ogre::MaterialManager::getSingleton().remove(matHandle);
                }
                else
                    ++it;
            }

            Ogre::LogManager::getSingleton().logMessage("Material Pool Remove " + Ogre::StringConverter::toString(REMOVE_SIZE - realRemoveSize));
        }
        
        mMaterialPool.push_back(handle);
        mMaterialCaches.insert( MaterialCaches::value_type( handle, 
            MaterialCache( Ogre::MaterialManager::getSingleton().getByHandle(handle), --mMaterialPool.end() ) ) );
    }
    //---------------------------------------------------------------------
    void LogicModelManager::_removeMaterialPool(void)
    {
        mMaterialCaches.clear();

        MaterialPool::iterator begIt = mMaterialPool.begin();
        MaterialPool::iterator endIt = mMaterialPool.end();

        while (begIt != endIt)
        {
            Ogre::MaterialManager::getSingleton().remove(*begIt);

            ++begIt;
        }

        mMaterialPool.clear();
    }
    //---------------------------------------------------------------------
    Ogre::Entity* LogicModelManager::_createSkeletalEntity(const Ogre::SkeletonPtr& skeleton)
    {
        Ogre::Entity* entity;

        SkeletalEntityCache::iterator i =
            mSkeletalEntityCache.find(skeleton.get());
        if (i != mSkeletalEntityCache.end())
        {
            // Gather from cache
            entity = i->second;
            mSkeletalEntityCache.erase(i);

            // Should reset all animation states
            Ogre::AnimationStateSet* ass = entity->getAllAnimationStates();
            while (ass->hasEnabledAnimationState())
            {
                Ogre::AnimationState* as = ass->getEnabledAnimationStateIterator().getNext();
                as->setEnabled(false);
            }
        }
        else
        {
            // Create null mesh first
            Ogre::MeshPtr mesh = createNullMeshForSkeleton(skeleton);

            // Create the entity
            static Ogre::ulong gsCount = 0;
            entity = mSystem->getSceneManager()->createEntity(
                mesh->getName() + Ogre::StringConverter::toString(gsCount++, 4, '0'),
                mesh->getName());
        }

        return entity;
    }
    //---------------------------------------------------------------------
    void LogicModelManager::_destroySkeletalEntity(Ogre::Entity* entity)
    {
        // Insert into cache
        mSkeletalEntityCache.insert(
            SkeletalEntityCache::value_type(entity->getMesh()->getSkeleton().get(), entity));
    }
    //---------------------------------------------------------------------
    void LogicModelManager::_cleanupSkeletalEntityCache(void)
    {
        SkeletalEntityCache::iterator i, iend;
        iend = mSkeletalEntityCache.end();
        for (i = mSkeletalEntityCache.begin(); i != iend; ++i)
        {
            Ogre::Entity* entity = i->second;
            mSystem->getSceneManager()->destroyEntity(entity);
        }
        mSkeletalEntityCache.clear();
    }
}
