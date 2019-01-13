#include "Buffer.h"

#include <cstring>

BufferBase::BufferBase(GLenum target, GLenum usage)
	: mTarget(target)
	, mUsage(usage)
	, mHandle(0)
{ }

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

void BufferBase::generate()
{
	glGenBuffers(1, &mHandle);
}

void BufferBase::generate(void * data, unsigned size_in_bytes)
{
	generate();
	glBindBuffer(mTarget, mHandle);
	glBufferData(mTarget, size_in_bytes, data, mUsage);
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
