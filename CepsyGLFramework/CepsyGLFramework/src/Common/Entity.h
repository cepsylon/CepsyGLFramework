#pragma once

#include "Base.h"

#include "Transform.h"

#include <memory>
#include <vector>

class Component;

class Entity : public Base
{
public:
	// Initializes all components
	void initialize();

	// Updates all components
	void update();

	// Shutdowns all components
	void shutdown();

	// Adds default constructed component
	template <typename T>
	T * add()
	{
		mComponents.emplace_back(std::make_unique<T>());
		return static_cast<T *>(mComponents.back().get());
	}

private:
	// Components
	std::vector<std::unique_ptr<Component>> mComponents;

	// Transform
	Transform mTransform;

	// Parent if any
	Entity * mParent = nullptr;
};
