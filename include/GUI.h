#pragma once

#include <string>
#include <memory>
#include <stack>
#include <Circe/Circe.h>
#include "RenderingPass.h"
#include "Events.h"
#include "Input.h"

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
	
	class Area
	{
		public:
			virtual bool isInBound(const float& x, const float& y) const = 0;
	};
	
	class AABBArea : public Area
	{
		public:
			AABBArea(const float& xmin, const float& xmax, const float& ymin, const float& ymax);
			
			virtual bool isInBound(const float& x, const float& y) const;
		
		private:
			float xmin, xmax, ymin, ymax;
	};
	
	class SelectableArea
	{
		public:
			SelectableArea(const std::shared_ptr<Area>& area);
		
			void setSelectedAction(const std::function<void(void)>& action);
			void setDeselectedAction(const std::function<void(void)>& action);
			
			void select();
			void deselect();
			
			bool isInBound(const float& x, const float& y) const;
			
		private:
			std::shared_ptr<Area> area;
			std::function<void(void)> selectedAction;
			std::function<void(void)> deselectedAction;		
	};
	
	class Button
	{
		public:
			Button(const Panel& panel, const std::function<void(void)>& action);
			
			void press();
			
			bool isInBound(const float& x, const float& y) const;
			
		private:
			Panel panel;
			SelectableArea selectableArea;
	};
	
	class GUI : public MouseListener
	{
		public:
			GUI(const std::shared_ptr<IRenderingPass>& hudPass);
			
			Panel addPanel(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension);
			
			Label addLabel(const int& nbChar, const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension);
			
			Button addButton(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension, const std::function<void(void)>& action);
			
			virtual void onLeftClick();
			
		private:
			std::weak_ptr<IRenderingPass> hudPass;
			std::vector<Panel> panels;
			std::vector<Label> labels;
			std::vector<Button> buttons;
	};
}