/********************************************************************
filename:   OgreMeshParticleRenderer.cpp

purpose:    Specialisation of ParticleSystemRenderer to render particles using 
mesh. 
*********************************************************************/

#include "OgreMeshParticleRenderer.h"
#include "OgreStringConverter.h"
#include "OgreMeshParticleVisualData.h"
#include "OgreMaterialManager.h"
#include "OgreException.h"
#include "OgreNode.h"

namespace Ogre	{

	MeshParticleRenderer::CmdMeshName MeshParticleRenderer::msMeshNameCmd;
	MeshParticleRenderer::CmdOrientationType MeshParticleRenderer::msOrientationTypeCmd;

	String meshRendererTypeName = "mesh";

	//-----------------------------------------------------------------------
	MeshParticleRenderer::MeshParticleRenderer() :
	mMaterialName(""),
	mMeshName(""),
	mWorldSpace(false),
	mParticleSystemNode(NULL),
	mMeshRotation(false),
	mMeshSizeChanged(false),
	mDefaultWidth(1.0f),
	mDefaultHeight(1.0f),
	mMeshOrientationType(MOT_ORIENTED_ORIGIN),
    mNextPoolIndex(0)
	{		
		if (createParamDictionary("MeshParticleRenderer"))
		{
			ParamDictionary* dict = getParamDictionary();
			dict->addParameter(ParameterDef("mesh_name", 
				"the name of the mesh.",
				PT_STRING),
				&msMeshNameCmd);	

			dict->addParameter(ParameterDef("orientation_type", 
				"the orientation type of mesh particle,"
				"'oriented_origin' means particles are oriened origin direction,"
				"'oriented_self' means particles are oriented around their own direction.",
				PT_STRING),
				&msOrientationTypeCmd);

		}

		_freeParticleMeshVisualDatas();
	}
	//-----------------------------------------------------------------------
	MeshParticleRenderer::~MeshParticleRenderer()
	{
		_freeParticleMeshVisualDatas();
	}
	//-----------------------------------------------------------------------
	const Ogre::String& MeshParticleRenderer::getType(void) const
	{
		return meshRendererTypeName;
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_updateRenderQueue(RenderQueue* queue, 
		std::list<Particle*>& currentParticles, bool cullIndividually)
	{		
        // not implement
	}
	void MeshParticleRenderer::visitRenderables(Renderable::Visitor* visitor, 
		bool debugRenderables)
	{
		//david-<<
		//mRibbonSet->visitRenderables(visitor, debugRenderables);
		//david->>
	}
    //-----------------------------------------------------------------------
    void MeshParticleRenderer::_notifyParticleMoved(std::list<Particle*>& currentParticles)
    {   
        // ����ͽ�������������Ϊ0(�п����ǵ�����ParticleSystem::clear()),�������������е�mesh particle
        if (currentParticles.size() == 0)
        {
            removeAllMeshParticle();

            return;
        }

        for (std::list<Particle*>::iterator i = currentParticles.begin();	
            i != currentParticles.end(); ++i)
        {
            Particle* p = *i;
            MeshParticleVisualData *pVisualData = static_cast<MeshParticleVisualData *>(p->getVisualData());

            // ���visual data��û��������ÿ��particle��һ�ν�����������������ȴ���
            if ( false == pVisualData->isInit() )
            {
                pVisualData->createVisualData(mMeshName, mMaterialName, mParticleSystemNode);
            }

            // ��Ϊp->position��local�������local���꣬������world���꣬����Ҫ��ȥ���ĸ��ڵ��������ܵõ��������
            if (mWorldSpace)
                pVisualData->setPosition(mParticleSystemNode->_getDerivedOrientation().UnitInverse() *
                (p->position - mParticleSystemNode->_getDerivedPosition()) / mParticleSystemNode->_getDerivedScale());
            else
                pVisualData->setPosition(p->position);

            // ���û����Ӱ�����仯����������ΪĬ�ϴ�С
            if (mMeshSizeChanged)
                pVisualData->setScale(p->mWidth, p->mHeight);
            else
                pVisualData->setScale(mDefaultWidth, mDefaultHeight);

            pVisualData->setColour(p->colour);

            if (mMeshOrientationType == MOT_ORIENTED_SELF)
            {
                pVisualData->setDirection(p->direction, mParticleSystemNode->_getDerivedOrientation());
            }

            // ���÷�λ
            pVisualData->setOrientation(mMeshRotation, mParticleSystemNode->_getDerivedOrientation());

            // ���ö�������
            // ���������animation affectorӰ����֮�󣬾͸��¶���ʱ��
            if (pVisualData->mAnimationUpdated)
                pVisualData->updateAnimation();
            else
                pVisualData->resetAnimation();

            // ���ø��±�־
            pVisualData->mAnimationUpdated = false;

            if (false == pVisualData->getVisible())
                pVisualData->setVisible(true);
        }        
    }
    //-----------------------------------------------------------------------
    void MeshParticleRenderer::_notifyParticleExpired(Particle* particle)
    {
        MeshParticleVisualData* data = static_cast<MeshParticleVisualData*>(particle->getVisualData());
        if (data->isInit())
        {
            data->setVisible(false);
            data->resetAnimation();
        }
    }
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_setMaterial(MaterialPtr& mat)
	{
		setMaterialName(mat->getName());
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_notifyCurrentCamera(Camera* cam)
	{
        // not implement
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_notifyParticleRotated(void)
	{
		mMeshRotation = true;
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_notifyDefaultDimensions(Ogre::Real width, Ogre::Real height)
	{
		mDefaultWidth = width;
		mDefaultHeight = height;
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_notifyParticleResized(void)
	{
		mMeshSizeChanged = true;
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_notifyParticleQuota(size_t equota)
	{
        size_t currentSize = mMeshParticleVisualDataPool.size();

        if (currentSize >= equota)
            return;

        // ɾ��ԭ����
        _freeParticleMeshVisualDatas();

        mMeshParticleVisualDataPool.resize(equota);

        for (size_t i=0; i<mMeshParticleVisualDataPool.size(); ++i)
        {
            mMeshParticleVisualDataPool[i] = new MeshParticleVisualData;
        }

        mNextPoolIndex = 0;
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_notifyAttached(Node* parent, bool isTagPoint)
	{
		mParticleSystemNode = parent;
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::setRenderQueueGroup(uint8 queueID)
	{
        for (size_t i=0; i<mMeshParticleVisualDataPool.size(); ++i)
        {
            mMeshParticleVisualDataPool[i]->setRenderQueueGroup(queueID);
        }
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::setKeepParticlesInLocalSpace(bool keepLocal)
	{
		mWorldSpace = !keepLocal;
	}
    //-----------------------------------------------------------------------
    Ogre::SortMode MeshParticleRenderer::_getSortMode(void) const
    {
        return Ogre::SM_DISTANCE;
    }
	//-----------------------------------------------------------------------
	ParticleVisualData* MeshParticleRenderer::_createVisualData(void)
	{
        MeshParticleVisualData* visualData = mMeshParticleVisualDataPool[mNextPoolIndex];

        assert (mNextPoolIndex < mMeshParticleVisualDataPool.size());
        visualData->setPoolIndex( mNextPoolIndex );

        // �����±����
        ++mNextPoolIndex;

		return visualData;
	}
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::_destroyVisualData(ParticleVisualData* vis)
	{
		if (vis)
		{
            size_t index = static_cast<MeshParticleVisualData*>(vis)->getPoolIndex();

            delete mMeshParticleVisualDataPool[index];
			mMeshParticleVisualDataPool[index] = NULL;            
		}
        // ���ﲢ����Ҫ��mMeshParticleVisualDataPool�еĶ�Ӧ��erase����ΪMeshParticleVisualData
        // ��¼����mMeshParticleVisualDataPool���±꣬���erase����ʹ����±�ʵЧ
	}
    //-----------------------------------------------------------------------
    void MeshParticleRenderer::removeAllMeshParticle(void)
    {
        for ( MeshParticleVisualDataPool::iterator i = mMeshParticleVisualDataPool.begin();
            i != mMeshParticleVisualDataPool.end(); ++i )
        {
            MeshParticleVisualData* data = *i;
            if (data && data->isInit())
            {
                data->setVisible(false);
                data->resetAnimation();
            }
        }
    }
	//---------------------------------------------------------------------
	void MeshParticleRenderer::setMeshName(const Ogre::String &name)
	{
		mMeshName = name;
		
		for ( MeshParticleVisualDataPool::iterator i = mMeshParticleVisualDataPool.begin();
			i != mMeshParticleVisualDataPool.end(); ++i )
		{
			if (*i)
			{
				if ( (*i)->isInit() )
					(*i)->modifyMesh(name);
			}
		}
	}
	//---------------------------------------------------------------------
	void MeshParticleRenderer::_freeParticleMeshVisualDatas(void)
	{
		for ( MeshParticleVisualDataPool::iterator i = mMeshParticleVisualDataPool.begin();
			i != mMeshParticleVisualDataPool.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mMeshParticleVisualDataPool.clear();		
	}
	//-----------------------------------------------------------------------
    void MeshParticleRenderer::setMaterialName( const Ogre::String& name )
    {
        mMaterialName = name;

		if ( "" != mMaterialName )
		{
			mpMaterial = MaterialManager::getSingleton().getByName(name);

			if (mpMaterial.isNull())
				OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Could not find material " + name,
					"BillboardSet::setMaterialName" );

			/* Ensure that the new material was loaded (will not load again if
			   already loaded anyway)
			*/
			mpMaterial->load();
		}
		
		for ( MeshParticleVisualDataPool::iterator i = mMeshParticleVisualDataPool.begin();
			i != mMeshParticleVisualDataPool.end(); ++i )
		{
			if (*i)
			{
				if ( (*i)->isInit() )
					(*i)->modifyMaterial(name);
			}
		}
    }
	//-----------------------------------------------------------------------
	void MeshParticleRenderer::setMeshOrientationType(MeshOrientationType mot)
	{
		mMeshOrientationType = mot;

		if (mMeshOrientationType == MOT_ORIENTED_ORIGIN)
		{
			for ( MeshParticleVisualDataPool::iterator i = mMeshParticleVisualDataPool.begin();
				i != mMeshParticleVisualDataPool.end(); ++i )
			{
				if (*i)
				{
					if ( (*i)->isInit() )
						(*i)->setOrientation(Quaternion::IDENTITY);
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	MeshOrientationType MeshParticleRenderer::getMeshOrientationType(void) const
	{
		return mMeshOrientationType;
	}
	//////////////////////////////////////////////////////////////////////////
	String MeshParticleRenderer::CmdMeshName::doGet(const void* target) const
	{
		return static_cast<const MeshParticleRenderer*>(target)->getMeshName();
	}
	void MeshParticleRenderer::CmdMeshName::doSet(void* target, const Ogre::String& val)
	{
		static_cast<MeshParticleRenderer *>(target)->setMeshName(val);
	}
	//-----------------------------------------------------------------------
	String MeshParticleRenderer::CmdOrientationType::doGet(const void* target) const
	{
		MeshOrientationType t = static_cast<const MeshParticleRenderer*>(target)->getMeshOrientationType();
		switch(t)
		{
		case MOT_ORIENTED_ORIGIN:
			return "oriented_origin";

		case MOT_ORIENTED_SELF:
			return "oriented_self";
		
		}
		// Compiler nicety
		return "oriented_origin";
	}
	void MeshParticleRenderer::CmdOrientationType::doSet(void* target, const Ogre::String& val)
	{
		MeshOrientationType t;
		if (val == "oriented_origin")
		{
			t = MOT_ORIENTED_ORIGIN;
		}
		else if (val == "oriented_self")
		{
			t = MOT_ORIENTED_SELF;
		}		
		else
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"Invalid mesh orientation type '" + val + "'", 
				"MeshParticleRenderer::CmdOrientationType::doSet");
		}

		static_cast<MeshParticleRenderer*>(target)->setMeshOrientationType(t);
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	const Ogre::String& MeshParticleRendererFactory::getType() const
	{
		return meshRendererTypeName;
	}
	//-----------------------------------------------------------------------
	ParticleSystemRenderer* MeshParticleRendererFactory::createInstance( 
		const Ogre::String& name )
	{
		return new MeshParticleRenderer();
	}
	//-----------------------------------------------------------------------
	void MeshParticleRendererFactory::destroyInstance( 
		ParticleSystemRenderer* inst)
	{
		delete inst;
	}
}