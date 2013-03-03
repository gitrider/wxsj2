#pragma once

namespace CEGUI
{
	class IFalagardWorldMap
	{
	public:
		virtual void setCityPos( int nCityID, const Point& pos ) = 0;
		virtual void setCityNamePos( int nCityID, const Point& pos ) = 0;
		virtual void setCityType( int nCityID,const String& szType ) = 0;
		virtual void setCityCurrentScene( int nCityID, int bCurrentScene ) = 0;
		virtual void setCityNameNormalImage( int nCityID, const String& szImageset, const String& szImage ) = 0;
		virtual void setCityNameHoverImage( int nCityID, const String& szImageset, const String& szImage ) = 0;
		
		/********** 标记(_WX@7)，添加一些设置函数 *******************************/
		virtual void setCityTooltipText( int nCityID, const String32& szCityTip ) = 0;
		virtual void setCityNameTooltipText( int nCityID, const String32& szCityNameTip ) = 0;

		virtual void SetCityHover( int nSceneID, bool bHover ) = 0;
		/************************************************************************/

		virtual void resetWorldMap() = 0;
		virtual void addPlayerToCity( int nCityID, const String& szPlayerName ) = 0;
		virtual void clearPlayerInfo() = 0;
		virtual int	 getCurrentSelectScene() = 0;
		virtual void createBackground() = 0;
	};
};