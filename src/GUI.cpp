#include "GUI.h"

namespace Medusa
{
	HUDPass::HUDPass():RenderingPass("HUD", true, false, false)
	{}
	
	void HUDPass::updateEntity(std::shared_ptr<RenderingEntity>& entity, const Camera& camera)
	{
		entity->updateModel();
	}
	
	std::shared_ptr<RenderingEntity> HUDPass::addEntity(const  std::string& texture, const shared_ptr<Transform<3>>& transform)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		std::shared_ptr<RenderingEntity> entity = RenderingPass::createEntity(screenMesh, transform);
		entity->setTexture(TextureType::DIFFUSE0, RenderingPass::getAssets()->getTexture(texture));
		entity->setUniform<Circe::Vec2>("textOffset", Circe::Vec2(0.0f, 0.0f));
		entity->setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f, 1.0f));
		entity->setUniform<Circe::Vec3>("setColor", Circe::Vec3(1.0f, 1.0f, 1.0f));
		return entity;
	}
	
	
	Panel::Panel(const Circe::Vec2& position, const Circe::Vec2& dimension, const std::shared_ptr<RenderingEntity>& entity):m_entity(entity)
	{
		transform = std::make_shared<Transform<3>>();
		transform->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, position(0), position(1), 0.0f));
		transform->setFrameScale(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, dimension(0), dimension(1), 1.0f));
		entity->setTransform(transform);
	}
	
	std::shared_ptr<RenderingEntity> Panel::getEntity() const
	{
		return m_entity;
	}
	
	std::shared_ptr<Transform<3>> Panel::getTransform() const
	{
		return transform;
	}
	
	Label::Label(const Circe::Vec2& position, const Circe::Vec2& dimension)
	{
		transform = std::make_shared<Transform<3>>();
		transform->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, position(0)-dimension(0), position(1)-dimension(1), 0.0f));
		transform->setFrameScale(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, dimension(0), dimension(1), 1.0f));
	}
	
	Panel Label::addCharacter(const std::shared_ptr<RenderingEntity>& entity)
	{
		Panel character(Circe::Vec2(charPos, 0.0f), Circe::Vec2(1.0f, 1.0f), entity);
		character.getTransform()->attachTo(transform);
		charPos+=2.0f;
		characters.push_back(character);
		return character;
	}
	
	void Label::setText(const std::string& text)
	{
		for(int i = 0; i<Circe::min<int>(text.length(),characters.size()); i++)
		{		
			int c = (int)text[i];
			characters[i].getEntity()->setVisibility(c != (int)(' '));
			characters[i].getEntity()->setUniform<Circe::Vec2>("textOffset", Circe::Vec2((c%16)/16.0f, (c/16)/16.0f));
			characters[i].getEntity()->setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f/16.0f, 1.0f/16.0f));
			characters[i].getEntity()->setUniform<Circe::Vec3>("setColor", Circe::Vec3(0.8f, 0.9f, 0.1f));
		}
		for(int i = text.length(); i<characters.size(); i++)
		{
			characters[i].getEntity()->setVisibility(false);
		}
	}
	
	std::vector<Panel> Label::getCharacters() const
	{
		return characters;
	}
	
	
	AABBArea::AABBArea(const float& xmin, const float& xmax, const float& ymin, const float& ymax):xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
	{}
	
	bool AABBArea::isInBound(const float& x, const float& y) const
	{
		if(x < xmin || x > xmax || y < ymin || y > ymax) return false;		
		return true;
	}
	
	
	SelectableArea::SelectableArea(const std::shared_ptr<Area>& area):area(area), selectedAction([](){}), deselectedAction([](){})
	{}
	
	void SelectableArea::setSelectedAction(const std::function<void(void)>& action)
	{
		selectedAction = action;
	}
	
	void SelectableArea::setDeselectedAction(const std::function<void(void)>& action)
	{
		deselectedAction = action;
	}
	
	void SelectableArea::select()
	{
		selectedAction();
	}
	
	void SelectableArea::deselect()
	{
		deselectedAction();
	}
	
	bool SelectableArea::isInBound(const float& x, const float& y) const
	{
		return area->isInBound(x,y);
	}
	
	
	
	Button::Button(const Panel& panel, const std::function<void(void)>& action):panel(panel), selectableArea(std::make_shared<AABBArea>(panel.getTransform()->getFramePosition()(0)-panel.getTransform()->getFrameScale()(0), panel.getTransform()->getFramePosition()(0)+panel.getTransform()->getFrameScale()(0), panel.getTransform()->getFramePosition()(1)-panel.getTransform()->getFrameScale()(1), panel.getTransform()->getFramePosition()(1)+panel.getTransform()->getFrameScale()(1)))
	{
		selectableArea.setSelectedAction(action);
	}
	
	bool Button::isInBound(const float& x, const float& y) const
	{
		return selectableArea.isInBound(x, y);
	}
	
	void Button::press()
	{
		selectableArea.select();
	}
	
	GUI::GUI(const std::shared_ptr<IRenderingPass>& hudPass):hudPass(hudPass)
	{
	}
	
	Panel GUI::addPanel(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension)
	{
		if(std::shared_ptr<IRenderingPass> pass = hudPass.lock())
		{
			Panel newPanel(position, dimension, pass->addEntity(texture, NULL));
			panels.push_back(newPanel);
			return newPanel;
		}
		return panels.at(0);
	}
	
	Label GUI::addLabel(const int& nbChar, const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension)
	{
		if(std::shared_ptr<IRenderingPass> pass = hudPass.lock())
		{
			Label newLabel(position, dimension);
			for(int i = 0; i<nbChar; i++)
			{
				newLabel.addCharacter(pass->addEntity(texture, NULL));
			}
			labels.push_back(newLabel);
			return newLabel;
		}
		return labels.at(0);
	}
	
	void GUI::onLeftClick()
	{
		for(Button button : buttons)
		{
			if(button.isInBound(2.0f*getX()-1.0f, -2.0f*getY()+1.0f))
			{
				button.press();
			}
		}
	}

	Button GUI::addButton(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension, const std::function<void(void)>& action)
	{
		if(std::shared_ptr<IRenderingPass> pass = hudPass.lock())
		{
			Panel newPanel(position, dimension, pass->addEntity(texture, NULL));
			Button newButton(newPanel, action);
			buttons.push_back(newButton);
			return newButton;
		}
		return buttons.at(0);
	}	
}
