#include "Framebuffer.h"

void Framebuffer::bind_back()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	free();
}

Framebuffer::Framebuffer(Framebuffer && rhs)
	: mTextures(std::move(rhs.mTextures))
	, mFramebufferID(rhs.mFramebufferID)
{
	rhs.mFramebufferID = 0;
}

Framebuffer & Framebuffer::operator=(Framebuffer && rhs)
{
	if (this != &rhs)
	{
		free();
		mTextures = std::move(rhs.mTextures);
		mFramebufferID = rhs.mFramebufferID; rhs.mFramebufferID = 0;
	}

	return *this;
}

void Framebuffer::generate(const std::vector<Attachment> & attachment_descriptors)
{
	// Generate buffer
	glGenFramebuffers(1, &mFramebufferID);
	bind();

	// Attach all textures
	GLenum current_color_attachment = GL_COLOR_ATTACHMENT0;
	std::vector<GLenum> color_attachments;
	for (unsigned i = 0; i < attachment_descriptors.size(); ++i)
	{
		// Attachment type
		GLenum attachment;
		switch (attachment_descriptors[i].mAttachmentType)
		{
		case Attachment::Type::COLOR:
			attachment = current_color_attachment++;
			color_attachments.emplace_back(attachment);
			break;
		case Attachment::Type::DEPTH:
			attachment = GL_DEPTH_ATTACHMENT;
			break;
		case Attachment::Type::DEPTH_STENCIL:
			attachment = GL_DEPTH_STENCIL_ATTACHMENT;
			break;
		}

		// Bind texture to framebuffer
		mTextures[i].bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, mTextures[i].textureID(), 0);
	}

	// Enable draw buffers
	glDrawBuffers(color_attachments.size(), color_attachments.data());

#ifdef _DEBUG
	// Error checking
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer failed to complete.\n");
#endif

	// Rebind back buffer
	Framebuffer::bind_back();
}

void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
}

void Framebuffer::resize(const glm::ivec2 & dimension)
{
	for (auto & texture : mTextures)
		texture.resize(dimension);
}

const Texture * Framebuffer::layer(int index) const { return &mTextures[index]; }

void Framebuffer::free() const
{
	if (mFramebufferID)
		glDeleteFramebuffers(1, &mFramebufferID);
}
