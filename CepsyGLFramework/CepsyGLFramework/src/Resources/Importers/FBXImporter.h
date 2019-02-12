#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Skeleton.h"

#include <fbxsdk/fbxsdk.h>

#include <vector>
#include <string>

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
	void import_skeleton(FbxSkeleton * skeleton);

	static void import_bones_rec(FbxSkeleton * skeleton_node, int parent_index, std::vector<Skeleton::Bone> & bones);

#ifdef _DEBUG
	static void print_scene(FbxScene * scene);
	static void print_node(FbxNode * node, int tab_count);
	static void print_attribute(FbxNodeAttribute * attribute, int tab_count);
	static std::string attribute_type(FbxNodeAttribute::EType type);
	static void print_tabs(int count);
#endif

	std::vector<Mesh> mMeshes;
	std::vector<Material> mMaterials;
};
