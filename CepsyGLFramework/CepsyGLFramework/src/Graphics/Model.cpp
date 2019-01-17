#include "Model.h"

Model::Model(std::vector<Mesh> && meshes)
	: mMeshes(std::move(meshes))
{ }

Model::Model(Model && rhs)
	: Base(std::move(rhs))
	, mMeshes(std::move(rhs.mMeshes))
{ }

Model & Model::operator=(Model && rhs)
{
	if (this != &rhs)
	{
		Base::operator=(std::move(rhs));
		mMeshes = std::move(rhs.mMeshes);
	}

	return *this;
}

void Model::draw() const
{
	// Draw all meshes
	// TODO: add material binding for meshes
	for (const auto & mesh : mMeshes)
	{
		mesh.bind();
		mesh.draw();
	}
}
