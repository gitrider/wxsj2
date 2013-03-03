#include "AddStaticEntityObjectPlugin.h"
#include "SceneManipulator.h"

#include "Core/FairyStaticEntityObject.h"
#include "Core/FairyObjectProxy.h"
#include "Core/FairySceneInfo.h"
#include "Core/TerrainData.h"

#include "Core/FairyUtils.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreMaterialManager.h>
#include <OgreAny.h>
#include "ui/TriInfoInMap.h"

namespace Fairy {

	class AddStaticEntityObjectPlugin::Indicator
	{
	public:
		Indicator(const ObjectPtr &object, SceneManipulator *sceneManipulator)
			: mProxy(NULL)
		{
			assert(sceneManipulator);

			mProxy = new ObjectProxy(object);

			Ogre::Vector3 pos = VariantCast<Ogre::Vector3>(object->getProperty("position"));
			mIndicatorSceneNode = sceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode(pos);

			Real radius = 5;
			int rings = 16;
			int segments = 16;

			Ogre::MeshPtr sphereMesh = createCommonSphere(radius, rings, segments);
			Ogre::MaterialPtr material = createPureColourMaterial(
				Ogre::ColourValue(1, 1, 0, 0.8)   );

			material->setDepthCheckEnabled(false);

			mIndicatorEntity = sceneManipulator->getSceneManager()->createEntity(mIndicatorSceneNode->getName(), sphereMesh->getName());
			//david-<<
			//mIndicatorEntity->setNormaliseNormals(true);
			//david->>
			mIndicatorEntity->setMaterialName(material->getName());
			mIndicatorEntity->setUserAny(Ogre::Any(mProxy));

			mIndicatorSceneNode->attachObject(mIndicatorEntity);
			mIndicatorSceneNode->setScale(2,2,2);

			setVisible(false);
		}

		~Indicator()
		{
			
			if (mIndicatorEntity)
			{
				mIndicatorSceneNode->getCreator()->destroyEntity(mIndicatorEntity);
				mIndicatorEntity = NULL;
			}

			if (mIndicatorSceneNode)
			{
				mIndicatorSceneNode->getCreator()->destroySceneNode(mIndicatorSceneNode);
				mIndicatorSceneNode = NULL;
			}

			if (mProxy)
			{
				delete mProxy;
				mProxy = NULL;
			}
		}

		void setVisible( bool show )
		{
			assert(mIndicatorSceneNode);           

			mIndicatorSceneNode->setVisible(show);
		}

		void setPosition( const Ogre::Vector3 &pos )
		{
			assert(mIndicatorSceneNode);

			mIndicatorSceneNode->setPosition(pos);
		}    

	protected:

		Ogre::Entity *mIndicatorEntity;

		Ogre::SceneNode *mIndicatorSceneNode;

		ObjectProxy* mProxy;
	};


	class AddStaticEntityObjectPlugin::CllRenderable: public Ogre::SimpleRenderable
    {
    public:
		typedef std::vector<Ogre::Vector3> POSITION_ARRAY;

        CllRenderable()
		{
			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST; 
			mRenderOp.useIndexes    = false; 

			setPolygonModeOverrideable(false);

			mDefaultColor = Ogre::ColourValue(1,1,0,1);  			
        }

        ~CllRenderable()
        {
			ClearVertexBuffer();
        }   

		void CllRenderable::ClearVertexBuffer()
		{
			if ( mRenderOp.vertexData )
			{
				delete mRenderOp.vertexData;
				mRenderOp.vertexData = 0;
			}
		}	

		void CllRenderable::FillData()
		{
			size_t posCount = mVertexArray.size();

			if ( posCount <= 0 )
			{
				return;
			}			
			CreateVertexBuffer();

			Ogre::Vector3 vmax(Ogre::Vector3::ZERO);
			Ogre::Vector3 vmin(Ogre::Vector3::ZERO);

			for (size_t ii = 0; ii < posCount; ++ii)
			{
				if(vmin.x > mVertexArray[ii].x)
				{
					vmin.x = mVertexArray[ii].x;
				}

				if(vmin.y > mVertexArray[ii].y)
				{
					vmin.y = mVertexArray[ii].y;
				}

				if(vmin.z > mVertexArray[ii].z)
				{
					vmin.z = mVertexArray[ii].z;
				}


				if(vmax.x < mVertexArray[ii].x)
				{
					vmax.x = mVertexArray[ii].x;
				}

				if(vmax.y < mVertexArray[ii].y)
				{
					vmax.y = mVertexArray[ii].y;
				}

				if(vmax.z < mVertexArray[ii].z)
				{
					vmax.z = mVertexArray[ii].z;
				}

			}

			vmin.x -= 100.0f;
			vmax.x += 100.0f;

			vmin.y -= 100.0f;
			vmax.y += 100.0f;

			vmin.z -= 100.0f;
			vmax.z += 100.0f;		

			// 更新包围盒
			mBox.setExtents(vmin,vmax);
			getParentNode()->needUpdate();

			// 锁定缓冲区， 填充数据.
			Ogre::RGBA *pCol;
			float *vertexPos = static_cast<float*>(mVbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
			for (size_t i = 0; i < posCount; ++i)
			{
				*vertexPos = mVertexArray[i].x;
				vertexPos++;
				*vertexPos = mVertexArray[i].y;
				vertexPos++;
				*vertexPos = mVertexArray[i].z;
				vertexPos++;	

				pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
				Ogre::Root::getSingleton().convertColourValue(mDefaultColor, pCol++ ) ;
				vertexPos = static_cast<float*>(static_cast<void*>(pCol));
			}

			mVbuf->unlock();    

		}

		void FillPointArray(const TRI_INFO_VECTOR& infoVec, Ogre::Matrix4 matrix)
		{
			int iFaceCount = infoVec.size();
			if (iFaceCount<=0)
			{
				return;
			}

			Ogre::Vector3 Info1;
			mVertexArray.clear();

			for(int i = 0; i < iFaceCount; i++)
			{
				Info1.x = infoVec[i].f1x;
				Info1.y = infoVec[i].f1y;
				Info1.z = infoVec[i].f1z;
				Info1 = matrix * Info1;
				mVertexArray.push_back(Info1);
				Info1.x = infoVec[i].f2x;
				Info1.y = infoVec[i].f2y;
				Info1.z = infoVec[i].f2z;
				Info1 = matrix * Info1;
				mVertexArray.push_back(Info1);
				Info1.x = infoVec[i].f3x;
				Info1.y = infoVec[i].f3y;
				Info1.z = infoVec[i].f3z;
				Info1 = matrix * Info1;
				mVertexArray.push_back(Info1);
			}
		}

		void CreateVertexBuffer()
		{
			if(0 == mVertexArray.size())
			{
				return;
			}

			// 先把上次创建的缓冲区删掉
			ClearVertexBuffer();

			mRenderOp.vertexData = new Ogre::VertexData;

			mRenderOp.vertexData->vertexCount = mVertexArray.size();
			mRenderOp.vertexData->vertexStart = 0;

			Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

			size_t offset = 0;
			decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

			mVbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(0),
				mRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

			mRenderOp.vertexData->vertexBufferBinding->setBinding(0, mVbuf);
		}

		Ogre::Real getBoundingRadius() const
		{
			return 0;
		}

		Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const
		{
			Ogre::Vector3 min, max, mid, dist;

			min = mBox.getMinimum();
			max = mBox.getMaximum();
			mid = ((min - max) * 0.5) + min;
			dist = cam->getDerivedPosition() - mid;

			return dist.squaredLength(); 
		}

		virtual void _updateRenderQueue( Ogre::RenderQueue * queue )
		{
			if(queue)
			{
				queue->addRenderable(this, Ogre::RENDER_QUEUE_8);
			}
		}


    protected:

		POSITION_ARRAY mVertexArray;
		Ogre::ColourValue mDefaultColor;

		Ogre::HardwareVertexBufferSharedPtr mVbuf;
    };

	class AddStaticEntityObjectPlugin::CllIndicator
	{
	public:
		CllIndicator(const ObjectPtr &object, Ogre::SceneManager *sceneMgr, Fairy::SceneManipulator *sceneManipulator)
			: mProxy(NULL)
		{
			assert(sceneMgr && sceneManipulator);

			mProxy = new ObjectProxy(object);

			mSceneManipulator = sceneManipulator;

			mTriInfoVector.clear();

			mIndicatorSceneNode = NULL;
			m_pRendable = NULL;  
			if (readCollisionData(object))
			{
				createItems();
				showIndicator(false);
			}
		}

		~CllIndicator()
		{
			if (mIndicatorSceneNode)
			{
				mIndicatorSceneNode->getCreator()->destroySceneNode(mIndicatorSceneNode);
				mIndicatorSceneNode = NULL;
			}

			if (m_pRendable)
			{
				delete m_pRendable;
				m_pRendable = NULL;
			}

			if (mProxy)
			{
				delete mProxy;
				mProxy = NULL;
			}
		}

		void createItems()
		{
			mIndicatorSceneNode = mSceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode(Ogre::Vector3::ZERO);

			if (mIndicatorSceneNode)
			{
				m_pRendable = new CllRenderable;
				if(m_pRendable)
				{
					m_pRendable->setQueryFlags(0);
					if(mIndicatorSceneNode)
					{
						mIndicatorSceneNode->attachObject(m_pRendable);
					}
					m_pRendable->FillPointArray(mTriInfoVector,mMatrix);
					m_pRendable->FillData();
					m_pRendable->setMaterial(_createMaterial()->getName()); 
				}
			}				
		}

		Ogre::MaterialPtr _createMaterial(void)
		{
			String name = "CllRendableMaterial";

			Ogre::ResourcePtr res = Ogre::MaterialManager::getSingleton().getByName(name);
			if (!res.isNull())
				return res;

			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			material->setLightingEnabled(false);
			material->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
			material->setDepthWriteEnabled(false);
			material->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);

			return material;
		}

		void showIndicator( bool show )
		{
			assert(mIndicatorSceneNode);

			if ( mIndicatorSceneNode )
			{
				mIndicatorSceneNode->setVisible(show);				
			}              
		}

		void updataRenderable(const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const Ogre::Vector3& scale)
		{
			if (m_pRendable)
			{
				updataPRS(pos,rot,scale);
			
				m_pRendable->FillPointArray(mTriInfoVector, mMatrix);			
				m_pRendable->FillData();
			}			
		}

		void updataPRS(const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const Ogre::Vector3& scale)
		{
			assert(mIndicatorSceneNode);

			Ogre::Matrix4 posMatrix;
			Ogre::Matrix4 scaleMatrix;
			Ogre::Matrix4 rotateMatrix(rot);

			posMatrix = Ogre::Matrix4::IDENTITY;
			posMatrix.setTrans(pos);

			scaleMatrix = Ogre::Matrix4::IDENTITY;
			scaleMatrix.setScale(scale);

			mMatrix = posMatrix * rotateMatrix * scaleMatrix;
		}

		bool readCollisionData(const ObjectPtr& object)
		{
			if(Fairy::StaticEntityObject::msType != object->getType())
			{
				return false;      
			}

			Ogre::String meshName = Fairy::VariantCast<Ogre::String>(object->getProperty("mesh name"));
			Ogre::String meshFile;
			Ogre::String meshPath;

			Ogre::StringUtil::splitFilename(meshName, meshFile, meshPath);
			int iPos = meshFile.find_last_of(".");

			Ogre::String cllFileName;
			if(iPos> 0)
			{
				cllFileName  = meshFile.substr(0, iPos);
			}
			else
			{
				cllFileName = meshFile;
			}

			cllFileName = cllFileName + ".cll";


			Ogre::Vector3 position = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));
			Ogre::Quaternion rotateQuaternion = Fairy::VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
			Ogre::Vector3 scale = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("scale"));
			updataPRS(position,rotateQuaternion,scale);

			std::string strFileName = "..\\resource\\building_collision\\" + cllFileName;
			FILE* pFile = NULL;
			pFile = ::fopen(strFileName.c_str(), "rb");
			if(NULL == pFile)
			{
				return false;
			}

			unsigned long dwVersion = 0;
			int   iTriCount = 0;

			TRI_INFO triInfo;
			mTriInfoVector.clear();

			::fread(&dwVersion, sizeof(dwVersion), 1, pFile);
			::fread(&iTriCount, sizeof(iTriCount), 1, pFile);

			for(int i = 0; i < iTriCount; i++)
			{
				::fread(&triInfo, sizeof(triInfo), 1, pFile);
				mTriInfoVector.push_back(triInfo);
			}

			::fclose(pFile);	
			return true;

		}

	protected:
		TRI_INFO_VECTOR mTriInfoVector;

		Ogre::SceneNode *mIndicatorSceneNode;

		Fairy::SceneManipulator *mSceneManipulator;
		ObjectProxy* mProxy;

		CllRenderable*		m_pRendable;  
		Ogre::Matrix4 mMatrix;
	};

    AddStaticEntityObjectPlugin::AddStaticEntityObjectPlugin(Fairy::SceneManipulator* sceneManipulator)
    {
        assert(sceneManipulator);
    
        mSceneManipulator = sceneManipulator;
        mSceneManipulator->addSceneListener(this);
    }

    AddStaticEntityObjectPlugin::~AddStaticEntityObjectPlugin()
    {
        mSceneManipulator->removeSceneListener(this);

        clearIndicators();
    }

    //////////////////////////////////////////////////////////////////////////
	void
    AddStaticEntityObjectPlugin::onSceneReset(void)
    {
        clearIndicators();

		mSceneManipulator->mGridObjsMap.clear();
		mSceneManipulator->mObjGridMap.clear();

        typedef Fairy::SceneInfo::Objects Objects;
        const Fairy::SceneInfo* sceneInfo = mSceneManipulator->getSceneInfo();
        const Objects& objects = sceneInfo->getObjects();
        for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
        {
            Ogre::String type = (*it)->getType();

            // 判断类型
			if(Fairy::StaticEntityObject::msType == type)
			{
			
                CllIndicator *indicator = new CllIndicator( *it, mSceneManipulator->getSceneManager(),
                    mSceneManipulator );

                std::pair<CllIndicators::iterator, bool> inserted =
                    mIndicators.insert(CllIndicators::value_type(*it, indicator)); 
				assert(inserted.second);

				// OriginIndicator
				Indicator * originIndicator = new Indicator( *it, mSceneManipulator);

				std::pair<Indicators::iterator, bool> originInserted =
					mOriginIndicators.insert(Indicators::value_type(*it, originIndicator)); 
				assert(originInserted.second);

				Fairy::StaticEntityObject* entity = static_cast<Fairy::StaticEntityObject*>((*it).get());

				Ogre::Vector3 pos = Fairy::VariantCast<Ogre::Vector3>(entity->getProperty("position"));

				std::pair<int, int> baseGrid = mSceneManipulator->getTerrainData()->getGrid(pos.x, pos.z);
				Ogre::Vector2 point(baseGrid.first,baseGrid.second);
				if (mSceneManipulator->mGridObjsMap.find(point) == mSceneManipulator->mGridObjsMap.end())
				{
					Fairy::SceneManipulator::StaticObjects vec;
					vec.push_back(entity);
					mSceneManipulator->mGridObjsMap.insert(Fairy::SceneManipulator::GridObjsMap::value_type(point,vec));   
				}   
				else
				{
					Fairy::SceneManipulator::GridObjsMap::iterator  iter = mSceneManipulator->mGridObjsMap.find(point);
					Fairy::SceneManipulator::StaticObjects& vec = iter->second;
					vec.push_back(entity);
				}

				mSceneManipulator->mObjGridMap.insert(Fairy::SceneManipulator::ObjGridMap::value_type(entity, point));

            }
        }
    }

    void 
    AddStaticEntityObjectPlugin::onAddObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();
		if(Fairy::StaticEntityObject::msType == type)
		{
			Fairy::StaticEntityObject *staticEntityObject = static_cast<StaticEntityObject *> (object.get());

			Ogre::Vector3 pos = Fairy::VariantCast<Ogre::Vector3>(staticEntityObject->getProperty("position"));
			
			std::pair<int, int> baseGrid = mSceneManipulator->getTerrainData()->getGrid(pos.x, pos.z);
			
			Ogre::Vector2 point(baseGrid.first,baseGrid.second);
			if (mSceneManipulator->mGridObjsMap.find(point) == mSceneManipulator->mGridObjsMap.end())
			{
				Fairy::SceneManipulator::StaticObjects vec;
				vec.push_back(staticEntityObject);
				mSceneManipulator->mGridObjsMap.insert(Fairy::SceneManipulator::GridObjsMap::value_type(point,vec));   
			}   
			else
			{
				Fairy::SceneManipulator::GridObjsMap::iterator  iter = mSceneManipulator->mGridObjsMap.find(point);
				Fairy::SceneManipulator::StaticObjects& vec = iter->second;
				vec.push_back(staticEntityObject);
			}

			mSceneManipulator->mObjGridMap.insert(Fairy::SceneManipulator::ObjGridMap::value_type(staticEntityObject, point));

			CllIndicator *indicator = new CllIndicator( object, mSceneManipulator->getSceneManager(),
				mSceneManipulator );

			std::pair<CllIndicators::iterator, bool> inserted =
				mIndicators.insert(CllIndicators::value_type(object, indicator));

			assert(inserted.second);

			// OriginIndicator
			Indicator * originIndicator = new Indicator( object, mSceneManipulator);

			std::pair<Indicators::iterator, bool> originInserted =
				mOriginIndicators.insert(Indicators::value_type(object, originIndicator)); 
			assert(originInserted.second);
		}
    }

    void 
    AddStaticEntityObjectPlugin::onRemoveObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();
		if(Fairy::StaticEntityObject::msType == type)
		{
			Fairy::StaticEntityObject *staticEntityObject = static_cast<StaticEntityObject *> (object.get());

			Ogre::Vector3 pos = Fairy::VariantCast<Ogre::Vector3>(staticEntityObject->getProperty("position"));

			std::pair<int, int> baseGrid = mSceneManipulator->getTerrainData()->getGrid(pos.x, pos.z);

			Ogre::Vector2 point(baseGrid.first,baseGrid.second);
			if (mSceneManipulator->mGridObjsMap.find(point) != mSceneManipulator->mGridObjsMap.end())
			{
				Fairy::SceneManipulator::GridObjsMap::iterator iter = mSceneManipulator->mGridObjsMap.find(point);
				Fairy::SceneManipulator::StaticObjects& vec = iter->second;
				for (Fairy::SceneManipulator::StaticObjects::iterator obj = vec.begin(); obj!= vec.end(); ++obj)
				{
					if (*obj == staticEntityObject)
					{
						vec.erase(obj);
						break;
					}
				}
			}

			mSceneManipulator->mObjGridMap.erase(staticEntityObject);

            CllIndicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                delete i->second;
                i->second = NULL;
                mIndicators.erase(i);                
            }

			// OriginIndicator
			Indicators::iterator originIter = mOriginIndicators.find(object);

			if ( originIter != mOriginIndicators.end() )
			{
				delete originIter->second;
				originIter->second = NULL;
				mOriginIndicators.erase(originIter);                
			}
        }
    }

    void 
    AddStaticEntityObjectPlugin::onSelectObject(const ObjectPtr& object)
    {
		if(Fairy::StaticEntityObject::msType == object->getType())
		{
			CllIndicators::iterator i = mIndicators.find(object);
			if (i != mIndicators.end())
			{
				if ( i->second )
				{				
					i->second->showIndicator(true);
				}
			}
		}  
    }

    void 
    AddStaticEntityObjectPlugin::onDeselectObject(const ObjectPtr& object)
    {
		if(Fairy::StaticEntityObject::msType == object->getType())
		{
			CllIndicators::iterator i = mIndicators.find(object);
			if (i != mIndicators.end())
			{
				if ( i->second )
				{				
					i->second->showIndicator(false);
				}
			}
		}
    }

    void 
    AddStaticEntityObjectPlugin::onDeselectAllObjects(void)
    {   	
		for (CllIndicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
		{
			if ( i->second )
			{				
				i->second->showIndicator(false);
			}
		}
    }

    void 
    AddStaticEntityObjectPlugin::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
        Ogre::String type = object->getType();
		if(Fairy::StaticEntityObject::msType == type)
		{
            CllIndicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                if ( name == "position" || name == "scale" || name == "orientation")
				{
					Ogre::Vector3 position = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));
					Ogre::Quaternion rotateQuaternion = Fairy::VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
					Ogre::Vector3 scale = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("scale"));

					i->second->updataRenderable(position, rotateQuaternion, scale);
					
					
					Fairy::StaticEntityObject *staticEntityObject = static_cast<StaticEntityObject *> (object.get());
					std::pair<int, int> newBaseGrid = mSceneManipulator->getTerrainData()->getGrid(position.x, position.z);
					Fairy::SceneManipulator::ObjGridMap::iterator iterator = mSceneManipulator->mObjGridMap.find(staticEntityObject);
					Ogre::Vector2 oldPoint(-1,-1);
					if(iterator != mSceneManipulator->mObjGridMap.end())
					{
						oldPoint = iterator->second;
						Ogre::Vector2 & newPoint = iterator->second;
						newPoint.x = newBaseGrid.first;
						newPoint.y = newBaseGrid.second;
					}

					if (mSceneManipulator->mGridObjsMap.find(oldPoint) != mSceneManipulator->mGridObjsMap.end())
					{
						Fairy::SceneManipulator::GridObjsMap::iterator iter = mSceneManipulator->mGridObjsMap.find(oldPoint);
						Fairy::SceneManipulator::StaticObjects& vec = iter->second;
						for (Fairy::SceneManipulator::StaticObjects::iterator obj = vec.begin(); obj!= vec.end(); ++obj)
						{
							if (*obj == staticEntityObject)
							{
								vec.erase(obj);
								break;
							}
						}
					}

					Ogre::Vector2 newPoint(	newBaseGrid.first, newBaseGrid.second);
					if (mSceneManipulator->mGridObjsMap.find(newPoint) == mSceneManipulator->mGridObjsMap.end())
					{
						Fairy::SceneManipulator::StaticObjects vec;
						vec.push_back(staticEntityObject);
						mSceneManipulator->mGridObjsMap.insert(Fairy::SceneManipulator::GridObjsMap::value_type(newPoint,vec));   
					}   
					else
					{
						Fairy::SceneManipulator::GridObjsMap::iterator  iter = mSceneManipulator->mGridObjsMap.find(newPoint);
						Fairy::SceneManipulator::StaticObjects& vec = iter->second;
						vec.push_back(staticEntityObject);
					}			
				} 
            }


			Indicators::iterator originIter = mOriginIndicators.find(object);

			if ( originIter != mOriginIndicators.end() )
			{
				if ( name == "position" )
				{
					Ogre::Vector3 position = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));

					originIter->second->setPosition(position);
				}
			}
        }
    }

    void 
	AddStaticEntityObjectPlugin::clearIndicators(void)
	{
		for (CllIndicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
		{
			if ( i->second )
			{
				delete i->second;
				i->second = NULL;
			}
		}

		mIndicators.clear();

		for (Indicators::iterator i = mOriginIndicators.begin(); i != mOriginIndicators.end(); ++i )
		{
			if ( i->second )
			{
				delete i->second;
				i->second = NULL;
			}
		}

		mOriginIndicators.clear();
	}

	void AddStaticEntityObjectPlugin::onSelectableChanged(const Ogre::String& objType, bool bSelectable)
	{
		if(Fairy::StaticEntityObject::msType == objType)
		{
			for (CllIndicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
			{
				if ( i->second )
				{
					i->second->showIndicator(false);
				}
			}
		}
	}

	void AddStaticEntityObjectPlugin::onSelectableChanged(const ObjectPtr& object, bool bSelectable)
	{
		if(Fairy::StaticEntityObject::msType == object->getType())
		{
			CllIndicators::iterator i = mIndicators.find(object);
			if ( i != mIndicators.end() )
			{
				i->second->showIndicator(false);
			}
		}
	}

	void AddStaticEntityObjectPlugin::onShowHintChanged(bool bShow)
	{
		for (Indicators::iterator i = mOriginIndicators.begin(); i != mOriginIndicators.end(); ++i )
		{
			if ( i->second )
			{
				i->second->setVisible(bShow);
			}
		}
	}

}