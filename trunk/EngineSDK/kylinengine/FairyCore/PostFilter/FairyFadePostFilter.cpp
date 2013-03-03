#include "FairyPostFilter.h"
#include "FairyPostFilterFactory.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTechnique.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>

#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreGpuProgram.h>
#include <OgreStringConverter.h>

namespace Fairy {

using namespace Ogre;

class FadePostFilter
    : public PostFilter
    , public CompositorInstance::Listener
{
private:
    class Factory : public PostFilterFactory
    {
    public:
        static const Ogre::String& _theType(void)
        {
            static const Ogre::String type = "Fade";
            return type;
        }

        const Ogre::String& getType(void) const
        {
            return _theType();
        }

        PostFilter* createInstance(const Ogre::String& name)
        {
            return new FadePostFilter(name, _theType());
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
            td->formatList.push_back( PF_X8R8G8B8);

            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
            tp->setOutputName("scene");

            tp = t->getOutputTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Fade");
            pass->setInput(0, "scene");
            pass->setIdentifier(0xDEADBADE);

            // receive default parameters from material script
            mColourAmount = 0;
            mGrayAmount = 1;
            if (!pass->getMaterial().isNull())
            {
                GpuProgramParametersSharedPtr parameters = pass->getMaterial()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
#if OGRE_VERSION >= 0x010300
                const GpuConstantDefinition* def;
                
                def = parameters->_findNamedConstantDefinition("colour_amount");
                if (def)
                    parameters->_readRawConstants(def->physicalIndex, 1, &mColourAmount);
                def = parameters->_findNamedConstantDefinition("gray_amount");
                if (def)
                    parameters->_readRawConstants(def->physicalIndex, 1, &mGrayAmount);
#else
                GpuProgramParameters::RealConstantEntry* entry;

                entry = parameters->getNamedRealConstantEntry("colour_amount");
                if (entry && entry->isSet)
                    mColourAmount = entry->val[0];

                entry = parameters->getNamedRealConstantEntry("gray_amount");
                if (entry && entry->isSet)
                    mGrayAmount = entry->val[0];
#endif
            }
        }

        Ogre::Real mColourAmount;
        Ogre::Real mGrayAmount;
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
    FadePostFilter(const Ogre::String& name, const Ogre::String& compositor)
        : PostFilter(name, compositor)
        , mColourAmount(getFactory()->mColourAmount)
        , mGrayAmount(getFactory()->mGrayAmount)
    {
    }

    ~FadePostFilter()
    {
        if (mCompositorInstance)
            mCompositorInstance->removeListener(this);
    }

    // from PostFilter
public:

    void setParameter(const Ogre::String& name, const Ogre::String& value)
    {
        if (name == "ColourAmount")
        {
            mColourAmount =Ogre::StringConverter::parseReal(value);
        }
        else if (name == "GrayAmount")
        {
            mGrayAmount =Ogre::StringConverter::parseReal(value);
        }
        else
        {
            PostFilter::setParameter(name, value);
        }
    }

   Ogre::String getParameter(const Ogre::String& name) const
    {
        if (name == "ColourAmount")
        {
            return Ogre::StringConverter::toString(mColourAmount);
        }
        else if (name == "GrayAmount")
        {
            return Ogre::StringConverter::toString(mGrayAmount);
        }
    
        return PostFilter::getParameter(name);
    }

    void _init(Viewport* vp)
    {
        PostFilter::_init(vp);
        if (mCompositorInstance)
            mCompositorInstance->addListener(this);
    }

    // from CompositorInstance::Listener
public:

    void notifyMaterialSetup(uint32 pass_id, MaterialPtr &mat)
    {
        if (pass_id == 0xDEADBADE)
        {
            mFragmentProgramParameters = mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
        }
    }

	void notifyMaterialRender(uint32 pass_id, MaterialPtr &mat)
    {
        if (pass_id == 0xDEADBADE)
        {
            // update gpu program parameters
            mFragmentProgramParameters->setNamedConstant("colour_amount", mColourAmount);
            mFragmentProgramParameters->setNamedConstant("gray_amount", mGrayAmount);
        }
    }

private:
    Ogre::Real mColourAmount;
    Ogre::Real mGrayAmount;
    GpuProgramParametersSharedPtr mFragmentProgramParameters;
};

//////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////

extern PostFilterFactory* getFadePostFilterFactory(void)
{
    return FadePostFilter::getFactory();
}

}
