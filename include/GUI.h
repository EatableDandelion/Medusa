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
			
			void setColor(const Circe::Vec3& color);
			
		private:
			std::shared_ptr<RenderingEntity> m_entity;
			std::shared_ptr<Transform<3>> transform;
	};
	
	class Label
	{
		public:
			Label(const Circe::Vec2& position, const Circe::Vec2& dimension, const Circe::Vec3& color = Circe::Vec3(1.0f, 1.0f, 1.0f));
			
			Panel addCharacter(const std::shared_ptr<RenderingEntity>& entity);
			
			void setText(const std::string& text);
			
			void setTextColor(const Circe::Vec3& color);
			
			std::vector<Panel> getCharacters() const;
			
		private:
			std::vector<Panel> characters;
			std::shared_ptr<Transform<3>> transform;
			float charPos;
			Circe::Vec3 color;
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
	
	class SelectionListener
	{
		public:
			virtual void onSelection();
			virtual void onDeselection();
	};
	
	class FunctionalListener : public SelectionListener
	{
		public:
			FunctionalListener(const std::function<void(void)>& action = ([](){}));
			virtual void onSelection();
			virtual void onDeselection();
			void setSelectedAction(const std::function<void(void)>& action);
			void setDeselectedAction(const std::function<void(void)>& action);
		
		private:
			std::function<void(void)> selectedAction;
			std::function<void(void)> deselectedAction;	
	};	
	
	class SelectableArea
	{
		public:
			SelectableArea(const std::shared_ptr<Area>& area);			
			
			void select();
			
			void addSelectionListener(std::shared_ptr<SelectionListener> listener);
			
			bool isInBound(const float& x, const float& y) const;
			
		private:
			std::shared_ptr<Area> area;
			WeakCollection<SelectionListener> listeners;
	};
	
	class Button : public SelectableArea
	{
		public:
			Button(const Panel& panel, const std::function<void(void)>& action);
			
			void setBackgroundColor(const Circe::Vec3& color);
			
		private:
			Panel panel;
			std::shared_ptr<SelectionListener> listener;
	};
	
	class GUI : public MouseListener
	{
		public:
			GUI(const std::shared_ptr<IRenderingPass>& hudPass);
			
			Panel addPanel(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension);
			
			Label addLabel(const int& nbChar, const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension);
			
			Label addLabel(const std::string& text, const float& posX, const float& posY, const float& size);
			
			Button addButton(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension, const std::function<void(void)>& action);
			
			void addSelectableArea(const SelectableArea& area);
			
			virtual void onLeftClick();
			
			void setFont(const std::string& font);
			
		private:
			std::weak_ptr<IRenderingPass> hudPass;
			std::vector<Panel> panels;
			std::vector<Label> labels;
			//std::vector<Button> buttons;
			std::vector<SelectableArea> selectableAreas;
			std::string font;
	};
}