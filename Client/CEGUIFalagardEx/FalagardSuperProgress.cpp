#include ".\falagardsuperprogress.h"
#include "CEGUIPropertyHelper.h"
namespace CEGUI
{

	const utf8  FalagardSuperProgress::WidgetTypeName[] = "Falagard/SuperProgress";

	FalagardSuperProgressProperties::ProgressImage			FalagardSuperProgress::d_effectImageProperty;
	FalagardSuperProgressProperties::EffectColor			FalagardSuperProgress::d_effectAlphaProperty;
	FalagardSuperProgressProperties::EffectColorLevel		FalagardSuperProgress::d_EffectColorLevelProperty;
	
	FalagardSuperProgressProperties::MoveSpeed				FalagardSuperProgress::d_MoveSpeedProperty;
	FalagardSuperProgressProperties::HorzImageFormating		FalagardSuperProgress::d_HorzImageFormatingProperty;
	FalagardSuperProgressProperties::VertImageFormating		FalagardSuperProgress::d_VertImageFormatingProperty;

	FalagardSuperProgressProperties::ProgressDirection		FalagardSuperProgress::d_ProgressDirectionProperty;
	FalagardSuperProgressProperties::ProgressRegular		FalagardSuperProgress::d_ProgressRegularProperty;
		
	FalagardSuperProgress::FalagardSuperProgress( const String& type, const String& name ) : 
		ProgressBar( type, name )
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardSuperProgress )
		   CEGUI_ADD_STATICPROPERTY( &d_effectImageProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_MoveSpeedProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_effectAlphaProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_EffectColorLevelProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_HorzImageFormatingProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_VertImageFormatingProperty  );

		   CEGUI_ADD_STATICPROPERTY( &d_ProgressDirectionProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_ProgressRegularProperty  );

		CEGUI_END_ADD_STATICPROPERTY

		d_BackImage.setHorzFormatting(RenderableImage::HorzStretched);
		d_BackImage.setVertFormatting(RenderableImage::VertStretched);

		d_MoveSpeed		= 1; // 每毫秒走0.01%;
		d_curPos		= 1;
		d_progress		= 1;

		// 设置移动方向和规则图片属性值			20100607 BLL
		d_nDirection    = 1;
		d_bProgressRegular = true;

		colour c(0xff000000);
		d_EffectColorLevel[0] = ColourRect(c,c,c,c);	
	
		d_EColorLevelIndex = 0;

	}

	FalagardSuperProgress::~FalagardSuperProgress(void)
	{

	}

	ColourRect& FalagardSuperProgress::getCurColor()
	{
		int nColor = int( d_curPos * 10 );
		if( nColor < 0 ) nColor = 0;
		if( nColor > 10 ) nColor = 10;
		return d_EffectColorLevel[ nColor ];
	}
	void	FalagardSuperProgress::populateRenderCache()
	{
	//	FalagardStaticImage::populateRenderCache();

		if( d_curPos == d_progress )
		{
			// 根据进度条移动方向，设置进度覆盖区域		20100607BLL
			if ( d_nDirection == 1 )
			{
				d_rectProgress = Rect( 0, 0, getAbsoluteWidth() * d_progress, getAbsoluteHeight() );
				
				// 在进度比例很小，进度显示区域不足一个像素宽度时，最小显示一格像素宽度
				float fDiff = d_rectProgress.d_right- d_rectProgress.d_left;
				if ( 0.0f < fDiff && fDiff < 1.0f  )
				{
					d_rectProgress.d_right = 1;
				}
			
			}
			else if ( d_nDirection == 2 )
			{			
				d_rectProgress = Rect( 0, getAbsoluteHeight() * ( 1 - d_progress ), getAbsoluteWidth(), getAbsoluteHeight() );

				// 在进度比例很小，进度显示区域不足一个像素高度时，最小显示一格像素高度
				float fDiff = d_rectProgress.d_bottom - d_rectProgress.d_top;
				if ( 0.0f < fDiff && fDiff < 1.0f )
				{
					d_rectProgress.d_top = d_rectProgress.d_bottom - 1;
				}
			}
		}

		// 如果是规则区域，则设置进度覆盖区域为完整图片绘制区域，否则设置进度覆盖区域为裁剪绘制区域		20100607BLL
		if ( d_bProgressRegular == true )
		{
			d_BackImage.setColours( getCurColor() );
			d_BackImage.setRect( d_rectProgress );
			d_BackImage.draw( d_renderCache);
			if( d_curPos != d_progress )
			{
				d_BackImage.setColours( d_EffectColor );
				d_BackImage.setRect( d_rectEffect );
				d_BackImage.draw( d_renderCache );
			}
		}
		else
		{
			Rect rectImageRect( 0, 0, getAbsoluteWidth(), getAbsoluteHeight() );

			d_BackImage.setColours( getCurColor() );
			d_BackImage.setRect( rectImageRect );
			d_BackImage.draw( d_renderCache, d_rectProgress );
			if( d_curPos != d_progress )
			{
				d_BackImage.setColours( d_EffectColor );
				d_BackImage.setRect( rectImageRect );
				d_BackImage.draw( d_renderCache, d_rectProgress );
			}

		}//if ( d_bProgressRegular == true )
	}

	void   FalagardSuperProgress::setEffectImage( const Image* pImage )
	{
		d_BackImage.setImage( pImage );
	}

	void    FalagardSuperProgress::setEffectColor( ColourRect& color )  
	{
		d_EffectColor = color;
	}
	void   FalagardSuperProgress::setEffectColorLevel( int nIndex, ColourRect&	color )
	{
		for( int i = nIndex; i < 11; i ++ ) // 把以后所有得颜色填成这个颜色
			d_EffectColorLevel[ i ] = color;

		d_EColorLevelIndex = nIndex;
	}

	void	FalagardSuperProgress::updateSelf(float elapsed)
	{

		if( d_curPos != d_progress )  // 说明需要更新了
		{
			
			float fMoveLength = elapsed * d_MoveSpeed;
			if( d_curPos > d_progress ) // 如果比当前得值,就说名是再减
			{
				d_curPos -= fMoveLength;
				if( d_curPos < d_progress )
					d_curPos = d_progress;
			}
			else
			{
				d_curPos += fMoveLength;
				if( d_curPos > d_progress )
					d_curPos = d_progress;
			}

			// 进度方向从左到右  20100607 BLL
			if ( d_nDirection == 1 )
			{
				if( d_curPos < d_progress )
				{	
					d_rectEffect = Rect( getAbsoluteWidth() * d_curPos, 0, getAbsoluteWidth() * d_progress, getAbsoluteHeight() );
					d_rectProgress = Rect( 0, 0, getAbsoluteWidth() * d_progress, getAbsoluteHeight() );
				}
				else
				{
					d_rectEffect = Rect( getAbsoluteWidth() * d_progress, 0, getAbsoluteWidth() * d_curPos, getAbsoluteHeight() );
					d_rectProgress = Rect( 0, 0, getAbsoluteWidth() * d_curPos, getAbsoluteHeight() );
				}

				// 在进度比例很小，进度显示区域不足一个像素宽度时，最小显示一格像素宽度
				float fDiff = d_rectProgress.d_right- d_rectProgress.d_left;
				if ( 0.0f < fDiff && fDiff < 1.0f  )
				{
					d_rectProgress.d_right = 1;
				}

			}
			else if ( d_nDirection == 2 ) // 进度方向从下到上
			{

				if( d_curPos < d_progress )
				{	
					d_rectEffect = Rect( 0, getAbsoluteHeight() * ( 1 - d_progress ), getAbsoluteWidth(), getAbsoluteHeight() * ( 1 - d_curPos ) );
					d_rectProgress = Rect( 0, getAbsoluteHeight() * ( 1 - d_curPos ), getAbsoluteWidth(), getAbsoluteHeight() );
				}
				else
				{
					d_rectEffect = Rect( 0, getAbsoluteHeight() * ( 1 - d_curPos ),  getAbsoluteWidth(), getAbsoluteHeight() * ( 1 - d_progress ) );
					d_rectProgress = Rect( 0, getAbsoluteHeight() * ( 1 - d_progress ), getAbsoluteWidth(), getAbsoluteHeight() );
				}

				// 在进度比例很小，进度显示区域不足一个像素高度时，最小显示一格像素高度
				float fDiff = d_rectProgress.d_bottom- d_rectProgress.d_top;
				if ( 0.0f < fDiff && fDiff < 1.0f  )
				{
					d_rectProgress.d_top = d_rectProgress.d_bottom - 1;
				}
			}
			
			if( d_curPos < 0 )
				d_curPos = 0;
			if( d_curPos > 1 )
				d_curPos = 1;
			requestRedraw();
			d_needsRedraw = true;
		}
	
	}

	void	FalagardSuperProgress::setHorzImageFormatting( RenderableImage::HorzFormatting format) 
	{
		d_BackImage.setHorzFormatting(format);
	}
	RenderableImage::HorzFormatting	FalagardSuperProgress::getHorzImageFormatting( void )const 
	{
		return d_BackImage.getHorzFormatting();
	}
	void	FalagardSuperProgress::setVertImageFormatting( RenderableImage::VertFormatting format) 
	{
		d_BackImage.setVertFormatting(format);
	}
	RenderableImage::VertFormatting	FalagardSuperProgress::getVertImageFormatting( void )const 
	{
		return d_BackImage.getVertFormatting();
	}
	
	namespace FalagardSuperProgressProperties
	{

		//////////////////////////////////////////////////////////////////////////////////////
		String ProgressImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardSuperProgress*>(receiver)->getEffectImage());
		}

		void ProgressImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperProgress*>(receiver)->setEffectImage(PropertyHelper::stringToImage(value));
		}

		//////////////////////////////////////////////////////////////////////////////////////
		String EffectColor::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::colourRectToString(static_cast<const FalagardSuperProgress*>(receiver)->getEffectColor());
		}

		void EffectColor::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperProgress*>(receiver)->setEffectColor( PropertyHelper::stringToColourRect(value) );
		}
		//////////////////////////////////////////////////////////////////////////////////////
		String EffectColorLevel::get(const PropertyReceiver* receiver) const
		{
		
			String str( "index:" );
			int	   nIndex = static_cast<const FalagardSuperProgress*>(receiver)->getEffectColorLevelIndex();

			str += PropertyHelper::intToString( nIndex );
			str += " ";
			str += PropertyHelper::colourRectToString( static_cast<const FalagardSuperProgress*>(receiver)->getEffectColorLevel());
			
			return str;
		}

		void EffectColorLevel::set(PropertyReceiver* receiver, const String& value)
		{
			int nIndex = 0;
			argb_t topLeft = 0xFF000000, topRight = 0xFF000000, bottomLeft = 0xFF000000, bottomRight = 0xFF000000;
			sscanf(value.c_str(), "index:%d tl:%8X tr:%8X bl:%8X br:%8X", &nIndex, &topLeft, &topRight, &bottomLeft, &bottomRight );
			static_cast<FalagardSuperProgress*>(receiver)->setEffectColorLevel( nIndex, ColourRect( topLeft, topRight, bottomLeft, bottomRight ) );
		}
		String MoveSpeed::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::floatToString(static_cast<const FalagardSuperProgress*>(receiver)->getMoveSpeed());
		}

		void MoveSpeed::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperProgress*>(receiver)->setMoveSpeed( PropertyHelper::stringToFloat(value));
		}

		// 背景图片的拉伸模式
		String   HorzImageFormating::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::ImageHorzFormatingToString(static_cast<const FalagardSuperProgress*>(receiver)->getHorzImageFormatting());
		}
		void   HorzImageFormating::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperProgress*>(receiver)->setHorzImageFormatting(PropertyHelper::stringToHorzImageFormating(value));
		}

		// 背景图片的拉伸模式
		String   VertImageFormating::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::ImageVertFormatingToString(static_cast<const FalagardSuperProgress*>(receiver)->getVertImageFormatting());
		}
		void   VertImageFormating::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperProgress*>(receiver)->setVertImageFormatting( PropertyHelper::stringToVertImageFormating(value));
		}

		// 进度条的移动方向		20100607 BLL
		String ProgressDirection::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardSuperProgress*>(receiver)->getProgressDirection());
		}

		void ProgressDirection::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperProgress*>(receiver)->setProgressDirection( PropertyHelper::stringToInt(value));
		}

		// 进度条是否规则图片	20100607 BLL
		String ProgressRegular::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardSuperProgress*>(receiver)->getProgressRegular());
		}

		void ProgressRegular::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperProgress*>(receiver)->setProgressRegular( PropertyHelper::stringToBool(value));
		}
	};


	//////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardSuperProgressFactory::createWindow(const String& name)
    {
        return new FalagardSuperProgress(d_type, name);
    }

    void FalagardSuperProgressFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}