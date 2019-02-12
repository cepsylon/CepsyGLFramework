#pragma once

#include "Common/Base.h"
#include "Mesh.h"
#include "Material.h"

#include <vector>

class Program;

// Collection of meshes and materials
class Model : public Base
{
public:
	RTTI_H;

	Model(std::vector<Mesh> && meshes, std::vector<Material> && materials);

	// No copies
	Model(const Model &) = delete;
	Model & operator=(const Model &) = delete;

	// Only moves
	Model(Model && rhs);
	Model & operator=(Model && rhs);

	// To gui
	// TODO: may need to move this elsewhere
	virtual void to_gui();

	// Draws model
	virtual void draw(const Program * program) const;
protected:
	std::vector<Mesh> mMeshes;
	std::vector<Material> mMaterials;
};
