#include "Window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>

Window::Window(const char * name)
	: mName(name)
	, mWindowStyle(WS_OVERLAPPEDWINDOW)
{ }

void Window::initialize(HINSTANCE__ * instance, int show, winproc message_handler)
{
	if (::AllocConsole())
	{
		FILE * fp;

		// Redirect the stdout to the console
		freopen_s(&fp, "CONOUT$", "w", stdout);

		// Change the console title
		SetConsoleTitle("Framework console");
	}

	if (create(instance, message_handler))
	{
		ShowWindow(mHandle, show);
		UpdateWindow(mHandle);
	}
}

void Window::update()
{
	MSG message;
	// Check for messages until there is none
	while (PeekMessage(&message, mHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void Window::shutdown()
{
	// Unregister class
	UnregisterClass(mName, mInstance);
}

int Window::width() const { return mClientSize.x; }
int Window::height() const { return mClientSize.y; }
HWND Window::handle() const { return mHandle; }

void Window::compute_window_size_with_client_size()
{
	RECT rect{ 0, 0, mClientSize.x, mClientSize.y };
	if (AdjustWindowRect(&rect, mWindowStyle, false))
		mSize = glm::uvec2{ rect.right - rect.left, rect.bottom - rect.top };
}

void Window::compute_initial_position()
{
	HWND desktop_handle = GetDesktopWindow();
	RECT desktop_rect;
	GetWindowRect(desktop_handle, &desktop_rect);
	mPosition = glm::ivec2{ desktop_rect.right - mSize.x, desktop_rect.bottom - mSize.y } / 2;
}

bool Window::register_class() const
{
	WNDCLASSEX window_class_extended;
	window_class_extended.cbSize = sizeof(WNDCLASSEX);
	window_class_extended.style = CS_HREDRAW | CS_VREDRAW;
	window_class_extended.lpfnWndProc = mMessageHandler; // Window procedure
	window_class_extended.cbClsExtra = 0;
	window_class_extended.cbWndExtra = 0;
	window_class_extended.hInstance = mInstance;
	window_class_extended.hIcon = nullptr;
	window_class_extended.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class_extended.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window_class_extended.lpszMenuName = nullptr;
	window_class_extended.lpszClassName = mName;
	window_class_extended.hIconSm = nullptr;

	return static_cast<bool>(RegisterClassEx(&window_class_extended));
}

bool Window::create(HINSTANCE__ * instance, winproc message_handler)
{
	mMessageHandler = message_handler;

	// Register class
	if (register_class())
	{
		mInstance = instance;

		compute_window_size_with_client_size();
		compute_initial_position();

		// Create window
		mHandle = CreateWindow(
			mName,
			mName,
			mWindowStyle,
			mPosition.x, mPosition.y,
			mSize.x, mSize.y,
			NULL,
			NULL,
			mInstance,
			NULL
		);

		return mHandle != nullptr;
	}

	return false;
}
