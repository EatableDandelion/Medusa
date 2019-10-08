#pragma once

#include "memory"
#include <Circe/Circe.h>
#include "Shader.h"
#include "RenderingEntity.h"
#include "RenderingPass.h"

namespace Medusa
{
	
	struct Light
	{
		public:
			Light(const float& intensity, const Circe::Vec3& color);
			
			virtual void update(RenderingEntity& screenEntity) = 0;
			
			void setIntensity(const float& intensity);
			
			Circe::Vec3 getColor() const;
			
		protected:
			Circe::Vec3 color;
	};
	
	struct DirectionalLight : public Light
	{
		public:
			DirectionalLight(const float& intensity, const Circe::Vec3& color, const Circe::Vec3& direction);
		
			Circe::Vec3 getDirection() const;
			
			virtual void update(RenderingEntity& screenEntity);
			
		protected:
			Circe::Vec3 direction;
	};
	
	struct AmbientLight : public Light
	{
		public:
			AmbientLight(const float& intensity, const Circe::Vec3& color);
			
			virtual void update(RenderingEntity& screenEntity);
	};
	
	template<typename LightType>
	class LightManager : public RenderingPass
	{
		public:
			virtual void render(RenderingEntity& screenEntity)
			{
				bind();
				for(LightType light : lights)
				{
					light.update(screenEntity);
					RenderingPass::render(screenEntity);
				}
				unbind();
			}

			template<typename... Args>
			void addLight(Args... args)
			{
				lights.push_back(LightType(std::forward<Args>(args)...));
			}

		private:
			std::vector<LightType> lights;
	};
}