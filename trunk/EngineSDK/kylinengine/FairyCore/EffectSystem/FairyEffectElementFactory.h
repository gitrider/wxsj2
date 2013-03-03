/********************************************************************
filename:   FairyEffectElementFactory.h

purpose:    the element factory create the effect element
*********************************************************************/

#ifndef __EFFECTELEMENTFACTORY_H__
#define __EFFECTELEMENTFACTORY_H__

// fairy header
#include "Core/FairyPrerequisites.h"

// stl header
#include <vector>

namespace Fairy	{
	class EffectElement;
	class System;
}

namespace Fairy	{

	class EffectElementFactory
	{		
		public:
			EffectElementFactory() {};
			virtual ~EffectElementFactory() {};

			/** Returns the name of the factory, the name which identifies the particle emitter type this factory creates. */
			virtual Ogre::String getName() const = 0;

			/** Creates a new emitter instance.
			@remarks
				The subclass MUST add a pointer to the created instance to mEmitters.
			*/
			virtual EffectElement *createElement(System *system) = 0;

			/** Destroys the emitter pointed to by the parameter (for early clean up if reauired). */
			virtual void destroyElement( EffectElement *e );

			typedef std::vector<EffectElement *> Elements;

		protected:
			Elements mElements;
	};
	
	///////////////////////////////////////////////////////////////////////
	class ParticleElementFactory : public EffectElementFactory
	{
	public:

		ParticleElementFactory();
		~ParticleElementFactory();

		virtual Ogre::String getName() const	{	return "Particle";	};
		virtual EffectElement *createElement(System *system);
	};

	///////////////////////////////////////////////////////////////////////
	class MeshElementFactory : public EffectElementFactory
	{
	public:

		MeshElementFactory();
		~MeshElementFactory();

		virtual Ogre::String getName() const	{	return "Mesh";	};
		virtual EffectElement *createElement(System *system);
	};

	///////////////////////////////////////////////////////////////////////
	class BillboardSetElementFactory : public EffectElementFactory
	{
	public:

		BillboardSetElementFactory();
		~BillboardSetElementFactory();

		virtual Ogre::String getName() const	{	return "BillboardSet";	};
		virtual EffectElement *createElement(System *system);
	};

	///////////////////////////////////////////////////////////////////////
	class ProjectorElementFactory : public EffectElementFactory
	{
	public:

		ProjectorElementFactory();
		~ProjectorElementFactory();

		virtual Ogre::String getName() const	{	return "Projector";	};
		virtual EffectElement *createElement(System *system);
	};

	///////////////////////////////////////////////////////////////////////
	class BeamElementFactory : public EffectElementFactory
	{
	public:

		BeamElementFactory();
		~BeamElementFactory();

		virtual Ogre::String getName() const	{	return "Beam";	};
		virtual EffectElement *createElement(System *system);
	};

	///////////////////////////////////////////////////////////////////////
	class PointLightElementFactory : public EffectElementFactory
	{
	public:

		PointLightElementFactory();
		~PointLightElementFactory();

		virtual Ogre::String getName() const	{	return "PointLight";	};
		virtual EffectElement *createElement(System *system);
	};

	///
	class RibbonEffectElementFactory : public EffectElementFactory
	{
	public:
		RibbonEffectElementFactory();
		~RibbonEffectElementFactory();

		virtual String getName() const	{	return "RibbonTrail";	};
		virtual EffectElement *createElement(System *system);
	};

	class CylinderEffectElementFactory : public EffectElementFactory
	{
	public:
		CylinderEffectElementFactory();
		~CylinderEffectElementFactory();

		virtual String getName() const	{	return "Cylinder";	};
		virtual EffectElement *createElement(System *system);
	};
}

#endif