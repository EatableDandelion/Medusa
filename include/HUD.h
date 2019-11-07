#pragma once

#include "RenderingPass.h"

namespace Medusa
{
	class Panel : public RenderingEntity
	{
		public:
			Panel(const Mesh& mesh, const shared_ptr<Transform<3>>& transform);			
	};
	
	class Label
	{
		public:
			Label(const shared_ptr<Transform<3>>& transform);
			
			void addCharacter(const std::shared_ptr<Panel>& letter);
			
			std::shared_ptr<Transform<3>> newSubTransform();
			
			void setText(const std::string& text);
			
		private:
			weak_ptr<Transform<3>> m_transform;
			std::vector<std::shared_ptr<Transform<3>>> transforms;
			std::vector<std::shared_ptr<Panel>> letters;
	};
	
	class Button
	{
		public:
			Button(const shared_ptr<Transform<3>>& transform);
			
		private:
			weak_ptr<Transform<3>> m_transform;
	};

	
	class HUDPanelPass : public RenderingPass<Panel>
	{
		public:
			HUDPanelPass();
			
			void bind();
			
			void updateEntity(std::shared_ptr<Panel>& entity, const Camera& camera);
			
			void unbind();
			
			void addEntity(const Texture& texture, const shared_ptr<Transform<3>>& transform);	
	};
	
	class HUDLabelPass
	{
		public:
			void render(const Camera& camera);
			
			void bind();
			
			void unbind();
			
			void init(const std::shared_ptr<Assets>& assets);
			
			Label addLabel(const Texture& texture, const shared_ptr<Transform<3>>& transform, const int& maxNbChar);
		
			void setLabel(const int& labelIndex, const std::string& text);
		
		private:		
			HUDPanelPass panelPass;
	};

	class HUDPass : public IRenderingPass
	{
		public:
			virtual void render(const Camera& camera);
			
			virtual void init(const std::shared_ptr<Assets>& assets);
			
			void addPanel(const Texture& texture, const shared_ptr<Transform<3>>& transform);
			
			Label addLabel(const Texture& texture, const shared_ptr<Transform<3>>& transform, const int& length);
			
			Button addButton(const Texture& texture, const shared_ptr<Transform<3>>& transform);
			
		private:
			HUDPanelPass panels;
			HUDLabelPass labels;
	};
	
}