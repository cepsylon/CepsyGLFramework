#include "Renderable.h"

#include "Program.h"
#include "Model.h"
#include "Application/Application.h"

#include <imgui/imgui.h>

RTTI_I(Renderable, Component);

Renderable::Renderable()
{
	application.graphics().add(this);
}

Renderable::~Renderable()
{
	application.graphics().remove(this);
}

void Renderable::initialize()
{

}

void Renderable::update()
{

}

void Renderable::shutdown()
{

}

void Renderable::to_gui()
{
	ImGui::Checkbox("Visible", &mVisible);
	//mProgram->to_gui();
	mModel->to_gui();
}

Program * Renderable::program() const { return mProgram; }
Model * Renderable::model() const { return mModel; }

void Renderable::set_program(const std::string & name)
{
	application.graphics().remove(this);
	mProgram = application.resources().get<Program>(name);
	application.graphics().add(this);
}

void Renderable::set_model(const std::string & name)
{
	application.graphics().remove(this);
	mModel = application.resources().get<Model>(name);
	application.graphics().add(this);
}
