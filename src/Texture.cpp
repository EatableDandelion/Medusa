#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <Circe/Circe.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>


namespace Medusa
{
	int TextureSlot(const std::string& name)
	{
		if(name == "diffuse0")
		{
			return (int)(TextureType::DIFFUSE0);
		}
		else if(name == "normalMap")
		{
			return (int)(TextureType::NORMAL);
		}
		else
		{
			return -1;
		}
	}
	
	/**		class Texture		**/
	Texture::Texture()
	{}
	
	Texture::Texture(TextureData& data, const int& textureSlot): ResourceHandle(data), m_textureSlot(textureSlot)
	{}
	
	void Texture::read() const
	{
		m_resource->activate(m_textureSlot);
		m_resource->bind();	
	}
	

	/**		class TextureData		**/
	
	TextureData::TextureData()
	{}
			
	TextureData::TextureData(const int& width, const int& height):width(width), height(height)
	{}
	
	void TextureData::activate(const int& textureSlot)
	{
		glActiveTexture(GL_TEXTURE0+textureSlot);
	}
	
	void TextureData::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, textureId);		
	}
	
	unsigned int& TextureData::getTextureId()
	{
		return textureId;
	}
	
	void TextureData::setTextureId(const unsigned int id)
	{
		textureId = id;
	}
			
	std::string TextureData::getTextureName() const
	{
		return name;
	}
	
	void TextureData::setTextureName(const std::string& textureName)
	{
		name = textureName;
	}
	
	int TextureData::getWidth() const
	{
		return width;
	}
			
	int TextureData::getHeight() const
	{
		return height;
	}
	
	
	/**		class ImageTextureLoader		**/
	
	void ImageTextureLoader::load(const string& folderLocation, const string& fileName, TextureData& resource)
	{
		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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
		
		resource.setTextureId(textureId);
		
		resource.setTextureName(fileName);
		
		CIRCE_INFO("Texture "+fileName+" loaded.");		
	}
			
	void ImageTextureLoader::unload(TextureData& resource)
	{
		glDeleteTextures(1, &(resource.getTextureId()));
		CIRCE_INFO("Unloading texture "+resource.getTextureName()+".");
	}
	
	
	/**		class BlankTextureLoader		**/
	
	void BlankTextureLoader::load(const string& folderLocation, const string& textureName, TextureData& resource)
	{		
		unsigned int textureId;
		glGenTextures(1, &textureId);	
		glBindTexture(GL_TEXTURE_2D, textureId);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resource.getWidth(), resource.getHeight(), 0, GL_RGBA, GL_FLOAT, 0);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+textureSlot, GL_TEXTURE_2D, textureId, 0);
		
		resource.setTextureId(textureId);
		resource.setTextureName(textureName);
		textureSlot++;
		
	}
			
	void BlankTextureLoader::unload(TextureData& resource)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &(resource.getTextureId()));
		CIRCE_INFO("Unloading texture "+resource.getTextureName()+".");
	}
	
	
	/**		class FrameBuffer		**/
	
	FrameBuffer::FrameBuffer(const int& width, const int& height):width(width), height(height), textureManager("")
	{
		
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		//Setting up the targets
		
		textureManager.load("diffuse0", width, height);
		textureManager.load("normalMap", width, height);
		
		textures.push_back(textureManager.getResource("diffuse0", 0));
		textures.push_back(textureManager.getResource("normalMap", 1));

		unsigned int att[textures.size()] =  {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(textures.size(), att);
		
		
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
		CIRCE_INFO("FrameBuffer initialized.");
		
		
	}
	
	FrameBuffer::~FrameBuffer()
	{
		textureManager.unloadAll();
		glDeleteFramebuffers(1, &fbo);
	}
	
	void FrameBuffer::bindForWrite()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0f);
	}
	
	void FrameBuffer::bindForRead()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		for(Texture texture : textures)
		{
			texture.read();
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		copyDepth();
	}
	
	void FrameBuffer::copyDepth()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
