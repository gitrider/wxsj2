/************************************************************************
    filename:   FalTabControl.cpp
    created:    Fri Jul 8 2005
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
#include "FalTabControl.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUITabButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardTabControl::WidgetTypeName[] = "Falagard/TabControl";
    FalagardTabControlProperties::TabButtonType FalagardTabControl::d_tabButtonTypeProperty;


    FalagardTabControl::FalagardTabControl(const String& type, const String& name) :
        TabControl(type, name)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardTabControl )
		   CEGUI_ADD_STATICPROPERTY( &d_tabButtonTypeProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

    FalagardTabControl::~FalagardTabControl()
    {
    }

    void FalagardTabControl::populateRenderCache()
    {
        const StateImagery* imagery;
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // render basic imagery
        imagery = &wlf.getStateImagery(isDisabled() ? "Disabled" : "Enabled");
        imagery->render(*this);
    }

    TabPane* FalagardTabControl::createTabContentPane(const String& name) const
    {
        // return wiget created by look'n'feel assignment.
        return static_cast<TabPane*>(WindowManager::getSingleton().getWindow(name));
    }

    Window* FalagardTabControl::createTabButtonPane(const String& name) const
    {
        try
        {
            // return wiget created by look'n'feel assignment.
            return static_cast<TabPane*>(WindowManager::getSingleton().getWindow(name));
        }
        // use default if none provided by LookNFeel
        catch (UnknownObjectException)
        {
            return TabControl::createTabButtonPane(name);
        }
    }

    TabButton* FalagardTabControl::createTabButton(const String& name) const
    {
        if (d_tabButtonType.empty())
        {
            throw InvalidRequestException("FalagardTabControl::createTabButton - d_tabButtonType has not been set!");
        }

        return static_cast<TabButton*>(WindowManager::getSingleton().createWindow(d_tabButtonType, name));
    }

    const String& FalagardTabControl::getTabButtonType() const
    {
        return d_tabButtonType;
    }

    void FalagardTabControl::setTabButtonType(const String& type)
    {
        d_tabButtonType = type;
    }

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardTabControlFactory::createWindow(const String& name)
    {
        return new FalagardTabControl(d_type, name);
    }

    void FalagardTabControlFactory::destroyWindow(Window* window)
    {
        delete window;
    }

} // End of  CEGUI namespace section
