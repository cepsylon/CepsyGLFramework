#pragma once

#include "myGlew.h"

#include <vector>

// Buffer base, it is the one we want to use if we don't care about debugging
// Interacts with the GPU, storing the vare minimum to work, does not keep
// track of the data passed to the GPU
class BufferBase
{
public:
	// Constructor does not generate GPU buffer, only initializes buffer description
	BufferBase(GLenum target, GLenum usage);
	// Free OpenGL data
	~BufferBase();

	// No copies
	BufferBase(const BufferBase &) = delete;
	BufferBase & operator=(const BufferBase &) = delete;

	// Only moves
	BufferBase(BufferBase && rhs);
	BufferBase & operator=(BufferBase && rhs);

	// Generates empty buffer
#ifdef _DEBUG
	virtual
#endif
	void generate();
	// Generates buffer with the data passed
#ifdef _DEBUG
	virtual
#endif
	void generate(void * data, unsigned size_in_bytes);

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

	// Due to having an overload of the same function, we neeed to create this overload too
	// although it will only do what the base class does
	void generate() override
	{
		BufferBase::generate();
	}

	void generate(void * data, unsigned size_in_bytes) override
	{
		// Copy debug data and generate the buffer
		mData.resize(size_in_bytes / sizeof(T));
		std::memcpy(mData.data(), data, size_in_bytes);
		BufferBase::generate(data, size_in_bytes);
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
