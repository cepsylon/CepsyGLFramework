#include "Mesh.h"

Mesh::Mesh(BufferF32 && vertices, BufferI32 && indices)
	: mVertices(std::move(vertices))
	, mIndices(std::move(indices))
{

}

Mesh::~Mesh()
{

}

Mesh::Mesh(Mesh && rhs)
	: mVertices(std::move(rhs.mVertices))
	, mIndices(std::move(rhs.mIndices))
{ }

Mesh & Mesh::operator=(Mesh && rhs)
{
	if (this != &rhs)
	{
		mVertices.clear();
		mVertices = std::move(rhs.mVertices);
		mIndices.clear();
		mIndices = std::move(rhs.mIndices);
	}

	return *this;
}
