#include "GraphicsDebug.h"

#include <iostream>

GLenum GraphicsDebug::Debug::get_error()
{
	return glGetError();
}

void GraphicsDebug::Debug::print_error()
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

void GraphicsDebug::Debug::initialize()
{
	// Create debug buffer
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	mLineBuffer.generate(&mVAO, sizeof(GLuint));
	glEnableVertexAttribArray(0);
	const GLuint stride = sizeof(glm::vec3) + sizeof(glm::vec4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(sizeof(glm::vec3)));
	glBindVertexArray(0);

	std::vector<Shader> shaders(2);
	shaders[0] = Shader{ "./data/shaders/line.v" };
	shaders[1] = Shader{ "./data/shaders/line.f" };
	mLineProgram = Program{ shaders };
}

void GraphicsDebug::Debug::draw_line(const glm::vec3 & start, const glm::vec3 & end, const glm::vec4 & color)
{
	mLines.emplace_back(DebugLine{ start, color, end, color });
}

void GraphicsDebug::Debug::update()
{
	mLineCount = mLines.size();
	if (mLineCount)
	{
		glBindVertexArray(mVAO);
		mLineBuffer.update(mLines.data(), mLineCount * sizeof(DebugLine));
		mLines.clear();
	}
}

void GraphicsDebug::Debug::render() const
{
	if (mLineCount)
	{
		mLineProgram.bind();
		glBindVertexArray(mVAO);
		glDrawArrays(GL_LINES, 0, mLineCount * 2);
	}
}

GraphicsDebug::Debug & GraphicsDebug::debug() { return mDebugData; }
const GraphicsDebug::Debug & GraphicsDebug::debug() const { return mDebugData; }
