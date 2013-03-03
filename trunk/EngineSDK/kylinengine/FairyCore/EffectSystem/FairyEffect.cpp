/********************************************************************
filename:   FairyEffect.cpp

purpose:    a effect contains a lot of effect elements, the info of effect will
be keep in the .effect files.
*********************************************************************/

#include "FairyEffect.h"
#include "FairyEffectElement.h"
#include "FairyEffectManager.h"

#include "Core/FairySystem.h"
#include "Core/FairyFlags.h"

// ogre header
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace Fairy	{

	Ogre::Real Effect::msDefaultIterativeInterval = 0;

	Effect::CmdLifeTime Effect::msLifeTimeCmd;

	//---------------------------------------------------------------------
	Effect::Effect(const Ogre::String &templateName, System *system)	: 
	EffectBasic(system),
	mTemplateName(templateName),
	mLifeTime(0.0f),
	mCurrentLifeTime(0.0f),
	mAliveEffect(true),
	mIterativeInterval(msDefaultIterativeInterval),
	mIterateRemainTime(0.0f),
    mNeedSetVisibleFlags(false),
    mVisibleFlags(OVF_DEFAULT),
    mRemoveParentNode(false),
    mDelayRemove(false),
    mPutIntoFreePool(true)
	{	
		initDefaults();
		mElementList.clear();
	}
	//---------------------------------------------------------------------
	Effect::~Effect()
	{		
		destroyAllElements();
	}
	//---------------------------------------------------------------------
	void Effect::dump(std::ofstream &of) const
	{
		of << "effect " << std::endl;

		for ( ElementList::const_iterator i = mElementList.begin();
			i != mElementList.end();	++i	)
		{
		//	(*i)->dump(of);			
		}
		of << "end of effect " << std::endl;
	}
	//---------------------------------------------------------------------
	void Effect::updateEffect(Ogre::Real value)
	{
		if ( mAliveEffect )
		{
			if (mIterativeInterval > 0)
			{
				mIterateRemainTime += value;

				while (mIterateRemainTime >= mIterativeInterval)
				{
					_updateSelf(mIterativeInterval);	

					_updateElements(mIterativeInterval);

					mIterateRemainTime -= mIterativeInterval;
				}				
			}
			else
			{
				_updateSelf(value);

				_updateElements(value);
			}

            if (mNeedSetVisibleFlags)
            {
                // �����е�Ԫ�ؽ���visible����
                if (mElementList.size() > 0)
                {			
                    ElementList::iterator i = mElementList.begin();

                    while ( i != mElementList.end() )
                    {
                        EffectElement *element = *i;
                        assert (element);

                        if ( element->isActive() && ( false == element->isDestroy() ) )
                        {				
                            element->setVisibleFlag(mVisibleFlags);
                        }				

                        ++i;
                    }
                }

                mNeedSetVisibleFlags = false;
            }
		}		
	}
	//---------------------------------------------------------------------
	void Effect::_updateSelf(Ogre::Real time)
	{
		if ( mLifeTime > 0.0f )
		{
			//��calculate the life time
			Ogre::Real currentTime = mCurrentLifeTime + time;

			// if the life time is zero ,the elements of the effect will not be deleted.
			if ( currentTime > mLifeTime )
			{
				// destroy
				removeAllElements();

				mAliveEffect = false;
				return;
			}
			else
				mCurrentLifeTime = currentTime;
		}
	}
	//---------------------------------------------------------------------
	void Effect::_updateElements(Ogre::Real time)
	{
		if (mElementList.size() > 0)
		{			
			ElementList::iterator i = mElementList.begin();

			while ( i != mElementList.end() )
			{
				EffectElement *element = *i;
				assert (element);

				if (element->isActive())
				{
					// the lifetime of the element is reached. 
					if (element->isDestroy())
					{
						//	i =	removeElement(i);
						removeElement(*i);
					}
					else
					{
						element->update(time, mTransformInfo);
					}
				}				

				++i;
			}
		}
	}
	//---------------------------------------------------------------------
	Effect& Effect::operator=(const Effect& rhs)
	{
		destroyAllElements();

		// copy elements
		for ( ElementList::const_iterator i = rhs.mElementList.begin();
			i != rhs.mElementList.end(); ++i )
		{
			EffectElement *temElement = (*i);
			assert (temElement);

			EffectElement *element = EffectManager::getSingleton().createElement( temElement->getType() );
			temElement->copyParametersTo(element);

			addElement(element);
		}

		// copy effect parameter
		rhs.copyParametersTo(this);

		return *this;
	}
	//---------------------------------------------------------------------
	void Effect::reborn(void)
	{
		mCurrentLifeTime = 0.0f;
		mAliveEffect = true;
		
        mDelayRemove = false;
        mRemoveParentNode = false;

        mNeedSetVisibleFlags = false;
        mVisibleFlags = OVF_DEFAULT;

 //       mTransformInfo.mPosition = Ogre::Vector3::ZERO;
 //       mTransformInfo.mRotation = Ogre::Quaternion::IDENTITY;

        if (mElementList.size() > 0)
		{			
			ElementList::iterator i = mElementList.begin();

			while ( i != mElementList.end() )
			{
				EffectElement *element = *i;
				assert (element);

				element->active();

                // ����visible����ΪĬ��ֵtrue
                element->setVisible(true);

                // ����visible flagΪĬ��ֵ
                element->setVisibleFlag(OVF_DEFAULT);

 //               element->setTransformInfo(mTransformInfo);

				++i;
			}
		}
	}
	//---------------------------------------------------------------------
	void Effect::addElement( EffectElement *element )
	{
		assert (element);

		mElementList.push_back(element);
		element->setParent(this);
	}
	//---------------------------------------------------------------------
	Effect::ElementIterator Effect::destroyElement( ElementIterator i )
	{
		delete (*i);
		(*i) = NULL;
		return mElementList.erase(i);
	}
	//---------------------------------------------------------------------
	void Effect::destroyElement( EffectElement *element )
	{
		assert (element);
		for ( ElementList::iterator i = mElementList.begin();
			i != mElementList.end();	++i	)
		{
			if ((*i) == element)
			{
				delete (*i);
				(*i) = NULL;
				mElementList.erase(i);
				break;
			}
		}		
	}
	//---------------------------------------------------------------------
	void Effect::destroyElement( unsigned short index )
	{
		assert(index < mElementList.size() && "element index out of bounds!");
		ElementList::iterator i = mElementList.begin() + index;
		destroyElement(i);
	}
	//---------------------------------------------------------------------
	void Effect::removeElement( EffectElement *element )
	{
		assert (element);
		element->sleep();
	}
	//---------------------------------------------------------------------
	void Effect::removeAllElements(void)
	{
		for ( ElementList::iterator i = mElementList.begin();
			i != mElementList.end();	++i	)
		{
			if ((*i))
			{
				removeElement(*i);
			}
		}
	}
	//---------------------------------------------------------------------
	void Effect::removeParentSceneNode(void)
	{
		mBasicNode->getParent()->removeChild(mBasicNode);
	}
	//---------------------------------------------------------------------
	void Effect::destroyAllElements(void)
	{
		for ( ElementList::iterator i = mElementList.begin();
			i != mElementList.end();	++i	)
		{			
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mElementList.clear();
	}
	//---------------------------------------------------------------------
	Ogre::SceneNode* Effect::createSceneNode(void)
	{
		// if the scene node isn't null, the effect is exist(it is recreated from free list), so
		// we don't need to create the scene node.
		if ( mBasicNode )
			return mBasicNode;

		assert (mSystem);
		Ogre::SceneNode *baseNode = mSystem->getBaseSceneNode();
		assert (baseNode);

		mBasicNode = baseNode->createChildSceneNode();

		assert (mBasicNode);

		//dumpSceneNode(mBasicNode);

		// create the scene node of the element
		for ( ElementList::iterator i = mElementList.begin();
			i != mElementList.end();	++i	)
		{
			if ((*i))
			{
				(*i)->createSceneNode(mBasicNode);
			}
		}

		return mBasicNode;
	}
	//---------------------------------------------------------------------
	void Effect::createSceneNode( Ogre::SceneNode *parentNode )
	{
		assert (parentNode);


		if (mBasicNode)
		{
			//assert (mBasicNode->getParent() == NULL);
            // ����Ѿ����ڱ�Ľڵ����ˣ����Ƚ��
            // ������effect�Ǵ�freelist�еõ��ģ��������ϴδ���������
            // createSceneNode����������������LogicModel�е�addEffect����
            // ��removeEffectʱ��������Ӵ����븸�ڵ�Ĺ�ϵ��������ʱ��
            // ����parent�ģ���ʱ����Ҫ�Ӵ����븸�ڵ�Ĺ�ϵ
            if (mBasicNode->getParent())
            {
                mBasicNode->getParent()->removeChild(mBasicNode);
            }
			parentNode->addChild(mBasicNode);
		}
		else
		{
			mBasicNode = parentNode->createChildSceneNode();

			// create the scene node of the element
			for ( ElementList::iterator i = mElementList.begin();
				i != mElementList.end();	++i	)
			{
				if ((*i))
				{
					(*i)->createSceneNode(mBasicNode);
				}
			}
		}

		assert (mBasicNode);
	}
	//---------------------------------------------------------------------
	EffectElement * Effect::getElement( unsigned short index )
	{
		assert(index < mElementList.size() && "element index out of bounds!");
		return mElementList[index];
	}
	//---------------------------------------------------------------------
	void Effect::addElement( const Ogre::String &type )
	{
		EffectElement *element = EffectManager::getSingleton().createElement(type);
		assert (element);
		element->createSceneNode(mBasicNode);
		addElement(element);
	}
	//---------------------------------------------------------------------
	bool Effect::initDefaults(void)
	{		
		if (createParamDictionary("Effect"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();
		
			dict->addParameter(Ogre::ParameterDef("LifeTime", 
				"the life time of this effect.",
				Ogre::PT_REAL),&msLifeTimeCmd);		

			return true;

		}
		return false;
	}
	//---------------------------------------------------------------------
	bool Effect::existVisualElements(void)
	{
		for (unsigned short i=0; i<getNumElements(); ++i)
		{
			EffectElement *element = getElement(i);

			if (element->existVisualElements())
				return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	void Effect::setExtraTransformInfos( const TransformInfos &infos )
	{
		for (unsigned short i=0; i<getNumElements(); ++i)
		{
			getElement(i)->updateExtraTransformInfos(infos);
		}
	}
	//---------------------------------------------------------------------
	void Effect::shutdown(void)
	{
		for (unsigned short i=0; i<getNumElements(); ++i)
		{
			EffectElement *element = getElement(i);

			element->shutdown();
		}
	}
	//---------------------------------------------------------------------
	void Effect::setTransformInfo( const TransformInfo &info )
	{	
		mTransformInfo = info;

		// ���϶�element����transform��Ϣ�����ã������ӳ�
		if (mElementList.size() > 0)
		{			
			ElementList::iterator i = mElementList.begin();

			while ( i != mElementList.end() )
			{
				EffectElement *element = *i;
				assert (element);

				if ( element->isActive() && ( false == element->isDestroy() ) )
				{				
					element->setTransformInfo(mTransformInfo);
				}				

				++i;
			}
		}
	}
    //---------------------------------------------------------------------
    void Effect::setVisible(bool visible)
    {	
        EffectBasic::setVisible(visible);

        // �����е�Ԫ�ؽ���visible����
        if (mElementList.size() > 0)
        {			
            ElementList::iterator i = mElementList.begin();

            while ( i != mElementList.end() )
            {
                EffectElement *element = *i;
                assert (element);

                if ( element->isActive() && ( false == element->isDestroy() ) )
                {				
                    element->setVisible(visible);
                }				

                ++i;
            }
        }
    }
    //---------------------------------------------------------------------
    void Effect::setColour( const Ogre::ColourValue& colour )
    {
        // �����е�Ԫ�ؽ���visible����
        if (mElementList.size() > 0)
        {			
            ElementList::iterator i = mElementList.begin();

            while ( i != mElementList.end() )
            {
                EffectElement *element = *i;
                assert (element);

                if ( element->isActive() && ( false == element->isDestroy() ) )
                {				
                    element->setColour(colour);
                }				

                ++i;
            }
        }
    }
    //---------------------------------------------------------------------
    void Effect::setVisibleFlag(uint32 flags)
    {
        mVisibleFlags = flags;
        mNeedSetVisibleFlags = true;
        //// �����е�Ԫ�ؽ���visible����
        //if (mElementList.size() > 0)
        //{			
        //    ElementList::iterator i = mElementList.begin();

        //    while ( i != mElementList.end() )
        //    {
        //        EffectElement *element = *i;
        //        assert (element);

        //        if ( element->isActive() && ( false == element->isDestroy() ) )
        //        {				
        //            element->setVisibleFlag(flags);
        //        }				

        //        ++i;
        //    }
        //}
    }

	//////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	String Effect::CmdLifeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const Effect *>(target)->getLifeTime() );
	}
	//---------------------------------------------------------------------
	void Effect::CmdLifeTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<Effect *>(target)->setLifeTime(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void Effect::CmdLifeTime::doCopy(void* target, const void* source)
    {
        static_cast<Effect *>(target)->setLifeTime(
            static_cast<const Effect *>(source)->getLifeTime() );
    }
#endif
}