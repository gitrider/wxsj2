/********************************************************************
    filename:   WXMotionBlurPostFilter.cpp
    
    purpose:    Motion blur post filter
*********************************************************************/

#include "FairyPostFilter.h"
#include "FairyPostFilterFactory.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTechnique.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>

#include <OgreGpuProgram.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

namespace Fairy {

using namespace Ogre;

class MotionBlurPostFilter
    : public PostFilter
{
private:
    class Factory : public PostFilterFactory
    {
    public:
        static const Ogre::String& _theType(void)
        {
            static const Ogre::String type = "MotionBlur";
            return type;
        }

        const Ogre::String& getType(void) const
        {
            return _theType();
        }

        PostFilter* createInstance(const Ogre::String& name)
        {
            return new MotionBlurPostFilter(name, _theType());
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
            td = t->createTextureDefinition("sum");
			td->width = 0;
			td->height = 0;
			td->widthFactor = 1.0f;
			td->heightFactor = 1.0f;
            td->formatList.push_back(PF_X8R8G8B8);
            td = t->createTextureDefinition("temp");
			td->width = 0;
			td->height = 0;
			td->widthFactor = 1.0f;
			td->heightFactor = 1.0f;
            td->formatList.push_back(PF_X8R8G8B8);

            /// Render scene
            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
            tp->setOutputName("scene");

            /// Initialisation pass for sum texture
            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("sum");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/MotionBlur/Copy");
            pass->setInput(0, "scene");
            tp->setOnlyInitial(true);

			/// Do the motion blur
            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("temp");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/MotionBlur/Combine");
            pass->setInput(0, "scene");
            pass->setInput(1, "sum");

			/// Copy back sum texture
            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("sum");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/MotionBlur/Copy");
            pass->setInput(0, "temp");

            /// Display result
            tp = t->getOutputTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_NONE);
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/MotionBlur/Copy");
            pass->setInput(0, "temp");
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
    MotionBlurPostFilter(const Ogre::String& name, const Ogre::String& compositor)
        : PostFilter(name, compositor)
    {
    }

    ~MotionBlurPostFilter()
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

extern PostFilterFactory* getMotionBlurPostFilterFactory(void)
{
    return MotionBlurPostFilter::getFactory();
}

}
