#pragma once

#include "ResourceHandle.h"
#include "RTTI/RTTI.h"

#include <string>
#include <map>
#include <unordered_map>
#include <memory>

class Resources
{
public:
	void initialize();
	void shutdown();

	template <typename T>
	T * get(const std::string & name)
	{
		auto it = mResources.find(&T::class_type());
		if (it != mResources.end())
		{
			auto it2 = it->second.find(name);
			if (it2 != it->second.end())
				return static_cast<T *>(it2->second.get()->get());
		}

		return nullptr;
	}

	template <typename T>
	const std::unordered_map<std::string, std::unique_ptr<ResourceHandleBase>> & get() const
	{
		const auto & it = mResources.find(&T::class_type());
		if (it != mResources.end())
			return it->second;

		throw "Resource map not found\n";
	}

	template <typename T>
	std::unordered_map<std::string, std::unique_ptr<ResourceHandleBase>> & get()
	{
		const auto & it = mResources.find(&T::class_type());
		if (it != mResources.end())
			return it->second;

		throw "Resource map not found\n";
	}

	template <typename T, typename... VA>
	void create(const std::string & key, VA &&... arguments)
	{
		auto & resources = mResources[&T::class_type()];
		std::unique_ptr<ResourceHandle<T>> value = std::make_unique<ResourceHandle<T>>(std::forward<VA>(arguments)...);
		resources.insert(std::make_pair(key, std::move(value)));
	}

private:
	std::map<const RTTI *, std::unordered_map<std::string, std::unique_ptr<ResourceHandleBase>>> mResources;
};
