#pragma once

#include "myGlew.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Viewport
{
public:
	struct ClearMask {
		enum { NONE = 0, COLOR = GL_COLOR_BUFFER_BIT, DEPTH = GL_DEPTH_BUFFER_BIT, STENCIL = GL_STENCIL_BUFFER_BIT };
	};

	// Sets the viewport and clears if needed
	void set() const;

	glm::vec4 mClearColor{ 1.0f, 1.0f, 1.0f, 1.0f };	// Clear color
	glm::vec2 mSize{ 1.0f, 1.0f };	// Size of the viewport normalized
	glm::vec2 mPosition{ 0.0f, 0.0f }; // Position of the viewport in normalized coordinates
	int mClearMask = ClearMask::COLOR | ClearMask::DEPTH;	// Clear viewport before rendering
};
