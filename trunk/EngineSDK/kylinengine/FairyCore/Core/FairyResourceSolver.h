#ifndef __FAIRYRESOURCESOLVER_H__
#define __FAIRYRESOURCESOLVER_H__

#include "FairyPrerequisites.h"

#include <OgreSharedPtr.h>
#include <OgreStringVector.h>

namespace Fairy {

    typedef Ogre::StringVector StringVector;
    typedef Ogre::StringVectorPtr StringVectorPtr;
    typedef std::set<Ogre::String> StringSet;
    typedef std::pair<Ogre::String,Ogre::String> ResourceLocation;
    typedef std::set<ResourceLocation> ResourceLocationSet;

    class UnsolvedResource
    {
    public:
        UnsolvedResource(void) {}
        virtual ~UnsolvedResource() {}

        virtual const Ogre::String& getOrigin(void) const = 0;
        virtual const Ogre::String& getDescription(void) const = 0;
        virtual const Ogre::StringVector* getRecommendValues(void) const = 0;
        virtual const Ogre::StringVector* getPossibleValues(void) const = 0;
        virtual bool setResolvedValue(const Ogre::String& value) = 0;
    };

    typedef Ogre::SharedPtr<UnsolvedResource> UnsolvedResourcePtr;
    typedef std::vector<UnsolvedResourcePtr> UnsolvedResourceList;

    class ResourceSolver
    {
    public:
        ResourceSolver(void) {}
        virtual ~ResourceSolver() {}

        virtual const Ogre::String& getName(void) const = 0;
        virtual const UnsolvedResourceList& getUnsolvedResources(void) const = 0;
        virtual bool commitPermanent(void) = 0;
    };

    typedef Ogre::SharedPtr<ResourceSolver> ResourceSolverPtr;
    typedef std::vector<ResourceSolverPtr> ResourceSolverList;

}

#endif 
