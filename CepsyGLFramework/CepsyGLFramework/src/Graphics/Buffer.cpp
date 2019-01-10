#include "Buffer.h"

#include <cstring>

BufferBase::BufferBase(GLenum type, GLenum usage)
	: mType(type)
	, mUsage(usage)
	, mHandle(0)
	, mSizeInBytes(0)
{
	glGenBuffers(1, &mHandle);
}

BufferBase::BufferBase(GLenum type, unsigned size_in_bytes, void * data, GLenum usage)
	: mType(type)
	, mUsage(usage)
	, mHandle(0)
	, mSizeInBytes(size_in_bytes)
{
	// Create the buffer and fill it
	glGenBuffers(1, &mHandle);
	glBindBuffer(mType, mHandle);
	glBufferData(mType, mSizeInBytes, data, mUsage);
}

BufferBase::~BufferBase()
{
	// Free if needed
	if(mHandle)
		glDeleteBuffers(1, &mHandle);
}

BufferBase::BufferBase(BufferBase && rhs)
	: mType(rhs.mType)
	, mUsage(rhs.mUsage)
	, mHandle(rhs.mHandle)
	, mSizeInBytes(rhs.mSizeInBytes)
{
	// Reset rhs
	std::memcpy(&rhs, 0, sizeof(BufferBase));
}

BufferBase & BufferBase::operator=(BufferBase && rhs)
{
	if (this != &rhs)
	{
		if (mHandle)
			glDeleteBuffers(1, &mHandle);

		// Copy and reset rhs
		std::memcpy(this, &rhs, sizeof(BufferBase));
		std::memcpy(&rhs, 0, sizeof(BufferBase));
	}

	return *this;
}

void BufferBase::bind() const
{
	glBindBuffer(mType, mHandle);
}

void BufferBase::bind_base(unsigned index) const
{
	glBindBufferBase(mType, index, mHandle);
}

void BufferBase::update(void * data, unsigned bytes)
{
	bind();
	glBufferData(mType, bytes, data, mUsage);
}

void BufferBase::clear()
{
	if (mHandle)
	{
		// Free and reset
		glDeleteBuffers(1, &mHandle);
		std::memcpy(this, 0, sizeof(BufferBase));
	}
}

#ifdef _DEBUG
Buffer<float> make_buffer(GLenum type, GLenum usage)
{
	return Buffer<float>{type, usage};
}
#endif
