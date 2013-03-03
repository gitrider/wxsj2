#pragma once

#include "fi_share/fi_autobuffer.h"
#include "fi_share/fi_wrapbuffer.h"

template<>
inline
int FI_GetConvertCharSize( const irr::core::vector2df &value )
{
	return sizeof(value.x)+sizeof(value.y);
}

template<>
inline
bool FI_ConvertToChar( FI_TSTR &pBegin, const irr::core::vector2df &value,  FI_CTSTR pEnd )
{
	bool b = FI_ConvertToChar( pBegin, value.x, pEnd );
	FI_FALSE_RETURN(b);
	return FI_ConvertToChar( pBegin, value.y, pEnd );
}

template<>
inline 
bool FI_ConvertFromChar( FI_CTSTR &pBegin, irr::core::vector2df &value, FI_CTSTR pEnd )
{
	bool b = FI_ConvertFromChar( pBegin, value.x, pEnd );
	FI_FALSE_RETURN(b);
	return FI_ConvertFromChar( pBegin, value.y, pEnd );
}

template<>
inline
int FI_GetConvertCharSize( const irr::core::vector3df &value )
{
	return sizeof(value.x)+sizeof(value.y)+sizeof(value.z);
}

template<>
inline
bool FI_ConvertToChar( FI_TSTR &pBegin, const irr::core::vector3df &value,  FI_CTSTR pEnd )
{
	bool b = FI_ConvertToChar( pBegin, value.x, pEnd );
	FI_FALSE_RETURN(b);
	b =  FI_ConvertToChar( pBegin, value.y, pEnd );
	FI_FALSE_RETURN(b);
	return FI_ConvertToChar( pBegin, value.z, pEnd );
}

template<>
inline 
bool FI_ConvertFromChar( FI_CTSTR &pBegin, irr::core::vector3df &value, FI_CTSTR pEnd )
{
	bool b = FI_ConvertFromChar( pBegin, value.x, pEnd );
	FI_FALSE_RETURN(b);
	b = FI_ConvertFromChar( pBegin, value.y, pEnd );
	FI_FALSE_RETURN(b);
	return FI_ConvertFromChar( pBegin, value.z, pEnd );
}

template<>
inline
int FI_GetConvertCharSize( const irr::core::quaternion &value )
{
	return sizeof(value.x)+sizeof(value.y)+sizeof(value.z)+sizeof(value.w);
}

template<>
inline
bool FI_ConvertToChar( FI_TSTR &pBegin, const irr::core::quaternion &value,  FI_CTSTR pEnd )
{
	bool b = FI_ConvertToChar( pBegin, value.x, pEnd );
	FI_FALSE_RETURN(b);
	b =  FI_ConvertToChar( pBegin, value.y, pEnd );
	FI_FALSE_RETURN(b);
	b = FI_ConvertToChar( pBegin, value.z, pEnd );
	FI_FALSE_RETURN(b);
	return FI_ConvertToChar( pBegin, value.w, pEnd );
}

template<>
inline 
bool FI_ConvertFromChar( FI_CTSTR &pBegin, irr::core::quaternion &value, FI_CTSTR pEnd )
{
	bool b = FI_ConvertFromChar( pBegin, value.x, pEnd );
	FI_FALSE_RETURN(b);
	b = FI_ConvertFromChar( pBegin, value.y, pEnd );
	FI_FALSE_RETURN(b);
	b = FI_ConvertFromChar( pBegin, value.z, pEnd );
	FI_FALSE_RETURN(b);
	return FI_ConvertFromChar( pBegin, value.z, pEnd );
}

