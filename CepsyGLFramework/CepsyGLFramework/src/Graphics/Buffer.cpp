#include "Buffer.h"

Buffer::Buffer(GLenum type, unsigned size_in_bytes, void * data, GLenum usage)
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

Buffer::~Buffer()
{
	// Free if needed
	if(mHandle)
		glDeleteBuffers(1, &mHandle);
}

Buffer::Buffer(Buffer && rhs)
	: mType(rhs.mType)
	, mUsage(rhs.mUsage)
	, mHandle(rhs.mHandle)
	, mSizeInBytes(rhs.mSizeInBytes)
{
	// Reset rhs
	std::memcpy(&rhs, 0, sizeof(Buffer));
}

Buffer & Buffer::operator=(Buffer && rhs)
{
	if (this != &rhs)
	{
		if (mHandle)
			glDeleteBuffers(1, &mHandle);

		// Copy and reset rhs
		std::memcpy(this, &rhs, sizeof(Buffer));
		std::memcpy(&rhs, 0, sizeof(Buffer));
	}

	return *this;
}

void Buffer::bind() const
{
	glBindBuffer(mType, mHandle);
}

void Buffer::bind_base(unsigned index) const
{
	glBindBufferBase(mType, index, mHandle);
}

void Buffer::clear()
{
	if (mHandle)
	{
		// Free and reset
		glDeleteBuffers(1, &mHandle);
		std::memcpy(this, 0, sizeof(Buffer));
	}
}
