/****************************************\
*										*
* 			SMGameDefine				*
*										*
*						 by Lybin Lee	*
\****************************************/

#pragma	once

#define	FIL_SMCONFIG	"SMConfig.ini"

class	SMGameDefine
{
public :
	SMGameDefine( void );
	~SMGameDefine( void );
	INT		GetVersion()	{ return m_nVersion; }

private :
	VOID	LoadConfig();
	INT		m_nVersion;
};

extern SMGameDefine		g_cSMDefine;
