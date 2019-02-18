#pragma once

#include "ResourceStorage.h"

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

	bool operator==(T * rhs) const
	{
		return mResource == rhs;
	}

	operator bool() const
	{
		return mResource;
	}

private:
	const T * mResource = nullptr;
};
