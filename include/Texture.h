#pragma once
#define CIRCE_VERBOSE
#include "Resource.h"


namespace Medusa
{
	
	class Texture;
	class TextureData;
	class TextureLoader;
	class FrameBuffer;
	
	/** Texture is a handle */
	class Texture : public ResourceHandle<Texture, TextureData>
	{
		public:
			Texture();
		
			Texture(TextureData& data, const int& textureSlot = 0);
						
			void read(const int& location) const;
			
		
		private:
			int m_textureSlot;
	};
	
	/** TextureData contains the info */
	class TextureData
	{
		public:
			TextureData();
			
			TextureData(const int& width, const int& height);
		
			void activate(const int& location, const int& textureSlot);
			
			void bind() const;
			
			unsigned int& getTextureId();
			
			void setTextureId(const unsigned int id);
			
			std::string getTextureName() const;
			
			void setTextureName(const std::string& name);
			
			int getWidth() const;
			
			int getHeight() const;
			
		
		private:
			unsigned int textureId;
			std::string name;
			int width;
			int height;
			bool first;
	};
	
	class ImageTextureLoader : public ResourceLoader<TextureData>
	{
		public:			
			virtual void load(const string& folderLocation, const string& fileName, TextureData& resource);
			
			virtual void unload(TextureData& resource);
	};
	
	class BlankTextureLoader : public ResourceLoader<TextureData>
	{
		public:
			virtual void load(const string& folderLocation, const string& textureName, TextureData& resource);
			
			virtual void unload(TextureData& resource);
			
		private:
			int textureSlot = 0;
	};
	
	//class 
	
	class FrameBuffer
	{
		public:
			FrameBuffer(const int& width, const int& height);
			
			~FrameBuffer();
			
			void write();
			
			void read();
			
			std::vector<Texture> getTextures();
			
		private:
			unsigned int fbo;
			unsigned int attachmentIds[1];
			unsigned int depthTargetId;
			std::vector<std::string> textures;
			ResourceManager<Texture, TextureData, BlankTextureLoader> gBufferTextures;
	};	
}
