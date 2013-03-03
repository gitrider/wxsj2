/********************************************************************
filename:   FairyMeshElement.h

purpose:    a mesh effect element.
*********************************************************************/

#ifndef __CYLINEDERELEMENT_H__
#define __CYLINEDERELEMENT_H__

// fairy header
#include "FairyEffectElement.h"
#include "Core/FairyPrerequisites.h"
#include "OgreManualObject.h"
// ogre header
namespace Ogre	{

}

namespace Fairy	{

	typedef Ogre::Vector3 CylinderScaleValue;

	class CylinderElement : public EffectElement
	{
	public:


		class CmdMaterialName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

        };
		/** Command object for area emitter size (see ParamCommand).*/
		class CmdDownRadius : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		class CmdUpRadius : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		class CmdHeight : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		/** Command object for area emitter size (see ParamCommand).*/
		class CmdStepCount : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);      
		};

		class CmdAngleSpeed : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		enum { MAX_STAGES = 4 };

		class CmdScaleAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);	
		};
		/** the scale time adjust of each stage */
		class CmdScaleTimeAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};
		/** the scale change time */
		class CmdScaleChangeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		class CmdColourAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		/** the colour time adjust of each stage */
		class CmdColourTimeAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

		};
		/** the colour change time */
		class CmdColourChangeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

		};


		/////////////////////////////////////////////////////////////

		class CmdUpRadiusAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};
		class CmdUpRadiusTimeAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

		};

		class CmdUpRadiusChangeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

		};

		/////////////////////////////////////////////////////////////

		class CmdDownRadiusAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};


		class CmdDownRadiusTimeAdjust : public Ogre::ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

		};

		class CmdDownRadiusChangeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

		};

		/////////////////////////////////////////////////////////////
		CylinderElement(const Ogre::String &type, System *system);
		~CylinderElement();
		
		virtual bool createRenderInstance(void);
		virtual void destroyRenderInstance(void);
		virtual void updateRenderInstance( Ogre::Real time, TransformInfo &info );

		virtual void destroyMe(void);


		void setMaterialName(const Ogre::String &name);

		const Ogre::String &getMaterialName(void) const	{	return mMaterialName;	}

		void setDownRadius(Ogre::Real radius);
		Ogre::Real getDownRadius() const;

		void setUpRadius(Ogre::Real radius);
		Ogre::Real getUpRadius() const;

		void setHeight(Ogre::Real height);
		Ogre::Real getHeight() const;

		void setAngleSpeed(Ogre::Real speed);
		Ogre::Real getAngleSpeed() const;

		void setStepCount(unsigned int count);
		unsigned int getStepCount() const;


		void setScaleAdjust(size_t index, CylinderScaleValue& scale);
		CylinderScaleValue getScaleAdjust(size_t index) const;

		void setScaleTimeAdjust(size_t index, Ogre::Real time);
		Ogre::Real getScaleTimeAdjust(size_t index) const;		

		void setScaleChangeTime(Ogre::Real time);
		Ogre::Real getScaleChangeTime(void) const
		{
			return mScaleChangeTime;
		}

		void setColourAdjust(size_t index, Ogre::ColourValue colour);
		Ogre::ColourValue getColourAdjust(size_t index) const;

		void setColourTimeAdjust(size_t index, Ogre::Real time);
		Ogre::Real getColourTimeAdjust(size_t index) const;

		void setColourChangeTime(Ogre::Real time);
		Ogre::Real getColourChangeTime(void) const
		{
			return mColourChangeTime;
		}


		void setUpRadiusAdjust(size_t index, Ogre::Real radius);
		Ogre::Real getUpRadiusAdjust(size_t index) const;

		void setUpRadiusTimeAdjust(size_t index, Ogre::Real time);
		Ogre::Real getUpRadiusTimeAdjust(size_t index) const;

		void setUpRadiusChangeTime(Ogre::Real time);
		Ogre::Real getUpRadiusChangeTime(void) const
		{
			return mUpRadiusChangeTime;
		}

		void setDownRadiusAdjust(size_t index, Ogre::Real radius);
		Ogre::Real getDownRadiusAdjust(size_t index) const;

		void setDownRadiusTimeAdjust(size_t index, Ogre::Real time);
		Ogre::Real getDownRadiusTimeAdjust(size_t index) const;

		void setDownRadiusChangeTime(Ogre::Real time);
		Ogre::Real getDownRadiusChangeTime(void) const
		{
			return mDownRadiusChangeTime;
		}


        /// …Ë÷√visible flag
        virtual void setVisibleFlag(uint32 flags);

	protected:

		bool _createMesh(void);
		void _destroyMesh(void);

		void _updateCylinderScale(void);
		void _updateCylneRadius(void );
		void _updateCylneRadiusScale(Ogre::Real upScale,Ogre::Real downScale );
		void _updateCylinderColour(void);

	protected:

		String mMaterialName;
		Ogre::ManualObject* mManualObject;

		Ogre::Real mDownRadius;
		Ogre::Real mUpRadius;
		Ogre::Real mHeight;
		Ogre::Real mAngleSpeed;
		unsigned int mStepCount;

		Ogre::Real mUpRadiusScale;
		Ogre::Real mDownRadiusScale;

		CylinderScaleValue		mScaleAdj[MAX_STAGES];
		float					mScaleTimeAdj[MAX_STAGES];

		Ogre::ColourValue		mColourAdj[MAX_STAGES];
		float					mColourTimeAdj[MAX_STAGES];

		float					mUpRadiusAdj[MAX_STAGES];
		float					mUpRadiusTimeAdj[MAX_STAGES];

		float					mDownRadiusAdj[MAX_STAGES];
		float					mDownRadiusTimeAdj[MAX_STAGES];

		float mScaleChangeTime;
		float mColourChangeTime;

		float mUpRadiusChangeTime;
		float mDownRadiusChangeTime;

		static CmdScaleAdjust	msScaleCmd[MAX_STAGES];
		static CmdScaleTimeAdjust	msScaleTimeCmd[MAX_STAGES];
		static CmdScaleChangeTime msScaleChangeTime;


		static CmdColourAdjust	msColourCmd[MAX_STAGES];
		static CmdColourTimeAdjust	msColourTimeCmd[MAX_STAGES];
		static CmdColourChangeTime msColourChangeTime;

		static CmdUpRadiusAdjust	msUpRadiusAdjustCmd[MAX_STAGES];
		static CmdUpRadiusTimeAdjust	msUpRadiusTimeCmd[MAX_STAGES];
		static CmdUpRadiusChangeTime msUpRadiusChangeTime;

		static CmdDownRadiusAdjust	msDownRadiusAdjustCmd[MAX_STAGES];
		static CmdDownRadiusTimeAdjust	msDownRadiusTimeCmd[MAX_STAGES];
		static CmdDownRadiusChangeTime msDownRadiusChangeTime;

		Ogre::ColourValue mCylinderColour;


		static CmdMaterialName msMaterialNameCmd;
		static CmdDownRadius msDownRadiusCmd;
		static CmdUpRadius msUpRadiusCmd;
		static CmdHeight msHeightCmd;
		static CmdAngleSpeed msAngleSpeed;
		static CmdStepCount msStepCountCmd;
	};
}

#endif