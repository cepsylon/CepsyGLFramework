#include "MeshImporter.h"

#include "Graphics/Buffer.h"
#include "Graphics/Mesh.h"

bool MeshImporter::Vertex::Instance::operator==(const Instance & rhs) const
{
	return mNormalIndex == rhs.mNormalIndex && mUVIndex == rhs.mUVIndex;
}

int MeshImporter::Vertex::find(const Instance & rhs) const
{
	for (unsigned i = 0; i < mInstances.size(); ++i)
	{
		if (mInstances[i] == rhs)
			return i;
	}
	return -1;
}


Mesh MeshImporter::load(FbxMesh * mesh, const Skeleton & skeleton)
{
	// TODO: Support more or less than just positions normals and uvs

	// For triangulating if needed later
	int polygon_count = mesh->GetPolygonCount();
	std::vector<Polygon> polygons(polygon_count);

	// Container to filter all vertices so we don't generate duplicates
	std::vector<Vertex> vertices_to_be_duplicated(mesh->GetControlPointsCount());
	unsigned vertex_count = 0;
	unsigned current_index = 0;

	// Read all vertices and polygons
	for (int i = 0; i < polygon_count; ++i)
	{
		// Read polygon
		Polygon & polygon = polygons[i];
		int polygon_size = mesh->GetPolygonSize(i);
		polygon.mIndices.reserve(polygon_size);
		for (int j = 0; j < polygon_size; ++j)
		{
			// Control point index will be used to index container for duplicates
			int control_point_index = mesh->GetPolygonVertex(i, j);

			// Add index to polygon
			polygon.mIndices.emplace_back(control_point_index);

			// Check if we need to create a duplicated vertex in the same position due to different
			// normals, uvs, etc
			Vertex::Instance instance = fill_instance(mesh, i, j, current_index);
			int index = vertices_to_be_duplicated[control_point_index].find(instance);

			// Add new instance for the vertex
			if (index != -1)
				vertices_to_be_duplicated[control_point_index].mInstances[index].mIndices.emplace_back(&polygon.mIndices.back());
			else
			{
				// We neeed to create a vertex for this configuration
				vertices_to_be_duplicated[control_point_index].mInstances.emplace_back(instance);
				vertices_to_be_duplicated[control_point_index].mInstances.back().mIndices.emplace_back(&polygon.mIndices.back());
				vertex_count++;
			}

			current_index++;
		}
	}

	// Import skin if we have a skeleton
	int skin_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int skin_index = 0; skin_index < skin_count; ++skin_index)
	{
		FbxSkin * skin = reinterpret_cast<FbxSkin *>(mesh->GetDeformer(skin_index, FbxDeformer::eSkin));
		
		int cluster_count = skin->GetClusterCount();
		for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
		{
			FbxCluster * cluster = skin->GetCluster(cluster_index);
			int bone_index = skeleton.find(cluster->GetLink()->GetName());
			if (bone_index == -1)
				continue;

			// Weights and indices
			int * indices = cluster->GetControlPointIndices();
			double * weights = cluster->GetControlPointWeights();
			int index_weight_count = cluster->GetControlPointIndicesCount();
			for (int i = 0; i < index_weight_count; ++i)
			{
				vertices_to_be_duplicated[indices[i]].mIndices.emplace_back(bone_index);
				vertices_to_be_duplicated[indices[i]].mWeights.emplace_back(static_cast<float>(weights[i]));
			}
		}
	}

	// Vertex data
	int floats_in_vertex = 16;
	std::vector<float> vertex_data(vertex_count * floats_in_vertex);

	// Duplicate vertices
	current_index = 0;
	unsigned offset = 0;
	for (unsigned i = 0; i < vertices_to_be_duplicated.size(); ++i)
	{
		for (unsigned j = 0; j < vertices_to_be_duplicated[i].mInstances.size(); ++j, ++current_index)
		{
			// Position
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[1]);
			vertex_data[offset++] = static_cast<float>(mesh->GetControlPointAt(i)[2]);

			const Vertex::Instance & instance = vertices_to_be_duplicated[i].mInstances[j];

			// Normal
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[1]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementNormal()->GetDirectArray().GetAt(instance.mNormalIndex)[2]);

			// UV
			vertex_data[offset++] = static_cast<float>(mesh->GetElementUV()->GetDirectArray().GetAt(instance.mUVIndex)[0]);
			vertex_data[offset++] = static_cast<float>(mesh->GetElementUV()->GetDirectArray().GetAt(instance.mUVIndex)[1]);

			// Indices
			unsigned bones_added = 0;
			while (bones_added < vertices_to_be_duplicated[i].mIndices.size())
				vertex_data[offset++] = static_cast<float>(vertices_to_be_duplicated[i].mIndices[bones_added++]);
			while (bones_added++ < 4)
				vertex_data[offset++] = 0.0f;
			
			// Weights
			bones_added = 0;
			while (bones_added < vertices_to_be_duplicated[i].mWeights.size())
				vertex_data[offset++] = vertices_to_be_duplicated[i].mWeights[bones_added++];
			while (bones_added++ < 4)
				vertex_data[offset++] = 0.0f;

			// Update indices that were refering to this instance to index the newly created vertex
			for (auto index : instance.mIndices)
				*index = current_index;
		}
	}

	// Triangulate
	std::vector<int> indices;
	for (const auto & polygon : polygons)
	{
		int v0_index = polygon.mIndices[0];
		for (unsigned i = 2; i < polygon.mIndices.size(); ++i)
		{
			int v1_index = polygon.mIndices[i];
			int v2_index = polygon.mIndices[i - 1];

			indices.emplace_back(v0_index);
			indices.emplace_back(v1_index);
			indices.emplace_back(v2_index);
		}
	}

	// Create the mesh
	BufferF32 buffer_data{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, GL_FLOAT };
	buffer_data.generate(vertex_data.data(), vertex_data.size() * sizeof(float));
	BufferI32 buffer_indices{ GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, GL_UNSIGNED_INT };
	buffer_indices.generate(indices.data(), indices.size() * sizeof(int));

	unsigned stride = floats_in_vertex * sizeof(float);
	Mesh::Layout::SizeOffset positions{ 3, 0 };
	Mesh::Layout::SizeOffset normals{ 3, 3 * sizeof(float) };
	Mesh::Layout::SizeOffset uvs{ 2, 6 * sizeof(float) };
	Mesh::Layout::SizeOffset indices_layout{ 4, 8 * sizeof(float) };
	Mesh::Layout::SizeOffset weights_layout{ 4, 12 * sizeof(float) };
	return Mesh{ std::move(buffer_data), std::move(buffer_indices), Mesh::Layout{ stride, { positions, normals, uvs, indices_layout, weights_layout } } };
}

MeshImporter::Vertex::Instance MeshImporter::fill_instance(FbxMesh * mesh, int polygon, int polygon_vertex, int vertex_count)
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
