#pragma once

#include "Buffer.h"
#include "Program.h"

#include "myGlew.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

class GraphicsDebug
{
public:
	class Debug
	{
	public:
		static GLenum get_error();
		static void print_error();

		void initialize();
		void draw_line(const glm::vec3 & start, const glm::vec3 & end, const glm::vec4 & color);

		void render();

	private:
		struct DebugLine
		{
			glm::vec4 mColor;
			glm::vec3 mStart;
			glm::vec3 mEnd;
		};
		std::vector<DebugLine> mLines;
		BufferF32 mLineBuffer{ GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT };
		Program mLineProgram;
	};

	Debug & debug();
	const Debug & debug() const;

private:
	Debug mDebugData;
};