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
	};
	
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
			
			virtual void bind() = 0
			{
				static_cast<DerivedClass*>(this)->bind();
			}			
			
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera)
			{
				static_cast<DerivedClass*>(this)->updateEntity(entity, camera);
			}
			
			void unbind()
			{
				static_cast<DerivedClass*>(this)->unbind();
			}
			
			void render(const std::shared_ptr<RenderingEntity>& entity)
			{
				m_shader->update(entity->getMaterial());
				entity->draw(1);
			}
			
			void render(const Camera& camera)
			{
				m_shader->bind();
				bind();
				for(std::shared_ptr<RenderingEntity>& entity : entities)
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
			
			template<typename EntityType, typename... Args>
			shared_ptr<EntityType> createEntity(const Mesh& mesh, Args... args)
			{
				std::shared_ptr<EntityType> entity = make_shared<EntityType>(mesh, std::forward<Args>(args)...);
				entities.push_back(entity);
				return entity;
			}
			
			void init(const std::shared_ptr<Assets>& assets)
			{
				m_shader = assets->getShader(shaderName);
				m_assets = assets;
			}
			
		protected:
			std::shared_ptr<Assets> getAssets()
			{
				return m_assets;
			}
			
		private:
			Shader m_shader;
			std::vector<shared_ptr<RenderingEntity>> entities;
			std::string shaderName;	
			std::shared_ptr<Assets> m_assets;			
	};
	
	class GeometryPass : public RenderingPass<GeometryPass>
	{
		public:
			GeometryPass();
		
			void bind();
			
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
			
			void unbind();
			
			void addEntity(const Mesh& mesh, const Texture& texture, const std::shared_ptr<Transform<3>>& transform);	
	};
	
	class DebugPass : public RenderingPass<DebugPass>
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
