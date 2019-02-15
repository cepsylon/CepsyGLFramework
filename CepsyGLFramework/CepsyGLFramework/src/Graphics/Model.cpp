#include "Model.h"

#include "Application/Application.h"

#include <imgui/imgui.h>

RTTI_I(Model, Base);

Model::Model(std::vector<Mesh> && meshes, std::vector<Material> && materials)
	: mMeshes(std::move(meshes))
	, mMaterials(std::move(materials))
{ }

Model::Model(Model && rhs)
	: Base(std::move(rhs))
	, mMeshes(std::move(rhs.mMeshes))
	, mMaterials(std::move(rhs.mMaterials))
{ }

Model & Model::operator=(Model && rhs)
{
	if (this != &rhs)
	{
		Base::operator=(std::move(rhs));
		mMeshes = std::move(rhs.mMeshes);
		mMaterials = std::move(rhs.mMaterials);
	}

	return *this;
}

void Model::to_gui()
{
	//const auto & models = application.resources().get<Model>();
	//std::string model_names;
	//int index = 0;
	//bool found = false;
	//for (auto it = models.begin(); it != models.end(); ++it)
	//{
	//	if (it->second.get()->get() == this)
	//		found = true;
	//	if(found == false)
	//		index++;
	//
	//	model_names += it->first + '\0';
	//}
	//
	//ImGui::Combo("Model", &index, model_names.c_str());	
}

void Model::draw(const Program * program) const
{
	// Draw all meshes
	for (unsigned i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].bind();
		mMaterials[i].bind(program);
		mMeshes[i].draw();
	}
}
