#include "MeshImporter.h"

#include "Graphics/Mesh.h"

#include <fbxsdk/fbxsdk.h>

#include <vector>

Mesh MeshImporter::load(FbxMesh * mesh)
{
	int polygon_count = mesh->GetPolygonCount();
	//FbxGeometryElementPolygon
	// Check if we need to duplicate vertices
	int vertex_count = mesh->GetControlPointsCount();
	int normal_count = mesh->GetElementNormalCount();
	FbxGeometryElementNormal * normals = mesh->GetElementNormal(0);
	int count1 = normals->GetDirectArray().GetCount();
	//FbxGeometryElementNormal::e
	switch (normals->GetMappingMode())
	{
	case FbxLayerElement::eNone:
		break;
	case FbxLayerElement::eByControlPoint:
		break;
	case FbxLayerElement::eByPolygonVertex:
		break;
	case FbxLayerElement::eByPolygon:
		break;
	case FbxLayerElement::eByEdge:
		break;
	case FbxLayerElement::eAllSame:
		break;
	}
	int uv_count = mesh->GetElementUVCount();
	FbxGeometryElementUV * uv = mesh->GetElementUV(0);
	int count = uv->GetDirectArray().GetCount();
	bool need_to_duplicate_vertices = mesh->GetControlPointsCount() != mesh->GetElementNormalCount();


	return Mesh{};
}
