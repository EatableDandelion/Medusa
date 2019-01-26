#include "RenderingEntity.h"

namespace Medusa
{	
	int RenderingEntity::allid=0;
	
	RenderingEntity::RenderingEntity(const std::shared_ptr<Mesh>& mesh, ITransform* transform):m_mesh(mesh), m_transform(transform), material(make_shared<Material>()), id(allid++)
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
	
	Mat<4> RenderingEntity::getModelMatrix()
	{
		return m_transform->getTransformMatrix();
	}
	
	std::shared_ptr<Material> RenderingEntity::getMaterial() const
	{
		return material;
	}
}
