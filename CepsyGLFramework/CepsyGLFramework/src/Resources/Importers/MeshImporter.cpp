#include "MeshImporter.h"

#include "Graphics/Buffer.h"
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
	int polygon_count = mesh->GetPolygonCount();
	std::vector<int> indices;
	indices.reserve(polygon_count * 3);

	// Container to filter all vertices so we don't generate duplicates
	std::vector<Vertex> vertices_to_be_duplicated(mesh->GetControlPointsCount());
	unsigned vertex_count = 0;

	int normal_size = mesh->GetElementNormal()->GetDirectArray().GetCount();
	int uv_size = mesh->GetElementUV()->GetDirectArray().GetCount();

	for (int i = 0; i < polygon_count; ++i)
	{
		// Triangulate
		for (int j = 2; j < mesh->GetPolygonSize(i); ++j)
		{
			// Triangle fan
			int v0_index = 0;
			int v1_index = j - 1;
			int v2_index = j;
			int control_point_index = mesh->GetPolygonVertex(i, v0_index);
			int vertex_index = indices.size();

			// V0
			// Add index
			indices.emplace_back(control_point_index);

			// Check if we already added the vertex instance
			Vertex::Instance instance = fill_instance(mesh, i, v0_index, vertex_index);
			int index = vertices_to_be_duplicated[control_point_index].find(instance);

			// Add new instance for the vertex
			if (index != -1)
				vertices_to_be_duplicated[control_point_index].mInstances[index].mIndices.emplace_back(vertex_index);
			else
			{
				vertices_to_be_duplicated[control_point_index].mInstances.emplace_back(instance);
				vertices_to_be_duplicated[control_point_index].mInstances.back().mIndices.emplace_back(vertex_index);
				vertex_count++;
			}

			// V1
			// Add index
			control_point_index = mesh->GetPolygonVertex(i, v1_index);
			vertex_index = indices.size();
			indices.emplace_back(control_point_index);

			// Check if we already added the vertex instance
			instance = fill_instance(mesh, i, v1_index, vertex_index);
			index = vertices_to_be_duplicated[control_point_index].find(instance);

			// Add new instance for the vertex
			if (index != -1)
				vertices_to_be_duplicated[control_point_index].mInstances[index].mIndices.emplace_back(vertex_index);
			else
			{
				vertices_to_be_duplicated[control_point_index].mInstances.emplace_back(instance);
				vertices_to_be_duplicated[control_point_index].mInstances.back().mIndices.emplace_back(vertex_index);
				vertex_count++;
			}

			// V2
			// Add index
			control_point_index = mesh->GetPolygonVertex(i, v2_index);
			vertex_index = indices.size();
			indices.emplace_back(control_point_index);

			// Check if we already added the vertex instance
			instance = fill_instance(mesh, i, v2_index, vertex_index);
			index = vertices_to_be_duplicated[control_point_index].find(instance);

			// Add new instance for the vertex
			if (index != -1)
				vertices_to_be_duplicated[control_point_index].mInstances[index].mIndices.emplace_back(vertex_index);
			else
			{
				vertices_to_be_duplicated[control_point_index].mInstances.emplace_back(instance);
				vertices_to_be_duplicated[control_point_index].mInstances.back().mIndices.emplace_back(vertex_index);
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
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[1]);
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[2]);

			const Vertex::Instance instance = vertices_to_be_duplicated[i].mInstances[j];

			// Normal
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[1]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[2]);

			// UV
			vertex_data[offset++] = static_cast<float>(mesh->GetElementUV()->GetDirectArray().GetAt(instance.mUVIndex)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementUV()->GetDirectArray().GetAt(instance.mUVIndex)[1]);

			// Update indices
			for (auto index_index : instance.mIndices)
				indices[index_index] = current_index;
		}
	}

	// Once data is parsed, we need to create the mesh
	BufferF32 buffer_data{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, GL_FLOAT };
	buffer_data.generate(vertex_data.data(), vertex_data.size() * sizeof(float));
	BufferI32 buffer_indices{ GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, GL_UNSIGNED_INT };
	buffer_indices.generate(indices.data(), indices.size() * sizeof(int));

	return Mesh{ std::move(buffer_data), std::move(buffer_indices) };
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
