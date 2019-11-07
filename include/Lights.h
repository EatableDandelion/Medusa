#pragma once

#include "memory"
#include <Circe/Circe.h>
#include "Shader.h"
#include "RenderingEntity.h"
#include "RenderingPass.h"

namespace Medusa
{
	
	class Light : public RenderingEntity
	{
		public:
			Light(const Mesh& mesh, const float& intensity, const Circe::Vec3& color);
			
			void setIntensity(const float& intensity);
			
			void setColor(const Circe::Vec3& color);
			
		private:
			Circe::Vec3 color;
	};
	
	class DirectionalLight : public Light
	{
		public:
			DirectionalLight(const Mesh& mesh, const float& intensity, const Circe::Vec3& color, const Circe::Vec3& direction);
		
			void setDirection(const Circe::Vec3& direction);
	};
	
	class AmbientLight : public Light
	{
		public:
			AmbientLight(const Mesh& mesh, const float& intensity, const Circe::Vec3& color);
	};
	/*
	class PointLight : public Light
	{
		public:
			PointLight(const Mesh& mesh, const float& intensity, const Circe::Vec3& color);
	};*/
	
	class DirectionalLightPass : public RenderingPass<DirectionalLight>
	{
		public:
			DirectionalLightPass();
		
			void bind();
		
			void updateEntity(std::shared_ptr<DirectionalLight>& entity, const Camera& camera);
		
			void unbind();
		
			void addEntity(const float& intensity, const Circe::Vec<3>& color, const Circe::Vec<3>& direction);
	};
	
	class AmbientLightPass : public RenderingPass<AmbientLight>
	{
		public:
			AmbientLightPass();
		
			void bind();
		
			void updateEntity(std::shared_ptr<AmbientLight>& entity, const Camera& camera);
		
			void unbind();
		
			void addEntity(const float& intensity, const Circe::Vec<3>& color);
	};
	/*
	class PointLightPass : public RenderingPass<PointLightPass>
	{
		public:
			void bind();
		
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
		
			void unbind();
		
			void addEntity(const std::shared_ptr<Transform<3>>& transform, const Mesh& screenMesh, const float& intensity, const Circe::Vec3& color);
	};*/
}