#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <Circe/Circe.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>


namespace Medusa
{
	/**		class Texture		**/
	
	Texture::Texture(TextureData& data, const int& textureSlot): ResourceHandle(data), m_textureSlot(textureSlot)
	{}
	
	void Texture::read(const int& location) const
	{
		m_resource->activate(location, m_textureSlot);
		m_resource->bind();	
	}
	

	/**		class TextureData		**/
	
	void TextureData::activate(const int& location, const int& textureSlot) const
	{
		glUniform1i(location, GL_TEXTURE0+textureSlot);	
		glActiveTexture(GL_TEXTURE0+textureSlot);
	}
	
	void TextureData::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, textureId);		
	}
	
	
	/**		class TextureLoader		**/
	
	void TextureLoader::load(const string& folderLocation, const string& fileName, TextureData& resource)
	{
		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		
		
		int width, height, nbChannels;
		unsigned char* data = stbi_load((folderLocation+fileName).c_str(), &width, &height, &nbChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			CIRCE_ERROR("Failed to load "+fileName+" texture.");
		}
		
		stbi_image_free(data);
		
		resource.textureId = textureId;
		
		resource.name = fileName;
		
		CIRCE_INFO("Texture "+fileName+" loaded.");		
	}
			
	void TextureLoader::unload(TextureData& resource)
	{
		glDeleteTextures(1, &resource.textureId);
		CIRCE_INFO("Unloading texture "+resource.name+".");
	}
	
	
	/**		class FrameBuffer		**/
	
	FrameBuffer::FrameBuffer(const int& width, const int& height)
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		//Setting up the color target
		glGenTextures(1, &colorTargetId);	
		glBindTexture(GL_TEXTURE_2D, colorTargetId);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTargetId, 0);
		
		//Setting up the depth target
		glGenRenderbuffers(1, &depthTargetId);
		glBindRenderbuffer(GL_RENDERBUFFER, depthTargetId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthTargetId);
		
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CIRCE_ERROR("Could not create render buffer.");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		CIRCE_INFO("FrameBuffer initialized.")
	}
	
	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &fbo);
	}
	
	void FrameBuffer::write()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	
	void FrameBuffer::read()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, colorTargetId);
	}
}
