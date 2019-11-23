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
	};
	
	class IRenderingPass
	{	
		public:
			virtual ~IRenderingPass() = default;
			void renderAll(const Camera& camera);
			virtual void render(const Camera& camera) = 0;
			virtual void init(const std::shared_ptr<Assets>& assets) = 0;
			void addNext(const std::shared_ptr<IRenderingPass>& nextPass);
			virtual std::shared_ptr<RenderingEntity> addEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<Transform<3>>& transform);
			virtual std::shared_ptr<RenderingEntity> addEntity(const std::string& meshName, const std::shared_ptr<Transform<3>>& transform);
			
		private:
			std::weak_ptr<IRenderingPass> m_next;
	};
	
	template<typename EntityType, typename PassSettings>
	class RenderingPass : public IRenderingPass
	{
		public:
			RenderingPass(const std::string& shaderName, const bool& blending, const bool& depthRead, const bool& depthWrite):shaderName(shaderName), settings(blending, depthRead, depthWrite)
			{
				CIRCE_INFO("Initializing rendering pass.");
			}
			
			RenderingPass(const Shader& shader, const bool& blending, const bool& depthRead, const bool& depthWrite):m_shader(shader), settings(blending, depthRead, depthWrite)
			{
				CIRCE_INFO("Initializing rendering pass.");
			}
			
			~RenderingPass()
			{
				CIRCE_INFO("Terminating rendering pass.");
			}
			
			virtual void updateEntity(std::shared_ptr<EntityType>& entity, const Camera& camera)
			{}
			
			
			
			void render(const std::shared_ptr<EntityType>& entity)
			{
				m_shader->update(entity->getMaterial());
				entity->draw(1);
			}
			
			void render(const Camera& camera)
			{
				bind();

				for(std::shared_ptr<EntityType> entity : entities)
				{
					updateEntity(entity, camera);
					render(entity);
				}

				unbind();
			}
			
			void setShader(const Shader& shader)
			{
				m_shader=shader;
			}
			
			template<typename... Args>
			shared_ptr<EntityType> createEntity(const Mesh& mesh, Args... args)
			{
				std::shared_ptr<EntityType> entity = make_shared<EntityType>(mesh, std::forward<Args>(args)...);
				int id = entities.add(entity);
				return entities.get(id);
			}
			
			virtual void init(const std::shared_ptr<Assets>& assets)
			{
				m_shader = assets->getShader(shaderName);
				m_assets = assets;
			}
			
			std::shared_ptr<Assets> getAssets()
			{
				return m_assets;
			}
			
		private:
			Shader m_shader;
			RenderingCollection<EntityType> entities;
			std::string shaderName;
			
			void bind()
			{
				m_shader->bind();
				settings.start();
			}		
			
			void unbind()
			{
				settings.end();
			}
		
		protected:
			std::shared_ptr<Assets> m_assets;			
			PassSettings settings;
	};
	
	class GeometryPass : public RenderingPass<RenderingEntity, GLPassSettings>
	{
		public:
			GeometryPass();
			
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
			
			virtual std::shared_ptr<RenderingEntity> addEntity(const std::string& mesh, const  std::string& texture, const std::shared_ptr<Transform<3>>& transform);	
	};
	
	class DebugPass : public RenderingPass<RenderingEntity, GLPassSettings>
	{
		public:
			DebugPass();

			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
			
			virtual std::shared_ptr<RenderingEntity> addEntity(const std::string& mesh, const std::shared_ptr<Transform<3>>& transform);	

		private:
			GeometryPass geometryPass;
	};
}
