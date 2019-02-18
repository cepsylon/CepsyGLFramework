#pragma once

#include <memory>

struct ResourceStorageBase
{
	virtual ~ResourceStorageBase() {}
};

template <typename T>
class ResourceStorage : public ResourceStorageBase
{
public:
	template <typename U>
	friend class ResourceHandle;

	ResourceStorage(const T & resource)
		: mResource(resource)
	{ }

	ResourceStorage(const ResourceStorage<T> & rhs)
		: mResource(rhs.mResource)
	{ }

	ResourceStorage(T && resource)
		: mResource(std::move(resource))
	{ }

	ResourceStorage(ResourceStorage<T> && rhs)
		: mResource(std::move(rhs.mResource))
	{ }

	template <typename... VA>
	ResourceStorage(VA &&... arguments)
		: mResource(std::forward<VA>(arguments)...)
	{ }

	ResourceStorage & operator=(const ResourceStorage<T> & rhs)
	{
		if (this != &rhs)
			mResource = rhs.mResource;

		return *this;
	}

	ResourceStorage & operator=(ResourceStorage<T> && rhs)
	{
		if (this != &rhs)
			mResource = std::move(rhs.mResource);

		return *this;
	}

private:
	T mResource;
};
