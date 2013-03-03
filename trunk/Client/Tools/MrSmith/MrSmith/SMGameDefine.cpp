#include "StdAfx.h"
#include ".\SMGameDefine.h"
#include "Version.h"

SMGameDefine	g_cSMDefine;

SMGameDefine::SMGameDefine( void )
{
	m_nVersion	= CURRENT_VERSION;

	LoadConfig();
}

SMGameDefine::~SMGameDefine( void )
{
}

VOID SMGameDefine::LoadConfig()
{
	FILE*	pf	= fopen( FIL_SMCONFIG, "r" );
	CHAR	lin[512];
	CHAR*	adr	= NULL;
	if( !pf )
		return;

	while( !feof( pf ) )
	{
		lin[0]	= '\0';
		fgets( lin, 512, pf );

		if( lin[0] == '/' && lin[1] == '/' )
			continue;
		if( lin[0] == '\0' || lin[0] == '\n' )
			continue;

		if( strncmp( lin, "version=", 8 ) == 0 )
		{
			sscanf( lin, "version=%x", &m_nVersion );
			continue;
		}
	}
	fclose( pf );
}
