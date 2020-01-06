#include "RenderingEntity.h"
#include <algorithm>

namespace Medusa
{	
	int EntityData::allid=0;
	
	EntityData::EntityData(const Mesh& mesh): m_mesh(mesh), id(allid++), visible(true)
	{
		material = std::make_shared<Material>();
		CIRCE_INFO("Initializing entity "+std::to_string(id));
	}
	
	EntityData::EntityData(const EntityData& other):m_mesh(other.m_mesh), m_transform(other.m_transform), material(other.material), id(other.id), visible(other.visible)
	{}
	
	EntityData::~EntityData()
	{
		CIRCE_INFO("Terminating entity "+std::to_string(id));
	}
	
	void EntityData::updateModel()
	{
		setUniform("Model", getTransformMatrix());
	}
	
	void EntityData::updateMVP(const Circe::Mat<4>& viewProjection)
	{
		setUniform("MVP", viewProjection*getTransformMatrix());		
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
	
	Mat<4> EntityData::getTransformMatrix() const
	{
		if(std::shared_ptr<EntityData> papa = parent.lock())
			return m_transform.getTransformMatrix()*papa->m_transform.getTransformMatrix();
		return m_transform.getTransformMatrix();
	}
	
	/*void EntityData::attachTo(const std::shared_ptr<Transform3>& parentTransform)
	{
		m_transform.attachTo(parentTransform);
	}*/
	
	int EntityData::getId() const
	{
		return id;
	}
	
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
	
	void EntityData::attachTo(const std::shared_ptr<EntityData>& parentEntity)
	{
		//m_transform.attachTo(std::shared_ptr<Transform3>(&(parentEntity->m_transform)));
		parent = parentEntity;
	}
	
	
	RenderingEntity::RenderingEntity(const std::shared_ptr<EntityData> entityData):entity(entityData)
	{}
	
	RenderingEntity::RenderingEntity(const RenderingEntity& other):entity(other.entity)
	{}
			
	void RenderingEntity::draw(const int& culling)
	{
		entity->draw(culling);
	}
	
	void RenderingEntity::updateModel()
	{
		entity->updateModel();
	}
	
	void RenderingEntity::updateMVP(const Circe::Mat<4>& viewProjection)
	{
		entity->updateMVP(viewProjection);
	}
	
	void RenderingEntity::setTexture(const TextureType& type, const Texture& texture)
	{
		entity->setTexture(type, texture);
	}
	
	std::shared_ptr<Material> RenderingEntity::getMaterial() const
	{
		return entity->getMaterial();
	}

	/*void RenderingEntity::attachTo(const std::shared_ptr<Transform3>& parentTransform)
	{
		entity->attachTo(parentTransform);
	}*/
	
	void RenderingEntity::setVisibility(const bool& visibility)
	{
		entity->setVisibility(visibility);
	}
	
	int RenderingEntity::getId() const
	{
		return entity->getId();
	}
	
	void RenderingEntity::setTransform(const Transform3& transform)
	{
		entity->setTransform(transform);
	}
	
	Direction3 RenderingEntity::getSize() const
	{
		return entity->getSize();
	}
	
	void RenderingEntity::setSize(const Vec3& size)
	{
		entity->setSize(size);
	}
	
	Position3 RenderingEntity::getPosition() const
	{
		return entity->getPosition();
	}
	
	void RenderingEntity::setPosition(const Vec3& position)
	{
		entity->setPosition(position);
	}
	
	void RenderingEntity::attachTo(const RenderingEntity& parentEntity)
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
	
	void EntityLoader::unload(RenderingEntity& entity)
	{}
}
