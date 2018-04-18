#include <platform/d3d11/system/platform_d3d11.h>
#include "Application/DoodleApp.h"
#include "Common/Common.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// initialisation
	gef::PlatformD3D11 platform(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, false, true);

	DoodleApp myApp(platform);
	myApp.Run();    

	return 0;
}