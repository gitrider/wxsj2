#ifndef __PathLibPrerequisites_H__
#define __PathLibPrerequisites_H__

namespace PathLib
{
#	ifdef PATHLIB_EXPORTS
#		define _PathLibExport __declspec(dllexport)
#	else
#    	define _PathLibExport __declspec(dllimport)
#	endif
}

#endif