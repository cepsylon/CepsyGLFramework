#include "Mesh.h"

#include <fbxsdk/fbxsdk.h>

void Mesh::load(const std::string & path)
{
	// Initialize the SDK manager. This object handles memory management.
	FbxManager* fbx_manager = FbxManager::Create(); 
	
	// Create the IO settings object.
	FbxIOSettings *io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(io_settings);

	// Create an importer using the SDK manager.
	FbxImporter* importer = FbxImporter::Create(fbx_manager, "");

	// Use the first argument as the filename for the importer.
	if (!importer->Initialize(path.c_str(), -1, fbx_manager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
		exit(-1);
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* scene = FbxScene::Create(fbx_manager, "myScene");

	// Import the contents of the file into the scene.
	importer->Import(scene);

	FbxNode * root = scene->GetRootNode();
	if (root)
	{
		FbxMesh * mesh = root->GetMesh();
		FbxNode * child = root->GetChild(0);
		FbxMesh * child_mesh = child->GetMesh();
		int * polygon_vertices = child_mesh->GetPolygonVertices();
	}

	// The file is imported, so get rid of the importer.
	importer->Destroy();

	// Destroy the SDK manager and all the other objects it was handling.
	fbx_manager->Destroy();
}
