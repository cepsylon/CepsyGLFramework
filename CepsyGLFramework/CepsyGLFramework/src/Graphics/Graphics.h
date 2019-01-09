#pragma once

#include "myGlew.h"

struct HGLRC__;

class Graphics
{
public:
	void initialize();
	void update();
	void render();
	void shutdown();

private:
	HGLRC__ * mGLContext = nullptr;
};
