#pragma once

#include "Window/Window.h"
#include "Graphics/Graphics.h"

class Application
{
public:
	// Initialize all systems needed for the application
	void initialize(HINSTANCE__ * instance, int show);
	// Update the systems
	void run();
	// Shutdown the systems
	void shutdown();

	// Set if the application is running
	void set_is(bool is);

	// Window gettors
	const Window & window() const;
	Window & window();

	// Graphics gettors
	const Graphics & graphics() const;
	Graphics & graphics();

private:
	Window mWindow;
	Graphics mGraphics;
	bool mRunning = false;
};

extern Application application;
