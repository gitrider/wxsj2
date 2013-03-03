#include "FairyPostFilter.h"
#include "FairyPostFilterFactory.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTechnique.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>

namespace Fairy {

using namespace Ogre;

class BloomPostFilter : public PostFilter
{
private:
    class Factory : public PostFilterFactory
    {
    public:
        static const Ogre::String& _theType(void)
        {
            static const Ogre::String type = "Bloom";
            return type;
        }

        const Ogre::String& getType(void) const
        {
            return _theType();
        }

        PostFilter* createInstance(const Ogre::String& name)
        {
            return new BloomPostFilter(name, _theType());
        }

        void destroyInstance(PostFilter* postFilter)
        {
            delete postFilter;
        }

        void _init(void)
        {
            CompositorPtr compositor = CompositorManager::getSingleton().create(
                _theType(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			CompositionTechnique *t;
            CompositionTechnique::TextureDefinition *td;
            CompositionTargetPass *tp;
            CompositionPass *pass;

            t = compositor->createTechnique();
            td = t->createTextureDefinition("scene");
            td->width = 0;
            td->height = 0;
			td->widthFactor = 1.0f;
			td->heightFactor = 1.0f;
            td->formatList.push_back(PF_X8R8G8B8);
            td = t->createTextureDefinition("blur0");
			td->width = 0;
			td->height = 0;
			td->widthFactor = 0.25f;
			td->heightFactor = 0.25f;
            td->formatList.push_back(PF_X8R8G8B8);
            td = t->createTextureDefinition("blur1");
			td->width = 0;
			td->height = 0;
			td->widthFactor = 0.25f;
			td->heightFactor = 0.25f;
            td->formatList.push_back(PF_X8R8G8B8);

            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
            tp->setOutputName("scene");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur0");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Bloom/Blur");
            pass->setInput(0, "scene");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur1");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Bloom/Blur");
            pass->setInput(0, "blur0");

            tp = t->getOutputTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Bloom/Final");
            pass->setInput(0, "scene");
            pass->setInput(1, "blur1");
        }
    };

public:
    static Factory* getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

    const Ogre::String& getType(void) const
    {
        return Factory::_theType();
    }

public:
    BloomPostFilter(const Ogre::String& name, const Ogre::String& compositor)
        : PostFilter(name, compositor)
    {
    }

    ~BloomPostFilter()
    {
    }

    // from PostFilter
public:

    // from CompositorInstance::Listener
public:

private:
};

//////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////

extern PostFilterFactory* getBloomPostFilterFactory(void)
{
    return BloomPostFilter::getFactory();
}

}
