#include "FalMultiIMEEditBox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIIme.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	//===================================================================================
	//
	// FalagardMultiIMEEditBox
	//
	//===================================================================================
	const utf8  FalagardMultiIMEEditBox::WidgetTypeName[] = "Falagard/MultiIMEEditBox";

	const String FalagardMultiIMEEditBox::EventNamespace("Falagard/FalagardMultiIMEEditBox");

	// Event Strings
	const String FalagardMultiIMEEditBox::EventMultiItemElementDelete("MultiItemElementDelete");
	const String FalagardMultiIMEEditBox::EventMultiItemelementJoinFailure("MultiItemelementJoinFailure");
	const String FalagardMultiIMEEditBox::EventMultiItemElementFull("MultiItemElementFull");

	const utf32 FalagardMultiIMEEditBox::infoBegin	= 0xE4000000;
	const utf32 FalagardMultiIMEEditBox::infoEnd		= 0xE5000000;

	FalagardMultiIMEEditBoxProperties::DefaultEditBox	FalagardMultiIMEEditBox::d_defaultEditBoxProperty;
	FalagardMultiIMEEditBoxProperties::DisableCopyPaste	FalagardMultiIMEEditBox::d_disableCopyPasteProperty;

	FalagardMultiIMEEditBox::FalagardMultiIMEEditBox(const String& type, const String& name) :
	FalagardMultiLineEditbox(type, name),
		d_lastTextOffset(0),
		d_caretOn(true),
		d_lastTextLen(0),
		d_addDirect(false),
		d_maxItemElement(3),
		d_bCopyPasteDisable(false)
	{
		addMultiIMEEditBoxProperties();

		d_blinkTime = Ime::GetCaretBlinkTime()*0.001f;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();

		d_softKeyOnly = false;
	}

	FalagardMultiIMEEditBox::~FalagardMultiIMEEditBox()
	{
	}

	void FalagardMultiIMEEditBox::addMultiIMEEditBoxProperties(void)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardMultiIMEEditBox )
			CEGUI_ADD_STATICPROPERTY( &d_defaultEditBoxProperty );
		CEGUI_ADD_STATICPROPERTY( &d_disableCopyPasteProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	bool FalagardMultiIMEEditBox::hasInputFocus(void) const
	{
		return Ime::GetActiveIMEEditBox() == this;
	}
	void FalagardMultiIMEEditBox::updateSelf(float elapsed)
	{
		float time_now = System::getSingleton().getCurTimeElapsed();

		int textLen = (int)getText().size();
		if(textLen != d_lastTextLen) 
		{
			resetCaretBlink();
			d_lastTextLen = textLen;
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

		FalagardMultiLineEditbox::updateSelf(elapsed);
	}

	void FalagardMultiIMEEditBox::findCaratIndexItemElement(ItemElementList::iterator& itCarat, ItemElementList& itemList, int newPos)
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

	bool FalagardMultiIMEEditBox::delCaratIndexItemElement(ItemElementList& itemList, int newPos, bool bFireEvent)
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

				fireEvent(EventMultiItemElementDelete, args, EventNamespace);
			}
			itemList.erase(it);
			return true;
		}
		return false;
	}

	bool FalagardMultiIMEEditBox::checkLength(const String32& str, const ItemElement* pElement)
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
			fireEvent(EventMultiItemElementFull, args, EventNamespace);

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

	void FalagardMultiIMEEditBox::findSelectionItemElements(ItemElementList::iterator& itDelBegin, ItemElementList::iterator& itDelEnd, ItemElementList& itemList)
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

	void FalagardMultiIMEEditBox::delSelectionItemElements(ItemElementList& itemList, bool bFireEvent)
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

					fireEvent(EventMultiItemElementDelete, args, EventNamespace);
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

						fireEvent(EventMultiItemElementDelete, args, EventNamespace);
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

	void FalagardMultiIMEEditBox::cacheCaratImagery(const Rect& textArea)
	{
		if( hasInputFocus() && d_caretOn )
			FalagardMultiLineEditbox::cacheCaratImagery( textArea );
	}

	void FalagardMultiIMEEditBox::resetCaretBlink(void)
	{
		d_caretOn = true;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();
		requestRedraw();
	}

	void FalagardMultiIMEEditBox::onActivated(ActivationEventArgs& e)
	{
		FalagardMultiLineEditbox::onActivated(e);

		Ime::SetActiveIMEEditBox(this, true);
	}

	void FalagardMultiIMEEditBox::onDeactivated(ActivationEventArgs& e)
	{
		FalagardMultiLineEditbox::onDeactivated(e);

		//Ime::SetActiveIMEEditBox(this, false);
	}

	/*************************************************************************
	Handler for mouse button pushed events
	*************************************************************************/
	void FalagardMultiIMEEditBox::onMouseButtonDown(MouseEventArgs& e)
	{
		// base class handling
		FalagardMultiLineEditbox::onMouseButtonDown(e);

		Ime::SetActiveIMEEditBox(this, true);
	}

	//void FalagardMultiIMEEditBox::onTextAcceptedEvent(WindowEventArgs& e)
	//{
	//	MultiLineEditbox::onTextAcceptedEvent(e);

	//	ItemElementList::iterator it = d_itemList.begin();
	//	for(; it != d_itemList.end(); ++it)
	//	{
	//		ItemElementEventArgs args(this);
	//		args.itemID = (*it).nID;

	//		fireEvent(EventMultiItemElementDelete, args, EventNamespace);
	//	}

	//	d_itemList.clear();
	//}

	void FalagardMultiIMEEditBox::onParentHidden(void)
	{
		// base class handling
		FalagardMultiLineEditbox::onParentHidden();

		d_itemList.clear();
		Ime::SetActiveIMEEditBox(this, false);
	}

	bool	FalagardMultiIMEEditBox::injectItemInfo(int nID, const String32& strDisplayName, int nLength)
	{
		//1. create a new element.
		ItemElement el;
		el.nID = nID;
		for ( size_t idx = 0; idx < strDisplayName.length(); ++idx )
		{
			if ( is_FireFox_Utf32_Code(strDisplayName[idx]) )
			{
				el.strColor += strDisplayName[idx];
			} 
			else
			{
				break;
			}
		}
		String32 strNoColor;
		remove_FireFox_Utf32_Code( strDisplayName, strNoColor );

		el.nBeginIndex = (int)getSelectionStartIndex();
		el.nEndIndex = el.nBeginIndex + (int)strNoColor.length() - 1;
		el.nLength = nLength;

		//1.5 length check before add this element
		String32 strTemp;
		if(!checkLength(strTemp, &el))
		{
			// Trigger itemelement add failure event
			ItemElementEventArgs args(this);
			args.itemID = el.nID;
			fireEvent(EventMultiItemelementJoinFailure, args, EventNamespace);

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

	void FalagardMultiIMEEditBox::addCharater(const utf32& codepoint, bool isFromSoftKey )
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

				//if (isStringValid(tmp))
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
				//else
				//{
				//	// Trigger invalid modification attempted event.
				//	//WindowEventArgs args(this);
				//	//onInvalidEntryAttempted(args);
				//}

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
	void FalagardMultiIMEEditBox::handleBackspace(void)
	{
		if (!isReadOnly())
		{
			String32 tmp(d_text);

			if (getSelectionLength() != 0)
			{
				tmp.erase(getSelectionStartIndex(), getSelectionLength());

				//if (isStringValid(tmp))
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
				//else
				//{
				//	// Trigger invalid modification attempted event.
				//	WindowEventArgs args(this);
				//	onInvalidEntryAttempted(args);
				//}

			}
			else if (getCaratIndex() > 0)
			{
				tmp.erase(d_caratPos - 1, 1);

				//if (isStringValid(tmp))
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
				//else
				//{
				//	// Trigger invalid modification attempted event.
				//	WindowEventArgs args(this);
				//	onInvalidEntryAttempted(args);
				//}

			}

		}

	}


	/*************************************************************************
	Processing for Delete key	
	*************************************************************************/
	void FalagardMultiIMEEditBox::handleDelete(void)
	{
		if (!isReadOnly())
		{
			String32 tmp(d_text);

			if (getSelectionLength() != 0)
			{
				tmp.erase(getSelectionStartIndex(), getSelectionLength());

				//if (isStringValid(tmp))
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
				//else
				//{
				//	// Trigger invalid modification attempted event.
				//	WindowEventArgs args(this);
				//	onInvalidEntryAttempted(args);
				//}

			}
			else if (getCaratIndex() < tmp.length())
			{
				tmp.erase(d_caratPos, 1);

				//if (isStringValid(tmp))
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
				//else
				//{
				//	// Trigger invalid modification attempted event.
				//	WindowEventArgs args(this);
				//	onInvalidEntryAttempted(args);
				//}

			}

		}

	}


	void FalagardMultiIMEEditBox::setItemElementString(const String32& szInfoString)
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

			fireEvent(EventMultiItemElementDelete, args, EventNamespace);
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

	String32 FalagardMultiIMEEditBox::getItemElementString()
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
	Window* FalagardMultiIMEEditBoxFactory::createWindow(const String& name)
	{
		return new FalagardMultiIMEEditBox(d_type, name);
	}

	void FalagardMultiIMEEditBoxFactory::destroyWindow(Window* window)
	{
		delete window;
	}

}

