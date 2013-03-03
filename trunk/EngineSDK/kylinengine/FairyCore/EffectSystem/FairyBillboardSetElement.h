/********************************************************************
filename:   FairyBillboardSetElement.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    a billboard set effect element,this can use in weapon effect
*********************************************************************/

#ifndef __BillboardSetElement_H__
#define __BillboardSetElement_H__

// fairy header
#include "FairyEffectElement.h"
#include "Core/FairyPrerequisites.h"

// ogre header
#include <OgreColourValue.h>

namespace Ogre	{
	class BillboardSet;
}

namespace Fairy	{	

	class BillboardSetElement : public EffectElement
	{
	public:

        typedef Ogre::Vector3 ScaleValue;

        enum { MAX_STAGES = 3 };

		// ��������
		class CmdBillboardColour : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

        class CmdBillboardAlpha : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		class CmdBillboardCount : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdBillboardMaterial : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdBillboardInterval : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdBillboardWidth : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdBillboardHeight : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

        class CmdBillboardScale : public Ogre::ParamCommand
        {
        public:
            size_t		mIndex;

        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdScaleIntervalTime : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdStacks : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

        class CmdSlices : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdTexIntervalTime : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		/////////////////////////////////////////////////////////////
		BillboardSetElement(const Ogre::String &type, System *system);
		~BillboardSetElement();

		virtual bool createRenderInstance(void);
		virtual void destroyRenderInstance(void);
        virtual void updateRenderInstance( Ogre::Real time, TransformInfo &info );

		//////////////////////////////////////////////////////////////////////////
		void setBillboardColour(const Ogre::ColourValue &colour);
		const Ogre::ColourValue& getBillboardColour(void) const
		{
			return mBillboardColour;
		}

        void setBillboardScale(size_t index, const ScaleValue& scale);
        const ScaleValue& getBillboardScale(size_t index) const
        {
            return mBillboardScale[index];
        }

        void setScaleIntervalTime(Ogre::Real time)
        {
            mScaleIntervalTime = time;
        }
        Ogre::Real getScaleIntervalTime(void) const
        {
            return mScaleIntervalTime;
        }

		void setBillboardCount(int count);
		int getBillboardCount(void) const
		{
			return mBillboardCount;
		}

		void setBillboardMaterial(const Ogre::String &material);
		String getBillboardMaterial(void) const
		{
			return mBillboardMaterial;
		}

		void setBillboardInterval(Ogre::Real interval);
		Ogre::Real getBillboardInterval(void) const
		{
			return mBillboardInterval;
		}

		void setBillboardWidth(Ogre::Real width);
		Ogre::Real getBillboardWidth(void) const
		{
			return mBillboardWidth;
		}

		void setBillboardHeight(Ogre::Real height);
		Ogre::Real getBillboardHeight(void) const
		{
			return mBillboardHeight;
		}

        void setBillboardAlpha(Ogre::Real alpha)
        {
            mAlphaValue = alpha;
            mColourChanged = true;
        }
        Ogre::Real getBillboardAlpha(void) const
        {
            return mAlphaValue;
        }

        /** sets of the stacks of texture coordinates */
        void setStacks(Ogre::ulong stacks);
       Ogre::ulong getStacks(void) const
        {
            return mStacks;
        }

        /** sets of the slices of texture coordinates */
        void setSlices(Ogre::ulong slices);
       Ogre::ulong getSlices(void) const
        {
            return mSlices;
        }

        void setTexIntervalTime(Ogre::Real time)
        {
            mTexIntervalTime = time;
        }
        Ogre::Real getTexIntervalTime(void) const
        {
            return mTexIntervalTime;
        }

        /// ����visible flag
        virtual void setVisibleFlag(uint32 flags);

	protected:

		bool _createBillboardSet(void);
		void _destroyBillboardSet(void);

        /** �����������е�billboard����ʼtexture index
        @remarks    ���stacks��slices�����˱仯����ô�͵������������ǵ���ʼtexture index
        */
        void _resetBillboardStartTexIndex(void);

        void _resetBillboardStartScaleTime(void);
		 
	protected:

        /// ����ЧԪ�����õ�billboard set
		Ogre::BillboardSet *mBillboardSet;

        /// ����billboard����ɫ
        Ogre::ColourValue mBillboardColour;
        /// billboard�ĸ���
		int mBillboardCount;
        /// billboard���õĲ�������
		String mBillboardMaterial;
        /// ����billboard���ĵ�֮��ļ��
		Ogre::Real mBillboardInterval;
        /// billboard�Ŀ��
		Ogre::Real mBillboardWidth;
        /// billboard�ĸ߶�
		Ogre::Real mBillboardHeight;

        ScaleValue mBillboardScale[MAX_STAGES];
        /// ��С�任�ļ��ʱ��
        Ogre::Real mScaleIntervalTime;
        /// ����billboard������ͬ�����ţ���������ṹ���ڱ���ÿ��billboard����Ϣ
        struct _CurrentScaleInfo
        {
            Ogre::Real mCurrentScaleIntervalTime;
            int mCurrentIndex;
        };
        std::vector<_CurrentScaleInfo> mCurrentScaleInfos;

        /// ��ʾ��ɫ�Ƿ���Ҫ�������õı�־
        bool mSizeChanged;

        /** billboard���õ�alphaֵ
        @remarks ����ʵ��������mBillboardColour�ϵ�һ���������������billboard�ϵ�
                 ��ɫֵ��mBillboardColour * mAlphaValue
        */
        Ogre::Real mAlphaValue;

        /// ��ʾ��ɫ�Ƿ���Ҫ�������õı�־
        bool mColourChanged;

        /// the stacks of texture coordinates
       Ogre::ulong mStacks;
        /// the slices of texture coordinates
       Ogre::ulong mSlices;

        /// ÿ��billboard���θı�texture index֮��ļ��ʱ��
        Ogre::Real mTexIntervalTime;

        Ogre::Real mCurrentTexIntervalTime;

		////////////////////////////////////////////////////////////////
		static CmdBillboardColour msBillboardColourCmd;
        static CmdBillboardAlpha msBillboardAlphaCmd;
		static CmdBillboardCount msBillboardCountCmd;
		static CmdBillboardMaterial msBillboardMaterialCmd;
		static CmdBillboardInterval msBillboardIntervalCmd;
		static CmdBillboardHeight msBillboardHeightCmd;
		static CmdBillboardWidth msBillboardWidthCmd;

        static CmdBillboardScale msBillboardScaleCmd[MAX_STAGES];
        static CmdScaleIntervalTime msScaleIntervalTimeCmd;

        static CmdStacks msStacksCmd;
        static CmdSlices msSlicesCmd;
        static CmdTexIntervalTime msTexIntervalTimeCmd;
	};
}

#endif // __BillboardSetElement_H__