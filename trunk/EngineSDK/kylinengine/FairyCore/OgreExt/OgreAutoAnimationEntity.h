/********************************************************************
    filename:   OgreAutoAnimationEntity.h
    
    purpose:    A subclassed OGRE entity for calculate animation
                only when visible
*********************************************************************/

#ifndef __AutoAnimationEntity_H__
#define __AutoAnimationEntity_H__

#include <OgreEntity.h>

namespace Ogre {

	class AutoAnimationEntity : public Entity
	{
		// Allow EntityFactory full access
		friend class AutoAnimationEntityFactory;
	protected:

		/** Private constructor (instances cannot be created directly).
		*/
		AutoAnimationEntity();
		/** Private constructor - specify name (the usual constructor used).
		*/
		AutoAnimationEntity(const Ogre::String& name, MeshPtr& mesh);

	public:
		/** Overridden from MovableObject */
		const Ogre::String& getMovableType(void) const;

		/** Overridden - see MovableObject.
		*/
		void _notifyCurrentCamera(Camera* camera);

		/** Overridden - see MovableObject.
		*/
		void _updateRenderQueue(RenderQueue* queue);

        /** Overridden member from ShadowCaster. */
        ShadowRenderableListIterator getShadowVolumeRenderableIterator(
            ShadowTechnique shadowTechnique, const Light* light,
            HardwareIndexBufferSharedPtr* indexBuffer,
            bool extrudeVertices, Ogre::Real extrusionDistance, unsigned long flags = 0);

		typedef std::map<AnimationState*, Ogre::Real> AnimationStateList;

        void _updateAutoAnimationStates(void);
		void addAutoUpdateAnimationState(const Ogre::String& name, Ogre::Real timeOffset = 0);
		void removeAutoUpdateAnimationState(const Ogre::String& name);
        void removeAllAutoUpdateAnimationStates(void);
		const AnimationStateList& getAutoUpdateAnimationStateList(void) const;

    protected:
		AnimationStateList mAutoUpdateAnimationStateList;
		Ogre::Real mLastUpdateTime;
	};

	/** Factory object for creating AutoAnimationEntity instances */
	class AutoAnimationEntityFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl(const Ogre::String& name, const Ogre::NameValuePairList* params);

	public:
		AutoAnimationEntityFactory() {}
		~AutoAnimationEntityFactory() {}

		static Ogre::String FACTORY_TYPE_NAME;

		const Ogre::String& getType(void) const;
		void destroyInstance( MovableObject* obj);  
	};

} // namespace

#endif // __AutoAnimationEntity_H__
