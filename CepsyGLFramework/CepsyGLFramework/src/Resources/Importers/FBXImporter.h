#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

#include <fbxsdk/fbxsdk.h>

#include <vector>
#include <string>

class Skeleton;

class Importer
{
public:
	virtual void load(const std::string & path) = 0;

protected:
};

class FBXImporter : public Importer
{
public:
	void load(const std::string & path) override;

private:
	void import(FbxNode * node);
	void import(FbxNodeAttribute * attribute);
	void import_mesh(FbxNodeAttribute * attribute);

	std::vector<Mesh> mMeshes;
	std::vector<Material> mMaterials;
	std::string mName;
	Skeleton * mSkeleton = nullptr;
};
