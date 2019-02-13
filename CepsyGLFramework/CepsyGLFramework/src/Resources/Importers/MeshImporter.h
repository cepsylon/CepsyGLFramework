#pragma once

#include "Graphics/Skeleton.h"

#include <fbxsdk/fbxsdk.h>

#include <vector>

class Mesh;


class MeshImporter
{
public:
	static Mesh load(fbxsdk::FbxMesh * mesh, const Skeleton & skeleton);

private:
	// Helper structure to filter vertices
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

		std::vector<int> mIndices;
		std::vector<float> mWeights;
		std::vector<Instance> mInstances; // Duplicates that will be needed in the same vertex position
	};

	// Helper vertices to keep track of polygons
	// Used for triangulation after parsing everything
	struct Polygon
	{
		std::vector<int> mIndices;
	};

	static Vertex::Instance fill_instance(FbxMesh * mesh, int polygon, int polygon_vertex, int vertex_count);
};
