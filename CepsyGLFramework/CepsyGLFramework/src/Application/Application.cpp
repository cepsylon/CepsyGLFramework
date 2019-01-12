#include "Application.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


Application application;

namespace
{
	LRESULT CALLBACK message_handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			application.set_is(false);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}
}

void Application::initialize(HINSTANCE__ * instance, int show)
{
	mRunning = true;
	mWindow.initialize(instance, show, &message_handler);
	mGraphics.initialize();
	mScene.initialize();
}

void Application::run()
{
	while (mRunning)
	{
		// Update everything
		mWindow.update();
		mGraphics.update();
		mScene.update();

		// Render
		mGraphics.render();
	}
}

void Application::shutdown()
{
	mScene.shutdown();
	mGraphics.shutdown();
	mWindow.shutdown();
}

void Application::set_is(bool is) { mRunning = is; }

const Window & Application::window() const { return mWindow; }
Window & Application::window() { return mWindow; }
const Graphics & Application::graphics() const { return mGraphics; }
Graphics & Application::graphics() { return mGraphics; }
const Scene & Application::scene() const { return mScene; }
Scene & Application::scene() { return mScene; }
