/********************************************************************
filename:   FairyBeamElement.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    a billboard set effect element,this can use in weapon effect
*********************************************************************/

#ifndef __BeamElement_H__
#define __BeamElement_H__

#include "FairyEffectElement.h"
#include "Core/FairyPrerequisites.h"

// ogre header
#include <OgreColourValue.h>

namespace Ogre	{
	class EffectBillboardChain;
}

namespace Fairy	{
	
	class BeamElement : public EffectElement
	{
	public:		

		// ��������
		class CmdNoiseXMin : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdNoiseXMax : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdNoiseYMin : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdNoiseYMax : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdNoiseZMin : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdNoiseZMax : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdNumElements : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdWidth : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };


/* laim �汾����
		class CmdWidthStart : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };


		class CmdWidthEnd : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

*/

		class CmdColour : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdColourRangeStart : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdColourRangeEnd : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdMaterial : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdFrequency : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);

/*laim �汾����
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdDestPos : public Ogre::ParamCommand
		{
		public:
			String doGet( const void *target ) const;
			void doSet( void *target, const String &val );
*/
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/////////////////////////////////////////////////////////////
		BeamElement(const Ogre::String &type, System *system);
		~BeamElement();

		virtual bool createRenderInstance(void);
		virtual void destroyRenderInstance(void);
		virtual void updateRenderInstance( Ogre::Real time, TransformInfo &info );

		virtual void updateExtraTransformInfos( const TransformInfos &infos );

		//////////////////////////////////////////////////////////////////////////
		void setNumBillboardElements( int num );
		int getNumBillboardElements(void) const
		{
			return mNumBillboardElements;
		}

		void setColour(const Ogre::ColourValue& colour);
		void setColour(const Ogre::ColourValue& colourStart, const Ogre::ColourValue& colourEnd);
		void setColourRangeStart(const Ogre::ColourValue& colour);
		void setColourRangeEnd(const Ogre::ColourValue& colour);
		const Ogre::ColourValue& getColour(void) const;
		const Ogre::ColourValue& getColourRangeStart(void) const;
		const Ogre::ColourValue& getColourRangeEnd(void) const;

		void setMaterialName(const Ogre::String &matName);
		const Ogre::String getMaterialName(void) const
		{
			return mMaterialName;
		}

		void setFrequency(float frequency);
		float getFrequency(void) const
		{
			return mFrequency;
		}

        /// ����visible flag
        virtual void setVisibleFlag(uint32 flags);

        virtual void sleep(void);

	protected:

		bool _createBillboardChains(void);
		void _destroyBillboardChains(void);
		void _updateBillboardChains( Ogre::Real time );
		
		void genColour(Ogre::ColourValue& destColour);

	public:

		float mNoiseXMin;
		float mNoiseXMax;
		float mNoiseYMin;
		float mNoiseYMax;
		float mNoiseZMin;
		float mNoiseZMax;

		Ogre::Vector3 mDestPos;	

		float mWidthStart;
		float mWidthEnd;
        	float mWidth;

	protected:

		int mNumBillboardElements;

		Ogre::EffectBillboardChain *mBillboardChain;

	//	Ogre::Vector3 mDestPos;		

		Ogre::ColourValue mColourRangeStart;
		Ogre::ColourValue mColourRangeEnd;

		String mMaterialName;

		float mFrequency;
		float mFrequencyTime;
		float mCurrentFrequencyTime;
		////////////////////////////////////////////////////////////////
		static CmdNoiseXMin msNoiseXMinCmd;
		static CmdNoiseXMax msNoiseXMaxCmd;
		static CmdNoiseYMin msNoiseYMinCmd;
		static CmdNoiseYMax msNoiseYMaxCmd;
		static CmdNoiseZMin msNoiseZMinCmd;
		static CmdNoiseZMax msNoiseZMaxCmd;
		static CmdNumElements msNumElementsCmd;
	//	static CmdWidthStart  msWidthStartCmd;
	//	static CmdWidthEnd    msWidthEndCmd;
		static CmdWidth       msWidthCmd;

		static CmdColour msColourCmd;
		static CmdColourRangeStart msColourRangeStartCmd;
		static CmdColourRangeEnd msColourRangeEndCmd;

		static CmdMaterial msMaterialCmd;

		static CmdFrequency msFrequencyCmd;

	//	static CmdDestPos	msDestPosCmd;
	};
}

#endif // __BeamElement_H__