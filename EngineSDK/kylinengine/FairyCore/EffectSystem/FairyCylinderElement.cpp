/********************************************************************
filename:   FairyMeshElement.cpp

purpose:    a mesh effect element.
*********************************************************************/

#include "FairyCylinderElement.h"
#include "FairyEffect.h"
#include "Core/FairySystem.h"

// ogre header
//#include <OgreParticleSystem.h>
//#include <OgreParticleSystemManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreQuaternion.h>
#include <OgreRenderOperation.h>

#define _PI    ((Ogre::Real)  3.141592654f)
namespace Fairy	{

	CylinderElement::CmdMaterialName CylinderElement::msMaterialNameCmd;
	CylinderElement::CmdDownRadius CylinderElement::msDownRadiusCmd;
	CylinderElement::CmdUpRadius CylinderElement::msUpRadiusCmd;
	CylinderElement::CmdHeight CylinderElement::msHeightCmd;
	CylinderElement::CmdAngleSpeed CylinderElement::msAngleSpeed;
	CylinderElement::CmdStepCount CylinderElement::msStepCountCmd;

	CylinderElement::CmdScaleAdjust		CylinderElement::msScaleCmd[MAX_STAGES];
	CylinderElement::CmdScaleTimeAdjust	CylinderElement::msScaleTimeCmd[MAX_STAGES];
	CylinderElement::CmdScaleChangeTime    CylinderElement::msScaleChangeTime;

	CylinderElement::CmdColourAdjust		CylinderElement::msColourCmd[MAX_STAGES];
	CylinderElement::CmdColourTimeAdjust	CylinderElement::msColourTimeCmd[MAX_STAGES];
	CylinderElement::CmdColourChangeTime   CylinderElement::msColourChangeTime;


	CylinderElement::CmdUpRadiusAdjust	CylinderElement::msUpRadiusAdjustCmd[MAX_STAGES];
	CylinderElement::CmdUpRadiusTimeAdjust	CylinderElement::msUpRadiusTimeCmd[MAX_STAGES];
	CylinderElement::CmdUpRadiusChangeTime CylinderElement::msUpRadiusChangeTime;

	CylinderElement::CmdDownRadiusAdjust	CylinderElement::msDownRadiusAdjustCmd[MAX_STAGES];
	CylinderElement::CmdDownRadiusTimeAdjust	CylinderElement::msDownRadiusTimeCmd[MAX_STAGES];
	CylinderElement::CmdDownRadiusChangeTime CylinderElement::msDownRadiusChangeTime;

	CylinderElement::CylinderElement(const Ogre::String &type, System *system) :
	EffectElement(type, system),
	mManualObject(NULL),
	mDownRadius(50.0f),
	mUpRadius(50.0f),
	mHeight(20.0f),
	mStepCount(20),
	mAngleSpeed(0.0f),
	mScaleChangeTime(0.0f),
	mColourChangeTime(0.0f),
	mUpRadiusChangeTime(0.0f),
	mDownRadiusChangeTime(0.0f),
	mUpRadiusScale(1.0f),
	mDownRadiusScale(1.0f),
	mCylinderColour(Ogre::ColourValue::White),
	mMaterialName("")
	{
		for (int i=0;i<MAX_STAGES;i++)
		{
			mScaleAdj[i]	= Ogre::Vector3(1.0f, 1.0f, 1.0f);
			mScaleTimeAdj[i]		= 1.0f;

			mColourAdj[i]	= Ogre::ColourValue::White;
			mColourTimeAdj[i]		= 1.0f;

			mUpRadiusAdj[i] = 1.0f;
			mUpRadiusTimeAdj[i] = 1.0f;

			mDownRadiusAdj[i] = 1.0f;
			mDownRadiusTimeAdj[i] = 1.0f;

		}
		if ( initDefaults(mElementType) )
		{
			Ogre::ParamDictionary* dict = getParamDictionary();


			dict->addParameter(Ogre::ParameterDef("Material", 
			"the name of material.",
			Ogre::PT_STRING),&msMaterialNameCmd);

			dict->addParameter(Ogre::ParameterDef("DownRadius", 
				"The down radius of Cylinder.",
				Ogre::PT_REAL),&msDownRadiusCmd);

			dict->addParameter(Ogre::ParameterDef("UpRadius", 
				"The up radius of Cylinder.",
				Ogre::PT_REAL),&msUpRadiusCmd);

			dict->addParameter(Ogre::ParameterDef("Height", 
				"The height of Cylinder.",
				Ogre::PT_REAL),&msHeightCmd);

			dict->addParameter(Ogre::ParameterDef("AngleSpeed", 
				"The angle speed of Cylinder.",
				Ogre::PT_REAL),&msAngleSpeed);

			dict->addParameter(Ogre::ParameterDef("StepCount", 
				"The step of Cylinder mesh",
				Ogre::PT_UNSIGNED_INT),&msStepCountCmd);

			for (int i=0;i<MAX_STAGES;i++)
			{
				msScaleCmd[i].mIndex	= i;
				msScaleTimeCmd[i].mIndex		= i;

				Ogre::StringUtil::StrStreamType stage;
				stage << i;
				String	scale_title	=Ogre::String("Scale") + stage.str();
				String	time_title		=Ogre::String("ScaleTime") + stage.str();
				String	scale_descr	=Ogre::String("Stage ") + stage.str() +Ogre::String(" Scale.");
				String	time_descr		=Ogre::String("Stage ") + stage.str() +Ogre::String(" ScaleTime.");

				dict->addParameter(Ogre::ParameterDef(scale_title, scale_descr, Ogre::PT_VECTOR3), &msScaleCmd[i]);
				dict->addParameter(Ogre::ParameterDef(time_title,   time_descr,   Ogre::PT_REAL),		 &msScaleTimeCmd[i]);
			}

			dict->addParameter(Ogre::ParameterDef("ScaleChangeTime", 
				"the size of projected terrain or object.",
				Ogre::PT_REAL),&msScaleChangeTime);  

			for (int i=0;i<MAX_STAGES;i++)
			{
				msColourCmd[i].mIndex	= i;
				msColourTimeCmd[i].mIndex		= i;

				Ogre::StringUtil::StrStreamType stage;
				stage << i;
				String	colour_title	=Ogre::String("Colour") + stage.str();
				String	time_title		=Ogre::String("ColourTime") + stage.str();
				String	colour_descr	=Ogre::String("Stage ") + stage.str() +Ogre::String(" Colour.");
				String	time_descr		=Ogre::String("Stage ") + stage.str() +Ogre::String(" ColourTime.");

				dict->addParameter(Ogre::ParameterDef(colour_title, colour_descr, Ogre::PT_COLOURVALUE), &msColourCmd[i]);
				dict->addParameter(Ogre::ParameterDef(time_title,   time_descr,   Ogre::PT_REAL),		 &msColourTimeCmd[i]);
			}

			dict->addParameter(Ogre::ParameterDef("ColourChangeTime", 
				"the size of projected terrain or object.",
				Ogre::PT_REAL),&msColourChangeTime); 


			for (int i=0;i<MAX_STAGES;i++)
			{
				msUpRadiusAdjustCmd[i].mIndex	= i;
				msUpRadiusTimeCmd[i].mIndex		= i;

				Ogre::StringUtil::StrStreamType stage;
				stage << i;
				String	upradius_title	=Ogre::String("UpRadiusScale") + stage.str();
				String	time_title		=Ogre::String("UpRadiusTime") + stage.str();
				String	upradius_descr	=Ogre::String("Stage ") + stage.str() +Ogre::String(" UpRadiusScale.");
				String	time_descr		=Ogre::String("Stage ") + stage.str() +Ogre::String(" UpRadiusTime.");

				dict->addParameter(Ogre::ParameterDef(upradius_title, upradius_descr, Ogre::PT_REAL), &msUpRadiusAdjustCmd[i]);
				dict->addParameter(Ogre::ParameterDef(time_title,   time_descr,   Ogre::PT_REAL), &msUpRadiusTimeCmd[i]);
			}

			dict->addParameter(Ogre::ParameterDef("UpRadiusScaleChangeTime", 
				"the size of projected terrain or object.",
				Ogre::PT_REAL),&msUpRadiusChangeTime); 



			for (int i=0;i<MAX_STAGES;i++)
			{
				msDownRadiusAdjustCmd[i].mIndex	= i;
				msDownRadiusTimeCmd[i].mIndex		= i;

				Ogre::StringUtil::StrStreamType stage;
				stage << i;
				String	downradius_title	=Ogre::String("DownRadiusScale") + stage.str();
				String	time_title		=Ogre::String("DownRadiusTime") + stage.str();
				String	downradius_descr	=Ogre::String("Stage ") + stage.str() +Ogre::String(" DownRadiusScale.");
				String	time_descr		=Ogre::String("Stage ") + stage.str() +Ogre::String(" DownRadiusTime.");

				dict->addParameter(Ogre::ParameterDef(downradius_title, downradius_descr, Ogre::PT_REAL), &msDownRadiusAdjustCmd[i]);
				dict->addParameter(Ogre::ParameterDef(time_title,   time_descr,   Ogre::PT_REAL),		 &msDownRadiusTimeCmd[i]);
			}

			dict->addParameter(Ogre::ParameterDef("DownRadiusScaleChangeTime", 
				"the size of projected terrain or object.",
				Ogre::PT_REAL),&msDownRadiusChangeTime); 
		}
	}
	//---------------------------------------------------------------------
	CylinderElement::~CylinderElement()
	{
		destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	bool CylinderElement::createRenderInstance(void)
	{
        return _createMesh();
	}
	//---------------------------------------------------------------------
	void CylinderElement::destroyRenderInstance(void)
	{	
		_destroyMesh();  

		EffectElement::destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	void CylinderElement::destroyMe(void)
	{
		EffectElement::destroyMe();
	}
	//---------------------------------------------------------------------
	bool CylinderElement::_createMesh(void)
	{
		assert ( NULL == mManualObject );
		//assert ( false == mMeshName.empty() );

		if (mBasicNode)
		{
			mManualObject = mSystem->getSceneManager()->createManualObject(mBasicNode->getName());
			mBasicNode->attachObject(mManualObject);
			mManualObject->setCastShadows(false);
			mManualObject->setVisible(mVisible);

			if(mHeight <0.01f || mStepCount < 3)
				return false;
			mManualObject->setDynamic(true);
			mManualObject->clear();

			mManualObject->begin(mMaterialName,Ogre::RenderOperation::OT_TRIANGLE_LIST);

			for(unsigned int i= 0;i <= mStepCount; i++)
			{
				Ogre::Real theta = (Ogre::Real)(2*_PI*i)/(Ogre::Real)(mStepCount);

				mManualObject->position(sinf(theta)*mDownRadius,0.0f,cosf(theta)*mDownRadius );
				mManualObject->textureCoord((Ogre::Real)i/(Ogre::Real)mStepCount,1.0f);
				mManualObject->position(sinf(theta)*mUpRadius,mHeight,cosf(theta)*mUpRadius );
				mManualObject->textureCoord((Ogre::Real)i/(Ogre::Real)mStepCount,0.0f);
			}
			for(unsigned int i= 0;i < mStepCount; i++)
			{

				mManualObject->index(i*2 + 0);
				mManualObject->index(i*2 + 3);
				mManualObject->index(i*2 + 1);

				mManualObject->index(i*2 + 0);
				mManualObject->index(i*2 + 2);
				mManualObject->index(i*2 + 3);
			}

			mManualObject->end();

			return true;
		}
		return false;
	}

	void CylinderElement::updateRenderInstance( Ogre::Real time, TransformInfo &info )
	{
		mCurrentLifeTime += time;
		
		_updateCylinderScale();
		_updateCylneRadius();
		_updateCylinderColour();
	}
	//---------------------------------------------------------------------
	void CylinderElement::_destroyMesh(void)
	{
		if (mManualObject && mBasicNode)
		{   

			mBasicNode->detachObject(mManualObject);

			mSystem->getSceneManager()->destroyManualObject(mManualObject);

			mManualObject = NULL;
		}
	}

	void CylinderElement::_updateCylinderScale(void)
	{
		if (mCurrentLifeTime > 0.0f && mScaleChangeTime > 0.0f)
		{
			Ogre::Real			cylinder_time	= fmod( mCurrentLifeTime / mScaleChangeTime, 1.0f );
			if(mBasicNode)
			{
				if (cylinder_time <= mScaleTimeAdj[0])
				{
					mBasicNode->setScale(mScaleAdj[0].x, mScaleAdj[0].y,mScaleAdj[0].z);
				} 
				else if (cylinder_time >= mScaleTimeAdj[MAX_STAGES - 1])
				{
					mBasicNode->setScale(mScaleAdj[MAX_STAGES-1].x, mScaleAdj[MAX_STAGES-1].y,mScaleAdj[MAX_STAGES-1].z);
				}
				else
				{
					for (int i=0;i<MAX_STAGES-1;i++)
					{
						if (cylinder_time >= mScaleTimeAdj[i] && cylinder_time < mScaleTimeAdj[i + 1])
						{
							cylinder_time -= mScaleTimeAdj[i];
							cylinder_time /= (mScaleTimeAdj[i+1]-mScaleTimeAdj[i]);

							mBasicNode->setScale(
								(mScaleAdj[i+1].x * cylinder_time) + (mScaleAdj[i].x * (1.0f - cylinder_time)),
								(mScaleAdj[i+1].y * cylinder_time) + (mScaleAdj[i].y * (1.0f - cylinder_time)),
								(mScaleAdj[i+1].z * cylinder_time) + (mScaleAdj[i].z * (1.0f - cylinder_time))
								);
							break;
						}
					}
				}

			}
			
		}
	}

	void CylinderElement::_updateCylneRadius(void)
	{
		if (mCurrentLifeTime > 0.0f && (mUpRadiusChangeTime > 0.0f || mDownRadiusChangeTime > 0.0f))
		{
			Ogre::Real			cylinder_time	= fmod( mCurrentLifeTime / mColourChangeTime, 1.0f );

		/*	if (cylinder_time <= mColourTimeAdj[0])
			{
				_updateCylneRadiusScale(mColourAdj[0],;
			} 
			else if (cylinder_time >= mColourTimeAdj[MAX_STAGES - 1])
			{
				mCylinderColour = mColourAdj[MAX_STAGES - 1];
			}
			else
			{
				for (int i=0;i<MAX_STAGES-1;i++)
				{
					if (cylinder_time >= mColourTimeAdj[i] && cylinder_time < mColourTimeAdj[i + 1])
					{
						cylinder_time -= mColourTimeAdj[i];
						cylinder_time /= (mColourTimeAdj[i+1]-mColourTimeAdj[i]);

						mCylinderColour.r = ((mColourAdj[i+1].r * cylinder_time) + (mColourAdj[i].r * (1.0f - cylinder_time)));
						mCylinderColour.g = ((mColourAdj[i+1].g * cylinder_time) + (mColourAdj[i].g * (1.0f - cylinder_time)));
						mCylinderColour.b = ((mColourAdj[i+1].b * cylinder_time) + (mColourAdj[i].b * (1.0f - cylinder_time)));
						mCylinderColour.a = ((mColourAdj[i+1].a * cylinder_time) + (mColourAdj[i].a * (1.0f - cylinder_time)));
						break;
					}
				}
			}*/

		}
	}

	void CylinderElement::_updateCylneRadiusScale(Ogre::Real upScale,Ogre::Real downScale )
	{
		if(mUpRadiusScale == upScale && mDownRadiusScale == downScale )
			return;
		if(!mManualObject)
			return;

		mManualObject->clear();

		mManualObject->begin(mMaterialName,Ogre::RenderOperation::OT_TRIANGLE_LIST);

		for(unsigned int i= 0;i <= mStepCount; i++)
		{
			Ogre::Real theta = (Ogre::Real)(2*_PI*i)/(Ogre::Real)(mStepCount);

			mManualObject->position(sinf(theta)*mDownRadius*downScale,0.0f,cosf(theta)*mDownRadius*downScale );
			mManualObject->textureCoord((Ogre::Real)i/(Ogre::Real)mStepCount,1.0f);
			mManualObject->position(sinf(theta)*mUpRadius*upScale,mHeight,cosf(theta)*mUpRadius*upScale);
			mManualObject->textureCoord((Ogre::Real)i/(Ogre::Real)mStepCount,0.0f);
		}
		for(unsigned int i= 0;i < mStepCount; i++)
		{

			mManualObject->index(i*2 + 0);
			mManualObject->index(i*2 + 3);
			mManualObject->index(i*2 + 1);

			mManualObject->index(i*2 + 0);
			mManualObject->index(i*2 + 2);
			mManualObject->index(i*2 + 3);
		}

		mManualObject->end();

		mUpRadiusScale = upScale;
		mDownRadiusScale = downScale;
	}

	void CylinderElement::_updateCylinderColour(void)
	{
		if (mCurrentLifeTime > 0.0f && mColourChangeTime > 0.0f)
		{
			Ogre::Real			cylinder_time	= fmod( mCurrentLifeTime / mColourChangeTime, 1.0f );

			if (cylinder_time <= mColourTimeAdj[0])
			{
				mCylinderColour = mColourAdj[0];
			} 
			else if (cylinder_time >= mColourTimeAdj[MAX_STAGES - 1])
			{
				mCylinderColour = mColourAdj[MAX_STAGES - 1];
			}
			else
			{
				for (int i=0;i<MAX_STAGES-1;i++)
				{
					if (cylinder_time >= mColourTimeAdj[i] && cylinder_time < mColourTimeAdj[i + 1])
					{
						cylinder_time -= mColourTimeAdj[i];
						cylinder_time /= (mColourTimeAdj[i+1]-mColourTimeAdj[i]);

						mCylinderColour.r = ((mColourAdj[i+1].r * cylinder_time) + (mColourAdj[i].r * (1.0f - cylinder_time)));
						mCylinderColour.g = ((mColourAdj[i+1].g * cylinder_time) + (mColourAdj[i].g * (1.0f - cylinder_time)));
						mCylinderColour.b = ((mColourAdj[i+1].b * cylinder_time) + (mColourAdj[i].b * (1.0f - cylinder_time)));
						mCylinderColour.a = ((mColourAdj[i+1].a * cylinder_time) + (mColourAdj[i].a * (1.0f - cylinder_time)));
						break;
					}
				}
			}
		}

		Ogre::MaterialPtr matPtr = mManualObject->getSection(0)->getMaterial();

		if (false == matPtr.isNull())
		{
			Ogre::Material::TechniqueIterator ti = matPtr->getTechniqueIterator();
			while (ti.hasMoreElements())
			{
				Ogre::Technique* technique = ti.getNext();
				Ogre::Technique::PassIterator pi = technique->getPassIterator();
				while (pi.hasMoreElements())
				{
					Ogre::Pass* pass = pi.getNext();

					// set the colour of projector material
					pass->setSelfIllumination(mCylinderColour);

				}
			}	
		}

	}

	//---------------------------------------------------------------------
	void CylinderElement::setMaterialName(const Ogre::String &name)
	{
		mMaterialName = name;

		if (mManualObject)
		{
			mManualObject->setMaterialName(0,name);
		}		
	}
    //-----------------------------------------------------------------------
    void CylinderElement::setVisibleFlag(uint32 flags)
    {
        if (mManualObject)
            mManualObject->setVisibilityFlags(flags);
    }
	Ogre::Real CylinderElement::getDownRadius() const
	{
		return mDownRadius;
	}

	void CylinderElement::setDownRadius(Ogre::Real radius)
	{
		mDownRadius = radius;
	}

	Ogre::Real CylinderElement::getUpRadius() const
	{
		return mUpRadius;
	}

	void CylinderElement::setUpRadius(Ogre::Real radius)
	{
		mUpRadius = radius;
	}

	void CylinderElement::setHeight(Ogre::Real height)
	{
		mHeight = height;
	}

	Ogre::Real CylinderElement::getHeight() const
	{
		return mHeight;
	}

	void CylinderElement::setAngleSpeed(Ogre::Real speed)
	{
		mAngleSpeed = speed;
	}
	Ogre::Real CylinderElement::getAngleSpeed() const
	{
		return mAngleSpeed;
	}

	void CylinderElement::setStepCount(unsigned int count)
	{
		mStepCount = count;
	}

	unsigned int CylinderElement::getStepCount() const
	{
		return mStepCount;
	}

	void CylinderElement::setScaleAdjust(size_t index, CylinderScaleValue& scale)
	{
		mScaleAdj[index] = scale;
	}
	//-----------------------------------------------------------------------
	CylinderScaleValue CylinderElement::getScaleAdjust(size_t index) const
	{
		return mScaleAdj[index];
	}
	//-----------------------------------------------------------------------
	void CylinderElement::setScaleTimeAdjust(size_t index, Ogre::Real time)
	{
		mScaleTimeAdj[index] = time;
	}
	//-----------------------------------------------------------------------
	Ogre::Real CylinderElement::getScaleTimeAdjust(size_t index) const
	{
		return mScaleTimeAdj[index];
	}

	void CylinderElement::setScaleChangeTime(Ogre::Real time)
	{
		mScaleChangeTime = time;
	}

	void CylinderElement::setColourAdjust(size_t index, Ogre::ColourValue colour)
	{
		mColourAdj[index] = colour;
	}

	Ogre::ColourValue CylinderElement::getColourAdjust(size_t index) const
	{
		return mColourAdj[index];
	}
	//-----------------------------------------------------------------------
	void CylinderElement::setColourTimeAdjust(size_t index, Ogre::Real time)
	{
		mColourTimeAdj[index] = time;
	}
	//-----------------------------------------------------------------------
	Ogre::Real CylinderElement::getColourTimeAdjust(size_t index) const
	{
		return mColourTimeAdj[index];
	}
	void CylinderElement::setColourChangeTime(Ogre::Real time)
	{
		mColourChangeTime = time;
	}


	void CylinderElement::setUpRadiusAdjust(size_t index, Ogre::Real radius)
	{
		mUpRadiusAdj[index] = radius;
	}

	Ogre::Real CylinderElement::getUpRadiusAdjust(size_t index) const
	{
		return mUpRadiusAdj[index];
	}
	//-----------------------------------------------------------------------
	void CylinderElement::setUpRadiusTimeAdjust(size_t index, Ogre::Real time)
	{
		mUpRadiusTimeAdj[index] = time;
	}
	//-----------------------------------------------------------------------
	Ogre::Real CylinderElement::getUpRadiusTimeAdjust(size_t index) const
	{
		return mUpRadiusTimeAdj[index];
	}
	void CylinderElement::setUpRadiusChangeTime(Ogre::Real time)
	{
		mUpRadiusChangeTime = time;
	}

	void CylinderElement::setDownRadiusAdjust(size_t index, Ogre::Real radius)
	{
		mDownRadiusAdj[index] = radius;
	}

	Ogre::Real CylinderElement::getDownRadiusAdjust(size_t index) const
	{
		return mDownRadiusAdj[index];
	}
	//-----------------------------------------------------------------------
	void CylinderElement::setDownRadiusTimeAdjust(size_t index, Ogre::Real time)
	{
		mDownRadiusTimeAdj[index] = time;
	}
	//-----------------------------------------------------------------------
	Ogre::Real CylinderElement::getDownRadiusTimeAdjust(size_t index) const
	{
		return mDownRadiusTimeAdj[index];
	}
	void CylinderElement::setDownRadiusChangeTime(Ogre::Real time)
	{
		mDownRadiusChangeTime = time;
	}



	String CylinderElement::CmdMaterialName::doGet(const void* target) const
	{
		return static_cast<const CylinderElement*>(target)->getMaterialName();
	}
	void CylinderElement::CmdMaterialName::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement *>(target)->setMaterialName(val);
	}
	String CylinderElement::CmdDownRadius::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(static_cast<const CylinderElement*>(target)->getDownRadius());
	}
	void CylinderElement::CmdDownRadius::doSet(void* target, const String& val)
	{
		assert (target);
		static_cast<CylinderElement*>(target)->setDownRadius(Ogre::StringConverter::parseReal(val));
	}
	String CylinderElement::CmdUpRadius::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(static_cast<const CylinderElement*>(target)->getUpRadius());
	}
	void CylinderElement::CmdUpRadius::doSet(void* target, const String& val)
	{
		assert (target);
		static_cast<CylinderElement*>(target)->setUpRadius(Ogre::StringConverter::parseReal(val));
	}

	String CylinderElement::CmdHeight::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(static_cast<const CylinderElement*>(target)->getHeight());
	}
	void CylinderElement::CmdHeight::doSet(void* target, const String& val)
	{
		assert (target);
		static_cast<CylinderElement*>(target)->setHeight(Ogre::StringConverter::parseReal(val));
	}

	String CylinderElement::CmdAngleSpeed::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(static_cast<const CylinderElement*>(target)->getAngleSpeed());
	}
	void CylinderElement::CmdAngleSpeed::doSet(void* target, const String& val)
	{
		assert (target);
		static_cast<CylinderElement*>(target)->setAngleSpeed(Ogre::StringConverter::parseReal(val));
	}

	String CylinderElement::CmdStepCount::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(static_cast<const CylinderElement*>(target)->getStepCount());
	}
	void CylinderElement::CmdStepCount::doSet(void* target, const String& val)
	{
		assert (target);
		static_cast<CylinderElement*>(target)->setStepCount(Ogre::StringConverter::parseUnsignedInt(val));
	}

	//-----------------------------------------------------------------------
	String CylinderElement::CmdScaleAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getScaleAdjust(mIndex) );
	}
	void CylinderElement::CmdScaleAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setScaleAdjust(mIndex,
			Ogre::StringConverter::parseVector3(val));
	}
	//-----------------------------------------------------------------------
	String CylinderElement::CmdScaleTimeAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getScaleTimeAdjust(mIndex) );
	}
	void CylinderElement::CmdScaleTimeAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setScaleTimeAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}

	String CylinderElement::CmdScaleChangeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getScaleChangeTime() );
	}
	void CylinderElement::CmdScaleChangeTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setScaleChangeTime(
			Ogre::StringConverter::parseReal(val));
	}

	String CylinderElement::CmdColourAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getColourAdjust(mIndex) );
	}
	void CylinderElement::CmdColourAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setColourAdjust(mIndex,
			Ogre::StringConverter::parseColourValue(val));
	}

	//-----------------------------------------------------------------------
	String CylinderElement::CmdColourTimeAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getColourTimeAdjust(mIndex) );
	}
	void CylinderElement::CmdColourTimeAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setColourTimeAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}
	String CylinderElement::CmdColourChangeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getColourChangeTime() );
	}
	void CylinderElement::CmdColourChangeTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setColourChangeTime(
			Ogre::StringConverter::parseReal(val));
	}


	//////////////////////////////////////////////////////////////////////////////
	String CylinderElement::CmdUpRadiusAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getUpRadiusAdjust(mIndex) );
	}
	void CylinderElement::CmdUpRadiusAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setUpRadiusAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}

	//-----------------------------------------------------------------------
	String CylinderElement::CmdUpRadiusTimeAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getUpRadiusTimeAdjust(mIndex) );
	}
	void CylinderElement::CmdUpRadiusTimeAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setUpRadiusTimeAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}
	String CylinderElement::CmdUpRadiusChangeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getUpRadiusChangeTime() );
	}
	void CylinderElement::CmdUpRadiusChangeTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setUpRadiusChangeTime(
			Ogre::StringConverter::parseReal(val));
	}


	//////////////////////////////////////////////////////////////////////////////
	String CylinderElement::CmdDownRadiusAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getDownRadiusAdjust(mIndex) );
	}
	void CylinderElement::CmdDownRadiusAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setDownRadiusAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}

	//-----------------------------------------------------------------------
	String CylinderElement::CmdDownRadiusTimeAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getDownRadiusTimeAdjust(mIndex) );
	}
	void CylinderElement::CmdDownRadiusTimeAdjust::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setDownRadiusTimeAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}
	String CylinderElement::CmdDownRadiusChangeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const CylinderElement*>(target)->getDownRadiusChangeTime() );
	}
	void CylinderElement::CmdDownRadiusChangeTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<CylinderElement*>(target)->setDownRadiusChangeTime(
			Ogre::StringConverter::parseReal(val));
	}


}