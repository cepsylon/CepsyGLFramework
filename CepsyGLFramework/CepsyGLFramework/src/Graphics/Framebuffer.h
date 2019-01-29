#pragma once

#include "Texture.h"

#include <glm/vec2.hpp>

#include <vector>

class Framebuffer
{
public:
	// Bind back framebuffer
	static void bind_back();

	struct Attachment
	{
		enum class Type{ COLOR = 0, DEPTH, DEPTH_STENCIL };

		Type mAttachmentType;
	};

	Framebuffer() = default;
	~Framebuffer();

	// No copies
	Framebuffer(const Framebuffer &) = delete;
	Framebuffer & operator=(const Framebuffer &) = delete;

	// Only moves
	Framebuffer(Framebuffer && rhs);
	Framebuffer & operator=(Framebuffer && rhs);

	// Arguments need to fit for the construction of a Texture
	template <typename... VA>
	void attach_layer(VA && ... arguments)
	{
		mTextures.emplace_back(std::forward<VA>(arguments)...);
	}

	// Generate framebuffer, needs to have textures previously attached
	void generate(const std::vector<Attachment> & attachment_descriptors);

	// Bind framebuffer to be rendered
	void bind() const;

	// Resize draw textures
	void resize(const glm::ivec2 & dimension);

	// Get layer, aka binded texture
	const Texture * layer(int index) const;

private:
	// Free GPU data, although being const, it does free memory under the hood
	void free() const;

	std::vector<Texture> mTextures;
	GLuint mFramebufferID = 0;
};
