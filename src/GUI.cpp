#include "GUI.h"

namespace Medusa
{
	HUDPass::HUDPass():RenderingPass("HUD", std::make_shared<GLPassSettings>(true, false, false))
	{}
	
	void HUDPass::updateEntity(std::shared_ptr<EntityData>& entity, const Camera& camera)
	{
		entity->updateModel();
	}
	
	Panel::Panel(const std::shared_ptr<EntityData> entityData, const Circe::Vec2& position, const Circe::Vec2& dimension):RenderingHandler(entityData)
	{
		RenderingHandler::setUniform<Circe::Vec2>("textOffset", Circe::Vec2(0.0f, 0.0f));
		RenderingHandler::setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f, 1.0f));
		RenderingHandler::setUniform<Circe::Vec3>("setColor", Circe::Vec3(1.0f, 1.0f, 1.0f));
		Transform3 transform;
		transform.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, position(0), position(1), 0.0f));
		transform.setFrameScale(Circe::Direction3(Circe::REF_FRAME::GLOBAL, dimension(0), dimension(1), 1.0f));
		RenderingHandler::setTransform(transform);
	}
	
	/*Transform<3> Panel::getTransform() const
	{
		return m_entity.getTransform();
	}*/
	
	void Panel::setColor(const Circe::Vec3& color)
	{
		RenderingHandler::setUniform<Circe::Vec3>("setColor", color);
	}
	
	
	Label::Label(const std::shared_ptr<EntityData> entityData, const Circe::Vec2& position, const Circe::Vec2& dimension):RenderingHandler(entityData), color(1.0f, 1.0f, 1.0f)
	{
		RenderingHandler::setUniform<Circe::Vec2>("textOffset", Circe::Vec2(0.0f, 0.0f));
		RenderingHandler::setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f, 1.0f));
		RenderingHandler::setUniform<Circe::Vec3>("setColor", Circe::Vec3(1.0f, 1.0f, 1.0f));
		transform.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, position(0)-dimension(0), position(1)-dimension(1), 0.0f));
		transform.setFrameScale(Circe::Direction3(Circe::REF_FRAME::GLOBAL, dimension(0), dimension(1), 1.0f));
		RenderingHandler::setTransform(transform);
		setVisibility(false);
	}
	
	void Label::addCharacter(Panel& character, const float& offset)
	{
		//Panel character(Circe::Vec2(charPos, 0.0f), Circe::Vec2(1.0f, 1.0f), entity);
		//character.getTransform().attachTo(transform);
		//charPos+=2.0f;
		//character.setTransform(transform);
		characters.push_back(character);
	}
	
	void Label::setText(const std::string& text)
	{
		//for(int i = 0; i<Circe::min<int>(text.length(),characters.size()); i++)
		int i = 0;
		for(Panel character : characters)
		{		
			if(i<text.length())
			{
				int c = (int)text[i];
				character.setVisibility(c != (int)(' '));
				character.setUniform<Circe::Vec2>("textOffset", Circe::Vec2((c%16)/16.0f, (c/16)/16.0f));
				character.setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f/16.0f, 1.0f/16.0f));
				character.setColor(color);
			}
			else
			{
				character.setVisibility(false);
			}
			i++;
		}
	}
	
	void Label::setTextColor(const Circe::Vec3& newColor)
	{
		color = newColor;
		for(int i = 0; i<characters.size(); i++)
		{		
			characters[i].setColor(color);
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
	
	
	void SelectionListener::onSelection()
	{}
	
	void SelectionListener::onDeselection()
	{}
	
	
	FunctionalListener::FunctionalListener(const std::function<void(void)>& action):selectedAction(action), deselectedAction([](){})
	{}
	
	void FunctionalListener::onSelection()
	{
		selectedAction();
	}
	
	void FunctionalListener::onDeselection()
	{
		deselectedAction();
	}
	
	void FunctionalListener::setSelectedAction(const std::function<void(void)>& action)
	{
		selectedAction = action;
	}
	
	void FunctionalListener::setDeselectedAction(const std::function<void(void)>& action)
	{
		deselectedAction = action;
	}
	
	
	SelectableArea::SelectableArea(const std::shared_ptr<Area>& area):area(area)
	{}
	
	void SelectableArea::select()
	{
		for(std::shared_ptr<SelectionListener> listener : listeners)
		{
			if(listener != NULL)
			{
				listener->onSelection();
			}
		}
	}
	
	void SelectableArea::addSelectionListener(std::shared_ptr<SelectionListener> listener)
	{
		listeners.add(listener);
	}
	
	bool SelectableArea::isInBound(const float& x, const float& y) const
	{
		return area->isInBound(x,y);
	}
	
	
	
	Button::Button(const Panel& panel, const std::function<void(void)>& action):SelectableArea(std::make_shared<AABBArea>(panel.getPosition()(0)-panel.getSize()(0), panel.getPosition()(0)+panel.getSize()(0), panel.getPosition()(1)-panel.getSize()(1), panel.getPosition()(1)+panel.getSize()(1))), panel(panel)
	{
		listener = std::make_shared<FunctionalListener>(action);
		SelectableArea::addSelectionListener(listener);
	}
	
	void Button::setBackgroundColor(const Circe::Vec3& color)
	{
		panel.setColor(color);
	}
	
	GUI::GUI(const std::shared_ptr<HUDPass>& hudPass):hudPass(hudPass), font("font.png")
	{}
	
	Panel GUI::addPanel(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension)
	{
		if(std::shared_ptr<HUDPass> pass = hudPass.lock())
		{
			//Panel newPanel(position, dimension, pass->addEntity(texture));
			Panel newPanel = pass->addEntity<Panel>(texture, position, dimension);
			panels.push_back(newPanel);
			return newPanel;
		}
		return panels.at(0);
	}
	
	Label GUI::addLabel(const int& nbChar, const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension)
	{
		if(std::shared_ptr<HUDPass> pass = hudPass.lock())
		{
			//Label newLabel(position, dimension);
			float charPos = 0.0f;
			Label newLabel = pass->addEntity<Label>(texture, position, dimension);
			for(int i = 0; i<nbChar; i++)
			{
				Panel character = pass->addEntity<Panel>(texture, Circe::Vec2(charPos, 0.0f), Circe::Vec2(1.0f, 1.0f));
				newLabel.addCharacter(character, charPos);
				charPos+=0.1f;
				character.setPosition(Vec3(charPos, 0.0f, 0.0f));
				character.attachTo(newLabel);
			}
			labels.push_back(newLabel);
			return newLabel;
		}
		return labels.at(0);
	}
	
	Label GUI::addLabel(const std::string& text, const float& posX, const float& posY, const float& size)
	{
		Label res = addLabel(text.size(), font, Circe::Vec2(posX, posY), Circe::Vec2(size, size));
		res.setText(text);
		return res;
	}
	
	void GUI::onLeftClick()
	{
		for(SelectableArea area : selectableAreas)
		{
			if(area.isInBound(2.0f*getX()-1.0f, -2.0f*getY()+1.0f))
			{
				area.select();
			}
		}
	}
	
	void GUI::addSelectableArea(const SelectableArea& area)
	{
		selectableAreas.push_back(area);
	}

	Button GUI::addButton(const std::string& texture, const Circe::Vec2& position, const Circe::Vec2& dimension, const std::function<void(void)>& action)
	{
		if(std::shared_ptr<HUDPass> pass = hudPass.lock())
		{
			//Panel newPanel(position, dimension, pass->addEntity(texture));
			Panel newPanel = pass->addEntity<Panel>(texture, position, dimension);
			Button newButton(newPanel, action);
			addSelectableArea(newButton);
			return newButton;
		}
		return Button(Panel(NULL, position, dimension), action);
	}
	
	void GUI::setFont(const std::string& newFont)
	{
		font = newFont;
	}
}
