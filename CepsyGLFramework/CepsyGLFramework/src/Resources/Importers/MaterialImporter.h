#pragma once

#include <fbxsdk/fbxsdk.h>

class Material;

class MaterialImporter
{
public:
	static Material load(FbxSurfaceMaterial * material);
};
