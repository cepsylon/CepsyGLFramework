#include "Model.h"

RTTI_I(Model, Base);

Model::Model(std::vector<Mesh> && meshes, std::vector<Material> && materials)
	: mMeshes(std::move(meshes))
	, mMaterials(std::move(materials))
{ }

Model::Model(Model && rhs)
	: Base(std::move(rhs))
	, mMeshes(std::move(rhs.mMeshes))
	, mMaterials(std::move(rhs.mMaterials))
{ }

Model & Model::operator=(Model && rhs)
{
	if (this != &rhs)
	{
		Base::operator=(std::move(rhs));
		mMeshes = std::move(rhs.mMeshes);
		mMaterials = std::move(rhs.mMaterials);
	}

	return *this;
}

void Model::draw(const Program * program) const
{
	// Draw all meshes
	for (unsigned i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].bind();
		mMaterials[i].bind(program);
		mMeshes[i].draw();
	}
}
