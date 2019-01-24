#pragma once

#include <memory>

struct ResourceHandleBase
{
	virtual ~ResourceHandleBase() {}

	virtual void * get() = 0;
};

template <typename T>
class ResourceHandle : public ResourceHandleBase
{
public:
	ResourceHandle(const T & resource)
		: mResource(resource)
	{ }

	ResourceHandle(const ResourceHandle<T> & rhs)
		: mResource(rhs.mResource)
	{ }

	ResourceHandle(T && resource)
		: mResource(std::move(resource))
	{ }

	ResourceHandle(ResourceHandle<T> && rhs)
		: mResource(std::move(rhs.mResource))
	{ }

	template <typename... VA>
	ResourceHandle(VA &&... arguments)
		: mResource(std::forward<VA>(arguments)...)
	{ }

	ResourceHandle & operator==(const ResourceHandle<T> & rhs)
	{
		if (this != &rhs)
			mResource = rhs.mResource;

		return *this;
	}

	ResourceHandle & operator==(ResourceHandle<T> && rhs)
	{
		if (this != &rhs)
			mResource = std::move(rhs.mResource);

		return *this;
	}

	const T * operator->() const
	{
		return &mResource;
	}

	T * operator->()
	{
		return &mResource;
	}

	void * get()
	{
		return static_cast<void *>(&mResource);
	}

private:
	T mResource;
};
