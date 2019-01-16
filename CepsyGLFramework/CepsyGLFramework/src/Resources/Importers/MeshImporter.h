#pragma once

#include <fbxsdk/fbxsdk.h>

#include <vector>

class Mesh;

struct Vertex
{
	struct Instance
	{
		bool operator==(const Instance & rhs) const;

		std::vector<int *> mIndices; // Indices that reference the current instance of the Vertex
		int mNormalIndex = -1;
		int mUVIndex = -1;
	};

	int find(const Instance & rhs) const;

	std::vector<Instance> mInstances; // Duplicates that will be needed in the same vertex position
};

class MeshImporter
{
public:
	static Mesh load(fbxsdk::FbxMesh * mesh);

private:
	static Vertex::Instance fill_instance(FbxMesh * mesh, int polygon, int polygon_vertex, int vertex_count);
};
