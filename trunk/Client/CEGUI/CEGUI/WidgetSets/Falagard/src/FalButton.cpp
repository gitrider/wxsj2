/************************************************************************
filename:   FalButton.cpp
created:    Wed Jun 22 2005
author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
Crazy Eddie's GUI System (http://www.cegui.org.uk)
Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "FalButton.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIAnimateManager.h"
#include "CEGUIAnimate.h"
// Start of CEGUI namespace section
namespace CEGUI
{
	const utf8 FalagardButton::WidgetTypeName[] = "Falagard/Button";
	FalagardButtonProperties::Flash FalagardButton::d_flashProperty;
	FalagardButtonProperties::BtnAnimation FalagardButton::d_btnAnimationProperty;

	FalagardButton::FalagardButton(const String& type, const String& name) :
	PushButton(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardButton )
			CEGUI_ADD_STATICPROPERTY( &d_flashProperty  );
		CEGUI_ADD_STATICPROPERTY( &d_btnAnimationProperty );
		CEGUI_END_ADD_STATICPROPERTY
			d_ReturnTimer = 0.0f;
		d_bflsh =false;
		setFlash(0);
		d_btnAnimate.d_animate = 0;
		d_enableAnimate = false;
	}

	FalagardButton::~FalagardButton()
	{
	}
	void FalagardButton::updateSelf(float elapsed)
	{
		const float TIME_SPACE = 0.6f; 
		if(!this->isVisible())
			return;
		if(d_btnAnimate.d_animate && d_enableAnimate)
		{
			updateAnimate();
			return;
		}
		if(this->getFlash())
		{
			if(d_bflsh)
				d_ReturnTimer+=elapsed;
			else
				d_ReturnTimer-=elapsed;			
			if(d_ReturnTimer<=0)
			{
				d_bflsh = true;
				d_ReturnTimer =0;
			}
			if(d_ReturnTimer>=TIME_SPACE)
			{
				d_bflsh = false;
				d_ReturnTimer = TIME_SPACE;
			}

			this->setAlpha(d_ReturnTimer/TIME_SPACE);
		}
		else
		{
			this->setAlpha(1.0);
		}
	}

	void FalagardButton::enableAnimate(bool yes)
	{
		if(!d_btnAnimate.d_animate)
		{
			d_enableAnimate = false;
			return;
		}	
		if(yes != d_enableAnimate)
		{
			d_enableAnimate = yes;
			if(yes)
			{
				int time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
				d_btnAnimate.d_time_start = time_now;
				const Image* pFrame = d_btnAnimate.d_animate->getFrame( time_now-d_btnAnimate.d_time_start, d_btnAnimate.d_time_total );
				d_btnAnimate.d_currentFrame = pFrame;
				requestRedraw();
			}
			else
			{
				d_btnAnimate.d_currentFrame = 0;
			}
		}
	}

	void FalagardButton::updateAnimate(void)
	{
		if(d_btnAnimate.d_animate && d_enableAnimate)
		{
			int time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);

			const Image* pFrame = d_btnAnimate.d_animate->getFrame( time_now-d_btnAnimate.d_time_start, d_btnAnimate.d_time_total );
			d_btnAnimate.d_currentFrame = pFrame;
			d_btnAnimate.d_alpha = d_btnAnimate.d_animate->getFrameAlpha( time_now-d_btnAnimate.d_time_start, d_btnAnimate.d_time_total );
			requestRedraw();
		}
	}

	void FalagardButton::drawSelf(float z)
	{
		// this is hackish and relies on insider knowlegde of the way that both PushButton (actually ButtonBase)
		// and Window implement things; Soon I'll get to updating things so that this can be replaced
		// with clean code.

		// do we need to update the cache?
		if (d_needsRedraw)
		{
			// remove old cached imagery
			d_renderCache.clearCachedImagery();
			// signal that we'll no loger need a redraw.
			d_needsRedraw = false;
			// call PushButton drawSelf method which will call one of the state drawing methods overridden in this class.
			PushButton::drawSelf(z);
		}
		if(d_btnAnimate.d_animate && d_enableAnimate && d_btnAnimate.d_currentFrame)
		{
			drawAnimate(z);
		}
		// call Window drawSelf to get it to send the cached imagery to the renderer.
		Window::drawSelf(z);
	}

	void FalagardButton::drawAnimate(float z)
	{
		if(d_btnAnimate.d_animate && d_enableAnimate && d_btnAnimate.d_currentFrame)
		{
			//Draw CornerChar
			ColourRect color(colour( 1.0f, 1.0f, 1.0f, 1.0f));

			color.setAlpha( d_btnAnimate.d_alpha );
			d_renderCache.cacheImage(*(d_btnAnimate.d_currentFrame),
				Rect(0, 0, getAbsoluteSize().d_width, getAbsoluteSize().d_height), 0, color);
		}
	}

	void FalagardButton::drawNormal(float z)
	{
		doButtonRender("Normal");

		// Support for legacy custom image setting
		if (d_useNormalImage)
		{
			ColourRect colours(d_normalImage.getColours());
			colours.setAlpha(getEffectiveAlpha());
			d_normalImage.setColours(colours);
			d_normalImage.draw(d_renderCache);
		}
	}

	void FalagardButton::drawHover(float z)
	{
		doButtonRender("Hover");

		// Support for legacy custom image setting
		if (d_useHoverImage)
		{
			ColourRect colours(d_hoverImage.getColours());
			colours.setAlpha(getEffectiveAlpha());
			d_hoverImage.setColours(colours);
			d_hoverImage.draw(d_renderCache);
		}
	}

	void FalagardButton::drawPushed(float z)
	{
		doButtonRender("Pushed");

		// Support for legacy custom image setting
		if (d_usePushedImage)
		{
			ColourRect colours(d_pushedImage.getColours());
			colours.setAlpha(getEffectiveAlpha());
			d_pushedImage.setColours(colours);
			d_pushedImage.draw(d_renderCache);
		}
	}

	void FalagardButton::drawDisabled(float z)
	{
		doButtonRender("Disabled");

		// Support for legacy custom image setting
		if (d_useDisabledImage)
		{
			ColourRect colours(d_disabledImage.getColours());
			colours.setAlpha(getEffectiveAlpha());
			d_disabledImage.setColours(colours);
			d_disabledImage.draw(d_renderCache);
		}
	}

	void FalagardButton::doButtonRender(const String& state)
	{
		// this is the second part of the hackish code.  We're duplicating the first section of code
		// from the Window::drawSelf method to decide whether to actually do anything.  It is likely
		// this will be replaced with cleaner code in the near future...

		// this conditional is just here to respect old legacy settings
		if (d_useStandardImagery)
		{
			const StateImagery* imagery;

			try
			{
				// get WidgetLookFeel for the assigned look.
				const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
				// try and get imagery for the state we were given, though default to Normal state if the
				// desired state does not exist
				imagery = wlf.isStateImageryPresent(state) ? &wlf.getStateImagery(state) : &wlf.getStateImagery("Normal");
			}
			// catch exceptions, but do not exit.
			catch (UnknownObjectException)
			{
				// don't try and draw using missing imagery!
				return;
			}

			// peform the rendering operation.
			// NB: This is not in the above try block since we want UnknownObjectException exceptions to be emitted from
			// the rendering code for conditions such as missing Imagesets and/or Images.
			imagery->render(*this);
		}
	}

	void FalagardButton::setBtnAnimation(const String& val)
	{
		if( AnimateManager::getSingletonPtr() == NULL )
			return;
		int time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
		int time_start = (int)(time_now );

		if(AnimateManager::getSingleton().isAnimatePresent(val))
		{
			d_btnAnimate.d_animate = ( Animate*)AnimateManager::getSingleton().getAnimate(val);
			d_btnAnimate.d_time_start = 0;
			d_btnAnimate.d_time_total = d_btnAnimate.d_animate->getTotalTime();
			d_btnAnimate.d_currentFrame = d_btnAnimate.d_animate->getFrame( 0 );
		}

	}

	Size FalagardButton::getRenderSize()
	{
		const FontBase* font = getFont();
		if( font == 0 )
			return Window::getRenderSize();
		Rect absarea( 0, 0, 1024,768 );
		Size size = font->getFormattedSize( d_text, absarea, (TextFormatting)LeftAligned );
		size.d_width += 30;
		size.d_height += 10;
		return size;
	}
	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Property 

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	namespace FalagardButtonProperties
	{
		String Flash::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardButton*>(receiver)->getFlash());
		}

		void Flash::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardButton*>(receiver)->setFlash(PropertyHelper::stringToInt(value));
		}

		String BtnAnimation::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void BtnAnimation::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardButton*>(receiver)->setBtnAnimation(value);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	Window* FalagardButtonFactory::createWindow(const String& name)
	{
		return new FalagardButton(d_type, name);
	}

	void FalagardButtonFactory::destroyWindow(Window* window)
	{
		delete window;
	}

} // End of  CEGUI namespace section
