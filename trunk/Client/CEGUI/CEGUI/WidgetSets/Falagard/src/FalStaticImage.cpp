/************************************************************************
    filename:   FalStaticImage.cpp
    created:    Tue Jul 5 2005
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
#include "FalStaticImage.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardStaticImage::WidgetTypeName[] = "Falagard/StaticImage";

    FalagardStaticImage::FalagardStaticImage(const String& type, const String& name) :
        StaticImage(type, name)
    {
    }

    FalagardStaticImage::~FalagardStaticImage()
    {
    }

    void FalagardStaticImage::populateRenderCache()
    {
		d_renderCache.clearCachedImagery();
		StaticImage::populateRenderCache();

        const StateImagery* imagery;
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

		bool is_enabled = !isDisabled();

        // render frame section
        if (d_frameEnabled)
        {
            imagery = &wlf.getStateImagery(is_enabled ? "EnabledFrame" : "DisabledFrame");
            // peform the rendering operation.
            imagery->render(*this);
        }

        // render background section
        if (d_backgroundEnabled)
        {
            imagery = &wlf.getStateImagery(is_enabled ? "EnabledBackground" : "DisabledBackground");
            // peform the rendering operation.
            imagery->render(*this);
        }

        // render basic imagery
        imagery = &wlf.getStateImagery(is_enabled ? "Enabled" : "Disabled");
        // peform the rendering operation.
        imagery->render(*this);

        // call base class
    }

    Rect FalagardStaticImage::getUnclippedInnerRect(void) const
    {
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

        String area_name(isFrameEnabled() ? "WithFrame" : "NoFrame");
        area_name += "ImageRenderArea";

        if (wlf.isNamedAreaDefined(area_name))
            return wlf.getNamedArea(area_name).getArea().getPixelRect(*this).offset(getUnclippedPixelRect().getPosition());
        else
            return StaticImage::getUnclippedInnerRect();
    }

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardStaticImageFactory::createWindow(const String& name)
    {
        return new FalagardStaticImage(d_type, name);
    }

    void FalagardStaticImageFactory::destroyWindow(Window* window)
    {
        delete window;
    }

} // End of  CEGUI namespace section
