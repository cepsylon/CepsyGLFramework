#include "Graphics.h"

#include "Application/Application.h"
#include "Camera.h"
#include "Common/Entity.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void Graphics::initialize()
{
	// Description for the pixel format of the framebuffer
	PIXELFORMATDESCRIPTOR pixel_format_descriptor =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	// Choose and set pixel format
	HDC render_device = application.window().render_device();
	int  pixel_format = ChoosePixelFormat(render_device, &pixel_format_descriptor);
	SetPixelFormat(render_device, pixel_format, &pixel_format_descriptor);

	// Create dummy OpenGL context to initialize OpenGL
	HGLRC dummy_gl_context = wglCreateContext(render_device);
	wglMakeCurrent(render_device, dummy_gl_context);

	// Initialze glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
		throw "OpenGL is not supported with the current device";

	// Version for OpenGL
	int attributes[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	// Create actual OpenGL context
	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		mGLContext = wglCreateContextAttribsARB(render_device, 0, attributes);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(dummy_gl_context);
		wglMakeCurrent(render_device, mGLContext);
	}
	else
		throw "Unsupported OpenGL verion 4.5, please update your drivers";
}

void Graphics::update()
{

}

void Graphics::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render for each camera
	for (auto camera : mCameras)
	{
		// Set viewport and pass camera matrices to GPU
		camera->set();
	}

	SwapBuffers(application.window().render_device());
}

void Graphics::shutdown()
{
	wglDeleteContext(mGLContext);
}

void Graphics::update_camera_matrices(const glm::mat4 & projection, const glm::mat4 & view)
{
	// TODO: update buffer
}

void Graphics::add(Camera * camera)
{
	mCameras.emplace_back(camera);
}

void Graphics::remove(Camera * camera)
{
	for (std::vector<Camera *>::iterator it = mCameras.begin(); it != mCameras.end(); ++it)
	{
		if (*it == camera)
		{
			mCameras.erase(it);
			break;
		}
	}
}
