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

class FloodlightingPostFilter
    : public PostFilter
    , public CompositorInstance::Listener
{
private:
    class Factory : public PostFilterFactory
    {
    public:
        static const Ogre::String& _theType(void)
        {
            static const Ogre::String type = "Floodlighting";
            return type;
        }

        const Ogre::String& getType(void) const
        {
            return _theType();
        }

        PostFilter* createInstance(const Ogre::String& name)
        {
            return new FloodlightingPostFilter(name, _theType());
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
            /*td = t->createTextureDefinition("blur0");
            td->width.viewport = 0.5;
            td->height.viewport = 0.5;
            td->format = PF_X8R8G8B8;
            td = t->createTextureDefinition("blur1");
            td->width.viewport = 0.5;
            td->height.viewport = 0.5;
            td->format = PF_X8R8G8B8;*/

            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
            tp->setOutputName("scene");

            /*tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur0");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/DownSample");
            pass->setInput(0, "scene");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur1");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/BlurHorizontal");
            pass->setInput(0, "blur0");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur1");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/BlurVertical");
            pass->setInput(0, "blur0");

            tp = t->getOutputTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/nBlend");
            pass->setInput(0, "scene");
            pass->setInput(1, "blur1");
            pass->setIdentifier(0xDEADBADE);*/

			tp = t->getOutputTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
			pass = tp->createPass();
			pass->setType(CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("PostFilters/Floodlighting/noBlurBlend");
			pass->setInput(0, "scene");
			pass->setIdentifier(0xDEADBADF);

            //mBlurAmount = 0.2f;
            mShineAmount = 0.3f;
			mScaleAmount = 0.2f;
			mModulateColor = Ogre::Vector3(1.0,0.5,1.0);
            // receive default parameters from material script
            if (!pass->getMaterial().isNull())
            {
                GpuProgramParametersSharedPtr parameters = pass->getMaterial()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

				//david-<<
                //GpuProgramParameters::RealConstantEntry* entry;

                //entry = parameters->getNamedRealConstantEntry("shine_amount");
                //if (entry && entry->isSet)
                //    mShineAmount = entry->val[0];
	

				//entry = parameters->getNamedRealConstantEntry("scale");
				//if (entry && entry->isSet)
				//	mScaleAmount = entry->val[0];

				//entry = parameters->getNamedRealConstantEntry("modulateColor");
				//if (entry && entry->isSet)
				//	mModulateColor = entry->val[0];
				//

				//// use param name to get index : use appropiate paramters ptr
				const Ogre::GpuConstantDefinition& shine_amount_def = parameters->getConstantDefinition("shine_amount");
				// use index to get RealConstantEntry
				const float* pShineFloat = parameters->getFloatPointer(shine_amount_def.physicalIndex);
					// set position of ScrollWidget as param value
				mShineAmount = pShineFloat[0];


				const Ogre::GpuConstantDefinition& scale_def = parameters->getConstantDefinition("scale");
				// use index to get RealConstantEntry
				const float* pScaleFloat = parameters->getFloatPointer(scale_def.physicalIndex);
				// set position of ScrollWidget as param value
				mScaleAmount = pScaleFloat[0];


				const Ogre::GpuConstantDefinition& modulateColor_def = parameters->getConstantDefinition("modulateColor");
				// use index to get RealConstantEntry
				const float* pModulateColorFloat = parameters->getFloatPointer(modulateColor_def.physicalIndex);
				// set position of ScrollWidget as param value
				mModulateColor = pModulateColorFloat[0];

				//david->>
				
            }
        }

        //Ogre::Real mBlurAmount;
        Ogre::Real mShineAmount;
		Ogre::Real mScaleAmount;
		Ogre::Vector3 mModulateColor;
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
    FloodlightingPostFilter(const Ogre::String& name, const Ogre::String& compositor)
        : PostFilter(name, compositor)
        //, mBlurAmount(getFactory()->mBlurAmount)
        , mShineAmount(getFactory()->mShineAmount)
		, mScaleAmount(getFactory()->mScaleAmount)
		, mModulateColor(getFactory()->mModulateColor)
    {
    }

    ~FloodlightingPostFilter()
    {
        if (mCompositorInstance)
            mCompositorInstance->removeListener(this);
    }

    // from PostFilter
public:

    void setParameter(const Ogre::String& name, const Ogre::String& value)
    {
        //if (name == "BlurAmount")
        //{
        //    mBlurAmount =Ogre::StringConverter::parseReal(value);
        //}
        if (name == "ShineAmount")
        {
            mShineAmount =Ogre::StringConverter::parseReal(value);
        }
		else if (name == "ScaleAmount")
		{
			mScaleAmount =Ogre::StringConverter::parseReal(value);
		}
		else if (name == "ModulateColor")
		{
			mModulateColor =Ogre::StringConverter::parseVector3(value);
		}
        else
        {
            PostFilter::setParameter(name, value);
        }
    }

   Ogre::String getParameter(const Ogre::String& name) const
    {
        //if (name == "BlurAmount")
        //{
        //    return Ogre::StringConverter::toString(mBlurAmount);
        //}
        if (name == "ShineAmount")
        {
            return Ogre::StringConverter::toString(mShineAmount);
        }
		else if (name == "ScaleAmount")
		{
			return Ogre::StringConverter::toString(mScaleAmount);
		}
		else if (name == "ModulateColor")
		{
			return Ogre::StringConverter::toString(mModulateColor);
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
        if (pass_id == 0xDEADBADF)
        {
            mFragmentProgramParameters = mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
        }
    }

	void notifyMaterialRender(uint32 pass_id, MaterialPtr &mat)
    {
        if (pass_id == 0xDEADBADF)
        {
            // update gpu program parameters
            //mFragmentProgramParameters->setNamedConstant("blur_amount", mBlurAmount);
            mFragmentProgramParameters->setNamedConstant("shine_amount", mShineAmount);
			mFragmentProgramParameters->setNamedConstant("scale", mScaleAmount);
			mFragmentProgramParameters->setNamedConstant("modulateColor", mModulateColor);
        }
    }

private:
    //Ogre::Real mBlurAmount;
    Ogre::Real mShineAmount;
	Ogre::Real mScaleAmount;
	Ogre::Vector3 mModulateColor;
    GpuProgramParametersSharedPtr mFragmentProgramParameters;
};

//////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////

extern PostFilterFactory* getFloodlightingPostFilterFactory(void)
{
    return FloodlightingPostFilter::getFactory();
}

}
