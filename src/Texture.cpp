#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <Circe/Circe.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>


namespace Medusa
{
	
	/*Texture::Texture(TextureData& data, const TextureType& textureType): ResourceHandle(data), m_textureType(textureType)
	{}
	
	void Texture::activate(const int& location)
	{
		m_resource->activate(location, m_textureType);
	}
	
	void Texture::bind() const
	{
		m_resource->bind();		
	}
	
	

	void TextureData::activate(const int& location, const int& textureType)
	{
		glUniform1i(location, textureType);
		
		glActiveTexture(GL_TEXTURE0 + textureType);
	}
	
	void TextureData::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, textureId);		
	}
	
	
	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &fbo);
	}
	
	void FrameBuffer::init(const int& width, const int& height)
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
		
		//Setting up the render buffer object
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CIRCE_ERROR("Could not create render buffer.");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}
	
	void FrameBuffer::bindForWrite()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	
	void FrameBuffer::bindForRead()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, colorTargetId);
	}

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
	}*/
	
	
	
	/**		class TextureData		**/
	TextureData::TextureData(const std::string& folderLocation, const std::string& fileName)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		
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
		
		m_fileName = fileName;
		
		CIRCE_INFO("Texture "+fileName+" loaded.");
	}
	
	TextureData::~TextureData()
	{
		glDeleteTextures(1, &m_id);
		CIRCE_INFO("Unloading texture "+m_fileName+".");
	}
	
	
	void TextureData::read()
	{
		//glUniform1i(location, textureType);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_id);		
	}
	
	/**		class TextureManager		**/
	TextureManager::TextureManager(const std::string& folderLocation):m_folderLocation(folderLocation)
	{}
	
	TextureManager::~TextureManager()
	{
		m_textures.clear();
	}
	
	Texture TextureManager::getTexture(const std::string& fileName)
	{
		size_t id = Circe::getId(fileName);
		if(m_textures.find(id)==m_textures.end())
		{
			m_textures[id] = std::make_unique<TextureData>(m_folderLocation, fileName);
		}
		return Texture(this, id);
	}
	
	void TextureManager::read(const std::size_t id)
	{
		m_textures[id]->read();
	}
	
	
	/**		class Texture		**/
	Texture::Texture(TextureManager* manager, const std::size_t id):m_manager(manager), m_id(id)
	{}
	
	Texture::~Texture()
	{
		delete m_manager;
	}
	
	void Texture::read()
	{
		m_manager->read(m_id);
	}	
}
