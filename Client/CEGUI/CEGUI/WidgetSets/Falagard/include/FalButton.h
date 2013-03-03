/************************************************************************
filename:   FalButton.h
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
#ifndef _FalButton_h_
#define _FalButton_h_

#include "FalModule.h"
#include "elements/CEGUIPushButton.h"
#include "CEGUIWindowFactory.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	class Animate;
	/*!
	\brief
	Button class for the FalagardBase module.

	This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

	States (missing states will default to 'Normal'):
	- Normal    - Rendering for when the button is neither pushed or has the mouse hovering over it.
	- Hover     - Rendering for then the button has the mouse hovering over it.
	- Pushed    - Rendering for when the button is pushed.
	- Disabled  - Rendering for when the button is disabled.
	*/
	namespace FalagardButtonProperties
	{
		class Flash : public Property
		{
		public:
			Flash() : Property(
				"Flash",
				"Property to get/set empty status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class BtnAnimation : public Property
		{
		public:
			BtnAnimation() : Property(
				"BtnAnimation",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	};

	class FALAGARDBASE_API FalagardButton : public PushButton
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
		static FalagardButtonProperties::Flash		d_flashProperty;
		static FalagardButtonProperties::BtnAnimation		d_btnAnimationProperty;
		/*!
		\brief
		Constructor
		*/
		FalagardButton(const String& type, const String& name);

		/*!
		\brief
		Destructor
		*/
		~FalagardButton();

		// overridden from ButtonBase.
		void drawSelf(float z);

		// implementation of abstract methods in ButtonBase
		void drawNormal(float z);
		void drawHover(float z);
		void drawPushed(float z);
		void drawDisabled(float z);
		void			setFlash( int nMode ){d_nFlash=nMode;};
		const int		getFlash() const{return d_nFlash;};
		virtual Size getRenderSize();
		virtual	void updateSelf(float elapsed);
	protected:
		void doButtonRender(const String& state);


	protected:
		int					d_nFlash;	 //0:ÉÁ£¬1²»ÉÁ
		float				d_ReturnTimer;
		bool				d_bflsh;
	protected:
		struct ANIMATE
		{
			Animate*		d_animate;
			int				d_time_start;
			int				d_time_total;
			const Image*	d_currentFrame;
			float			d_alpha;
			ANIMATE(){
				d_animate = NULL;
				d_time_start = 0;
				d_time_total = 0;
				d_currentFrame = 0;
				d_alpha			= 0;

			};
		};

		ANIMATE			d_btnAnimate;
		bool			d_enableAnimate;

		void updateAnimate(void);
		void drawAnimate(float z);
	public:
		void enableAnimate(bool yes);

		void setBtnAnimation(const String& val);
	};

	/*!
	\brief
	WindowFactory for FalagardButton type Window objects.
	*/
	class FALAGARDBASE_API FalagardButtonFactory : public WindowFactory
	{
	public:
		FalagardButtonFactory(void) : WindowFactory(FalagardButton::WidgetTypeName) { }
		~FalagardButtonFactory(void){}
		Window* createWindow(const String& name);
		void destroyWindow(Window* window);
	};

} // End of  CEGUI namespace section


#endif  // end of guard _FalButton_h_
