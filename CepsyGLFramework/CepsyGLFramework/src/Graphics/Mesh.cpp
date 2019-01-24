#include "Mesh.h"

Mesh::Mesh(BufferF32 && vertices, BufferI32 && indices, const Layout & layout)
	: mVertices(std::move(vertices))
	, mIndices(std::move(indices))
{
	// Create vao
	glGenVertexArrays(1, &mID);
	glBindVertexArray(mID);
	
	// Attach buffers to vao
	mVertices.bind();
	
	// Set input assembler layout
	for (unsigned i = 0; i < layout.mIndexOffset.size(); ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, layout.mIndexOffset[i].mSize, mVertices.type(), GL_FALSE, layout.mStride, (void*)(layout.mIndexOffset[i].mOffset));
	}

	mIndices.bind();
}

Mesh::~Mesh()
{
	if (mID)
		glDeleteVertexArrays(1, &mID);
}

Mesh::Mesh(Mesh && rhs)
	: mVertices(std::move(rhs.mVertices))
	, mIndices(std::move(rhs.mIndices))
	, mID(rhs.mID)
{
	rhs.mID = 0;
}

Mesh & Mesh::operator=(Mesh && rhs)
{
	if (this != &rhs)
	{
		mVertices.clear();
		mVertices = std::move(rhs.mVertices);
		mIndices.clear();
		mIndices = std::move(rhs.mIndices);
		mID = rhs.mID; rhs.mID = 0;
	}

	return *this;
}

void Mesh::bind() const
{
	glBindVertexArray(mID);
}

void Mesh::draw() const
{
	glDrawElements(GL_TRIANGLES, mIndices.count(), GL_UNSIGNED_INT, 0);
}
