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
			Texture(TextureData& data, const int& textureSlot = 0);
						
			void read(const int& location) const;
		
		private:
			int m_textureSlot;
	};
	
	/** TextureData contains the info */
	class TextureData
	{
		public:
			void activate(const int& location, const int& textureSlot) const;
			
			void bind() const;
		
		private:
			unsigned int textureId;
			std::string name;
			
			friend class TextureLoader;
	};
	
	class TextureLoader : public ResourceLoader<TextureData>
	{
		public:			
			virtual void load(const string& folderLocation, const string& fileName, TextureData& resource);
			
			virtual void unload(TextureData& resource);
			
		private:
	};
	
	class FrameBuffer
	{
		public:
			FrameBuffer(const int& width, const int& height);
			
			~FrameBuffer();
			
			void write();
			
			void read();
			
		private:
			unsigned int fbo;
			unsigned int colorTargetId;
			unsigned int depthTargetId;
			
	};	
}
