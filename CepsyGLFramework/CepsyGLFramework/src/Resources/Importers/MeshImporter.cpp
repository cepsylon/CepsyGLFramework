#include "MeshImporter.h"

#include "Graphics/Mesh.h"

bool Vertex::Instance::operator==(const Instance & rhs) const
{
	return mNormalIndex == rhs.mNormalIndex && mUVIndex == rhs.mUVIndex;
}

int Vertex::find(const Instance & rhs) const
{
	for (unsigned i = 0; i < mInstances.size(); ++i)
	{
		if (mInstances[i] == rhs)
			return i;
	}

	return -1;
}


Mesh MeshImporter::load(FbxMesh * mesh)
{
	// Indices, we assume that all will be triangles at the beginning
	// TODO: Check polygons are triangles and update the container as needed
	// TODO: Support more or less than just positions normals and uvs
	std::vector<int> indices;
	indices.reserve(mesh->GetPolygonCount() * 3);

	// Container to filter all vertices so we don't generate duplicates
	std::vector<Vertex> vertices_to_be_duplicated(mesh->GetControlPointsCount());
	unsigned vertex_count = 0;

	for (int i = 0; i < mesh->GetPolygonCount(); ++i)
	{
		int polygon_indices = mesh->GetPolygonSize(i);
		for (int j = 0; j < polygon_indices; ++j)
		{
			int control_point_index = mesh->GetPolygonVertex(i, j);
			int vertex_index = indices.size();

			// Add index
			indices.emplace_back(control_point_index);

			// Check if we already added the vertex instance
			Vertex::Instance instance = fill_instance(mesh, i, j, vertex_index);
			int index = vertices_to_be_duplicated[control_point_index].find(instance);

			// Add new instance for the vertex
			if (index != -1)
				vertices_to_be_duplicated[control_point_index].mInstances[index].mIndices.emplace_back(&indices.back());
			else
			{
				vertices_to_be_duplicated[control_point_index].mInstances.emplace_back(instance);
				vertices_to_be_duplicated[control_point_index].mInstances.back().mIndices.emplace_back(&indices.back());
				vertex_count++;
			}
		}
	}

	// Create vertices
	std::vector<float> vertex_data(vertex_count * 8);

	// Duplicate vertices
	unsigned current_index = 0;
	unsigned offset = 0;
	for (unsigned i = 0; i < vertices_to_be_duplicated.size(); ++i)
	{
		for (unsigned j = 0; j < vertices_to_be_duplicated[i].mInstances.size(); ++j, ++current_index)
		{
			// Position
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[2]);
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[1]);

			const Vertex::Instance instance = vertices_to_be_duplicated[i].mInstances[j];

			// Normal
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[2]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[1]);

			// UV
			vertex_data[offset++] = static_cast<float>(mesh->GetElementUV()->GetDirectArray().GetAt(instance.mUVIndex)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementUV()->GetDirectArray().GetAt(instance.mUVIndex)[1]);

			// Update indices
			for (auto p_index : instance.mIndices)
				*p_index = current_index;
		}
	}

	return Mesh{};
}

Vertex::Instance MeshImporter::fill_instance(FbxMesh * mesh, int polygon, int polygon_vertex, int vertex_count)
{
	Vertex::Instance instance;
	int control_point_index = mesh->GetPolygonVertex(polygon, polygon_vertex);

	// Normals
	// We only handle one array of normals
	FbxGeometryElementNormal * normals = mesh->GetElementNormal();
	switch (normals->GetMappingMode())
	{
	case FbxLayerElement::eByControlPoint:
		switch (normals->GetReferenceMode())
		{
		case FbxLayerElement::eDirect:
			instance.mNormalIndex = control_point_index;
			break;
		case FbxLayerElement::eIndexToDirect:
			instance.mNormalIndex = normals->GetIndexArray().GetAt(control_point_index);
			break;
		default:
			break;
		}
		break;
	case FbxLayerElement::eByPolygonVertex:
		switch (normals->GetReferenceMode())
		{
		case FbxLayerElement::eDirect:
			instance.mNormalIndex = vertex_count;
			break;
		case FbxLayerElement::eIndexToDirect:
			instance.mNormalIndex = normals->GetIndexArray().GetAt(vertex_count);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	// UVs
	// We only handle one array of normals
	FbxGeometryElementUV * uvs = mesh->GetElementUV();
	switch (uvs->GetMappingMode())
	{
	case FbxLayerElement::eByControlPoint:
		switch (uvs->GetReferenceMode())
		{
		case FbxLayerElement::eDirect:
			instance.mNormalIndex = control_point_index;
			break;
		case FbxLayerElement::eIndexToDirect:
			instance.mNormalIndex = uvs->GetIndexArray().GetAt(control_point_index);
			break;
		default:
			break;
		}
		break;
	case FbxLayerElement::eByPolygonVertex:

		switch (uvs->GetReferenceMode())
		{
		case FbxLayerElement::eDirect:
		case FbxLayerElement::eIndexToDirect:
			instance.mUVIndex = mesh->GetTextureUVIndex(polygon, polygon_vertex);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return instance;
}
