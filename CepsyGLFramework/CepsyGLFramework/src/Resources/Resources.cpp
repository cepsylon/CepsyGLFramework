#include "Resources.h"

#include "Importers/FBXImporter.h"
#include "Graphics/Model.h"
#include "Graphics/Program.h"
#include "Application/Application.h"

void Resources::initialize()
{
	FBXImporter importer;
	importer.load("./data/meshes/xbot.fbx");
	importer.load("./data/animations/idle_foot.fbx");
	importer.load("./data/animations/idle_look_around.fbx");

	// Basic
	std::vector<Shader> shaders(2);
	shaders[0] = Shader{ "./data/shaders/basic.v" };
	shaders[1] = Shader{ "./data/shaders/basic.f" };
	create<Program>("basic", shaders);

	// Skinning program
	shaders[0] = Shader{ "./data/shaders/skinning.v" };
	shaders[1] = Shader{ "./data/shaders/basic.f" };
	create<Program>("skinning", shaders);

	// To back buffer
	shaders[0] = Shader{ "./data/shaders/to_back.v" };
	shaders[1] = Shader{ "./data/shaders/to_back.f" };
	create<Program>("to_back", shaders);
}

void Resources::shutdown()
{
	mResources.clear();
}
