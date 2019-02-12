#include "Resources.h"

#include "Importers/FBXImporter.h"
#include "Graphics/Model.h"
#include "Graphics/Program.h"
#include "Application/Application.h"

void Resources::initialize()
{
	FBXImporter importer;
	importer.load("./data/meshes/xbot.fbx");
	importer.load("./data/meshes/box.fbx");

	// Basic
	std::vector<Shader> shaders(2);
	shaders[0] = Shader{ "./data/shaders/basic.v" };
	shaders[1] = Shader{ "./data/shaders/basic.f" };
	create<Program>("basic", shaders);

	// To back buffer
	shaders[0] = Shader{ "./data/shaders/to_back.v" };
	shaders[1] = Shader{ "./data/shaders/to_back.f" };
	create<Program>("to_back", shaders);
}

void Resources::shutdown()
{
	mResources.clear();
}
