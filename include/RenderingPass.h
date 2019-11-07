#pragma once

#include "Shader.h"
#include "RenderingEntity.h"
#include "Assets.h"
#include <vector>
#include <memory>

namespace Medusa
{
	class IRenderingPass
	{	
		virtual void render(const Camera& camera) = 0;
		virtual void init(const std::shared_ptr<Assets>& assets) = 0;
	};
	
	template<typename EntityType>
	class RenderingPass : IRenderingPass
	{
		public:
			RenderingPass(const std::string& shaderName):shaderName(shaderName)
			{
				CIRCE_INFO("Initializing rendering pass.");
			}
			
			RenderingPass(const Shader& shader):m_shader(shader)
			{
				CIRCE_INFO("Initializing rendering pass.");
			}
			
			~RenderingPass()
			{
				CIRCE_INFO("Terminating rendering pass.");
			}
			
			virtual void bind()
			{}			
			
			virtual void updateEntity(std::shared_ptr<EntityType>& entity, const Camera& camera)
			{}
			
			virtual void unbind()
			{}
			
			void render(const std::shared_ptr<EntityType>& entity)
			{
				m_shader->update(entity->getMaterial());
				entity->draw(1);
			}
			
			void render(const Camera& camera)
			{
				m_shader->bind();
				bind();
				for(std::shared_ptr<EntityType>& entity : entities)
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
				entities.push_back(entity);
				return entity;
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
			std::vector<shared_ptr<EntityType>> entities;
			std::string shaderName;	
			std::shared_ptr<Assets> m_assets;			
	};
	
	class GeometryPass : public RenderingPass<RenderingEntity>
	{
		public:
			GeometryPass();
		
			void bind();
			
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
			
			void unbind();
			
			void addEntity(const Mesh& mesh, const Texture& texture, const std::shared_ptr<Transform<3>>& transform);	
	};
	
	class DebugPass : public RenderingPass<RenderingEntity>
	{
		public:
			DebugPass();
		
			void bind();
			
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
			
			void unbind();
			
			void addEntity(const Mesh& mesh, const std::shared_ptr<Transform<3>>& transform);	

		private:
			GeometryPass geometryPass;
	};
}
