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

		// 参数命令
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

        /// 设置visible flag
        virtual void setVisibleFlag(uint32 flags);

	protected:

		bool _createBillboardSet(void);
		void _destroyBillboardSet(void);

        /** 重新设置所有的billboard的起始texture index
        @remarks    如果stacks和slices发生了变化，那么就得重新设置他们的起始texture index
        */
        void _resetBillboardStartTexIndex(void);

        void _resetBillboardStartScaleTime(void);
		 
	protected:

        /// 该特效元素所用的billboard set
		Ogre::BillboardSet *mBillboardSet;

        /// 所有billboard的颜色
        Ogre::ColourValue mBillboardColour;
        /// billboard的个数
		int mBillboardCount;
        /// billboard所用的材质名称
		String mBillboardMaterial;
        /// 两个billboard中心点之间的间隔
		Ogre::Real mBillboardInterval;
        /// billboard的宽度
		Ogre::Real mBillboardWidth;
        /// billboard的高度
		Ogre::Real mBillboardHeight;

        ScaleValue mBillboardScale[MAX_STAGES];
        /// 大小变换的间隔时间
        Ogre::Real mScaleIntervalTime;
        /// 由于billboard并不是同步缩放，所以这个结构用于保存每个billboard的信息
        struct _CurrentScaleInfo
        {
            Ogre::Real mCurrentScaleIntervalTime;
            int mCurrentIndex;
        };
        std::vector<_CurrentScaleInfo> mCurrentScaleInfos;

        /// 表示颜色是否需要进行设置的标志
        bool mSizeChanged;

        /** billboard所用的alpha值
        @remarks 这其实是作用在mBillboardColour上的一个参数，最后真正billboard上的
                 颜色值是mBillboardColour * mAlphaValue
        */
        Ogre::Real mAlphaValue;

        /// 表示颜色是否需要进行设置的标志
        bool mColourChanged;

        /// the stacks of texture coordinates
       Ogre::ulong mStacks;
        /// the slices of texture coordinates
       Ogre::ulong mSlices;

        /// 每个billboard两次改变texture index之间的间隔时间
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