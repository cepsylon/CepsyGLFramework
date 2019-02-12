#pragma once

#include "Common/Component.h"

class Model;
class Program;

class Renderable : public Component
{
public:
	RTTI_H;

	Renderable();
	virtual ~Renderable();

	// Initialize
	void initialize() override;
	// Update
	void update() override;
	// Shutdown
	void shutdown() override;
	// GUI
	void to_gui() override;

	// Gettors
	Program * program() const;
	Model * model() const;

	// Settors
	void set_program(const std::string & name);
	void set_model(const std::string & name);

private:
	Model * mModel = nullptr;
	Program * mProgram = nullptr;
	bool mVisible = true;
};
