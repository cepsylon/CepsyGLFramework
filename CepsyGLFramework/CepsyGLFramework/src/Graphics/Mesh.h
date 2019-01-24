#pragma once

#include "Buffer.h"

#include <string>
#include <vector>

class Mesh
{
public:
	struct Layout
	{
		struct SizeOffset
		{
			unsigned mSize = 0, mOffset = 0; // Size in element count, offset in bytes
		};
		unsigned mStride = 0; // In bytes
		std::vector<SizeOffset> mIndexOffset;
	};

	Mesh(BufferF32 && vertices, BufferI32 && indices, const Layout & layout);
	~Mesh();

	// No copy
	Mesh(const Mesh &) = delete;
	Mesh & operator=(const Mesh &) = delete;

	// Only move
	Mesh(Mesh && rhs);
	Mesh & operator=(Mesh && rhs);

	// Bind for rendering
	void bind() const;
	// Draw call
	void draw() const;

private:
	BufferF32 mVertices;
	BufferI32 mIndices;
	GLuint mID = 0;
};
