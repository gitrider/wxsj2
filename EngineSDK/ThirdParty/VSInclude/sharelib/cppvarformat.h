#pragma once

#include "stdarg.h"
#include "stdio.h"

class CVarFormat
{
public:
	CVarFormat( const char *format, ... )
	{
		va_list args;
		va_start( args, format );
		vsnprintf( m_buffer,1023, format, args );	
		m_buffer[1023] = 0;
		va_end(args);
	}

	typedef const char * __CTSTR;
	operator __CTSTR()
	{
		return m_buffer;
	}

protected:
	char		m_buffer[1024];
};