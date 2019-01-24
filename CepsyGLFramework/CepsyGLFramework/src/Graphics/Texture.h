#pragma once

#include "Common/Base.h"

#include "myGlew.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <string>

class Texture : public Base
{
public:
	struct Sampler
	{
		struct WrapMode { 
			enum { 
				REPEAT = GL_REPEAT, 
				MIRROR_REPEAT = GL_MIRRORED_REPEAT, 
				EDGE = GL_CLAMP_TO_EDGE, 
				COLOR = GL_CLAMP_TO_BORDER
		}; };

		struct FilterMode {
			enum {
				NEAREST = GL_NEAREST,
				LINEAR = GL_LINEAR,
				NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
				NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
				LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
				LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
		}; };

		// Sets binded texture's sampling parameters
		void set(GLenum target) const;

		glm::vec4 mColor{ 0.0f };
		GLenum mWrapMode[3] = { WrapMode::REPEAT };
		GLenum mFilterMode[2] = { FilterMode::NEAREST };
		GLuint mMipLevel = 0;
	};

	Texture(const std::string & path);
	~Texture();

	// No copy
	Texture(const Texture &) = delete;
	Texture & operator=(const Texture &) = delete;

	// Only move
	Texture(Texture && rhs);
	Texture & operator=(Texture && rhs);


	// Frees and clears
	void clear();
	
private:
	// Get format from component count
	static GLenum format(int component_count);

	// Free OpenGL data
	// It's const, but it actually frees data under mTextureID
	// No longer usable
	void free() const;

	Sampler mSampler;
	glm::ivec2 mSize;
	GLenum mTarget = GL_TEXTURE_2D;
	GLuint mTextureID = 0;
};
