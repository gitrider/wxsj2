#pragma once

//// impl buffer array build
#include "fi_messagestruct.h"
#pragma warning ( disable : 4018 4267 4244 )

	template< typename T > inline
		bool	LBS_AddStatusToBufferArray( const T *pStatus, std::vector<FI_AutoBufferPointer> &arrayBuffer )
	{
		FI_AutoBufferPointer p = FI_DefaultBuildBuffer( *pStatus, 4 );
		if( p == NULL )
		{
			return false;
		}
		else
		{
			p->writeInt( __getType(*pStatus), 0 );
			arrayBuffer.push_back( p );
			return true;
		}
	}

	template< typename T >
		bool	LBS_AddStatusToBufferArray( const T *pStatus, int nType, std::vector<FI_AutoBufferPointer> &arrayBuffer )
	{
		FI_AutoBufferPointer p = FI_DefaultBuildBuffer( *pStatus, 4 );
		if( p == NULL )
		{
			return false;
		}
		else
		{
			p->writeInt( nType, 0 );
			arrayBuffer.push_back( p );
			return true;
		}
	}

	template < typename T >
		bool	LBS_GetSpecialLieStatusInBufferArray( T *pStauts, int nType, int nStatusLie,  std::vector<FI_AutoBufferPointer> &arrayBuffer)
	{
		if( nStatusLie < arrayBuffer.size() )
		{
			FI_AutoBufferPointer p = arrayBuffer[nStatusLie];
			if( nType != 0 )
			{
				int nSaveType;
				p->readInt(nSaveType,0);
				if( nSaveType!=0 && nSaveType!=nType )
					return false;
			}
			if( FI_ConvertBuildFromBuffer( *pStauts, *p, 4 ) )
			{
				++nStatusLie;
				return true;
			}
		}
		return false;
	}

	template < typename T >
		bool	LBS_GetSpecialLieStatusInBufferArray( T *pStauts, int nStatusLie,  std::vector<FI_AutoBufferPointer> &arrayBuffer )
	{
		if( nStatusLie < arrayBuffer.size() )
		{
			FI_AutoBufferPointer p = arrayBuffer[nStatusLie];
			
			{
				int nSaveType;
				p->readInt(nSaveType,0);
				if( nSaveType!=0 && nSaveType!=__getType(*pStauts) )
					return false;
			}
			if( FI_ConvertBuildFromBuffer( *pStauts, *p, 4 ) )
			{
				++nStatusLie;
				return true;
			}
		}
		return false;
	}

	template < typename T >
		bool	LBS_GetStatusInBufferArray( T *pStatus, std::vector<FI_AutoBufferPointer> &arrayBuffer )
	{
		int nBuffeType;
		int nSize = arrayBuffer.size();
		for( size_t i=0; i<nSize; ++i )
		{
			FI_AutoBufferPointer p = arrayBuffer[i];
			p->readInt( nBuffeType, 0 );
			if( nBuffeType == __getType(*pStatus) )
			{
				if( FI_ConvertBuildFromBuffer( *pStatus, *p, 4 ) )
					return true;
			}
		}
		return false;
	}

	template < typename T >
		bool	LBS_GetStatusInBufferArray( T *pStatus, int nType, std::vector<FI_AutoBufferPointer> &arrayBuffer )
	{
		int nBuffeType;
		int nSize = exArray.size();
		for( size_t i=0; i<nSize; ++i )
		{
			FI_AutoBufferPointer p = exArray[i];
			p->readInt( nBuffeType, 0 );
			if( nBuffeType == nType )
			{
				if( FI_ConvertBuildFromBuffer( *pStatus, *p, 4 ) )
					return true;
			}
		}
		return false;
	}

#pragma warning ( default : 4018 4267 4244 )

