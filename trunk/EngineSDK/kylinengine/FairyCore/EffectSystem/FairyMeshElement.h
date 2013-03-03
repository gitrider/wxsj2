/********************************************************************
filename:   FairyMeshElement.h

purpose:    a mesh effect element.
*********************************************************************/

#ifndef __MESHELEMENT_H__
#define __MESHELEMENT_H__

// fairy header
#include "FairyEffectElement.h"
#include "Core/FairyPrerequisites.h"

// ogre header
namespace Ogre	{
	class Entity;
}

namespace Fairy	{

	class MeshElement : public EffectElement
	{
	public:

		// ≤Œ ˝√¸¡Ó
		class CmdMeshName : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

		class CmdMaterialName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/////////////////////////////////////////////////////////////
		MeshElement(const Ogre::String &type, System *system);
		~MeshElement();
		
		virtual bool createRenderInstance(void);
		virtual void destroyRenderInstance(void);

		virtual void destroyMe(void);

		void setMeshName(const Ogre::String &name);

		const Ogre::String &getMeshName(void) const	{	return mMeshName;	}

		void setMaterialName(const Ogre::String &name);

		const Ogre::String &getMaterialName(void) const	{	return mMaterialName;	}

        /// …Ë÷√visible flag
        virtual void setVisibleFlag(uint32 flags);

	protected:

		bool _createMesh(void);
		void _destroyMesh(void);

	protected:

		String mMeshName;
		String mMaterialName;
		Ogre::Entity *mEntity;

		////////////////////////////////////////////////////////////////
		static CmdMeshName msMeshNameCmd;
		static CmdMaterialName msMaterialNameCmd;
	};
}

#endif