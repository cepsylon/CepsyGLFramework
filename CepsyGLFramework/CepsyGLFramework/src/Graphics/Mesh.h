#pragma once

#include "Buffer.h"

#include <string>

class Mesh
{
public:
	Mesh(BufferF32 && vertices, BufferI32 && indices);
	~Mesh();

	// No copy
	Mesh(const Mesh &) = delete;
	Mesh & operator=(const Mesh &) = delete;

	// Only move
	Mesh(Mesh && rhs);
	Mesh & operator=(Mesh && rhs);

private:
	BufferF32 mVertices;
	BufferI32 mIndices;
};