#pragma once

#include <glm/vec2.hpp>

// Windows structs
struct HWND__;
struct HINSTANCE__;

class Window
{
private:
#ifdef _WIN64
	using winproc = __int64 (__stdcall *)(HWND__ *, unsigned, unsigned __int64, __int64);
#else
	using winproc = long (__stdcall *)(HWND__ *, unsigned, unsigned, long);
#endif

public:
	Window(const char * name = "Framework");

	// Creates the window
	void initialize(HINSTANCE__ * instance, int show, winproc message_handler);

	// Handle window messages
	void update();

	// Release the window
	void shutdown();

	// Width of the window
	int width() const;
	// Height of the window
	int height() const;
	// Handle of the window
	HWND__ * handle() const;

private:
	// Gets the size of the window from the client size
	void compute_window_size_with_client_size();
	// Computes the centered position of the window
	void compute_initial_position();
	// Registers window class
	bool register_class() const;
	// Creates the window
	bool create(HINSTANCE__ * instance, winproc message_handler);

	glm::ivec2 mClientSize{ 1280, 720 };
	glm::ivec2 mSize;
	glm::ivec2 mPosition{ 0, 0 };
	HINSTANCE__ * mInstance = nullptr;
	HWND__ * mHandle = nullptr;
	winproc mMessageHandler = nullptr;
	const char * mName = "Framework";
	long int mWindowStyle;
};
