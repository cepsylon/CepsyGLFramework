#pragma once

#include "Buffer.h"
#include "Mesh.h"
#include "Framebuffer.h"
#include "GraphicsDebug.h"
#include "Resources/ResourceHandle.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <unordered_map>

struct HGLRC__;
class Camera;
class Program;
class Model;
class Renderable;

class Graphics : public GraphicsDebug
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

	// Sets draw dimension
	void set_dimension(const glm::ivec2 & dimension);
	// Gets draw dimension
	const glm::ivec2 & dimension() const;

	// Tells if the dimension of the render texture is the same size as the window
	bool is_dimension_client_size() const;

	BufferF32 & skeleton_buffer();

private:
	std::vector<Camera *> mCameras;
	std::vector<Renderable *> mUnableToDrawRenderables;
	std::unordered_map<const Program *, std::unordered_map<const Model *, std::vector<Renderable *>>> mRenderables;
	Framebuffer mFramebuffer;
	BufferF32 mCameraMatrixBuffer{ GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT };
	BufferF32 mSkeletonBuffer{ GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT };
	HGLRC__ * mGLContext = nullptr;
	mutable ResourceHandle<Program> mPostProcess;
	std::unique_ptr<Mesh> mPlane = nullptr;
	glm::ivec2 mDimension{ 1280, 720 };
	bool mDimensionIsClientSize = true;
};
