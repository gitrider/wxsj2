
#include "FalIMEEditBox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIIme.h"
#include "windows.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	//===================================================================================
	//
	// FalagardIMEEditBox
	//
	//===================================================================================
	const utf8  FalagardIMEEditBox::WidgetTypeName[] = "Falagard/IMEEditBox";

	const utf32 FalagardIMEEditBox::infoBegin	= 0xE4000000;
	const utf32 FalagardIMEEditBox::infoEnd		= 0xE5000000;

	FalagardIMEEditBoxProperties::DefaultEditBox	FalagardIMEEditBox::d_defaultEditBoxProperty;
	FalagardIMEEditBoxProperties::ClearOffset		FalagardIMEEditBox::d_clearOffsetProperty;
	FalagardIMEEditBoxProperties::ElementNum		FalagardIMEEditBox::d_elementNumProperty;
	FalagardIMEEditBoxProperties::DisableCopyPaste	FalagardIMEEditBox::d_disableCopyPasteProperty;
	FalagardIMEEditBoxProperties::SoftKeyOnly		FalagardIMEEditBox::d_softKeyOnlyProperty;

	// Event Namespace
	const String FalagardIMEEditBox::EventNamespace("Falagard/FalagardIMEEditBox");

	// Event Strings
	const String FalagardIMEEditBox::EventItemElementDelete("ItemElementDelete");
	const String FalagardIMEEditBox::EventItemelementJoinFailure("ItemelementJoinFailure");
	const String FalagardIMEEditBox::EventItemElementFull("ItemElementFull");

	FalagardIMEEditBox::FalagardIMEEditBox(const String& type, const String& name) :
	FalagardEditbox(type, name), 
		d_lastTextOffset(0),
		d_caretOn(true),
		d_lastTextLen(0),
		d_clearOffset(false),
		d_addDirect(false),
		d_maxItemElement(3),
		d_bCopyPasteDisable(false)
	{
		addIMEEditBoxProperties();

		d_blinkTime = Ime::GetCaretBlinkTime()*0.001f;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();
		d_softKeyOnly = false;
	}

	FalagardIMEEditBox::~FalagardIMEEditBox()
	{
	}

	void FalagardIMEEditBox::addIMEEditBoxProperties(void)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardIMEEditBox )
			CEGUI_ADD_STATICPROPERTY( &d_defaultEditBoxProperty );
		CEGUI_ADD_STATICPROPERTY( &d_clearOffsetProperty );
		CEGUI_ADD_STATICPROPERTY( &d_elementNumProperty );
		CEGUI_ADD_STATICPROPERTY( &d_disableCopyPasteProperty );
		CEGUI_ADD_STATICPROPERTY( &d_softKeyOnlyProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	bool FalagardIMEEditBox::hasInputFocus(void) const
	{
		return Ime::GetActiveIMEEditBox() == this;
	}

	void FalagardIMEEditBox::resetCaretBlink(void)
	{
		d_caretOn = true;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();
		requestRedraw();
	}

	bool FalagardIMEEditBox::injectItemInfo(int nID, const String32& strDisplayName, int nLength)
	{
		//char dbgmsg[128] = {0};
		//1. create a new element.
		ItemElement el;
		el.nID = nID;
		for(size_t idx = 0; idx<strDisplayName.length(); ++idx)
		{
			if(is_FireFox_Utf32_Code(strDisplayName[idx])) 
			{
				el.strColor += strDisplayName[idx];
			}
			else
			{
				break;
			}
		}
		String32 strNoColor;
		remove_FireFox_Utf32_Code(strDisplayName, strNoColor);

		el.nBeginIndex = (int)getSelectionStartIndex();
		el.nEndIndex = el.nBeginIndex + (int)strNoColor.length() - 1;
		el.nLength = nLength;

		//1.5 length check before add this element.
		String32 strTemp;
		if(!checkLength(strTemp, &el))
		{
			// Trigger itemelement add failure event
			ItemElementEventArgs args(this);
			args.itemID = el.nID;
			fireEvent(EventItemelementJoinFailure, args, EventNamespace);

			return false;
		}
		d_addDirect = true;

		//2. insert display name into label txt.
		for(int i = 0; i < (int)strNoColor.size(); ++i)
		{
			utf32 codepoint = strNoColor.at(i);
			addCharater(codepoint);
		}

		d_addDirect = false;
		//4. ask cegui system to redraw.
		requestRedraw();

		//5. add new element to list.
		ItemElementList::iterator it = d_itemList.begin();
		for(; it != d_itemList.end(); ++it)
		{
			if((*it).nBeginIndex > el.nEndIndex) break;
		}
		d_itemList.insert(it, el);

		/*
		it = d_itemList.begin();
		_snprintf(dbgmsg, 127, "ItemElementList Size:%u\n", d_itemList.size());
		::OutputDebugString(dbgmsg);
		for(; it != d_itemList.end(); ++it)
		{
		_snprintf(dbgmsg,127,"ItemElement ID:%d nBeginIndex:%d nEndIndex:%d\n", (*it).nID,(*it).nBeginIndex,(*it).nEndIndex);
		::OutputDebugString(dbgmsg);
		}
		*/
		return true;
	}

	void FalagardIMEEditBox::setClearOffset(const bool set)
	{
		if(set != d_clearOffset)
		{
			d_clearOffset = set;
			requestRedraw();
		}
	}

	bool FalagardIMEEditBox::getClearOffset(void) const
	{
		return d_clearOffset;
	}

	Rect FalagardIMEEditBox::getCaratTextExtent() const
	{
		Rect rc = Editbox::getCaratTextExtent();
		rc.d_left += d_lastTextOffset;

		Rect pixelRc = getPixelRect();
		if(pixelRc.getWidth() < 0.01f && pixelRc.getHeight() < 0.01f) return pixelRc;

		return rc;
	}

	void FalagardIMEEditBox::addCharater(const utf32& codepoint, bool isFromSoftKey )
	{
		if( !isVisible() )
			return;
		if( d_softKeyOnly && !isFromSoftKey )
			return;
		//*-*-*-* BigCharacter
		String32 str;
		str = codepoint;
		((FontBase*)getFont())->prepareString( str );
		//*-*-*-* BigCharacter

		// only need to take notice if we have focus
		if ((getFont()->isCodepointAvailable(codepoint) || is_FireFox_Utf32_Code(codepoint)) && !isReadOnly())
		{
			// backup current text
			String32 tmp(d_text);
			tmp.erase(getSelectionStartIndex(), getSelectionLength());

			// if there is room
			//if (tmp.mbcs_length() + str.mbcs_length() <= d_maxTextLen)
			if(d_addDirect || checkLength(str))
			{
				tmp.insert(getSelectionStartIndex(), 1, codepoint);

				if (isStringValid(tmp))
				{
					delSelectionItemElements(d_itemList);
					delCaratIndexItemElement(d_itemList);

					//0. adjust element list's old elemnts's begin and end.
					ItemElementList::iterator it = d_itemList.begin();
					for(;it!=d_itemList.end();++it)
					{
						if((int)d_caratPos <= (*it).nBeginIndex)
						{
							(*it).nBeginIndex -= (int)getSelectionLength();
							(*it).nEndIndex -= (int)getSelectionLength();
						}
					}
					// erase selection using mode that does not modify d_text (we just want to update state)
					eraseSelectedText(false);

					//0. adjust element list's old elemnts's begin and end.
					it = d_itemList.begin();
					for(;it!=d_itemList.end();++it)
					{
						if((int)d_caratPos <= (*it).nBeginIndex)
						{
							(*it).nBeginIndex += 1;
							(*it).nEndIndex += 1;
						}
					}
					// advance carat (done first so we can "do stuff" in event handlers!)
					d_caratPos++;

					// set text to the newly modified string
					setText(tmp);

					/*
					char dbgmsg[128] = {0};
					it = d_itemList.begin();
					_snprintf(dbgmsg, 127, "ItemElementList Size:%u\n", d_itemList.size());
					::OutputDebugString(dbgmsg);
					for(; it != d_itemList.end(); ++it)
					{
					_snprintf(dbgmsg,127,"ItemElement ID:%d nBeginIndex:%d nEndIndex:%d\n", (*it).nID,(*it).nBeginIndex,(*it).nEndIndex);
					::OutputDebugString(dbgmsg);
					}
					*/
				}
				else
				{
					// Trigger invalid modification attempted event.
					WindowEventArgs args(this);
					onInvalidEntryAttempted(args);
				}

			}
			else
			{
				// Trigger text box full event
				WindowEventArgs args(this);
				onEditboxFullEvent(args);
			}

		}
	}

	/*************************************************************************
	Processing for backspace key	
	*************************************************************************/
	void FalagardIMEEditBox::handleBackspace(void)
	{
		if (!isReadOnly())
		{
			String32 tmp(d_text);

			if (getSelectionLength() != 0)
			{
				tmp.erase(getSelectionStartIndex(), getSelectionLength());

				if (isStringValid(tmp))
				{
					delSelectionItemElements(d_itemList);

					//0. adjust element list's old elemnts's begin and end.
					ItemElementList::iterator it = d_itemList.begin();
					for(;it!=d_itemList.end();++it)
					{
						if((int)d_caratPos <= (*it).nBeginIndex)
						{
							(*it).nBeginIndex -= (int)getSelectionLength();
							(*it).nEndIndex -= (int)getSelectionLength();
						}
					}

					// erase selection using mode that does not modify d_text (we just want to update state)
					eraseSelectedText(false);

					// set text to the newly modified string
					setText(tmp);
				}
				else
				{
					// Trigger invalid modification attempted event.
					WindowEventArgs args(this);
					onInvalidEntryAttempted(args);
				}

			}
			else if (getCaratIndex() > 0)
			{
				tmp.erase(d_caratPos - 1, 1);

				if (isStringValid(tmp))
				{
					setCaratIndex(d_caratPos - 1);

					if(!delCaratIndexItemElement(d_itemList))
						delCaratIndexItemElement(d_itemList,(int)(d_caratPos+1));

					//0. adjust element list's old elemnts's begin and end.
					ItemElementList::iterator it = d_itemList.begin();
					for(;it!=d_itemList.end();++it)
					{
						if((int)d_caratPos <= (*it).nBeginIndex)
						{
							(*it).nBeginIndex -= 1;
							(*it).nEndIndex -= 1;
						}
					}

					// set text to the newly modified string
					setText(tmp);
				}
				else
				{
					// Trigger invalid modification attempted event.
					WindowEventArgs args(this);
					onInvalidEntryAttempted(args);
				}

			}

		}

	}


	/*************************************************************************
	Processing for Delete key	
	*************************************************************************/
	void FalagardIMEEditBox::handleDelete(void)
	{
		if (!isReadOnly())
		{
			String32 tmp(d_text);

			if (getSelectionLength() != 0)
			{
				tmp.erase(getSelectionStartIndex(), getSelectionLength());

				if (isStringValid(tmp))
				{
					delSelectionItemElements(d_itemList);
					delCaratIndexItemElement(d_itemList);

					//0. adjust element list's old elemnts's begin and end.
					ItemElementList::iterator it = d_itemList.begin();
					for(;it!=d_itemList.end();++it)
					{
						if((int)d_caratPos <= (*it).nBeginIndex)
						{
							(*it).nBeginIndex -= (int)getSelectionLength();
							(*it).nEndIndex -= (int)getSelectionLength();
						}
					}

					// erase selection using mode that does not modify d_text (we just want to update state)
					eraseSelectedText(false);

					// set text to the newly modified string
					setText(tmp);
				}
				else
				{
					// Trigger invalid modification attempted event.
					WindowEventArgs args(this);
					onInvalidEntryAttempted(args);
				}

			}
			else if (getCaratIndex() < tmp.length())
			{
				tmp.erase(d_caratPos, 1);

				if (isStringValid(tmp))
				{
					if(!delCaratIndexItemElement(d_itemList,(int)(d_caratPos)))
						delCaratIndexItemElement(d_itemList,(int)(d_caratPos+1));

					//0. adjust element list's old elemnts's begin and end.
					ItemElementList::iterator it = d_itemList.begin();
					for(;it!=d_itemList.end();++it)
					{
						if((int)d_caratPos <= (*it).nBeginIndex)
						{
							(*it).nBeginIndex -= 1;
							(*it).nEndIndex -= 1;
						}
					}

					// set text to the newly modified string
					setText(tmp);
				}
				else
				{
					// Trigger invalid modification attempted event.
					WindowEventArgs args(this);
					onInvalidEntryAttempted(args);
				}

			}

		}

	}

	void FalagardIMEEditBox::populateRenderCache()
	{
		const StateImagery* imagery;

		// draw container etc
		// get WidgetLookFeel for the assigned look.
		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
		// try and get imagery for the approprite state.
		if( wlf.isStateImageryPresent( "Normal" ) )
		{
			imagery = &wlf.getStateImagery("Normal");
			// peform the rendering operation for the container.
			imagery->render(*this);
		}

		// get destination area for text
		Rect textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*this));

		//
		// Required preliminary work for text rendering operations
		//
		const FontBase* font = getFont();


		// no font == no more rendering
		if (!font)
			return;


		// This will point to the final string to be used for rendering.  Useful because it means we
		// do not have to have duplicate code or be copying d_text for handling masked/unmasked text.
		String32* editText;

		// Create a 'masked' version of the string if needed.
		String32 maskedText;
		if (isTextMasked())
		{
			maskedText.insert(0, d_text.length(), getMaskCodePoint());
			editText = &maskedText;
		}
		// text not masked to editText will be the windows d_text String32.
		else
		{
			editText = &d_text;
		}

		Rect absarea(Point(0,0), getAbsoluteSize());
		Size textSize = font->getFormattedSize( *editText, absarea, (TextFormatting)LeftAligned, 1);
		if(textSize.d_height == 0)
			textSize.d_height = font->getLineSpacing();


		textArea.d_top = PixelAligned((absarea.getHeight() - textSize.d_height) * 0.5f);
		textArea.d_bottom = textArea.d_top + textSize.d_height;
		// calculate best position to render text to ensure carat is always visible
		float textOffset;
		float extentToCarat = font->getTextExtent(editText->substr(0, getCaratIndex()));

		// get carat imagery
		const ImagerySection& caratImagery = wlf.getImagerySection("Carat");
		// store carat width
		float caratWidth = caratImagery.getBoundingRect(*this, textArea).getWidth();

		// if need clear text offset
		if (d_clearOffset)
		{
			textOffset = 0.0f;
			d_clearOffset = false;
		}
		// if box is inactive
		else if (!hasInputFocus())
		{
			textOffset = d_lastTextOffset;
		}
		// if carat is to the left of the box
		else if ((d_lastTextOffset + extentToCarat) < 0)
		{
			textOffset = -extentToCarat;
		}
		// if carat is off to the right.
		else if ((d_lastTextOffset + extentToCarat) >= (textArea.getWidth() - caratWidth))
		{
			textOffset = textArea.getWidth() - extentToCarat - caratWidth;
		}
		// else carat is already within the box
		else
		{
			textOffset = d_lastTextOffset;
		}

		ColourRect colours;
		float alpha_comp = getEffectiveAlpha();

		//
		// Draw label text
		//
		Rect text_part_rect(textArea);
		text_part_rect.d_left += textOffset;

		// draw pre-highlight text
		String32 sect = editText->substr(0, getSelectionStartIndex());

		ItemElementList::iterator it = d_itemList.begin();
		size_t preIndex = 0;
		for(;it != d_itemList.end(); ++it)
		{
			ItemElement el = *it;
			if(el.nBeginIndex >= (int)sect.length()) break;

			String32 strNormal = sect.substr(preIndex, el.nBeginIndex-preIndex);
			colours.setColours(d_normalTextColour);
			colours.modulateAlpha(alpha_comp);
			d_renderCache.cacheText(this, strNormal, font, LeftAligned, text_part_rect, 0, colours, &textArea);

			// adjust rect for next section
			text_part_rect.d_left += font->getTextExtent(strNormal);
			preIndex = el.nBeginIndex;

			size_t endIndex = (el.nEndIndex >= (int)getSelectionStartIndex())?getSelectionStartIndex():el.nEndIndex+1;
			String32 strItemElement = sect.substr(preIndex, endIndex-preIndex);
			colours.setColours(d_normalTextColour);
			colours.modulateAlpha(alpha_comp);
			d_renderCache.cacheText(this, el.strColor+strItemElement, font, LeftAligned, text_part_rect, 0, colours, &textArea);

			// adjust rect for next section
			text_part_rect.d_left += font->getTextExtent(strItemElement);
			preIndex = endIndex;
		}

		if(preIndex < sect.length())
		{
			String32 strNormal = sect.substr(preIndex);
			colours.setColours(d_normalTextColour);
			colours.modulateAlpha(alpha_comp);
			d_renderCache.cacheText(this, strNormal, font, LeftAligned, text_part_rect, 0, colours, &textArea);

			// adjust rect for next section
			text_part_rect.d_left += font->getTextExtent(strNormal);
		}

		// draw highlight text
		sect = editText->substr(getSelectionStartIndex(), getSelectionLength());
		colours.setColours(d_selectTextColour);
		colours.modulateAlpha(alpha_comp);
		d_renderCache.cacheText(this, sect, font, LeftAligned, text_part_rect, 0, colours, &textArea);

		// adjust rect for next section
		text_part_rect.d_left += font->getTextExtent(sect);

		// draw post-highlight text
		sect = editText->substr(getSelectionEndIndex());

		it = d_itemList.begin();
		preIndex = 0;
		for(;it != d_itemList.end(); ++it)
		{
			ItemElement el = *it;
			if(el.nEndIndex < (int)getSelectionEndIndex()) continue;

			if(el.nBeginIndex <= (int)getSelectionEndIndex())
			{
				String32 strItemElement = sect.substr(preIndex, el.nEndIndex-getSelectionEndIndex()+1);
				colours.setColours(d_normalTextColour);
				colours.modulateAlpha(alpha_comp);
				d_renderCache.cacheText(this, el.strColor+strItemElement, font, LeftAligned, text_part_rect, 0, colours, &textArea);

				// adjust rect for next section
				text_part_rect.d_left += font->getTextExtent(strItemElement);
				preIndex = el.nEndIndex-getSelectionEndIndex()+1;
				continue;
			}

			String32 strNormal = sect.substr(preIndex, el.nBeginIndex-preIndex-getSelectionEndIndex());
			colours.setColours(d_normalTextColour);
			colours.modulateAlpha(alpha_comp);
			d_renderCache.cacheText(this, strNormal, font, LeftAligned, text_part_rect, 0, colours, &textArea);

			// adjust rect for next section
			text_part_rect.d_left += font->getTextExtent(strNormal);
			preIndex = el.nBeginIndex-getSelectionEndIndex();

			String32 strItemElement = sect.substr(preIndex, el.nEndIndex-preIndex+1-getSelectionEndIndex());
			colours.setColours(d_normalTextColour);
			colours.modulateAlpha(alpha_comp);
			d_renderCache.cacheText(this, el.strColor+strItemElement, font, LeftAligned, text_part_rect, 0, colours, &textArea);

			// adjust rect for next section
			text_part_rect.d_left += font->getTextExtent(strItemElement);
			preIndex = el.nEndIndex-getSelectionEndIndex()+1;
		}

		if(preIndex < sect.length())
		{
			String32 strNormal = sect.substr(preIndex);
			colours.setColours(d_normalTextColour);
			colours.modulateAlpha(alpha_comp);
			d_renderCache.cacheText(this, strNormal, font, LeftAligned, text_part_rect, 0, colours, &textArea);
		}
		preIndex = 0;

		// remember this for next time.
		d_lastTextOffset = textOffset;
		// adjust ime composition window.
		Ime::SetImeWindowPos();

		// see if the editbox is active or inactive.
		bool active = (!isReadOnly()) && hasInputFocus();

		//
		// Render selection imagery.
		//
		if (getSelectionLength() != 0)
		{
			// calculate required start and end offsets of selection imagery.
			float selStartOffset = font->getTextExtent(editText->substr(0, getSelectionStartIndex()));
			float selEndOffset   = font->getTextExtent(editText->substr(0, getSelectionEndIndex()));

			// calculate area for selection imagery.
			Rect hlarea(textArea);
			hlarea.d_left += textOffset + selStartOffset;
			hlarea.d_right = hlarea.d_left + (selEndOffset - selStartOffset);

			// render the selection imagery.
			wlf.getStateImagery(active ? "ActiveSelection" : "InactiveSelection").render(*this, hlarea, 0, &textArea);
		}

		//
		// Render carat
		//
		if (active && d_caretOn)
		{
			Rect caratRect(textArea); 
			caratRect.d_left += extentToCarat + textOffset;

			caratImagery.render(*this, caratRect, 0, 0, &caratRect);
		}
	}

	void FalagardIMEEditBox::updateSelf(float elapsed)
	{
		float time_now = System::getSingleton().getCurTimeElapsed();

		int textLen = (int)getText().size();
		if(textLen != d_lastTextLen) 
		{
			resetCaretBlink();
			d_lastTextLen = textLen;
		}

		if( time_now - d_lastBlink >= d_blinkTime/2)
		{
			// Change Blink text
			changeBlinkText();
		}

		//
		// Blink the caret
		//
		if( time_now - d_lastBlink >= d_blinkTime )
		{
			d_caretOn = !d_caretOn;
			d_lastBlink = time_now;

			requestRedraw();
		}

		Editbox::updateSelf(elapsed);
	}

	void FalagardIMEEditBox::changeBlinkText(void)
	{
		static bool bSet = false;
		if(d_blinkText.empty())	return;

		if(bSet)
		{
			setText(d_blinkText);
			d_blinkText.erase();

			clearSelection();
			bSet = false;
		}
		else
		{
			setSelection(0, getText().length());
			bSet = true;
		}

		requestRedraw();
	}

	void FalagardIMEEditBox::findCaratIndexItemElement(ItemElementList::iterator& itCarat, ItemElementList& itemList, int newPos)
	{
		int oldCaratIndex = (newPos < 0)?(int)getCaratIndex():newPos;

		itCarat = itemList.begin();
		for(; itCarat != itemList.end(); ++itCarat)
		{
			if((*itCarat).nBeginIndex < oldCaratIndex && (*itCarat).nEndIndex+1 > oldCaratIndex)
			{
				break;
			}
		}
	}

	bool FalagardIMEEditBox::delCaratIndexItemElement(ItemElementList& itemList, int newPos, bool bFireEvent)
	{
		if(itemList.empty()) return false;

		ItemElementList::iterator it;
		findCaratIndexItemElement(it,itemList,newPos);

		//1. modify list.
		if(it != itemList.end())
		{
			if(bFireEvent)
			{
				ItemElementEventArgs args(this);
				args.itemID = (*it).nID;

				fireEvent(EventItemElementDelete, args, EventNamespace);
			}
			itemList.erase(it);
			return true;
		}
		return false;
	}

	bool FalagardIMEEditBox::checkLength(const String32& str, const ItemElement* pElement)
	{
		static size_t extLen = 11; // "#{_INFOMSG}" length.

		String32 tmp(d_text);
		ItemElementList tmpList = d_itemList;

		//0.delete first.
		tmp.erase(getSelectionStartIndex(), getSelectionLength());
		delSelectionItemElements(tmpList, false);
		delCaratIndexItemElement(tmpList, (int)d_caratPos, false);

		//0.2 check max element
		if(pElement && (int)(tmpList.size()+1) > d_maxItemElement)
		{
			ItemElementEventArgs args(this);
			args.itemID = pElement->nID;
			fireEvent(EventItemElementFull, args, EventNamespace);

			return false;
		}

		//0.5 adjust element list's old elemnts's begin and end.
		ItemElementList::iterator it = tmpList.begin();
		for(;it!=tmpList.end();++it)
		{
			if((int)d_caratPos <= (*it).nBeginIndex)
			{
				(*it).nBeginIndex -= (int)getSelectionLength();
				(*it).nEndIndex -= (int)getSelectionLength();
			}
		}

		//1.cacul current length.
		size_t len = 0;
		it = tmpList.begin();
		for(String32::size_type i = 0; i < tmp.length(); ++i)
		{
			if(it != tmpList.end() && i == (*it).nBeginIndex)
			{
				//add element length
				len += (*it).nLength + extLen;

				i += (*it).nEndIndex - (*it).nBeginIndex;
				it++;
			}
			else
			{
				String32 strNormal;
				strNormal = tmp.at(i);
				//add normal length
				len += strNormal.mbcs_length();
			}
		}

		//2.add new length.
		if(pElement)
		{//add new element length
			len += pElement->nLength + extLen;
		}
		else
		{//add new str length
			len += str.mbcs_length();
		}

		return (len <= d_maxTextLen)?true:false;
	}

	void FalagardIMEEditBox::findSelectionItemElements(ItemElementList::iterator& itDelBegin, ItemElementList::iterator& itDelEnd, ItemElementList& itemList)
	{
		int oldSelectionStart = (int)getSelectionStartIndex();
		int oldSelectionEnd = (int)getSelectionEndIndex();

		//find begin
		ItemElementList::iterator it = itemList.begin();
		for(; it != itemList.end(); ++it)
		{
			ItemElement el = *it;
			if(el.nBeginIndex >= oldSelectionEnd) break;

			if(el.nBeginIndex <= oldSelectionStart && oldSelectionStart < el.nEndIndex)
			{
				itDelBegin = it;
				break;
			}

			if(el.nBeginIndex >= oldSelectionStart && oldSelectionStart < el.nEndIndex)
			{
				itDelBegin = it;
				break;
			}
		}

		//find end
		it = itDelBegin;
		for(; it != itemList.end(); ++it)
		{
			ItemElement el = *it;
			if(el.nBeginIndex >= oldSelectionEnd) break;

			if(el.nBeginIndex < oldSelectionEnd && oldSelectionEnd <= el.nEndIndex)
			{
				itDelEnd = it;
				break;
			}

			if(el.nBeginIndex < oldSelectionEnd && oldSelectionEnd >= el.nEndIndex)
			{
				itDelEnd = it;
				continue;
			}
		}
	}

	void FalagardIMEEditBox::delSelectionItemElements(ItemElementList& itemList, bool bFireEvent)
	{
		if(!getSelectionLength() || itemList.empty()) return;

		ItemElementList::iterator itDelBegin = itemList.end();
		ItemElementList::iterator itDelEnd = itemList.end();

		findSelectionItemElements(itDelBegin, itDelEnd, itemList);
		//modify list.
		if(itDelBegin != itemList.end())
		{
			if(itDelBegin == itDelEnd) 
			{
				if(bFireEvent)
				{
					ItemElementEventArgs args(this);
					args.itemID = (*itDelBegin).nID;

					fireEvent(EventItemElementDelete, args, EventNamespace);
				}

				itemList.erase(itDelBegin);
			}
			else
			{
				if(bFireEvent)
				{
					ItemElementList::iterator it = itDelBegin;
					for(; it != itemList.end(); ++it)
					{

						ItemElementEventArgs args(this);
						args.itemID = (*it).nID;

						fireEvent(EventItemElementDelete, args, EventNamespace);
						if(it == itDelEnd) break;
					}
				}
				itemList.erase(itDelBegin, ++itDelEnd);
			}
		}

		/*
		char dbgmsg[128] = {0};
		it = itemList.begin();
		_snprintf(dbgmsg, 127, "ItemElementList Size:%u\n", itemList.size());
		::OutputDebugString(dbgmsg);
		for(; it != itemList.end(); ++it)
		{
		_snprintf(dbgmsg,127,"ItemElement ID:%d nBeginIndex:%d nEndIndex:%d\n", (*it).nID,(*it).nBeginIndex,(*it).nEndIndex);
		::OutputDebugString(dbgmsg);
		}
		*/
	}

	size_t FalagardIMEEditBox::getTextIndexFromPosition(const Point& pt) const
	{
		//
		// calculate final window position to be checked
		//
		float wndx = screenToWindowX(pt.d_x);

		if (getMetricsMode() == Relative)
		{
			wndx = relativeToAbsoluteX(wndx);
		}

		wndx -= d_lastTextOffset;

		//
		// Return the proper index
		//
		if (isTextMasked())
		{
			return getFont()->getCharAtPixel(String32(d_text.length(), getMaskCodePoint()), wndx);
		}
		else
		{
			return getFont()->getCharAtPixel(d_text, wndx);
		}
	}

	void FalagardIMEEditBox::onActivated(ActivationEventArgs& e)
	{
		FalagardEditbox::onActivated(e);

		Ime::SetActiveIMEEditBox(this, true);
	}

	void FalagardIMEEditBox::onDeactivated(ActivationEventArgs& e)
	{
		FalagardEditbox::onDeactivated(e);

		//Ime::SetActiveIMEEditBox(this, false);
	}

	void FalagardIMEEditBox::onParentHidden(void)
	{
		FalagardEditbox::onParentHidden();

		Ime::SetActiveIMEEditBox(this, false);
	}

	/*************************************************************************
	Handler for mouse button pushed events
	*************************************************************************/
	void FalagardIMEEditBox::onMouseButtonDown(MouseEventArgs& e)
	{
		// base class handling
		Editbox::onMouseButtonDown(e);

		Ime::SetActiveIMEEditBox(this, true);
	}

	void FalagardIMEEditBox::onTextAcceptedEvent(WindowEventArgs& e)
	{
		Editbox::onTextAcceptedEvent(e);

		ItemElementList::iterator it = d_itemList.begin();
		for(; it != d_itemList.end(); ++it)
		{
			ItemElementEventArgs args(this);
			args.itemID = (*it).nID;

			fireEvent(EventItemElementDelete, args, EventNamespace);
		}

		d_itemList.clear();
	}

	void FalagardIMEEditBox::subscribeItemElementDeleteEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventItemElementDelete, subscriber);
	}

	void FalagardIMEEditBox::setItemElementString(const String32& szInfoString)
	{
		enum c_sign
		{
			c_nameBegin,
			c_normal,
		};

		CEGUI::utf32 beginType = (infoBegin>>24)&0xFF;
		CEGUI::utf32 endType = (infoEnd>>24)&0xFF;

		//0.delete already exist item element.
		ItemElementList::iterator it = d_itemList.begin();
		for(; it != d_itemList.end(); ++it)
		{
			ItemElementEventArgs args(this);
			args.itemID = (*it).nID;

			fireEvent(EventItemElementDelete, args, EventNamespace);
		}

		d_itemList.clear();
		setText("");
		//1.insert string
		String32 strDisplayName;
		int	itemId, itemLength;

		c_sign	sign = c_normal;

		for(size_t c = 0; c < szInfoString.length(); ++c)
		{
			CEGUI::utf32 code = szInfoString[c];
			CEGUI::utf32 type = (szInfoString[c]>>24)&0xFF;

			if(type == beginType)
			{
				sign = c_nameBegin;
				strDisplayName.erase();

				itemId = (int)(code & 0x00FFFFFF);
			}
			else if(type == endType)
			{
				itemLength = (int)(code & 0x00FFFFFF);
				injectItemInfo(itemId, strDisplayName, itemLength);
				sign = c_normal;
			}
			else
			{
				switch(sign) {
				case c_nameBegin:
					strDisplayName += code;
					break;
				case c_normal:
				default:
					addCharater(code);
					break;
				}
			}
		}
	}

	void FalagardIMEEditBox::setBlinkText(const String32& text)
	{
		if(text.empty()) return;
		d_blinkText = text;
	}

	String32 FalagardIMEEditBox::getItemElementString()
	{
		if(d_text.empty() || d_itemList.empty())	return d_text;

		String32 textWithInfo;
		ItemElementList::const_iterator it = d_itemList.begin();

		int preIndex = 0;
		for(; it != d_itemList.end(); it++)
		{
			ItemElement el = *it;

			//1.split d_txt
			String32 strTemp = d_text.substr(preIndex, el.nBeginIndex-preIndex);
			preIndex = el.nEndIndex+1;

			//2.construct new txt
			//ignore the display name. because no need to know this info.
			//ignore the length. because no need to know this info.
			CEGUI::utf32 id = el.nID;
			id |= infoBegin;

			textWithInfo += strTemp + id + infoEnd;
			if(preIndex >= (int)d_text.length()) break;
		}

		if(preIndex < (int)d_text.length())
		{
			textWithInfo += d_text.substr(preIndex);
		}

		return textWithInfo;
	}
	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	Window* FalagardIMEEditBoxFactory::createWindow(const String& name)
	{
		return new FalagardIMEEditBox(d_type, name);
	}

	void FalagardIMEEditBoxFactory::destroyWindow(Window* window)
	{
		delete window;
	}

}
