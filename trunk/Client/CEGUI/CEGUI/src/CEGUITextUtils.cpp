/************************************************************************
filename: 	CEGUITextUtils.cpp
created:	30/5/2004
author:		Paul D Turner

purpose:	Implementation of text support class
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
#include "CEGUITextUtils.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*************************************************************************
	Constants
	*************************************************************************/
	const String32	TextUtils::DefaultWhitespace		= (utf8*)" \n\t\r";
	const String32	TextUtils::DefaultAlphanumerical	= (utf8*)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	const String32	TextUtils::DefaultWrapDelimiters	= (utf8*)" \n\t\r";


	/*************************************************************************
	return a String32 containing the the next word in a String32.
	*************************************************************************/
	String32 TextUtils::getNextWord(const String32& str, String32::size_type start_idx, const String32& delimiters)
	{
		String32::size_type	word_start = str.find_first_not_of(delimiters, start_idx);

		if (word_start == String32::npos)
		{
			word_start = start_idx;
		}

		//String32::size_type	word_end = str.find_first_of(delimiters, word_start);

		//if (word_end == String32::npos)
		//{
		//	word_end = str.length();
		//}

		//return str.substr(start_idx, (word_end - start_idx));
		return str.substr(start_idx, 1);
	}


	/*************************************************************************
	Return the index of the first character of the word at 'idx'.	
	*************************************************************************/
	String32::size_type TextUtils::getWordStartIdx(const String32& str, String32::size_type idx)
	{
		String32 temp = str.substr(0, idx);

		trimTrailingChars(temp, DefaultWhitespace);

		if (temp.length() <= 1) {
			return 0;
		}

		// identify the type of character at 'pos'
		if (String32::npos != DefaultAlphanumerical.find(temp[temp.length() - 1]))
		{
			idx = temp.find_last_not_of(DefaultAlphanumerical);
		}
		// since whitespace was stripped, character must be a symbol
		else
		{
			idx = temp.find_last_of(DefaultAlphanumerical + DefaultWhitespace);
		}

		// make sure we do not go past end of string (+1)
		if (idx == String32::npos)
		{
			return 0;
		}
		else
		{
			return idx + 1;
		}

	}


	/*************************************************************************
	Return the index of the first character of the word after the word
	at 'idx'.	
	*************************************************************************/
	String32::size_type TextUtils::getNextWordStartIdx(const String32& str, String32::size_type idx)
	{
		String32::size_type str_len = str.length();

		// do some checks for simple cases
		if ((idx >= str_len) || (str_len == 0))
		{
			return str_len;
		}

		// is character at 'idx' alphanumeric
		if (String32::npos != DefaultAlphanumerical.find(str[idx]))
		{
			// find position of next character that is not alphanumeric
			idx = str.find_first_not_of(DefaultAlphanumerical, idx);
		}
		// is character also not whitespace (therefore a symbol)
		else if (String32::npos == DefaultWhitespace.find(str[idx]))
		{
			// find index of next character that is either alphanumeric or whitespace
			idx = str.find_first_of(DefaultAlphanumerical + DefaultWhitespace, idx);
		}

		// check result at this stage.
		if (String32::npos == idx)
		{
			idx = str_len;
		}
		else
		{
			// if character at 'idx' is whitespace
			if (String32::npos != DefaultWhitespace.find(str[idx]))
			{
				// find next character that is not whitespace
				idx = str.find_first_not_of(DefaultWhitespace, idx);
			}

			if (String32::npos == idx)
			{
				idx = str_len;
			}

		}

		return idx;
	}


	/*************************************************************************
	Trim all characters from the set specified in \a chars from the
	begining of 'str'.	
	*************************************************************************/
	void TextUtils::trimLeadingChars(String32& str, const String32& chars)
	{
		String32::size_type idx = str.find_first_not_of(chars);

		if (idx != String32::npos)
		{
			str.erase(0, idx);
		}
		else
		{
			str.erase();
		}

	}


	/*************************************************************************
	Trim all characters from the set specified in \a chars from the end
	of 'str'.	
	*************************************************************************/
	void TextUtils::trimTrailingChars(String32& str, const String32& chars)
	{
		String32::size_type idx = str.find_last_not_of(chars);

		if (idx != String32::npos)
		{
			str.resize(idx + 1);
		}
		else
		{
			str.erase();
		}

	}

} // End of  CEGUI namespace section
