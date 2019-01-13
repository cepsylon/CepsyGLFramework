#pragma once

#include "Window/Window.h"
#include "Graphics/Graphics.h"
#include "GUI/GUI.h"
#include "Scene/Scene.h"

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

	// Scene gettors
	const Scene & scene() const;
	Scene & scene();

private:
	Window mWindow;
	Graphics mGraphics;
	GUI mGUI;
	Scene mScene;
	bool mRunning = false;
};

extern Application application;
