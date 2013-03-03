/********************************************************************
filename:   OgreTexCoordBillboardParticleRenderer.h

purpose:    this particle renderer is used to renderer particle that want to 
			change the texture coordinate at runtime. 
*********************************************************************/

#ifndef __TexCoordBillboardParticleRenderer_H__
#define __TexCoordBillboardParticleRenderer_H__

#include "OgrePrerequisites.h"
#include "OgreBillboardParticleRenderer.h"
#include "OgreParticle.h"
#include <list>
namespace Ogre	{

	//////////////////////////////////////////////////////////////////////////	
	class TexCoordBillboardParticleRenderer : public BillboardParticleRenderer
	{
	public:

		/** Command object for texture coord stacks (see ParamCommand).*/
		class CmdStacks : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};
		/** Command object for texture coord slices (see ParamCommand).*/
		class CmdSlices : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};
		/** Command object for texture coord slices (see ParamCommand).*/
		class CmdRepeatTimes : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		//////////////////////////////////////////////////////////////////////////
		TexCoordBillboardParticleRenderer();

		/// @copydoc ParticleSystemRenderer::getType
		virtual const Ogre::String& getType(void) const;
		/// @copydoc ParticleSystemRenderer::_updateRenderQueue
		virtual void _updateRenderQueue(RenderQueue* queue, 
			std::list<Particle*>& currentParticles, bool cullIndividually);
		
		/** sets of the stacks of texture coordinates */
		void setStacks(Ogre::ulong stacks);
		Ogre::ulong getStacks(void) const
		{
			return mStacks;
		}

		/** sets of the slices of texture coordinates */
		void setSlices(Ogre::ulong slices);
		Ogre::ulong getSlices(void) const
		{
			return mSlices;
		}

		/** sets of the repeat times */
		void setRepeatTimes(Ogre::Real times)
		{
			mRepeatTimes = times;
		}
		Ogre::Real getRepeatTimes(void) const
		{
			return mRepeatTimes;
		}

	protected:

		/// the stacks of texture coordinates
		Ogre::ulong mStacks;
		/// the slices of texture coordinates
		Ogre::ulong mSlices;

		Ogre::Real                    mRepeatTimes;

		//////////////////////////////////////////////////////////////////////////
		static CmdStacks msStacksCmd;
		static CmdSlices msSlicesCmd;
		static CmdRepeatTimes   msRepeatTimesCmd;
	};

	/** Factory class for MeshParticleRenderer */
	class TexCoordBillboardParticleRendererFactory : public ParticleSystemRendererFactory
	{
	public:
		/// @copydoc FactoryObj::getType
		virtual const Ogre::String& getType() const;
		/// @copydoc FactoryObj::createInstance
		virtual ParticleSystemRenderer* createInstance( const Ogre::String& name );    
		/// @copydoc FactoryObj::destroyInstance
		virtual void destroyInstance( ParticleSystemRenderer* inst);    
	};
}

#endif