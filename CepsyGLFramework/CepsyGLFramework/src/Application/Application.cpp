#include "Application.h"

#include <imgui/imgui.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

Application application;

namespace
{
	LRESULT CALLBACK message_handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();
		switch (message)
		{
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		{
			int button = 0;
			if (message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) button = 0;
			if (message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK) button = 1;
			if (message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK) button = 2;
			if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
				::SetCapture(hWnd);
			io.MouseDown[button] = true;
			return 0;
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			int button = 0;
			if (message == WM_LBUTTONUP) button = 0;
			if (message == WM_RBUTTONUP) button = 1;
			if (message == WM_MBUTTONUP) button = 2;
			io.MouseDown[button] = false;
			if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hWnd)
				::ReleaseCapture();
			return 0;
		}
		case WM_MOUSEWHEEL:
			io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
			return 0;
		case WM_MOUSEHWHEEL:
			io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
			return 0;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (wParam < 256)
				io.KeysDown[wParam] = 1;
			return 0;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (wParam < 256)
				io.KeysDown[wParam] = 0;
			return 0;
		case WM_CHAR:
			// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
			if (wParam > 0 && wParam < 0x10000)
				io.AddInputCharacter((unsigned short)wParam);
			return 0;
		case WM_DESTROY:
			application.set_is(false);
			PostQuitMessage(0);
			return 0;
			// Window resizing
		case WM_SIZE:
			application.window().set_client_size(LOWORD(lParam), HIWORD(lParam));
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}

void Application::initialize(HINSTANCE__ * instance, int show)
{
	mRunning = true;
	mWindow.initialize(instance, show, &message_handler);
	mGraphics.initialize();
	mResources.initialize();
	mGUI.initialize();
	mScene.initialize();
}

void Application::run()
{
	while (mRunning)
	{
		// Update everything
		mWindow.update();
		mGraphics.update();
		mGUI.update();
		mScene.update();

		// Here goes gui
		mScene.to_gui();

		// Render
		mGraphics.render();
		mGUI.render();
		mGraphics.present();
	}
}

void Application::shutdown()
{
	mScene.shutdown();
	mGUI.shutdown();
	mResources.shutdown();
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
const Resources & Application::resources() const { return mResources; }
Resources & Application::resources() { return mResources; }
