#pragma once

#include "Window/Window.h"

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

private:
	Window mWindow;
	bool mRunning = false;
};

extern Application application;
