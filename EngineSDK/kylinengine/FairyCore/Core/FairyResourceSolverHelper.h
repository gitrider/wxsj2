#ifndef __FAIRYRESOURCESOLVERHELPER_H__
#define __FAIRYRESOURCESOLVERHELPER_H__

#include "FairyResourceSolver.h"

namespace Fairy {

    class UnsolvedResourceBase : public UnsolvedResource
    {
    public:
       Ogre::String origin;
       Ogre::String description;
       Ogre::StringVectorPtr recommends;
       Ogre::StringVectorPtr possibles;

    public:
        UnsolvedResourceBase(void);
        ~UnsolvedResourceBase();

        const Ogre::String& getOrigin(void) const;
        const Ogre::String& getDescription(void) const;
        const Ogre::StringVector* getRecommendValues(void) const;
        const Ogre::StringVector* getPossibleValues(void) const;
    };

    class UnsolvedResourceSimple : public UnsolvedResourceBase
    {
    public:
        std::vector<String*> references;

    public:
        UnsolvedResourceSimple(void);
        ~UnsolvedResourceSimple();

        bool setResolvedValue(const Ogre::String& value);
    };

    class ResourceSolverBase : public ResourceSolver
    {
    public:
        UnsolvedResourceList unsolvedResources;

    public:
        ResourceSolverBase(void);
        ~ResourceSolverBase();

        virtual const UnsolvedResourceList& getUnsolvedResources(void) const;
    };

    extern Ogre::StringVectorPtr findResourceFilenames(const Ogre::String& group, const Ogre::String& filename);
    extern Ogre::StringVectorPtr findImageResources(const Ogre::String& group);
    extern void moveMatchToFront(StringVector& names, const Ogre::String& name);

    template <class Container>
   Ogre::StringVectorPtr toStringVectorPtr(const Container& container)
    {
        if (container.empty())
            return Ogre::StringVectorPtr();

        return Ogre::StringVectorPtr(new Ogre::StringVector(container.begin(), container.end()));
    }

}

#endif 
