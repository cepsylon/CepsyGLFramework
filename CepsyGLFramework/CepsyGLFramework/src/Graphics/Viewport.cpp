#include "Viewport.h"

#include "Application/Application.h"

void Viewport::set() const
{
	// Compute size and position in window space
	const Window & window = application.window();
	glm::ivec2 size{ mSize.x * window.width(), mSize.y * window.height() };
	glm::ivec2 position{ mPosition.x * window.width(), mPosition.y * window.height() };

	// Set viewport and scissor
	glViewport(position.x, position.y, size.x, size.y);
	glScissor(position.x, position.y, size.x, size.y);

	// Set clear color if needed
	if(mClearMask & ClearMask::COLOR)
		glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);

	// Clear if needed
	if (mClearMask)
		glClear(mClearMask);
}

float Viewport::ratio() const
{
	const Window & window = application.window();
	glm::vec2 size{ mSize.x * window.width(), mSize.y * window.height() };
	return size.x / size.y;
}
