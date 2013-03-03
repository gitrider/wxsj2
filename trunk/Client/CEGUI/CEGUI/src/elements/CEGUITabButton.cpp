/************************************************************************
	filename: 	CEGUITabButton.cpp
	created:	8/8/2004
	author:		Steve Streeting
	
	purpose:	Implementation of TabButton widget base class
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
#include "elements/CEGUITabButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String TabButton::EventNamespace("TabButton");

/*************************************************************************
	Event name constants
*************************************************************************/
const String TabButton::EventClicked( (utf8*)"Clicked" );


/*************************************************************************
	Constructor
*************************************************************************/
TabButton::TabButton(const String& type, const String& name) :
	ButtonBase(type, name),
    d_selected(false), 
    d_rightOfSelected(true)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
TabButton::~TabButton(void)
{
}

/*************************************************************************
	handler invoked internally when the button is clicked.	
*************************************************************************/
void TabButton::onClicked(WindowEventArgs& e)
{
	fireEvent(EventClicked, e, EventNamespace);
}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void TabButton::onMouseButtonUp(MouseEventArgs& e)
{
	if ((e.button == LeftButton) && isPushed())
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet != NULL)
		{
			// if mouse was released over this widget
			if (this == sheet->getChildAtPosition(e.position))
			{
				// fire event
				WindowEventArgs args(this);
				onClicked(args);
			}

		}

		e.handled = true;
	}

	// default handling
	ButtonBase::onMouseButtonUp(e);
}
/*************************************************************************
Draw method
*************************************************************************/
void TabButton::drawSelf(float z)
{
	if (isDisabled())
	{
		drawDisabled(z);
	}
	else if (isHovering())
	{
		drawHover(z);
	}
    else if (d_selected)
    {
        drawPushed(z);
    }
    else
    {
        drawNormal(z);
    }
}
/*************************************************************************
Set target window
*************************************************************************/
void TabButton::setTargetWindow(Window* wnd)
{
    d_targetWindow = wnd;
    // Copy initial text
    setText(wnd->getText());
    // Parent control will keep text up to date, since changes affect layout
}

} // End of  CEGUI namespace section
