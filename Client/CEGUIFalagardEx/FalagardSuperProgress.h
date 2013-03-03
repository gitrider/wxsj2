#pragma once
#include "falprogressbar.h"
#include "ceguirenderableimage.h"
namespace CEGUI
{

	namespace FalagardSuperProgressProperties
	{
		// 进度条得图片
		class ProgressImage: public Property
		{
		public:
			ProgressImage() : Property(
				"ProgressImage",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 进度条特效得颜色
		class EffectColor: public Property
		{
		public:
			EffectColor() : Property(
				"EffectColor",
				"Property to get/set empty status.",
				"tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 设置一级效果,用于产生特效
		class EffectColorLevel: public Property
		{
		public:
			EffectColorLevel() : Property(
				"EffectColorLevel",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 设置变化速度, 毫秒
		class MoveSpeed: public Property
		{
		public:
			MoveSpeed() : Property(
				"MoveSpeed",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		// 背景图片的拉伸模式
		class HorzImageFormating: public Property
		{
		public:
			HorzImageFormating() : Property(
				"HorzImageFormating",
				"Property to get/set HorzImageFormating status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		// 背景图片的拉伸模式
		class VertImageFormating: public Property
		{
		public:
			VertImageFormating() : Property(
				"VertImageFormating",
				"Property to get/set HorzImageFormating status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		// 进度条的方向	20100607 BLL
		class ProgressDirection: public Property
		{
		public:
			ProgressDirection() : Property(
				"ProgressDirection",
				"Property to get/set ProgressDirection status.",
				"1")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void     set(PropertyReceiver* receiver, const String& value);
		};


		// 进度条是否规则图片	20100607 BLL
		class ProgressRegular: public Property
		{
		public:
			ProgressRegular() : Property(
				"ProgressRegular",
				"Property to get/set ProgressRegular status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void     set(PropertyReceiver* receiver, const String& value);
		};


	};


	class FalagardSuperProgress : public ProgressBar
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
	public:
		FalagardSuperProgress( const String& type, const String& name );
		~FalagardSuperProgress(void);


		static FalagardSuperProgressProperties::ProgressImage			d_effectImageProperty;
		static FalagardSuperProgressProperties::EffectColor				d_effectAlphaProperty;
		static FalagardSuperProgressProperties::EffectColorLevel		d_EffectColorLevelProperty;

		static FalagardSuperProgressProperties::MoveSpeed				d_MoveSpeedProperty;

		static FalagardSuperProgressProperties::HorzImageFormating				d_HorzImageFormatingProperty;
		static FalagardSuperProgressProperties::VertImageFormating				d_VertImageFormatingProperty;

		static FalagardSuperProgressProperties::ProgressDirection				d_ProgressDirectionProperty;
		static FalagardSuperProgressProperties::ProgressRegular					d_ProgressRegularProperty;


		void	setEffectImage( const Image* pImage );		// 设置进度条得底板图片
		const Image*	getEffectImage(void) const{ return d_BackImage.getImage(); }
		void    setEffectColor( ColourRect& color );
		ColourRect	getEffectColor(void) const { return d_EffectColor;};

		void	setEffectColorLevel( int nIndex, ColourRect& color);
		ColourRect  getEffectColorLevel()const 	{ return d_EffectColorLevel[d_EColorLevelIndex]; } 

		int		getEffectColorLevelIndex()const { return d_EColorLevelIndex; } 


		void	setMoveSpeed( float fMoveSpeed ) { d_MoveSpeed = fMoveSpeed; };
		float	getMoveSpeed( void )const { return d_MoveSpeed; }
		void	setHorzImageFormatting( RenderableImage::HorzFormatting format) ;
		RenderableImage::HorzFormatting	getHorzImageFormatting( void )const ;
		void	setVertImageFormatting( RenderableImage::VertFormatting format) ;
		RenderableImage::VertFormatting	getVertImageFormatting( void )const ;

		// 进度条的移动方向
		void	setProgressDirection( int nDirection ) { d_nDirection = nDirection; };
		int		getProgressDirection( void )const { return d_nDirection; }

		// 进度条是否是规则图片
		void	setProgressRegular( bool bProgressRegular ) { d_bProgressRegular = bProgressRegular; };
		bool	getProgressRegular( void )const { return d_bProgressRegular; }
	protected:
		virtual void	populateRenderCache();
		virtual void	updateSelf(float elapsed);
	private:
		ColourRect			d_EffectColorLevel[ 11 ];// 进度条3级得颜色
		ColourRect			d_EffectColor;	// 特效颜色
		
		RenderableImage		d_BackImage;  // 底板得图片

		float				d_MoveSpeed;
		float				d_curPos;


		Rect				d_rectProgress;
		Rect				d_rectEffect;

		int					d_nDirection;		// 进度条的移动方向，1 从左到右，2 从下到上				20100607 BLL
		bool				d_bProgressRegular; // 进度条是否是规则图片(矩形) true 规则，false 不规则

		int					d_EColorLevelIndex;	// setEffectColorLevel设置的index

		ColourRect&			getCurColor();

	};

	class FALAGARDBASE_API FalagardSuperProgressFactory : public WindowFactory
    {
    public:
        FalagardSuperProgressFactory(void) : WindowFactory( FalagardSuperProgress::WidgetTypeName ) { }
        ~FalagardSuperProgressFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};