/************************************************************************
filename:   FalMultiLineEditbox.cpp
created:    Thu Jul 7 2005
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
#include "FalMultiLineEditbox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUIIme.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	const utf8 FalagardMultiLineEditbox::WidgetTypeName[] = "Falagard/MultiLineEditbox";
	FalagardMultiLineEditboxProperties::SelectionBrushImage FalagardMultiLineEditbox::d_selectionBrushProperty;


	FalagardMultiLineEditbox::FalagardMultiLineEditbox(const String& type, const String& name) :
	MultiLineEditbox(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardMultiLineEditbox )
			CEGUI_ADD_STATICPROPERTY( &d_selectionBrushProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	FalagardMultiLineEditbox::~FalagardMultiLineEditbox()
	{
	}

	Rect FalagardMultiLineEditbox::getTextRenderArea(void) const
	{
		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
		bool v_visible = false;
		if( d_vertScrollbar )
			v_visible = d_vertScrollbar->isVisible(true);
		bool h_visible = false;
		if( d_horzScrollbar )
			h_visible = d_horzScrollbar->isVisible(true);
		// if either of the scrollbars are visible, we might want to use another text rendering area
		if (v_visible || h_visible)
		{
			String area_name("TextArea");

			if (h_visible)
			{
				area_name += "H";
			}
			if (v_visible)
			{
				area_name += "V";
			}
			area_name += "Scroll";

			if (wlf.isNamedAreaDefined(area_name))
			{
				return wlf.getNamedArea(area_name).getArea().getPixelRect(*this);
			}
		}

		// default to plain TextArea
		return wlf.getNamedArea("TextArea").getArea().getPixelRect(*this);
	}

	Scrollbar* FalagardMultiLineEditbox::createVertScrollbar(const String& name) const
	{
		// return component created by look'n'feel assignment.
		return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(name));
	}

	Scrollbar* FalagardMultiLineEditbox::createHorzScrollbar(const String& name) const
	{
		// return component created by look'n'feel assignment.
		return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(name));
	}

	void FalagardMultiLineEditbox::cacheEditboxBaseImagery()
	{
		const StateImagery* imagery;

		// get WidgetLookFeel for the assigned look.
		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
		// try and get imagery for our current state
		imagery = &wlf.getStateImagery(isDisabled() ? "Disabled" : (isReadOnly() ? "ReadOnly" : "Enabled"));
		// peform the rendering operation.
		imagery->render(*this);
	}

	void FalagardMultiLineEditbox::cacheCaratImagery(const Rect& textArea)
	{
		const FontBase* fnt = getFont();

		// require a font so that we can calculate carat position.
		if ( fnt  )
		{
			// get line that carat is in
			size_t caratLine = getLineNumberFromIndex(d_caratPos);

			// if carat line is valid.
			if (caratLine < d_lines.size())
			{
				// calculate pixel offsets to where carat should be drawn
				size_t caratLineIdx = d_caratPos - d_lines[caratLine].d_startIdx;
				float ypos = caratLine * fnt->getLineSpacing();
				float xpos = fnt->getTextExtent(d_text.substr(d_lines[caratLine].d_startIdx, caratLineIdx));

				// get base offset to target layer for cursor.
				Renderer* renderer = System::getSingleton().getRenderer();
				float baseZ = renderer->getZLayer(7) - renderer->getCurrentZ();

				// get WidgetLookFeel for the assigned look.
				const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
				// get carat imagery
				const ImagerySection& caratImagery = wlf.getImagerySection("Carat");

				// calculate finat destination area for carat
				Rect caratArea;
				caratArea.d_left    = textArea.d_left + xpos;
				caratArea.d_top     = textArea.d_top + ypos;
				caratArea.setWidth(caratImagery.getBoundingRect(*this).getSize().d_width);
				caratArea.setHeight(fnt->getLineSpacing());
				float fHPosition = 0;
				if( d_horzScrollbar )
					fHPosition = d_horzScrollbar->getScrollPosition();
				float fVPosition = 0;
				if( d_vertScrollbar )
					fVPosition = d_vertScrollbar->getScrollPosition();
				caratArea.offset( Point(-fHPosition, -fVPosition ));

				// adjust ime composition window.
				d_caratRect = caratArea;
				Ime::SetImeWindowPos();
				// cache the carat image for rendering.
				caratImagery.render(*this, caratArea, baseZ, 0, &textArea);
			}
		}
	}

	Rect FalagardMultiLineEditbox::getCaratTextExtent() const
	{
		Rect rc = MultiLineEditbox::getCaratTextExtent();
		rc.d_left += d_caratRect.d_left;
		rc.d_bottom = rc.d_top + d_caratRect.d_bottom;
		rc.d_top += d_caratRect.d_top;

		Rect pixelRc = getPixelRect();
		if(pixelRc.getWidth() < 0.01f && pixelRc.getHeight() < 0.01f) return pixelRc;

		return rc;
	}

	const Image* FalagardMultiLineEditbox::getSelectionBrushImage() const
	{
		return d_selectionBrush;
	}

	void FalagardMultiLineEditbox::setSelectionBrushImage(const Image* image)
	{
		d_selectionBrush = image;
		requestRedraw();
	}

	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	Window* FalagardMultiLineEditboxFactory::createWindow(const String& name)
	{
		return new FalagardMultiLineEditbox(d_type, name);
	}

	void FalagardMultiLineEditboxFactory::destroyWindow(Window* window)
	{
		delete window;
	}

} // End of  CEGUI namespace section
