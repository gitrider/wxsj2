/***************************************************

                     Font Util		
				
                           (C) Copyright  Jean. 2007
***************************************************/

#include "StdAfx.h"
#include "FU_MainDialog.h"

CAppModule _AppModule;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int nCmdShow)
{
	// Init WTL app module
	::InitCommonControls();
	_AppModule.Init(NULL, hInstance);

	// Show main dialog
	CMainDialog dlgMain;
	dlgMain.DoModal();

	// Close WTL app module
	_AppModule.Term();
	return 0;
}
