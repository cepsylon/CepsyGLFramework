#pragma once

#include "ResourceStorage.h"

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
	ResourceStorage<T> * get(const std::string & name)
	{
		auto it = mResources.find(&T::class_type());
		if (it != mResources.end())
		{
			auto it2 = it->second.find(name);
			if (it2 != it->second.end())
				return static_cast<ResourceStorage<T> *>(it2->second.get());
		}

		return nullptr;
	}

	template <typename T>
	const std::unordered_map<std::string, std::unique_ptr<ResourceStorageBase>> & get() const
	{
		const auto & it = mResources.find(&T::class_type());
		if (it != mResources.end())
			return it->second;

		return mResources[nullptr];
	}

	template <typename T>
	std::unordered_map<std::string, std::unique_ptr<ResourceStorageBase>> & get()
	{
		const auto & it = mResources.find(&T::class_type());
		if (it != mResources.end())
			return it->second;

		return mResources[nullptr];
	}

	template <typename T, typename... VA>
	ResourceStorage<T> * create(const std::string & key, VA &&... arguments)
	{
		auto & resources = mResources[&T::class_type()];
		std::unique_ptr<ResourceStorage<T>> value = std::make_unique<ResourceStorage<T>>(std::forward<VA>(arguments)...);
		return static_cast<ResourceStorage<T> *>(resources.insert(std::make_pair(key, std::move(value))).first->second.get());
	}

private:
	std::map<const RTTI *, std::unordered_map<std::string, std::unique_ptr<ResourceStorageBase>>> mResources;
};

Resources & resources();
