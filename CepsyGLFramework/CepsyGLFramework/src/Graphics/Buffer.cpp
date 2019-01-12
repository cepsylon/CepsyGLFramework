#include "Buffer.h"

#include <cstring>

BufferBase::BufferBase(GLenum target, GLenum usage)
	: mTarget(target)
	, mUsage(usage)
	, mHandle(0)
	, mSizeInBytes(0)
{
	glGenBuffers(1, &mHandle);
}

BufferBase::BufferBase(GLenum target, unsigned size_in_bytes, void * data, GLenum usage)
	: mTarget(target)
	, mUsage(usage)
	, mHandle(0)
	, mSizeInBytes(size_in_bytes)
{
	// Create the buffer and fill it
	glGenBuffers(1, &mHandle);
	glBindBuffer(mTarget, mHandle);
	glBufferData(mTarget, mSizeInBytes, data, mUsage);
}

BufferBase::~BufferBase()
{
	// Free if needed
	if(mHandle)
		glDeleteBuffers(1, &mHandle);
}

BufferBase::BufferBase(BufferBase && rhs)
	: mTarget(rhs.mTarget)
	, mUsage(rhs.mUsage)
	, mHandle(rhs.mHandle)
	, mSizeInBytes(rhs.mSizeInBytes)
{
	// Reset rhs
	std::memset(&rhs, 0, sizeof(BufferBase));
}

BufferBase & BufferBase::operator=(BufferBase && rhs)
{
	if (this != &rhs)
	{
		if (mHandle)
			glDeleteBuffers(1, &mHandle);

		// Copy and reset rhs
		std::memcpy(this, &rhs, sizeof(BufferBase));
		std::memset(&rhs, 0, sizeof(BufferBase));
	}

	return *this;
}

void BufferBase::bind() const
{
	glBindBuffer(mTarget, mHandle);
}

void BufferBase::bind_base(unsigned index) const
{
	glBindBufferBase(mTarget, index, mHandle);
}

void BufferBase::update(void * data, unsigned bytes)
{
	bind();
	glBufferData(mTarget, bytes, nullptr, mUsage);
	glBufferData(mTarget, bytes, data, mUsage);
}

void BufferBase::clear()
{
	if (mHandle)
	{
		// Free and reset
		glDeleteBuffers(1, &mHandle);
		std::memset(this, 0, sizeof(BufferBase));
	}
}
