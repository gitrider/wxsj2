/************************************************************************
	filename: 	CEGUIMenuItem.cpp
	created:	2/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
	
	purpose:	Implementation of MenuItem widget base class
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
#include "elements/CEGUIMenuItem.h"
#include "elements/CEGUIMenubar.h"
#include "elements/CEGUIPopupMenu.h"

#include "CEGUILogger.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Definition of Properties for this class
*************************************************************************/
MenuItemProperties::HoverColour			MenuItem::d_hoverColourProperty;
MenuItemProperties::PushedColour		MenuItem::d_pushedColourProperty;
MenuItemProperties::OpenedColour		MenuItem::d_openedColourProperty;
MenuItemProperties::NormalTextColour	MenuItem::d_normalTextColourProperty;
MenuItemProperties::DisabledTextColour	MenuItem::d_disabledTextColourProperty;


/*************************************************************************
	Constants
*************************************************************************/
// default colours for rendering
const colour	MenuItem::DefaultHoverColour		= 0xFFFFFFFF;
const colour	MenuItem::DefaultPushedColour		= 0xFFFFFFFF;
const colour	MenuItem::DefaultOpenedColour		= 0xFFEFEFEF;
const colour	MenuItem::DefaultNormalTextColour	= 0xFFFFFFFF;
const colour	MenuItem::DefaultDisabledTextColour	= 0xFF7F7F7F;

// event strings
const String MenuItem::EventNamespace("MenuItem");
const String MenuItem::EventClicked( (utf8*)"Clicked" );


/*************************************************************************
	Constructor for MenuItem base class.
*************************************************************************/
MenuItem::MenuItem(const String& type, const String& name)
	: TextItem(type, name),
	d_pushed(false),
	d_hovering(false),
	d_opened(false),
	d_hoverColour(DefaultHoverColour),
	d_pushedColour(DefaultPushedColour),
	d_openedColour(DefaultOpenedColour),
	d_normalTextColour(DefaultNormalTextColour),
	d_disabledTextColour(DefaultDisabledTextColour),
	d_popup(NULL)
{
	// menuitems dont want multi-click events
	setWantsMultiClickEvents(false);
	
	addMenuItemProperties();
}


/*************************************************************************
	Destructor for MenuItem base class.
*************************************************************************/
MenuItem::~MenuItem(void)
{
}


/*************************************************************************
	Update the internal state of the Widget
*************************************************************************/
void MenuItem::updateInternalState(const Point& mouse_pos)
{
	bool oldstate = d_hovering;

	// assume not hovering 
	d_hovering = false;

	// if input is captured, but not by 'this', then we never hover highlight
	const Window* capture_wnd = getCaptureWindow();

	if ((capture_wnd == NULL) || (capture_wnd == this))
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet != NULL)
		{
			// check if hovering highlight is required, which is basically ("mouse over widget" XOR "widget pushed").
			if ((this == sheet->getChildAtPosition(mouse_pos)) != d_pushed)
			{
				d_hovering = true;

				// are we attached to a menu ?
				if (getParent()->testClassName("MenuBase"))
				{
					MenuBase* menu = (MenuBase*)getParent();

					// is item really in list ?
					if (menu->isItemInList(this))
					{
						// does this menubar only allow one popup open? and is there a popup open?
						if ( !menu->isMultiplePopupsAllowed() && menu->getPopupMenuItem()!=NULL )
						{
							// open this popup instead
							openPopupMenu();
						}

					}

				}

			}

		}

	}

	// if state has changed, trigger a re-draw
	if (oldstate != d_hovering)
	{
		requestRedraw();
	}

}


/*************************************************************************
	Set the popup menu for this item.
*************************************************************************/
void MenuItem::setPopupMenu(PopupMenu* popup)
{
	// already attached?
	if (isChild(popup))
	{
		d_popup = popup;
		return;
	}

	// attach as child
	addChildWindow(popup);
	d_popup = popup;
}

/*************************************************************************
	Open the PopupMenu attached to this item.
*************************************************************************/
void MenuItem::openPopupMenu()
{
	// no popup? or already open...
	if ( d_popup == NULL || d_opened )
		return;

	// if we are attached to a menubar, we let it handle the "activation"
	MenuBase* menu = (Menubar*)getParent();
	if (getParent()->testClassName("Menubar") && menu->isItemInList(this))
	{
		// align the popup to the bottom-left of the menuitem
		Point pos(0,getAbsoluteRect().getHeight());
		d_popup->setPosition(Absolute,pos);

		menu->changePopupMenuItem(this);
	}
	// or maybe a popup menu?
	else if (getParent()->testClassName("PopupMenu") && menu->isItemInList(this))
	{
		// align the popup to the top-right of the menuitem
		const Rect absrect = getAbsoluteRect();
		Point pos(absrect.getWidth(),0);
		d_popup->setPosition(Absolute,pos);

		menu->changePopupMenuItem(this);
	}
	// otherwise we do ourselves
	else
	{
		// match up with Menubar::changePopupMenu
		d_popup->openPopupMenu();
	}

	d_opened = true;
	requestRedraw();
}


/*************************************************************************
	Close the PopupMenu attached to this item.
*************************************************************************/
void MenuItem::closePopupMenu(bool notify)
{
	// no popup? or not open...
	if ( d_popup == NULL || !d_opened )
		return;

	// should we notify the parent menubar? otherwise...
	// if we are attached to a menubar, we let it handle the "deactivation"
	// only if the menubar does not allow multiple popups
	MenuBase* menu = (MenuBase*)getParent();
	if (notify && getParent()->testClassName("MenuBase") && menu->isItemInList(this) && !menu->isMultiplePopupsAllowed())
	{
		menu->changePopupMenuItem(NULL);
	}
	// otherwise we do ourselves
	else
	{
		// match up with Menubar::changePopupMenu
		//d_popup->hide();
		d_popup->closePopupMenu();
	}

	d_opened = false;
	requestRedraw();
}


/*************************************************************************
	Toggles the PopupMenu.
*************************************************************************/
bool MenuItem::togglePopupMenu(void)
{
	if (d_opened)
	{
		closePopupMenu();
		return false;
	}

	openPopupMenu();
	return true;
}

/*************************************************************************
	Set the colour to use for the label text when rendering in the
	hover / highlighted states.	
*************************************************************************/
void MenuItem::setHoverColour(const colour& col)
{
	if (d_hoverColour != col)
	{
		d_hoverColour = col;
		requestRedraw();
	}

}


/*************************************************************************
	Set the colour to use for the label text when rendering in the
	pushed state.
*************************************************************************/
void MenuItem::setPushedColour(const colour& col)
{
	if (d_pushedColour != col)
	{
		d_pushedColour = col;
		requestRedraw();
	}

}


/*************************************************************************
	Set the colour to use for the label text when rendering in the
	opened state.
*************************************************************************/
void MenuItem::setOpenedColour(const colour& col)
{
	if (d_openedColour != col)
	{
		d_openedColour = col;
		requestRedraw();
	}

}


/*************************************************************************
	Set the colour to use for the label text when rendering in the
	normal state.	
*************************************************************************/
void MenuItem::setNormalTextColour(const colour& col)
{
	if (d_normalTextColour != col)
	{
		d_normalTextColour = col;
		requestRedraw();
	}

}


/*************************************************************************
	Set the colour to use for the label text when rendering in the
	disabled state.	
*************************************************************************/
void MenuItem::setDisabledTextColour(const colour& col)
{
	if (d_disabledTextColour != col)
	{
		d_disabledTextColour = col;
		requestRedraw();
	}

}


/*************************************************************************
	Recursive function that closes all popups down the hierarcy starting
	with this one.
*************************************************************************/
void MenuItem::closeAllMenuItemPopups()
{
	// are we attached to a PopupMenu?
	PopupMenu* pop = (PopupMenu*)getParent();
	if (getParent()->testClassName("PopupMenu") && pop->isItemInList(this))
	{
		// is this parent popup attached to a menu item?
		MenuItem* item = (MenuItem*)pop->getParent();
		if (item!=NULL && pop->getParent()->testClassName("MenuItem"))
		{
			// close popup
			item->closePopupMenu();
			// recurse
			item->closeAllMenuItemPopups();
		}
		// otherwise we just hide the parent popup
		else
		{
			pop->closePopupMenu();
		}
	}
}


/*************************************************************************
	handler invoked internally when the menuitem is clicked.	
*************************************************************************/
void MenuItem::onClicked(WindowEventArgs& e)
{
	// close the popup if we did'nt spawn a child
	if (!d_opened && !d_popupWasClosed)
	{
		closeAllMenuItemPopups();
	}
	d_popupWasClosed=false;
	fireEvent(EventClicked, e, EventNamespace);
}


/*************************************************************************
	Handler for when the mouse moves
*************************************************************************/
void MenuItem::onMouseMove(MouseEventArgs& e)
{
	// this is needed to discover whether mouse is in the widget area or not.
	// The same thing used to be done each frame in the rendering method,
	// but in this version the rendering method may not be called every frame
	// so we must discover the internal widget state here - which is actually
	// more efficient anyway.

	// base class processing
	Window::onMouseMove(e);

	updateInternalState(e.position);
	e.handled = true;
}


/*************************************************************************
	Handler for mouse button pressed events
*************************************************************************/
void MenuItem::onMouseButtonDown(MouseEventArgs& e)
{
	// default processing
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		d_popupWasClosed = false;
		if (captureInput())
		{
			d_pushed = true;
			updateInternalState(e.position);
			d_popupWasClosed = !togglePopupMenu();
			requestRedraw();
		}

		// event was handled by us.
		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void MenuItem::onMouseButtonUp(MouseEventArgs& e)
{
	// default processing
	Window::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		releaseInput();
		
		// was the button released over this window?
		if ( !d_popupWasClosed && System::getSingleton().getGUISheet()->getChildAtPosition(e.position) == this )
		{
			WindowEventArgs we(this);
			onClicked(we);
		}

		// event was handled by us.
		e.handled = true;
	}

}

/*************************************************************************
	Handler for when mouse capture is lost
*************************************************************************/
void MenuItem::onCaptureLost(WindowEventArgs& e)
{
	// Default processing
	Window::onCaptureLost(e);

	d_pushed = false;
	updateInternalState(MouseCursor::getSingleton().getPosition());
	requestRedraw();

	// event was handled by us.
	e.handled = true;
}


/*************************************************************************
	Handler for when mouse leaves the widget
*************************************************************************/
void MenuItem::onMouseLeaves(MouseEventArgs& e)
{
	// deafult processing
	Window::onMouseLeaves(e);

	d_hovering = false;
	requestRedraw();

	e.handled = true;
}


/*************************************************************************
	Internal version of adding a child window.
*************************************************************************/
void MenuItem::addChild_impl(Window* wnd)
{
	TextItem::addChild_impl(wnd);

	// if this is a PopupMenu we add it like one. only if there is not already a popup attached.
	if (wnd->testClassName("PopupMenu") && d_popup==NULL)
	{
		setPopupMenu((PopupMenu*)wnd);
	}

}


/*************************************************************************
	Add MenuItem specific properties
*************************************************************************/
void MenuItem::addMenuItemProperties(void)
{
	CEGUI_START_ADD_STATICPROPERTY( MenuItem );
	   CEGUI_ADD_STATICPROPERTY( &d_hoverColourProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_pushedColourProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_openedColourProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_normalTextColourProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_disabledTextColourProperty );
	CEGUI_END_ADD_STATICPROPERTY
}

} // End of  CEGUI namespace section
