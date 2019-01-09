#include "Application/Application.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	application.initialize(hInstance, nCmdShow);
	application.update();
	application.shutdown();
	
	return 0;
}