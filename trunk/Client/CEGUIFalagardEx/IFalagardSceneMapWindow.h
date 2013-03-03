#pragma once

namespace CEGUI
{
	class IFalagardSceneMapWindow
	{
	public:
		virtual const Point GetMouseScenePos() const = 0;
		virtual void setMyselfDir( const int nDirect ) = 0;
		virtual void setMyselfPos( const Point& pos ) = 0;

		virtual void addTeammatePos( const Point& pos, const String32& name ) = 0;
		virtual void Clear() = 0;

		virtual void SetSceneSize( Size size ) = 0;
		virtual void SetSceneFileName( const String& str ,const String& strPostFix) = 0;
		virtual void SetZoomMode( int nMode ) = 0;

		virtual void setMinimapData( int nType, float fX, float fY, String32& szName  ) = 0;

		virtual void setSceneMapCenter() = 0;

		virtual void moveMapByDirection(int direction, int step) = 0;		//direction 上下左右,分别是1234

		
	};
};