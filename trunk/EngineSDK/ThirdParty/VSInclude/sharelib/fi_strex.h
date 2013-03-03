#ifndef __FI_STRING_EX_H__
#define __FI_STRING_EX_H__

//#include "fi_messagestruct.h"
#include <algorithm>
#include "time.h"

#pragma warning( disable : 4244 4018 4267)


//static const std::string s_StringTrue = "true";
//static const std::string s_StringFalse = "false";


inline
int FI_StringCaseCmp( const char *l, const char *r )
{
	return _stricmp( l, r );
}

inline 
int FI_StringCaseNCmp( const char *l, const char *r, int n )
{
	return _strnicmp( l, r, n );
}



inline
bool FI_KeyStringExist( const char *sSource, const char *sKey )
{
	std::string sS = sSource;
	std::for_each( sS.begin(), sS.end(), tolower );
	std::string sK = sKey;
	std::for_each( sK.begin(), sK.end(), tolower );
	return strstr( sS.c_str(), sK.c_str() ) != NULL;
}


///没有进行转化,仅仅是类型变化,值未变
inline
void	UTF16_2_UTF32( unsigned short *pSource, int nLen16, unsigned int *pDest, int nLen32 )
{
	int nLen = nLen16;
	if( nLen > nLen32 )
		nLen = nLen32;
	for( int i=0; i<nLen; ++i )
		pDest[i] = pSource[i];
}

inline
void	UTF32_2_UTF16( unsigned int *pSource, int nLen32, unsigned short *pDest, int nLen16 )
{
	int nLen = nLen16;
	if( nLen > nLen32)
		nLen = nLen32;
	for( int i=0; i<nLen; ++i )
		pDest[i] = pSource[i];
}


#ifdef WIN32
#ifdef WCHAR_CONVERT
inline int FI_StringToWString(std::string Source,std::wstring &wout)
{
	int usLen=Source.length()+1;
	wchar_t *pWStr=new wchar_t[usLen];
	usLen=MultiByteToWideChar(CP_ACP, 0,Source.c_str(),Source.length(), pWStr, usLen);
	pWStr[usLen]=L'\0';
	wout=pWStr;
	delete []pWStr;
	return wout.length();
}

inline int FI_StringToWString( const char *Source, int nLength, std::wstring &wOut)
{
	int usLen=nLength+1;
	wchar_t *pWStr=new wchar_t[usLen];
	usLen=MultiByteToWideChar(CP_ACP, 0,Source,nLength, pWStr, usLen);
	pWStr[usLen]=L'\0';
	wOut=pWStr;
	delete []pWStr;
	return wOut.length();
}

inline int FI_StringToWString( int nPage, const char *Source, int nLength, std::wstring &wOut)
{
	int usLen=nLength+1;
	wchar_t *pWStr=new wchar_t[usLen];
	usLen=MultiByteToWideChar(nPage, 0,Source,nLength, pWStr, usLen);
	pWStr[usLen]=L'\0';
	wOut=pWStr;
	delete []pWStr;
	return wOut.length();
}

inline int FI_WStringToString(std::wstring &Source, std::string &sOut )
{
	int useLen = Source.length()*2+1;
	char *p = new char[useLen];
	useLen = WideCharToMultiByte(CP_ACP, 0, Source.c_str(), Source.length(), p, useLen, NULL, NULL );
	p[useLen] = '\0';
	sOut = p;
	delete []p;
	return sOut.size();
}

inline int FI_WStringToString(wchar_t *Source,int nLength, std::string &sOut )
{
	int useLen = nLength*2+1;
	char *p = new char[useLen];
	useLen = WideCharToMultiByte(CP_ACP, 0, Source, nLength, p, useLen, NULL, NULL );
	p[useLen] = '\0';
	sOut = p;
	delete []p;
	return sOut.size();
}

inline int FI_WStringToString(const char *Source,int nLength, std::string &sOut )
{
	int useLen = nLength+3;
	char *p = new char[useLen];
	const wchar_t* wSource = (wchar_t *)Source;
	useLen = WideCharToMultiByte(CP_ACP, 0, wSource, nLength/2, p, useLen, NULL, NULL );
	p[useLen] = '\0';
	sOut = p;
	delete []p;
	return sOut.size();
}

inline int FI_WStringToString(int nPage, const char *Source,int nLength, std::string &sOut )
{
	int useLen = nLength+3;
	char *p = new char[useLen];
	const wchar_t* wSource = (wchar_t *)Source;
	useLen = WideCharToMultiByte(nPage, 0, wSource, nLength/2, p, useLen, NULL, NULL );
	p[useLen] = '\0';
	sOut = p;
	delete []p;
	return sOut.size();
}
#endif
#endif

/*
inline
FI_GID_T FI_ID_FromString( const char *t )
{
	FI_GID_T tmp;
	char *p = strchr( t, '-' );
	tmp.nRandom = atoi( t );
	if( p )
		tmp.nPlayerID = atoi(p+1);
	return tmp;
}

inline
std::string FI_ID_ToString( FI_GID_T id )
{
	char buf[64];
	sprintf( buf, "%d-%d", id.nRandom, id.nPlayerID );
	return buf;
}*/

inline
std::string FI_FormatCTime( unsigned int nTime )
{
	///
	time_t cT = nTime;
	tm *pTm = localtime( &cT );
	char buf[128];
	sprintf( buf, "%d-%d-%d %d:%d:%d", pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec );
	return buf;
}

inline
std::string FI_FormatCTimeDate( unsigned int nTime )
{
	///
	time_t cT = nTime;
	tm *pTm = localtime( &cT );
	char buf[128];
	sprintf( buf, "%d-%d-%d", pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday );
	return buf;
}

inline
std::string FI_FormatCTimeTime( unsigned int nTime )
{
	///
	time_t cT = nTime;
	tm *pTm = localtime( &cT );
	char buf[128];
	sprintf( buf, "%d:%d:%d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec  );
	return buf;
}


template<typename T>
inline
std::string FI_IntToString( T n )
{
	char buf[64];
	sprintf( buf, "%d", n );
	return buf;
}

inline
std::string FI_BoolToString( bool b )
{
	if( b )
		return "true";
	else
		return "false";
}

inline
bool		FI_StringToBool( const char *s )
{
	if ( FI_StringCaseNCmp( s, "true", 4 ) == 0 )
		return true;
	else if( FI_StringCaseNCmp( s, "false", 5 ) == 0 )
		return false;
	else
	{
		int n = atoi(s);
		return n!=0;
	}
}

inline
int		FI_IntFromString( const char * s )
{
	return atoi(s);
}

/*!
	\des The c has been passed
*/
inline
const char * FI_Tokien( const char *p, char c)
{
	const char *pT = strchr(p,c);
	if( pT )
		return  pT+1;
	else
		return NULL;
}


/*!
	\des from current lie, to read to first c, in cInc
*/

inline const char * FI_TokienEx( const char *p, char c, char cChange='\\' )
{
	const char *pFirst = strchr(p,c);
	if( pFirst == p )
		return pFirst;
	while( pFirst )
	{
		int nChangeNum = 0;
		for( const char *pChange=pFirst-1; pChange>=p; --pChange )
		{
			if( (*pChange) == cChange )
				++nChangeNum;
			else
				break;
		}

		if( nChangeNum%2 == 0 )
		{
			return pFirst;
		}
		else
		{
			pFirst = strchr( pFirst+1, c );
		}		
	}

	return NULL;
}

inline
const char* FI_EndTokien( const char *p, char c,  char cInc='\"' )
{
	const char *pChar = strchr( p, c );
	if( pChar == NULL )
		return NULL;

	const char *pInc = FI_TokienEx( p, cInc, '\\' );
	
	while( pInc && (pInc < pChar) )
	{
		pInc = FI_TokienEx( pInc+1, cInc, '\\' );
		while( pChar < pInc )
		{
			pChar = FI_TokienEx( pChar+1, c );
			if( pChar == NULL )
				return NULL;
		}

		pInc = FI_TokienEx( pInc+1, cInc );
	}

	return pChar;
}

inline
bool LBS_ReadOneFullParam ( const char *pBegin, const char *pEnd, const char *&pParamBegin, const char *&pParamEnd, char cSep )
{
	while( (*pBegin) == cSep ) ++pBegin;
	if( pBegin >= pEnd )
		return false;
	const char *pE = strchr( pBegin, cSep );
	if( pE == NULL )
	{
		pParamBegin = pBegin;
		pParamEnd = pEnd;
		return true;
	}

	const char *pLS = strchr( pBegin, '\"' );
	if( (pLS == NULL) || (pLS>pE ) )
	{
		pParamBegin = pBegin;
		pParamEnd = pE;
		return true;
	}

	const char *pLE = strchr( pLS+1, '\"') ;
	if( (pLE == NULL) || (pLE < pE) )
	{
		pParamBegin = pBegin;
		pParamEnd = pE;
		return true;
	}

	pE = strchr( pLE, cSep );
	if( pE )
	{
		pParamBegin = pBegin;
		pParamEnd = pE;
		return true;
	}
	else
	{
		pParamBegin = pBegin;
		pParamEnd = pEnd;
		return true;
	}

}

/*!
	\des The last was include in string
*/
inline
const char *FI_BackTokien( const char *pBegin,  char c )
{
/*	for(;pEnd!=pBegin;--pBegin)
	{
		if( FI_StringCaseNCmp (pEnd, &c, 1) == 0 )
			return pEnd;
	}
	return NULL;*/
	const char *pT = strrchr(pBegin, c );
	if( pT )
		return pT-1;
	else
		return NULL;
}



inline
void	FI_String2FloatArray( const char *sPos, std::vector<float> &floatArray )
{
	while( sPos )
	{
		float n = atof(sPos);
		floatArray.push_back( n );
		sPos = FI_Tokien( sPos, ',' );
	}
}

inline
std::string FI_FloatArray2String(  std::vector<float> &floatArray )
{
	std::string s;
	int nFloatNum = floatArray.size();
	s.reserve( 10 * nFloatNum );

	char buf[64];
	for( int i=0; i<nFloatNum; ++i )
	{
		sprintf( buf, "%.2f", floatArray[i] );
		s += buf;
		if( i != (nFloatNum-1) )
			s += ',';
	}

	return s;
}

inline
std::string FI_FloatArray2String(  float *floatArray, int nFloatNum )
{
	std::string s;
	s.reserve( 10 * nFloatNum );

	char buf[64];
	for( int i=0; i<nFloatNum; ++i )
	{
		sprintf( buf, "%.2f", floatArray[i] );
		s += buf;
		if( i != (nFloatNum-1) )
			s += ',';
	}

	return s;
}

inline
int	FI_String2FloatArray( const char *sPos, float *floatArray, int nFloatNum )
{
	int i = 0;
	while( sPos )
	{
		if( i >= nFloatNum )
			return i;	
		float n = atof(sPos);
		floatArray[i] = n;
		sPos = FI_Tokien( sPos, ',' );
		++i;
	}
	return i;
}

inline
int	FI_String2FloatArray( const char *sPos, const char *sPosEnd, float *floatArray, int nFloatNum, const char cSep=' ' )
{
	int i = 0;
	while( sPos && (sPos<sPosEnd) )
	{
		if( i >= nFloatNum )
			return i;	
		float n = atof(sPos);
		floatArray[i] = n;
		sPos = FI_Tokien( sPos, cSep );
		++i;
	}
	return i;
}

template< typename T>
inline 
std::string FI_FloatToString( T n )
{
	char buf[64];
	sprintf( buf, "%.4f", n );
	return buf;
}

inline 
float FI_StringToFloat( const char *s )
{
	return atof(s);
}

inline bool	LBS_IsVisibleChar( const char c )
{
	if( (c == '\r')
		|| (c == '\n')
		|| (c == ' ')
		|| (c == '\t')
		|| ( (0x09<=c) && (c<=0x0D) )
		)
		return false;
	else
		return true;
}

inline
const char * LBS_FirstVisibleValue( const char *s, const char *pEnd )
{
	const char *tmp = (const char *)s;
	while( (*tmp) && (tmp<=pEnd || pEnd==NULL) )
	{
		if( LBS_IsVisibleChar(*tmp) )
			return tmp;
		++tmp;
	}
	return NULL;
}

inline
const char *LBS_BackVisibleValue( const char *sBegin, const char *sEnd  )
{
	const  char *tmp = (const char *)sEnd;
	while( tmp>=sBegin )
	{
		if( LBS_IsVisibleChar(*tmp) )
			return tmp;
		--tmp;
	}
	return NULL;
}

template< typename T >
inline
std::string FI_ToString( const T &value )
{
	return "";
}

template<  >
inline
std::string FI_ToString( const short &value )
{
	return FI_IntToString(value);
}

template<  >
inline
std::string FI_ToString( const int &value )
{
	return FI_IntToString(value);
}

template<  >
inline
std::string FI_ToString( const unsigned short &value )
{
	return FI_IntToString(value);
}

template<  >
inline
std::string FI_ToString( const unsigned int &value )
{
	return FI_IntToString(value);
}

template<  >
inline
std::string FI_ToString( const float &value )
{
	return FI_FloatToString( value);
}

template<  >
inline
std::string FI_ToString( const double &value )
{
	return FI_FloatToString( value);
}

template<  >
inline
std::string FI_ToString( const bool &value )
{
	return FI_BoolToString( value);
}

/*
template<  >
inline
std::string FI_ToString( const FI_GID_T &value )
{
	return FI_ID_ToString( value);
}

template<>
inline
FI_GID_T FI_FromString( const char *value )
{
	return FI_ID_FromString(value );
}

*/

template< typename T >
inline
T FI_FromString( const char *value )
{
///	return T(); This should not be call so is a error function
}

template<>
inline
int FI_FromString( const char *value )
{
	return FI_IntFromString(value );
}



template<>
inline
bool	FI_FromString( const char *value )
{
	return FI_StringToBool( value );
}

template<>
inline
float FI_FromString( const char *value )
{
	return FI_StringToFloat(value);
}

template<>
inline
double FI_FromString( const char *value )
{
	return FI_StringToFloat(value);
}

template<>
inline
long FI_FromString( const char *value )
{
	return FI_IntFromString(value);
}

template<>
inline
short FI_FromString( const char *value )
{
	return FI_IntFromString(value);
}

template<>
inline
unsigned short FI_FromString( const char *value )
{
	return FI_IntFromString(value);
}

template<>
inline
unsigned long FI_FromString( const char *value )
{
	return FI_IntFromString(value);
}

template<>
inline
unsigned int FI_FromString( const char *value )
{
	return FI_IntFromString(value);
}

inline const char * FI_StringFindByIntValue( const char *pString[], const int *pI, int nV)
{
	for(int i=0; pI[i]!=0; ++i)
	{
		if( pI[i] == nV )
			return pString[ i ];
	}
	return "";
}

inline int			FI_IntTypeFindByString( const char *pString[], const int *pI, const char *pFind )
{
	for( int i=0; strlen(pString[i])>0; i++ )
	{
		if( FI_StringCaseCmp( pString[i], pFind ) == 0 )
			return pI[i];
	}
	return 0;
}

inline
int			FI_GetIntTypeNumber( const int *pI )
{
	int i=0;
	while( pI[i] != 0 )
		++i;
	return i;
}

inline
int			FI_GetStringTypeNumber( const char *pString[])
{
	int i=0;
	while( strlen(pString[i]) != 0 )
		++i;
	return i;
}

#ifdef IRR_STR_CONVERT_SUPPORT

template<  >
inline
std::string FI_ToString( const irr::core::vector3df &value )
{
	return FI_Pos2String( value);
}

template<>
inline
std::string FI_ToString( const irr::core::aabbox3df &value )
{
	std::vector<float> fArray;fArray.reserve(6);
	fArray.push_back( value.MinEdge.x );
	fArray.push_back( value.MinEdge.y );
	fArray.push_back( value.MinEdge.z );
	fArray.push_back( value.MaxEdge.x );
	fArray.push_back( value.MaxEdge.y );
	fArray.push_back( value.MaxEdge.z );
	return FI_FloatArray2String( fArray );
}

inline
irr::core::vector3df FI_String2Pos( const char *sPos )
{
	irr::core::vector3df vPos;
	vPos.x = atof( sPos );
	sPos = FI_Tokien(sPos,',');
	if( sPos )
	{
		vPos.y = atof( sPos );
		sPos = FI_Tokien(sPos, ',');
		if( sPos )
			vPos.z = atof( sPos );
	}
	return vPos;
}

inline
std::string FI_Pos2String( const irr::core::vector3df &vPos )
{
	char tmpBuf[256];
	sprintf( tmpBuf, "%.4f,%.4f,%.4f", vPos.x, vPos.y, vPos.z );
	return tmpBuf;
}

template<>
inline
irr::core::vector3df FI_FromString( const char *value )
{
	return FI_String2Pos(value);
}

template<>
inline
irr::core::aabbox3df FI_FromString( const char *value )
{
	std::vector<float> fArray;
	fArray.reserve(6);
	FI_String2FloatArray( value, fArray );
	irr::core::aabbox3df vBox;
	if(fArray.size() > 5 )
	{
		vBox.MinEdge.x=fArray[0];
		vBox.MinEdge.y=fArray[1];
		vBox.MinEdge.z=fArray[2];
		vBox.MaxEdge.x=fArray[3];
		vBox.MaxEdge.y=fArray[4];
		vBox.MaxEdge.z=fArray[5];
	}
	return vBox;
}

#endif


#pragma warning( default : 4244 4018 4267 )

#endif



