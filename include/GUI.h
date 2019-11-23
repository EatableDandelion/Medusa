#pragma once

#include <string>
#include <memory>
#include <stack>
#include <Circe/Circe.h>
#include "RenderingPass.h"
#include "Events.h"

namespace Medusa
{	
	class HUDPass : public RenderingPass<RenderingEntity, GLPassSettings>
	{
		public:
			HUDPass();
			
			void updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera);
			
			virtual std::shared_ptr<RenderingEntity> addEntity(const std::string& texture, const shared_ptr<Transform<3>>& transform);	
	};
	
	class Panel
	{
		public:
			Panel(const Circe::Vec2& position, const Circe::Vec2& dimension, const std::shared_ptr<RenderingEntity>& entity);
			
			std::shared_ptr<Transform<3>> getTransform() const;
			
			std::shared_ptr<RenderingEntity> getEntity() const;
			
		private:
			std::shared_ptr<RenderingEntity> m_entity;
			std::shared_ptr<Transform<3>> transform;
	};
	
	class Label
	{
		public:
			Label(const Circe::Vec2& position, const Circe::Vec2& dimension);
			
			Panel addCharacter(const std::shared_ptr<RenderingEntity>& entity);
			
			void setText(const std::string& text);
			
			std::vector<Panel> getCharacters() const;
			
		private:
			std::vector<Panel> characters;
			std::shared_ptr<Transform<3>> transform;
			float charPos;
			
	};
	
	
	class Button
	{
		public:
			Button(const Panel& panel);
			
			//void listenTo(const Mouse& mouse);
	
		private:
			Panel panel;
			Observer<Circe::Vec2> mouseObserver;
			
			//bool isInBound(const Circe::Vec2& cursorPosition);
	};
	
	class GUI
	{
		public:
			GUI(const std::shared_ptr<IRenderingPass>& hudPass, const std::shared_ptr<Messenger>& messenger);
			
			void update();
			
			Panel addPanel(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension);
			
			Label addLabel(const int& nbChar, const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension);
			
			Button addButton(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension);
					
		private:
			std::weak_ptr<IRenderingPass> hudPass;
			std::vector<Panel> panels;
			std::vector<Label> labels;
			std::vector<Button> buttons;
			std::shared_ptr<Subscriber> subscriber;
	};
}