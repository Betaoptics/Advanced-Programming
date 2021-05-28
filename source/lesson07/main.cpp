/**
 * ============================================================================
 *  Name        : main.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : entry point for a superduper game app
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/


#include "TheApp.h"

#if defined (_WINDOWS)
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow)
#endif
#if defined (_LINUX)
int main(int argc, char** argv)
#endif
{
	// create the app instance
	CTheApp* pApp = new CTheApp;
	if (!pApp->Create(1280, 720, "SuperGame"))
	{
		// something went wrong in initialization, exit
		IApplication::Debug("APP START FAILED\n");
		return 0;
	}

	// run and delete when done
	pApp->Run();
	delete pApp;
	return 0;
}

