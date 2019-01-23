#pragma once

#include "Buffer.h"

#include "myGlew.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include <unordered_map>

struct HGLRC__;
class Camera;
class Program;
class Model;
class Renderable;

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
	// Adds renderable
	void add(Renderable * renderable);

	// Removes a camera to render with
	void remove(Camera * camera);
	// Remove renderable
	void remove(Renderable * renderable);

private:
	std::vector<Camera *> mCameras;
	std::vector<Renderable *> mUnableToDrawRenderables;
	std::unordered_map<Program *, std::unordered_map<Model *, std::vector<Renderable *>>> mRenderables;
	BufferF32 mCameraMatrixBuffer{ GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT };
	HGLRC__ * mGLContext = nullptr;
};
