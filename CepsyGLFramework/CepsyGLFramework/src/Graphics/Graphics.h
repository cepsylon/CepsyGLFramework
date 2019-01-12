#pragma once

#include "Buffer.h"

#include "myGlew.h"

#include <glm/mat4x4.hpp>
#include <vector>

struct HGLRC__;
class Camera;

class Graphics
{
public:
	void initialize();
	void update();
	void render();
	void shutdown();

	// Updates camera matrices
	void update_camera_matrices(const glm::mat4 & projection, const glm::mat4 & view);

	// Adds camera to render with
	void add(Camera * camera);

	// Removes a camera to render with
	void remove(Camera * camera);

private:
	std::vector<Camera *> mCameras;
	BufferF32 mCameraMatrixBuffer{ GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW };
	HGLRC__ * mGLContext = nullptr;
};
