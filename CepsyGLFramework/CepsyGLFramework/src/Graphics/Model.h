#pragma once

#include "Common/Base.h"
#include "Mesh.h"

#include <vector>

// Collection of meshes and materials
// TODO: materials
class Model : public Base
{
public:
	Model(std::vector<Mesh> && meshes);

	// No copies
	Model(const Model &) = delete;
	Model & operator=(const Model &) = delete;

	// Only moves
	Model(Model && rhs);
	Model & operator=(Model && rhs);

	// Draws model
	void draw() const;
private:
	std::vector<Mesh> mMeshes;
};
