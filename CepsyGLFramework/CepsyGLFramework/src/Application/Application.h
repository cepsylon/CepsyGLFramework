#pragma once

#include "Window/Window.h"

class Application
{
public:
	void initialize(HINSTANCE__ * instance, int show);
	void update();
	void shutdown();

	void set_is(bool is);

	const Window & window() const;
	Window & window();

private:
	Window mWindow;
	bool mRunning = false;
};

extern Application application;
