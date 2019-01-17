#include "Mesh.h"

Mesh::Mesh(BufferF32 && vertices, BufferI32 && indices)
	: mVertices(std::move(vertices))
	, mIndices(std::move(indices))
{
	// Create vao
	glGenVertexArrays(1, &mID);
	glBindVertexArray(mID);
	
	// Attach buffers to vao
	mVertices.bind();
	
	// Set input assembler layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

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
