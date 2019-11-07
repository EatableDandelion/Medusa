#include "HUD.h"

namespace Medusa
{
	
	Panel::Panel(const Mesh& mesh, const shared_ptr<Transform<3>>& transform):RenderingEntity(mesh, transform)	
	{
		RenderingEntity::setUniform<Circe::Vec2>("textOffset", Circe::Vec2(0.0f, 0.0f));
		RenderingEntity::setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f, 1.0f));
	}
	
	Label::Label(const shared_ptr<Transform<3>>& transform):m_transform(weak_ptr<Transform<3>>(transform))
	{}
			
	void Label::addCharacter(const std::shared_ptr<Panel>& letter)
	{
		if(auto transform = m_transform.lock())
		{		
			letters.push_back(letter);
		}
	}
	
	std::shared_ptr<Transform<3>> Label::newSubTransform()
	{
		std::shared_ptr<Transform<3>> subTransform = std::make_shared<Transform<3>>();
		
		if(auto transform = m_transform.lock())
		{
			subTransform->translate(Circe::Direction<3>(Circe::REF_FRAME::LOCAL, 1.0f*transforms.size(), 0.0f, 0.0f));		
			subTransform->attachTo(transform);
			transforms.push_back(subTransform);
		}

		return subTransform;
	}
	
	void Label::setText(const std::string& text)
	{
		for(int i = 0; i<min(text.length(),letters.size()); i++)
		{		
			int c = (int)text[i];
			letters[i]->setVisibility(c != (int)(' '));
			letters[i]->setUniform<Circe::Vec2>("textOffset", Circe::Vec2((c%16)/16.0f, (c/16)/16.0f));
			letters[i]->setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f/16.0f, 1.0f/16.0f));
			letters[i]->setUniform<Circe::Vec3>("setColor", Circe::Vec3(0.8f, 0.9f, 0.1f));
		}
		for(int i = text.length(); i<letters.size(); i++)
		{
			letters[i]->setVisibility(false);
		}
	}
	
	Button::Button(const shared_ptr<Transform<3>>& transform):m_transform(weak_ptr<Transform<3>>(transform))
	{}
	
	HUDPanelPass::HUDPanelPass():RenderingPass("HUD")
	{}
	
	void HUDPanelPass::bind()
	{
		glDepthMask(false);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
	}
	
	void HUDPanelPass::updateEntity(std::shared_ptr<Panel>& entity, const Camera& camera)
	{
		entity->updateModel();
	}
	
	void HUDPanelPass::unbind()
	{
		glDepthFunc(GL_LESS);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(true);	
	}
	
	void HUDPanelPass::addEntity(const Texture& texture, const shared_ptr<Transform<3>>& transform)
	{
		Mesh screenMesh = RenderingPass::getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		std::shared_ptr<Panel> entity = RenderingPass::createEntity(screenMesh, transform);
		entity->setTexture(TextureType::DIFFUSE0, texture);		
	}
	
	
	void HUDLabelPass::render(const Camera& camera)
	{
		panelPass.render(camera);
	}
			
	void HUDLabelPass::init(const std::shared_ptr<Assets>& assets)
	{
		panelPass.init(assets);
	}
	
	void HUDLabelPass::bind()
	{
		glDepthMask(false);
		glDisable(GL_DEPTH_TEST);	
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_DST_ALPHA);
	}
	
	void HUDLabelPass::unbind()
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
	
	Label HUDLabelPass::addLabel(const Texture& texture, const shared_ptr<Transform<3>>& transform, const int& maxNbChar)
	{	
		//std::shared_ptr<Label> label = std::make_shared<Label>(transform);
		//labels.push_back(label);
		Label label(transform);
		Mesh screenMesh = panelPass.getAssets()->getMesh("plane.obj", Medusa::TRIANGLE_RENDERING);
		for(int i = 0; i<maxNbChar; i++)
		{
			std::shared_ptr<Panel> character = panelPass.createEntity(screenMesh, label.newSubTransform());
			character->setTexture(TextureType::DIFFUSE0, texture);		
			character->setUniform<Circe::Vec2>("textOffset", Circe::Vec2(0.0f, 0.0f));
			character->setUniform<Circe::Vec2>("textSize", Circe::Vec2(1.0f, 1.0f));
			label.addCharacter(character);		
		}
		return label;
	}
	
	void HUDLabelPass::setLabel(const int& labelIndex, const std::string& text)
	{
		//labels[labelIndex]->setText(text);
	}
	

	
	void HUDPass::render(const Camera& camera)
	{
		panels.render(camera);
		labels.bind();
		labels.render(camera);
		labels.unbind();
	}
	
	void HUDPass::init(const std::shared_ptr<Assets>& assets)
	{
		panels.init(assets);
		labels.init(assets);
	}
	
	void HUDPass::addPanel(const Texture& texture, const shared_ptr<Transform<3>>& transform)
	{
		panels.addEntity(texture, transform);
	}
	
	Label HUDPass::addLabel(const Texture& texture, const shared_ptr<Transform<3>>& transform, const int& length)
	{
		return labels.addLabel(texture, transform, length);
	}
	
	Button HUDPass::addButton(const Texture& texture, const shared_ptr<Transform<3>>& transform)
	{
		panels.addEntity(texture, transform);
		return Button(transform);
	}
}
