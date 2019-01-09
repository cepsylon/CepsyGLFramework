#pragma once

#include "myGlew.h"

#include <vector>

// Buffer base, it is the one we want to use if we don't care about debugging
// Interacts with the GPU, storing the vare minimum to work, does not keep
// track of the data passed to the GPU
class Buffer
{
public:
	Buffer(GLenum type, unsigned size_in_bytes, void * data, GLenum usage);
	~Buffer();

	// No copies
	Buffer(const Buffer &) = delete;
	Buffer & operator=(const Buffer &) = delete;

	// Only moves
	Buffer(Buffer && rhs);
	Buffer & operator=(Buffer && rhs);

	// Binds buffer
	void bind() const;
	// Binds buffer to base, aka, binds buffer to gpu buffer slot
	// Only use with buffers allowed in glBindBufferBase function
	void bind_base(unsigned index) const;

	// Frees OpenGL data
	void clear();
private:
	GLenum mType;
	GLenum mUsage;
	GLuint mHandle = 0;
	unsigned mSizeInBytes = 0;
};

template <typename T>
class BufferDebug : public Buffer
{
public:
	BufferDebug(GLenum type, unsigned count, T * data, GLenum usage)
		: Buffer(type, sizeof(T) * count, data, usage)
		, mData(count)
	{
		std::memcpy(mData.data(), data, mSizeInBytes);
	}
	
	// No copies
	BufferDebug(const BufferDebug &) = delete;
	BufferDebug & operator=(const BufferDebug &) = delete;
	
	// Only moves
	BufferDebug(BufferDebug && rhs)
		: Buffer(rhs)
		, mData(std::move(rhs.mData))
	{	}
	
	BufferDebug & operator=(BufferDebug && rhs)
	{
		if (this != &rhs)
		{
			Buffer::operator=(rhs);
			mData = std::move(rhs.mData);
		}
		
		return *this;
	}

private:
	std::vector<T> mData; // For debugging purposes
};
