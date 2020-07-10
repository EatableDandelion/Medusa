#include "RenderingEntity.h"
#include <algorithm>

namespace Medusa
{	
	
	/*SpriteComponent::SpriteComponent()
	{
		
	}
	
	SpriteComponent::~SpriteComponent()
	{
		
	}
	
	void SpriteComponent::update(World& world, EntityHandle& entity)
	{
		
	}*/
	//int Entity::allid=0;

	EntityData::EntityData(const Mesh& mesh):Entity(), m_mesh(mesh), visible(true)
	{
		material = std::make_shared<Material>();
	}

	EntityData::EntityData(const EntityData& other):Entity(other), m_mesh(other.m_mesh), material(other.material), visible(other.visible)
	{}
	
	void EntityData::updateModel()
	{
		setUniform("Model", Entity::getTransformMatrix());
	}
	
	void EntityData::updateMVP(const Circe::Mat<4>& viewProjection)
	{
		setUniform("MVP", viewProjection*Entity::getTransformMatrix());		
	}
	
	void EntityData::draw(const int& culling)
	{
		if(visible)
		{
			m_mesh.draw(culling);
		}
	}
	
	void EntityData::setTexture(const TextureType& type, const Texture& texture)
	{
		material->setTexture(type, texture);
	} 
	
	std::shared_ptr<Material> EntityData::getMaterial() const
	{ 
		return material;
	}
	
	void EntityData::setVisibility(const bool& visibility)
	{
		visible = visibility;
	}
	/*
	int EntityData::getId() const
	{
		return id;
	}
	
	void EntityData::updateModelMatrix(const Mat<4>& modelMatrix)
	{
		model = modelMatrix;
	}*/
	/*
	void EntityData::setTransform(const Transform3& transform)
	{
		m_transform = transform;
	}
	
	Direction3 EntityData::getSize() const
	{
		return m_transform.getFrameScale();
	}
	
	void EntityData::setSize(const Vec3& size)
	{
		m_transform.setFrameScale(size(0),size(1),size(2));
	}
	
	Position3 EntityData::getPosition() const
	{
		return m_transform.getFramePosition();
	}
	
	void EntityData::setPosition(const Vec3& position)
	{
		m_transform.setFramePosition(position(0),position(1),position(2));
	}
	
	void EntityData::setRotation(const Vec3& leftAxis, const Vec3& fwdAxis)
	{
		m_transform.setFrameRotation(leftAxis, fwdAxis);
	}
	
	void EntityData::attachTo(const std::shared_ptr<Entity>& parentEntity)
	{
		parent = parentEntity;
	}
	 */
	
	RenderingHandler::RenderingHandler(const std::shared_ptr<EntityData> entityData):entity(entityData)
	{}
	
	RenderingHandler::RenderingHandler(const RenderingHandler& other):entity(other.entity)
	{}
	
	/*void RenderingHandler::updateModelMatrix(const Mat<4>& modelMatrix)
	{
		entity->updateModelMatrix(modelMatrix);
	}*/
		
	void RenderingHandler::draw(const int& culling)
	{
		entity->draw(culling);
	}
	
	void RenderingHandler::updateModel()
	{
		entity->updateModel();
	}
	
	void RenderingHandler::updateMVP(const Circe::Mat<4>& viewProjection)
	{
		entity->updateMVP(viewProjection);
	}
	
	void RenderingHandler::setTexture(const TextureType& type, const Texture& texture)
	{
		entity->setTexture(type, texture);
	}
	
	std::shared_ptr<Material> RenderingHandler::getMaterial() const
	{
		return entity->getMaterial();
	}
	
	void RenderingHandler::setVisibility(const bool& visibility)
	{
		entity->setVisibility(visibility);
	}
	
	int RenderingHandler::getId() const
	{
		return entity->getId();
	}
	
	void RenderingHandler::setTransform(const Transform3& transform)
	{
		entity->setTransform(transform);
	}
	
	Direction3 RenderingHandler::getSize() const
	{
		return entity->getSize();
	}
	
	void RenderingHandler::setSize(const Vec3& size)
	{
		entity->setSize(size);
	}
	
	Position3 RenderingHandler::getPosition() const
	{
		return entity->getPosition();
	}
	
	void RenderingHandler::setPosition(const Vec3& position)
	{
		entity->setPosition(position);
	}
	
	void RenderingHandler::setRotation(const Vec3& leftAxis, const Vec3& fwdAxis)
	{
		entity->setRotation(leftAxis, fwdAxis);
	}
	
	void RenderingHandler::attachTo(const RenderingHandler& parentEntity)
	{
		entity->attachTo(parentEntity.entity);
	}
	
	

	void EntityLoader::load(const std::string& folderLocation, const std::string& fileName)//, RenderingEntity& entity)
	{
		ifstream file;
		file.open(folderLocation+fileName);
		std::string line;
		while(std::getline(file, line))
		{
			std::istringstream iss(line);
			std::vector<std::string> tokens((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
			std::string varValue = tokens[1];
			varValue.erase(remove(varValue.begin(), varValue.end(), '\"'), varValue.end());
			loadComponent(tokens[0], varValue);
		}
		file.close();
	}
	
	void EntityLoader::loadComponent(const std::string& type, const std::string& value)
	{
		std::cout << value << std::endl;
		if(type.find("MESH") != std::string::npos)
		{
			
		}
		else if(type.find("TEXTURE") != std::string::npos)
		{
			int slot = 0;
			if(type.find("DIFFUSE") != std::string::npos)
			{
				slot = 0;
			}
			else if(type.find("NORMAL") != std::string::npos)
			{
				slot = 1;
			}
			
		}
		else
		{
			CIRCE_ERROR(type+" not recognized.");
		}
	}
	
	void EntityLoader::unload(RenderingHandler& entity)
	{}
}
