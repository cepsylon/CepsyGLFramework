#pragma once

#include "ResourceStorage.h"

#include "Resources.h"

#include <imgui/imgui.h>

#include <string>

template <typename T>
class ResourceHandle
{
public:
	ResourceHandle() = default;

	ResourceHandle(const ResourceStorage<T> * storage)
		: mResource(&storage->mResource)
	{ }

	template <typename U>
	ResourceHandle(const ResourceStorage<U> * storage)
		: mResource(&storage->mResource)
	{ }

	const T * get() const
	{
		return mResource;
	}

	const T * operator->() const
	{
		return mResource;
	}

	const T & operator*() const
	{
		return *mResource;
	}

	bool operator==(const T * rhs) const
	{
		return mResource == rhs;
	}

	operator bool() const
	{
		return mResource;
	}

	bool to_gui()
	{
		std::string names;
		int index = 0;
		auto & map = resources().get<T>();
		int i = 0;
		for (auto it = map.begin(); it != map.end(); ++it, ++i)
		{
			ResourceHandle<T> handle = static_cast<ResourceStorage<T> *>(it->second.get());

			// Get index
			if (handle == mResource)
				index = i;

			// Names
			names += handle->name();
			names += '\0';
		}

		if (ImGui::Combo("TestResource", &index, names.c_str()))
		{
			auto it = map.begin();
			for (i = 0; i < index; ++it, ++i);
			*this = static_cast<ResourceStorage<T> *>(it->second.get());
			return true;
		}

		return false;
	}

private:
	const T * mResource = nullptr;
};
