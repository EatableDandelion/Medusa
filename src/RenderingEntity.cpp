#include "RenderingEntity.h"

namespace Medusa
{	
	int RenderingEntity::allid=0;
	
	RenderingEntity::RenderingEntity(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<ITransform>& transform):m_mesh(mesh), m_transform(weak_ptr<ITransform>(transform)), material(make_shared<Material>()), id(allid++)
	{
		CIRCE_INFO("Initializing entity "+std::to_string(id));
	}
	
	RenderingEntity::~RenderingEntity()
	{
		CIRCE_INFO("Terminating entity "+std::to_string(id));
	}
	
	void RenderingEntity::render(Shader& renderingPass, Camera& camera)
	{
	}
	
	void RenderingEntity::draw()
	{
		m_mesh->draw();
	}
	
	bool RenderingEntity::update(const Mat<4>& projectionMatrix)
	{		
		if(auto transform = m_transform.lock()){
			Mat<4> m = transform->getTransformMatrix();
			setUniform("MVP", projectionMatrix*m);
			return true;
		}else{
			return false;
		}
	}
	
	void RenderingEntity::setTexture(const Texture& texture)
	{
		material->setTexture(texture);
	}
	
	std::shared_ptr<Material> RenderingEntity::getMaterial() const
	{
		return material;
	}
}
