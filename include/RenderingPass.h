#pragma once

#include "Shader.h"
#include "RenderingEntity.h"
#include "Assets.h"
#include <vector>
#include <stack>
#include <memory>

namespace Medusa
{
	
	class IPassSettings
	{
		
		public:
			IPassSettings(const bool& blending, const bool& depthRead, const bool& depthWrite);
		
			void setBlending(const bool& activate);
		
			void setDepthRead(const bool& activate);
			
			void setDepthWrite(const bool& activate);
			
			virtual void setLineThickness(const float& thickness) = 0;
			
			void start();
			
			void end();
		
		private:
			virtual void startBlending() = 0;
			virtual void stopBlending() = 0;
			
			virtual void startDepthRead() = 0;
			virtual void stopDepthRead() = 0;
			
			virtual void startDepthWrite() = 0;
			virtual void stopDepthWrite() = 0;
		
		
			bool blending;
			bool depthRead;
			bool depthWrite;
			
	};
	
	class GLPassSettings : public IPassSettings
	{
		public:
			GLPassSettings(const bool& blending, const bool& depthRead, const bool& depthWrite);
		
			virtual void startBlending();
			virtual void stopBlending();
			
			virtual void startDepthRead();
			virtual void stopDepthRead();
			
			virtual void startDepthWrite();
			virtual void stopDepthWrite();
			
			virtual void setLineThickness(const float& thickness);
			
	};
	
	/*class IRenderingPass
	{	
		public:
			virtual ~IRenderingPass() = default;
			
			//void renderAll(const Camera& camera);
			virtual void render(const Camera& camera) = 0;
			
			//void init(const std::shared_ptr<Assets>& assets);
			virtual void initAssets(const std::shared_ptr<Assets>& assets) = 0;
			
			//void addNext(const std::shared_ptr<IRenderingPass> nextPass);
			
			//void setActive(const bool& isActive);
			
		private:
			//std::weak_ptr<IRenderingPass> m_next;
			//bool active = true;
	};*/
	
	class RenderingPass
	{
		public:
			RenderingPass(const std::string& shaderName, const std::shared_ptr<IPassSettings> settings);
			
			RenderingPass(const Shader& shader, const std::shared_ptr<IPassSettings> settings);
			
			~RenderingPass();
			
			virtual void updateEntity(std::shared_ptr<EntityData>& entity, const Camera& camera);
			
			void renderAll(const Camera& camera);
			
			void render(const std::shared_ptr<EntityData>& entity);
			
			void render(const Camera& camera);
			
			void setShader(const Shader& shader);
			
			void init(const std::shared_ptr<Assets>& assets);
			
			virtual void initAssets(const std::shared_ptr<Assets>& assets);
			
			std::shared_ptr<Assets> getAssets();
			
			void addNext(const std::shared_ptr<RenderingPass> nextPass);
			
			void setActive(const bool& isActive);
			
			template<typename EntityType>
			void removeEntity(const EntityType& entity)
			{
				entities.remove(entity.getId());
			}
			
		private:
			Shader m_shader;
			RenderingCollection<EntityData> entities;
			std::string shaderName;
			std::weak_ptr<RenderingPass> m_next;
			bool active = true;
			
			void bind();	
			
			void unbind();
		
		protected:
			std::shared_ptr<Assets> m_assets;			
			std::shared_ptr<IPassSettings> settings;
			
			template<typename EntityType, typename... Args>
			EntityType createEntity(const Mesh& mesh, Args... args)
			{
				std::shared_ptr<EntityData> entity = std::make_shared<EntityData>(mesh);
				int id = entities.add(entity);
				return EntityType(entities(id), std::forward<Args>(args)...);
			}
	};
	
	class GeometryPass : public RenderingPass
	{
		public:
			GeometryPass();
			
			virtual void updateEntity(std::shared_ptr<EntityData>& entity, const Camera& camera);
			
			RenderingEntity addEntity(const std::string& mesh, const  std::string& texture);	
	};
	
	class DebugPass : public RenderingPass
	{
		public:
			DebugPass();

			virtual void updateEntity(std::shared_ptr<EntityData>& entity, const Camera& camera);
			
			RenderingEntity addEntity(const std::string& mesh);	
			
			void setLineThickness(const float& thickness);
			
		private:
			GeometryPass geometryPass;
	};
}
