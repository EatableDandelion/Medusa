#pragma once

#include "RenderingPass.h"

namespace Medusa
{
	class HUDComponent : public RenderingEntity
	{
		public:
			HUDComponent(const Mesh& mesh, const shared_ptr<Transform<3>>& transform);			
	};
	
	class HUDLabel : public HUDComponent
	{
		public:
			HUDLabel(const Mesh& mesh, const shared_ptr<Transform<3>>& transform, const std::string& lable, const int& maxNbChar);
		
			void setLabel(const std::string& label);
			
			virtual void draw(const int& culling);
		
		private:
			int nbChar;
			std::string label;
			shared_ptr<Transform<3>> transform;
	};
	
	
	class HUDPass : public RenderingPass<HUDPass>
	{
		public:
			HUDPass();
			
			void bind();
			
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
			
			void unbind();
			
			void addEntity(const Texture& texture, const shared_ptr<Transform<3>>& transform);
	
			void addLabel(const Texture& texture, const shared_ptr<Transform<3>>& transform);
	};

	
	
}