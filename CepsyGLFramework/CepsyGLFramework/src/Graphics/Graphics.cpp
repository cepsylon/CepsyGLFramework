#include "Graphics.h"

#include "Camera.h"
#include "Program.h"
#include "Model.h"
#include "Renderable.h"
#include "Application/Application.h"
#include "Common/Entity.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

GLenum Graphics::Debug::get_error()
{
	return glGetError();
}

void Graphics::Debug::print_error()
{
	GLenum error;
	do
	{
		error = get_error();
		switch (error)
		{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("Error: %s\n", "An unacceptable value is specified for an enumerated argument.");
			break;
		case GL_INVALID_VALUE:
			printf("Error: %s\n", "A numeric argument is out of range.");
			break;
		case GL_INVALID_OPERATION:
			printf("Error: %s\n", "The specified operation is not allowed in the current state.");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf("Error: %s\n", "The framebuffer object is not complete.");
			break;
		case GL_OUT_OF_MEMORY:
			printf("Error: %s\n", "There is not enough memory left to execute the command.");
			break;
		case GL_STACK_UNDERFLOW:
			printf("Error: %s\n", "An attempt has been made to perform an operation that would cause an internal stack to underflow.");
			break;
		case GL_STACK_OVERFLOW:
			printf("Error: %s\n", "An attempt has been made to perform an operation that would cause an internal stack to overflow.");
			break;
		}
	} while (error != GL_NO_ERROR);
}


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
		0,                   // Number of bits for the depthbuffer
		0,                    // Number of bits for the stencilbuffer
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

	// Create camera matrix buffer and bind it
	// NOTE: we need to fill the buffer with dummy matrices before binding
	// to use debug utility since the buffer is not actually created until
	// we pass data to it. We use dummy data, will be overwritten later
	mCameraMatrixBuffer.generate(&mCameraMatrixBuffer, sizeof(float));
	mCameraMatrixBuffer.bind_base(0);

	// Create framebuffer
	mFramebuffer.attach_layer(GL_TEXTURE_2D, GL_RGBA, application.window().width(), application.window().height());
	mFramebuffer.attach_layer(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, application.window().width(), application.window().height());
	std::vector<Framebuffer::Attachment> attachments(2);
	attachments[0].mAttachmentType = Framebuffer::Attachment::Type::COLOR;
	attachments[1].mAttachmentType = Framebuffer::Attachment::Type::DEPTH;
	mFramebuffer.generate(attachments);

	// Create plane
	float vertices[] = {
		-1.0f, 1.0f, 0.0f, 1.0f,	// V0
		-1.0f, -1.0f, 0.0f, 0.0f,	// V1
		1.0f, -1.0f, 1.0f, 0.0f,	// V2
		1.0f, 1.0f, 1.0f, 1.0f		// V3
	};
	BufferF32 vertex_buffer{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, GL_FLOAT };
	vertex_buffer.generate(vertices, sizeof(vertices));

	int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	BufferI32 index_buffer{ GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, GL_UNSIGNED_INT };
	index_buffer.generate(indices, sizeof(indices));
	unsigned stride = 4 * sizeof(float);
	Mesh::Layout::SizeOffset positions{ 2, 0 };
	Mesh::Layout::SizeOffset uvs{ 2, 2 * sizeof(float) };
	mPlane = std::make_unique<Mesh>(std::move(vertex_buffer), std::move(index_buffer), Mesh::Layout{ stride, { positions, uvs } });

	// Enable depth testing, for now
	glEnable(GL_DEPTH_TEST);
}

void Graphics::update()
{

}

void Graphics::render() const
{
	// Bind own framebuffer
	mFramebuffer.bind();

	// Render for each camera
	for (auto camera : mCameras)
	{
		// Set viewport and pass camera matrices to GPU
		camera->set();

		// Render everything
		for (const auto & program_model : mRenderables)
		{
			// Bind program
			const Program * program = program_model.first;
			program->bind();
			program->set_uniform("light_direction", glm::vec3{ 0.0f, 1.0f, 1.0f });

			for (const auto & model_renderable : program_model.second)
			{
				const Model * model = model_renderable.first;
				// Should bing the model here

				// Draw all renderables
				for (const auto & renderable : model_renderable.second)
				{
					program->set_uniform("model", renderable->owner().transform().model());
					model->draw(program);
				}
			}
		}
	}

	// Post process
	Framebuffer::bind_back();

	// For now, render framebuffer to back buffer
	if (mPostProcess == nullptr)
		mPostProcess = application.resources().get<Program>("to_back");

	glDisable(GL_DEPTH_TEST);

	mPostProcess->bind();
	Texture::slot(0);
	mFramebuffer.layer(0)->bind();
	mPlane->bind();
	mPlane->draw();

	glEnable(GL_DEPTH_TEST);
}

void Graphics::shutdown()
{
	wglDeleteContext(mGLContext);
}

void Graphics::present() const
{
	// Swap buffers
	SwapBuffers(application.window().render_device());
}

void Graphics::update_camera_matrices(const glm::mat4 & projection, const glm::mat4 & view)
{
	glm::mat4 matrices[2] = { projection, view };

	// Update buffer
	mCameraMatrixBuffer.bind();
	mCameraMatrixBuffer.update(matrices, sizeof(matrices));
}

void Graphics::add(Camera * camera)
{
	mCameras.emplace_back(camera);
}

void Graphics::add(Renderable * renderable)
{
	if (renderable->program() == nullptr || renderable->model() == nullptr)
		mUnableToDrawRenderables.emplace_back(renderable);
	else
		mRenderables[renderable->program()][renderable->model()].emplace_back(renderable);
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

void Graphics::remove(Renderable * renderable)
{
	std::vector<Renderable *> & renderables = renderable->program() == nullptr || renderable->model() == nullptr ? mUnableToDrawRenderables : mRenderables[renderable->program()][renderable->model()];
	for (unsigned i = 0; i < renderables.size(); ++i)
	{
		if (renderables[i] == renderable)
		{
			renderables[i] = renderables.back();
			renderables.pop_back();
			break;
		}
	}
}
