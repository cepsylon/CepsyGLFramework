#include "Viewport.h"

#include "Application/Application.h"

#include <imgui/imgui.h>

void Viewport::set() const
{
	// Compute size and position in window space
	const Graphics & graphics = application.graphics();
	const glm::ivec2 & dimension = graphics.dimension();
	glm::ivec2 size{ mSize.x * dimension.x, mSize.y * dimension.y };
	glm::ivec2 position{ mPosition.x * dimension.x, mPosition.y * dimension.y };

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
	const Graphics & graphics = application.graphics();
	glm::vec2 size{ mSize.x * graphics.dimension().x, mSize.y * graphics.dimension().y };
	return size.x / size.y;
}

void Viewport::to_gui()
{
	ImGui::InputFloat2("Position", &mPosition[0]);
	ImGui::InputFloat2("Size", &mSize[0]);
	ImGui::ColorEdit4("Clear color", &mClearColor[0]);
}
