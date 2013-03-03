#include "falagardanimateimage.h"
#include "ceguianimate.h"
#include "ceguianimatemanager.h"
#include "windows.h"
#include "ceguiimage.h"
#include "ceguipropertyhelper.h"
namespace CEGUI
{
	

	namespace FalagardAnimateImageProperties
	{
		String Animate::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString( static_cast<const FalagardAnimate*>(receiver)->IsFlash() );
		}

		void Animate::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardAnimateImage*>(receiver)->SetAnimate( value );
		}
		String AnimateID::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString( static_cast<const FalagardAnimate*>(receiver)->IsFlash() );
		}

		void AnimateID::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardAnimateImage*>(receiver)->SetAnimateThroughId(  PropertyHelper::stringToInt( value ) );
		}

		String UseImageSize::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString( static_cast<const FalagardAnimate*>(receiver)->IsFlash() );
		}

		void UseImageSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardAnimateImage*>(receiver)->SetUseImageSize(  PropertyHelper::stringToBool( value ) );
		}
	};
	const utf8 FalagardAnimateImage::WidgetTypeName[] = "Falagard/AnimateImage";
	FalagardAnimateImageProperties::Animate			FalagardAnimateImage::d_animateProperty;
	FalagardAnimateImageProperties::AnimateID		FalagardAnimateImage::d_animateIDProperty;
	FalagardAnimateImageProperties::UseImageSize	FalagardAnimateImage::d_useImageSizeProperty;


	FalagardAnimateImage::FalagardAnimateImage(const String& type, const String& name) :
        StaticImage(type, name)
	{

		CEGUI_START_ADD_STATICPROPERTY( FalagardAnimateImage )
			CEGUI_ADD_STATICPROPERTY( &d_animateProperty );
			CEGUI_ADD_STATICPROPERTY( &d_animateIDProperty );
			CEGUI_ADD_STATICPROPERTY( &d_useImageSizeProperty );
		CEGUI_END_ADD_STATICPROPERTY

		m_pAnimate = 0;
		m_bPlay = true;
		m_bUseImageSize = true;
		m_nAnimateStart = int( System::getSingleton().getCurTimeElapsed() * 1000.0f );
	}

	FalagardAnimateImage::~FalagardAnimateImage(void)
	{
	}
	void FalagardAnimateImage::SetAnimate( const String& strAnimate )
	{
		if( AnimateManager::getSingletonPtr() == NULL )
			return;
		if(AnimateManager::getSingleton().isAnimatePresent(strAnimate))
		{
			m_pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( strAnimate );
		}
	}
	void FalagardAnimateImage::SetAnimateThroughId(int id)
	{
		if( AnimateManager::getSingletonPtr() == NULL )
			return;
		if(AnimateManager::getSingleton().isAnimatePresent(id))
		{
			m_pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( id );
			//m_nAnimateStart = int( System::getSingleton().getCurTimeElapsed() * 1000.0f );
		}
	}
	void	FalagardAnimateImage::drawSelf(float z)
	{
		StaticImage::drawSelf(z);
	}
	void	FalagardAnimateImage::Play( bool bPlay )
	{
		m_bPlay = bPlay;
		m_nAnimateStart = int( System::getSingleton().getCurTimeElapsed() * 1000.0f );
	}
	void	FalagardAnimateImage::updateSelf(float elapsed)
	{
		// 得到相应得播放了得时间
		if( m_pAnimate && isVisible() )
		{
			if( m_bPlay )
			{
				int time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
				const Image* pFrame = m_pAnimate->getFrame( time_now - m_nAnimateStart, -1 );
				if( pFrame )
				{
					if( m_bUseImageSize )
						setSize( Absolute, pFrame->getSourceTextureArea().getSize() );
					setAlpha( m_pAnimate->getFrameAlpha( time_now - m_nAnimateStart, -1 ) );
					setImage( pFrame ) ;
					if( !m_pAnimate->isLoop() && ( m_pAnimate->getTotalTime() < ( time_now - m_nAnimateStart ) ) )
					{
						m_bPlay = false;
					}
				}
				else
				{
					setImage( NULL ) ;
				}
			} 
			else
			{
				setImage( NULL ) ;
			}
			
			requestRedraw();
		}
		
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardAnimateImageFactory::createWindow(const String& name)
    {
        return new FalagardAnimateImage(d_type, name);
    }

    void FalagardAnimateImageFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }
}