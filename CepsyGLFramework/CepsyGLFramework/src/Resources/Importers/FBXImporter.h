#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

#include <vector>
#include <string>

class Skeleton;

namespace fbxsdk
{
	class FbxNode;
	class FbxNodeAttribute;
}

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
	void import(fbxsdk::FbxNode * node);
	void import(fbxsdk::FbxNodeAttribute * attribute);
	void import_mesh(fbxsdk::FbxNodeAttribute * attribute);

	std::vector<Mesh> mMeshes;
	std::vector<Material> mMaterials;
	std::string mName;
	Skeleton * mSkeleton = nullptr;
};
