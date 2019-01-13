#pragma once

#include "Graphics/myGlew.h"

class GUI
{
public:
	GUI();
	~GUI();

	// Initialize ImGui
	void initialize();
	// Start new frame
	void update();
	// Render GUI
	void render() const;
	// Shutdown ImGui
	void shutdown();

private:
	__int64 mTime = 0;
	__int64 mTicksPerSecond = 0;
	GLuint mFontTexture = 0;
	GLuint mProgram = 0, mVertexShader = 0, mFragmentShader = 0;
	int mAttribLocationTex = 0, mAttribLocationProjMtx = 0;
	int mAttribLocationPosition = 0, mAttribLocationUV = 0, mAttribLocationColor = 0;
	unsigned int mVBO = 0, mElementsHandle = 0;
};
