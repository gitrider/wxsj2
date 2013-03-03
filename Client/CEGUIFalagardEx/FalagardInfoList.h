#pragma once
#include "fallistbox.h"
#include "falimagelistproperties.h"
#include "elements/ceguilistboxtextitem.h"
#include "IFalagardInfoList.h"

namespace CEGUI
{
	/*----------------------------------------------------------------------*/
	namespace FalagardInfoListProperties
	{
		//*----------------------------------------------------------------------------------
		// 生存周期
		class Life: public Property
		{
		public:
			Life() : Property(
				"Life",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// 减隐速度
		class FadeSpeed: public Property
		{
		public:
			FadeSpeed() : Property(
				"FadeSpeed",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		//*----------------------------------------------------------------------------------
		// 最大显示多少条
		class MaxInfo: public Property
		{
		public:
			MaxInfo() : Property(
				"MaxInfo",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

	};
	/*----------------------------------------------------------------------*/
	class FALAGARDBASE_API FalagardInfoItem : public ListboxTextItem
	{
		float   m_fCurAlpha;
		int		m_nTimer;		// 计时器
		int		m_nFadeSpeed;
	public:
		virtual void draw(RenderCache& cache,const Rect& targetRect, float zBase,  float alpha, const Rect* clipper) const;

		FalagardInfoItem(const String32& text, uint item_id = 0, void* item_data = NULL, bool disabled = false, bool auto_delete = true);
		~FalagardInfoItem() {};

		void setTimer( int nLife ){ m_nTimer = nLife; };
		void setFadeSpeed( int nSpeed ) { m_nFadeSpeed = nSpeed; };

		bool update( float elapsed );
	};
	/*----------------------------------------------------------------------*/
	class FALAGARDBASE_API FalagardInfoList :public FalagardListbox, public IFalagardInfoList
	{
		int  m_nLife; // 生存周期
		int  m_nFadeSpeed;
		int	 m_nMaxInfoNumber;
	public:
		FalagardInfoList(const String& type, const String& name);
		~FalagardInfoList(void);

		static const utf8   WidgetTypeName[];       

		virtual void updateSelf( float elapsed );

		void	setLife( int nLife ) { m_nLife = nLife; };
		void	setFadeSpeed( int nSpeed ) { m_nFadeSpeed = nSpeed; };

		const int  	getLife() const { return m_nLife; };
		const int	getFadeSpeed() const { return m_nFadeSpeed; };

		void	setMaxInfo( int nNumber ) { m_nMaxInfoNumber = nNumber; };

		void	NormalizeItemNumber();
		virtual void AddInfo(const String32& strInfo);
		virtual void RemoveAllInfo(void);
		static FalagardInfoListProperties::FadeSpeed	d_FadeSpeedProperty;
		static FalagardInfoListProperties::Life			d_LifeProperty;
		virtual bool	testClassName_impl(const String& class_name) const
		{
			if (class_name==(const utf8*)"FalagardInfoList")	return true;
			return Window::testClassName_impl(class_name);
		}
	};

	/*----------------------------------------------------------------------*/
	class FALAGARDBASE_API FalagardInfoListFactory : public WindowFactory
	{
	public:
		FalagardInfoListFactory(void) : WindowFactory(FalagardInfoList::WidgetTypeName) { }
		~FalagardInfoListFactory(void){}
		Window* createWindow(const String& name);
		void destroyWindow(Window* window);
	};
	/*----------------------------------------------------------------------*/
}