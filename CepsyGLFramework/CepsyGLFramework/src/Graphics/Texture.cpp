#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void Texture::slot(int slot_index)
{
	glActiveTexture(GL_TEXTURE0 + slot_index);
}

void Texture::Sampler::set(GLenum target) const
{
	// Wrap mode
	glTexParameteri(target, GL_TEXTURE_WRAP_S, mWrapMode[0]);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, mWrapMode[1]);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, mWrapMode[2]);
	glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, &mColor[0]);

	// Filter mode
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, mFilterMode[0]);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mFilterMode[1]);
}

Texture::Texture(const std::string & path)
{
	// Load texture
	int component_count;
	unsigned char * data = stbi_load(path.c_str(), &mSize.x, &mSize.y, &component_count, 0);
	mFormat = format(component_count);

	// Generate texture
	glGenTextures(1, &mTextureID);
	bind();
	glTexImage2D(mTarget, mSampler.mMipLevel, mFormat, mSize.x, mSize.y, 0, mFormat, GL_UNSIGNED_BYTE, data);

	// Sampling parameters
	mSampler.set(mTarget);

	// Free texture
	stbi_image_free(data);
}

Texture::Texture(GLenum target, GLenum format, int width, int height)
	: mSize(width, height)
	, mTarget(target)
	, mFormat(format)
{
	// Generate empty texture
	glGenTextures(1, &mTextureID);
	bind();
	glTexImage2D(mTarget, mSampler.mMipLevel, mFormat, mSize.x, mSize.y, 0, mFormat, GL_UNSIGNED_BYTE, nullptr);

	// Sampling parameters
	mSampler.set(mTarget);
}

Texture::~Texture()
{
	free();
}

Texture::Texture(Texture && rhs)
	: mSampler(rhs.mSampler)
	, mSize(rhs.mSize)
	, mTarget(rhs.mTarget)
	, mTextureID(rhs.mTextureID)
{
	rhs.mSize = glm::ivec2{ 0, 0 };
	rhs.mTextureID = 0;
}

Texture & Texture::operator=(Texture && rhs)
{
	if (this != &rhs)
	{
		free();
		mSampler = rhs.mSampler;
		mSize = rhs.mSize; rhs.mSize = glm::ivec2{ 0, 0 };
		mTarget = rhs.mTarget;
		mTextureID = rhs.mTextureID; rhs.mTextureID = 0;
	}

	return *this;
}

void Texture::generate_mipmap() const
{
	glGenerateMipmap(mTarget);
}

void Texture::bind() const
{
	glBindTexture(mTarget, mTextureID);
}
#include "Graphics.h"
void Texture::resize(const glm::ivec2 & dimension)
{
	mSize = dimension;
	bind();
	glTexImage2D(mTarget, mSampler.mMipLevel, mFormat, mSize.x, mSize.y, 0, mFormat, GL_UNSIGNED_BYTE, nullptr);
	Graphics::Debug::print_error();
}

void Texture::clear()
{
	free();
	mTextureID = 0;
}

Texture::Sampler & Texture::sampler() { return mSampler; }
const Texture::Sampler & Texture::sampler() const { return mSampler; }
int Texture::textureID() const { return mTextureID; }

GLenum Texture::format(int component_count)
{
	switch (component_count)
	{
	case 1:
		return GL_R;
	case 2:
		return GL_RG;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	default:
		return GL_RGBA;
	}
}

void Texture::free() const
{
	if (mTextureID)
		glDeleteTextures(1, &mTextureID);
}
