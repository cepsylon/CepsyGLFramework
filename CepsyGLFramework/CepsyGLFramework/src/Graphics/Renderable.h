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

	// Binds needed data for rendering
	virtual void bind() const;

	// Gettors
	Program * program() const;
	Model * model() const;
	bool is_visible() const;

	// Settors
	void set_program(const std::string & name);
	void set_model(const std::string & name);

protected:
	Model * mModel = nullptr;
	Program * mProgram = nullptr;
	bool mVisible = true;
};
