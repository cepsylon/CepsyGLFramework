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

	void initialize(HINSTANCE__ * instance, int show, winproc message_handler);
	void update();
	void shutdown();

	void set_size(const glm::ivec2 & size);
	void set_client_size(const glm::ivec2 & size);
	void set_position(const glm::ivec2 & position);

	int width() const;
	int height() const;
	HWND__ * handle() const;

private:
	void compute_window_size_with_client_size();
	void compute_initial_position();
	bool register_class() const;
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
