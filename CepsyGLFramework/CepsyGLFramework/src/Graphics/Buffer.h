#pragma once

#include "myGlew.h"

#include <vector>

// Buffer base, it is the one we want to use if we don't care about debugging
// Interacts with the GPU, storing the vare minimum to work, does not keep
// track of the data passed to the GPU
class BufferBase
{
public:
	// Build empty buffer
	BufferBase(GLenum target, GLenum usage);
	// Build buffer with provided data
	BufferBase(GLenum target, unsigned size_in_bytes, void * data, GLenum usage);
	// Free OpenGL data
	~BufferBase();

	// No copies
	BufferBase(const BufferBase &) = delete;
	BufferBase & operator=(const BufferBase &) = delete;

	// Only moves
	BufferBase(BufferBase && rhs);
	BufferBase & operator=(BufferBase && rhs);

	// Binds buffer
	void bind() const;
	// Binds buffer to base, aka, binds buffer to gpu buffer slot
	// Only use with buffers allowed in glBindBufferBase function
	void bind_base(unsigned index) const;

	// Update buffer, should only be used if mUsage is DYNAMIC type
	void update(void * data, unsigned bytes);

	// Frees OpenGL data
	void clear();
protected:
	GLenum mTarget;
	GLenum mUsage;
	GLuint mHandle = 0;
	unsigned mSizeInBytes = 0;
};

#ifdef _DEBUG
template <typename T>
class Buffer : public BufferBase
{
public:
	Buffer(GLenum type, GLenum usage)
		: BufferBase(type, usage)
		, mData()
	{ }

	Buffer(GLenum type, unsigned count, T * data, GLenum usage)
		: BufferBase(type, sizeof(T) * count, data, usage)
		, mData(count)
	{
		std::memcpy(mData.data(), data, mSizeInBytes);
	}
	
	// No copies
	Buffer(const Buffer &) = delete;
	Buffer & operator=(const Buffer &) = delete;
	
	// Only moves
	Buffer(Buffer && rhs)
		: BufferBase(rhs)
		, mData(std::move(rhs.mData))
	{	}
	
	Buffer & operator=(Buffer && rhs)
	{
		if (this != &rhs)
		{
			BufferBase::operator=(rhs);
			mData = std::move(rhs.mData);
		}
		
		return *this;
	}

private:
	std::vector<T> mData; // For debugging purposes
};

using BufferF32 = Buffer<float>;
using BufferI32 = Buffer<int>;

#else
using BufferF32 = BufferBase;
using BufferI32 = BufferBase;
#endif
