/************************************************************************
	filename: 	CEGUIString.cpp
	created:	26/2/2004
	author:		Paul D Turner
	
	purpose:	Implements string class
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
#include "CEGUIString.h"

#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{

// definition of 'no position' value
const String32::size_type String32::npos = (String32::size_type)(-1);

int g_theString32Counts = 0;

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
String32::~String32(void)
{
	g_theString32Counts--;

	if (d_reserve > STR_QUICKBUFF_SIZE)
	{
		delete[] d_buffer;
	}
		if (d_encodedbufflen > 0)
	{
		delete[] d_encodedbuff;
	}
}

bool String32::grow(size_type new_size)
{
    // check for too big
    if (max_size() <= new_size)
        std::length_error("Resulting CEGUI::String32 would be too big");

    // increase, as we always null-terminate the buffer.
    ++new_size;

    if (new_size > d_reserve)
    {
        utf32* temp = new utf32[new_size];

        if (d_reserve > STR_QUICKBUFF_SIZE)
        {
            memcpy(temp, d_buffer, (d_cplength + 1) * sizeof(utf32));
            delete[] d_buffer;
        }
        else
        {
            memcpy(temp, d_quickbuff, (d_cplength + 1) * sizeof(utf32));
        }

        d_buffer = temp;
        d_reserve = new_size;

        return true;
    }

    return false;
}

// perform re-allocation to remove wasted space.
void String32::trim(void)
{
    size_type min_size = d_cplength + 1;

    // only re-allocate when not using quick-buffer, and when size can be trimmed
    if ((d_reserve > STR_QUICKBUFF_SIZE) && (d_reserve > min_size))
    {
            // see if we can trim to quick-buffer
        if (min_size <= STR_QUICKBUFF_SIZE)
        {
            memcpy(d_quickbuff, d_buffer, min_size * sizeof(utf32));
            delete[] d_buffer;
            d_reserve = STR_QUICKBUFF_SIZE;
        }
        // re-allocate buffer
        else
        {
            utf32* temp = new utf32[min_size];
            memcpy(temp, d_buffer, min_size * sizeof(utf32));
            delete[] d_buffer;
            d_buffer = temp;
            d_reserve = min_size;
        }

    }

}

// build an internal buffer with the string encoded as utf8 (remains valid until string is modified).
utf8* String32::build_utf8_buff(void) const
{
    size_type buffsize = encoded_size(ptr(), d_cplength) + 1;

    if (buffsize > d_encodedbufflen) {

        if (d_encodedbufflen > 0)
        {
            delete[] d_encodedbuff;
        }

        d_encodedbuff = new utf8[buffsize];
        d_encodedbufflen = buffsize;
    }

    encode(ptr(), d_encodedbuff, buffsize, d_cplength);

    // always add a null at end
    d_encodedbuff[buffsize-1] = ((utf8)0);
    d_encodeddatlen = buffsize;

    return d_encodedbuff;
}



//////////////////////////////////////////////////////////////////////////
// Comparison operators
//////////////////////////////////////////////////////////////////////////
bool	operator==(const String32& str1, const String32& str2)
{
	return (str1.compare(str2) == 0);
}

bool	operator==(const String32& str, const std::string& std_str)
{
	return (str.compare(std_str) == 0);
}

bool	operator==(const std::string& std_str, const String32& str)
{
	return (str.compare(std_str) == 0);
}

bool	operator==(const String32& str, const utf8* utf8_str)
{
	return (str.compare(utf8_str) == 0);
}

bool	operator==(const utf8* utf8_str, const String32& str)
{
	return (str.compare(utf8_str) == 0);
}


bool	operator!=(const String32& str1, const String32& str2)
{
	return (str1.compare(str2) != 0);
}

bool	operator!=(const String32& str, const std::string& std_str)
{
	return (str.compare(std_str) != 0);
}

bool	operator!=(const std::string& std_str, const String32& str)
{
	return (str.compare(std_str) != 0);
}

bool	operator!=(const String32& str, const utf8* utf8_str)
{
	return (str.compare(utf8_str) != 0);
}

bool	operator!=(const utf8* utf8_str, const String32& str)
{
	return (str.compare(utf8_str) != 0);
}


bool	operator<(const String32& str1, const String32& str2)
{
	return (str1.compare(str2) < 0);
}

bool	operator<(const String32& str, const utf8* utf8_str)
{
	return (str.compare(utf8_str) < 0);
}

bool	operator<(const utf8* utf8_str, const String32& str)
{
	return (str.compare(utf8_str) >= 0);
}

bool	operator>(const String32& str1, const String32& str2)
{
	return (str1.compare(str2) > 0);
}

bool	operator>(const String32& str, const utf8* utf8_str)
{
	return (str.compare(utf8_str) > 0);
}

bool	operator>(const utf8* utf8_str, const String32& str)
{
	return (str.compare(utf8_str) <= 0);
}


bool	operator<=(const String32& str1, const String32& str2)
{
	return (str1.compare(str2) <= 0);
}

bool	operator<=(const String32& str, const std::string& std_str)
{
	return (str.compare(std_str) <= 0);
}

bool	operator<=(const std::string& std_str, const String32& str)
{
	return (str.compare(std_str) >= 0);
}

bool	operator<=(const String32& str, const utf8* utf8_str)
{
	return (str.compare(utf8_str) <= 0);
}

bool	operator<=(const utf8* utf8_str, const String32& str)
{
	return (str.compare(utf8_str) >= 0);
}


bool	operator>=(const String32& str1, const String32& str2)
{
	return (str1.compare(str2) >= 0);
}

bool	operator>=(const String32& str, const std::string& std_str)
{
	return (str.compare(std_str) >= 0);
}

bool	operator>=(const std::string& std_str, const String32& str)
{
	return (str.compare(std_str) <= 0);
}

bool	operator>=(const String32& str, const utf8* utf8_str)
{
	return (str.compare(utf8_str) >= 0);
}

bool	operator>=(const utf8* utf8_str, const String32& str)
{
	return (str.compare(utf8_str) <= 0);
}

//////////////////////////////////////////////////////////////////////////
// c-string operators
//////////////////////////////////////////////////////////////////////////
bool operator==(const String32& str, const char* c_str)
{
	return (str.compare(c_str) == 0);
}

bool operator==(const char* c_str, const String32& str)
{
	return (str.compare(c_str) == 0);
}

bool operator!=(const String32& str, const char* c_str)
{
	return (str.compare(c_str) != 0);
}

bool operator!=(const char* c_str, const String32& str)
{
	return (str.compare(c_str) != 0);
}

bool operator<(const String32& str, const char* c_str)
{
	return (str.compare(c_str) < 0);
}

bool operator<(const char* c_str, const String32& str)
{
	return (str.compare(c_str) >= 0);
}

bool operator>(const String32& str, const char* c_str)
{
	return (str.compare(c_str) > 0);
}

bool operator>(const char* c_str, const String32& str)
{
	return (str.compare(c_str) <= 0);
}

bool operator<=(const String32& str, const char* c_str)
{
	return (str.compare(c_str) <= 0);
}

bool operator<=(const char* c_str, const String32& str)
{
	return (str.compare(c_str) >= 0);
}

bool operator>=(const String32& str, const char* c_str)
{
	return (str.compare(c_str) >= 0);
}

bool operator>=(const char* c_str, const String32& str)
{
	return (str.compare(c_str) <= 0);
}

//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
String32	operator+(const String32& str1, const String32& str2)
{
	String32 temp(str1);
	temp.append(str2);
	return temp;
}

String32	operator+(const String32& str, const std::string& std_str)
{
	String32 temp(str);
	temp.append(std_str);
	return temp;
}

String32	operator+(const std::string& std_str, const String32& str)
{
	String32 temp(std_str);
	temp.append(str);
	return temp;
}

String32	operator+(const String32& str, const utf8* utf8_str)
{
	String32 temp(str);
	temp.append(utf8_str);
	return temp;
}

String32	operator+(const utf8* utf8_str, const String32& str)
{
	String32 temp(utf8_str);
	temp.append(str);
	return temp;
}

String32	operator+(const String32& str, utf32 code_point)
{
	String32 temp(str);
	temp.append(1, code_point);
	return temp;
}

String32	operator+(utf32 code_point, const String32& str)
{
	String32 temp(1, code_point);
	temp.append(str);
	return temp;
}

String32 operator+(const String32& str, const char* c_str)
{
	String32 tmp(str);
	tmp.append(c_str);
	return tmp;
}

String32 operator+(const char* c_str, const String32& str)
{
	String32 tmp(c_str);
	tmp.append(str);
	return tmp;
}

//////////////////////////////////////////////////////////////////////////
// Output (stream) functions
//////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& s, const String32& str)
{
	return s << str.c_str();
}

//////////////////////////////////////////////////////////////////////////
// Modifying operations
//////////////////////////////////////////////////////////////////////////
// swap the contents of str1 and str2
void	swap(String32& str1, String32& str2)
{
	str1.swap(str2);
}



/*!

-----------------------------------------------------------

			String32 from std::string

-----------------------------------------------------------

*/


bool	operator==(const String& str, const utf8* utf8_str)
{
	return str == (const char*)utf8_str;
}

bool	operator==(const utf8* utf8_str, const String& str)
{
	return str == (const char*)utf8_str;
}

String operator+(const String& str, const utf8* utf8_str)
{
	String temp(str);
	temp.append((const char*)utf8_str);
	return temp;
}

String operator+(const utf8* utf8_str, const String& str)
{
	String temp((const char*)utf8_str);
	temp.append(str);
	return temp;
}

} // End of  CEGUI namespace section
