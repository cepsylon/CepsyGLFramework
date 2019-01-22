#pragma once

#include "Buffer.h"

#include "myGlew.h"
// TODO: move to resources
#include "Program.h"
#include "Model.h"
#include <memory>
//------------------------------

#include <glm/mat4x4.hpp>
#include <vector>

struct HGLRC__;
class Camera;

class Graphics
{
public:
	void initialize();
	void update();
	void render() const;
	void shutdown();

	void present() const;

	// Updates camera matrices
	void update_camera_matrices(const glm::mat4 & projection, const glm::mat4 & view);

	// Adds camera to render with
	void add(Camera * camera);

	// Removes a camera to render with
	void remove(Camera * camera);

private:
	std::vector<Camera *> mCameras;
	BufferF32 mCameraMatrixBuffer{ GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT };
	HGLRC__ * mGLContext = nullptr;
	std::unique_ptr<Program> mProgram = nullptr;
};
