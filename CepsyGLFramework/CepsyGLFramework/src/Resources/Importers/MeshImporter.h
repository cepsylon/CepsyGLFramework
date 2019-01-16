#pragma once

#include <fbxsdk/fbxsdk.h>

class Mesh;

class MeshImporter
{
public:
	static Mesh load(fbxsdk::FbxMesh * mesh);
};
