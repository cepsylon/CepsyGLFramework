#pragma once

#include "Base.h"

#include "Transform.h"
#include "Component.h"

#include <memory>
#include <vector>


class Entity : public Base
{
public:
	// Initializes all components
	void initialize();

	// Updates all components
	void update();

	// Shutdowns all components
	void shutdown();

	// Outputs to gui
	void to_gui();

	// Adds default constructed component
	template <typename T>
	T * add()
	{
		mComponents.emplace_back(std::make_unique<T>());
		T * component = static_cast<T *>(mComponents.back().get());
		component->set_owner(this);
		return component;
	}

	// Transform gettors
	const Transform & transform() const;
	Transform & transform();

private:
	// Components
	std::vector<std::unique_ptr<Component>> mComponents;

	// Transform
	Transform mTransform;

	// Parent if any
	Entity * mParent = nullptr;
};
