
/** µ÷ÊÔÆ÷½Ó¿Ú
 */

#pragma once


#include "GINode.h"


class KYLIN_API tDebuger : public tNode
{
public:

	virtual void	InfoTrack_Reset(INT nWidth, INT nHeight) = 0;
	virtual void	InfoTrack_AddPoint(INT nX, INT nY) = 0;

protected:

	KL_DECLARE_DYNAMIC(tDebuger);

};
