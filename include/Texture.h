#pragma once
#define CIRCE_VERBOSE
#include "Resource.h"


namespace Medusa
{
	
	/*class Texture;
	class TextureData;
	class FrameBuffer;
	class TextureLoader;
	
	enum TextureType{DIFFUSE=0, NORMAL=1, TANGENT=2};
	
	class Texture : public ResourceHandle<Texture, TextureData>
	{
		public:
		
			Texture(TextureData& data, const TextureType& textureType);
						
			void activate(const int& location);
			
			void bind() const;
			
		private:
			TextureType m_textureType;
	};
	
	class TextureData
	{
		public:
			void activate(const int& location, const int& textureType);
			
			void bind() const;
		
		private:
			unsigned int textureId;
			std::string name;
			friend class TextureLoader;
	};
	
	
	
	class FrameBuffer
	{
		public:
			
			~FrameBuffer();
			
			void init(const int& width, const int& height);
			
			void bindForWrite();
			
			void bindForRead();
			
		private:
			unsigned int fbo;
			unsigned int colorTargetId;
			unsigned int rbo;
			
	};
	
	class TextureLoader : public ResourceLoader<TextureData>
	{
		public:			
			virtual void load(const string& folderLocation, const string& fileName, TextureData& resource);
			
			virtual void unload(TextureData& resource);
			
		private:
	};
	*/
	
	
	class TextureData;
	class TextureManager;
	class Texture;
	
	class TextureData
	{
		public:
			TextureData(const std::string& folderLocation, const std::string& fileName);
		
			~TextureData();
			
			void read(const int& location) const;
		
		private:
			unsigned int m_id;
			std::string m_fileName;
	};
	
	class TextureManager
	{
		public:
			TextureManager(const std::string& folderLocation);
			
			TextureManager(const TextureManager& textureManager);
			
			~TextureManager();
			
			Texture getTexture(const std::string& fileName);
			
			void read(const std::size_t id, const int& location) const;
			
		private:
			std::map<std::size_t, std::unique_ptr<TextureData>> m_textures;
			std::string m_folderLocation;
	};
	
	class Texture
	{
		public:
			Texture(TextureManager* manager, const std::size_t id);
			
			Texture(const Texture& texture);
			
			void read(const int& location) const;
			
		private:
			std::size_t m_id;
			std::shared_ptr<TextureManager> m_manager;
	};
}
