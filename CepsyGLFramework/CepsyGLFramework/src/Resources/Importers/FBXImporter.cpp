#include "FBXImporter.h"

#include "MeshImporter.h"
#include "Graphics/Mesh.h"

#include <fbxsdk/fbxsdk.h>
#include <glm/vec3.hpp>

#include <vector>

void FBXImporter::load(const std::string & path) const
{
	// Create new SDK manager
	FbxManager * fbx_manager = FbxManager::Create();

	// Create input settings
	FbxIOSettings * io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(io_settings);

	// Create importer with the path as a name, this does not load the file
	FbxImporter * importer = FbxImporter::Create(fbx_manager, path.c_str());

	// Initialize importer
	if (!importer->Initialize(path.c_str(), -1, fbx_manager->GetIOSettings()))
	{
		printf("Error loading file %s\n", path.c_str());
		return;
	}

	// Create scene and import it
	FbxScene * scene = FbxScene::Create(fbx_manager, "Scene");
	importer->Import(scene);

#ifdef _DEBUG
	// Print the layout of the file we are importing for debugging purposes
	print_scene(scene);
#endif

	// Import all nodes
	FbxNode * root = scene->GetRootNode();
	if (root)
	{
		for (int i = 0; i < root->GetChildCount(); ++i)
			import(root->GetChild(i));
	}

	// Free everything
	importer->Destroy();
	fbx_manager->Destroy();
}

void FBXImporter::import(FbxNode * node) const
{
	// Import all attributes to the node
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		import(node->GetNodeAttributeByIndex(i));

	// Import all children
	for (int i = 0; i < node->GetChildCount(); ++i)
		import(node->GetChild(i));
}

void DisplayControlsPoints(FbxMesh* pMesh)
{
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	printf("    Control Points\n");

	for (i = 0; i < lControlPointsCount; i++)
	{
		printf("        Control Point %d\n", i);
		printf("            Coordinates: %f, %f, %f\n", lControlPoints[i][0], lControlPoints[i][1], lControlPoints[i][2]);

		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "            Normal Vector: ");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
					printf("%s%f, %f, %f\n", header, leNormals->GetDirectArray().GetAt(i)[0], leNormals->GetDirectArray().GetAt(i)[1], leNormals->GetDirectArray().GetAt(i)[2]);
			}
		}
	}

	printf("\n");
}

void FBXImporter::import(FbxNodeAttribute * attribute) const
{
	if (!attribute)
		return;

	// Check the type and import depending on that
	// For now, we will only import meshes
	switch (attribute->GetAttributeType())
	{
	case FbxNodeAttribute::eMesh:
		import_mesh(attribute);
		break;
	default:
		break;
	}
}

void FBXImporter::import_mesh(FbxNodeAttribute * attribute) const
{
	// We already know it's a mesh, so we can cast
	FbxMesh * mesh = static_cast<FbxMesh *>(attribute);

	// Load mesh
	Mesh loaded_mesh = MeshImporter::load(mesh);

	DisplayControlsPoints(mesh);
}

#ifdef _DEBUG
void FBXImporter::print_scene(FbxScene * scene)
{
	FbxNode * root = scene->GetRootNode();
	if (root)
	{
		// Print everything
		for (int i = 0; i < root->GetChildCount(); ++i)
			print_node(root->GetChild(i), 0);
	}
}

void FBXImporter::print_node(FbxNode * node, int tab_count)
{
	// Print node name
	print_tabs(tab_count);
	const char * name = node->GetName();
	printf("<node name='%s'>\n", name);

	int tabs = tab_count + 1;
	// Print node attributes
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		print_attribute(node->GetNodeAttributeByIndex(i), tabs);

	// Print node children
	for (int i = 0; i < node->GetChildCount(); ++i)
		print_node(node->GetChild(i), tabs);

	print_tabs(tab_count);
	printf("</node>\n");
}

void FBXImporter::print_attribute(FbxNodeAttribute * attribute, int tab_count)
{
	if (!attribute)
		return;

	std::string type = attribute_type(attribute->GetAttributeType());
	print_tabs(tab_count);
	printf("<attribute type='%s' name='%s'>\n", type.c_str(), attribute->GetName());
}


std::string FBXImporter::attribute_type(FbxNodeAttribute::EType type)
{
	switch (type)
	{
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

void FBXImporter::print_tabs(int count)
{
	for (int i = 0; i < count; ++i)
		printf("\t");
}
#endif
