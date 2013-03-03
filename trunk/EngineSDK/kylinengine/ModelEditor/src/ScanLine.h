#ifndef __SCANLINE_H
#define __SCANLINE_H

#include <OgreVector2.h>
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class ScanLine
	{
	public:
		ScanLine(void);
		~ScanLine(void);

		void setStartPoint(const Point& pt);
		void setEndPoint(const Point& pt);

		const Point& getStartPoint();
		const Point& getEndPoint();

	private:
		Point mStartPt;
		Point mEndPt;
	};

}

#endif