#include "Resources.h"

#include "Importers/FBXImporter.h"
#include "Graphics/Model.h"
#include "Application/Application.h"

void Resources::initialize()
{
	FBXImporter importer;
	importer.load("data/meshes/xbot.fbx");

	int a;
	a = 2;
}

void Resources::shutdown()
{
	mResources.clear();
}
