#include "Medusa.h"
#include <initializer_list>

namespace Medusa
{
	
	
	MedusaInterface::MedusaInterface():engine(600,400), geometryPass(std::make_shared<GeometryPass>()), ambientLights(std::make_shared<AmbientLightPass>()),
		directionalLights(std::make_shared<DirectionalLightPass>()),	
		debugPass(std::make_shared<DebugPass>()),
		hudPass(std::make_shared<HUDPass>())
	{
		debugPass->addNext(hudPass);
		directionalLights->addNext(debugPass);
		ambientLights->addNext(directionalLights);
		
		gui = std::make_shared<GUI>(hudPass);
	}
	
	void MedusaInterface::load(const char* resourceDirectory)
	{
		assets = std::make_shared<Assets>();
		assets->loadDirectory(resourceDirectory);
		engine.init(geometryPass, ambientLights, assets);
	}
	
	void MedusaInterface::listenToInput(Mouse& mouse)
	{
		mouse.addListener(gui);
	}
	
	void MedusaInterface::draw(const int& width, const int& height)
	{
		engine.draw(width, height);		
	}
	
	RenderingHandler MedusaInterface::newWorldEntity(const std::string& mesh, const std::string& texture)
	{
		return geometryPass->addEntity(mesh, texture);
	}
	
	void MedusaInterface::removeWorldEntity(const RenderingHandler& entity)
	{
		geometryPass->removeEntity<RenderingHandler>(entity);
	}
	
	DirectionalLight MedusaInterface::newDirectionalLight(const float& intensity, const float& r, const float& g, const float& b, const float& directionX, const float& directionY, const float& directionZ)
	{
		return directionalLights->addEntity(intensity, Circe::Vec3(r,g,b), Circe::Vec3(directionX, directionY, directionZ));
	}
	
	AmbientLight MedusaInterface::newAmbientLight(const float& intensity, const float& r, const float& g, const float& b)
	{
		return ambientLights->addEntity(intensity, Circe::Vec3(r,g,b));
	}		
	
	RenderingHandler MedusaInterface::newDebugEntity(const Shape& shape)
	{
		std::string meshFile;
		if(shape == Shape::TRIANGLE)
		{
			meshFile = "triangle.obj";
		}
		else if(shape == Shape::CIRCLE)
		{
			meshFile = "circle.obj";
		}
		else if(shape == Shape::SQUARE)
		{
			meshFile = "square.obj";
		}
		else if(shape == Shape::HEXAGON)
		{
			meshFile = "hexagon.obj";
		}
		return debugPass->addEntity(meshFile);
	}
	
	void MedusaInterface::removeDebugEntity(const RenderingHandler& entity)
	{
		debugPass->removeEntity<RenderingHandler>(entity);
	}
	
	void MedusaInterface::setDebugLineThickness(const float& thickness)
	{
		debugPass->setLineThickness(thickness);
	}
	
	Camera MedusaInterface::getCamera()
	{
		return engine.getCamera();
	}
	
	Label MedusaInterface::newLabel(const std::string& text, const float& posX, const float& posY, const float& size)
	{
		return gui->addLabel(text, posX, posY, size);
	}
			
	Panel MedusaInterface::newPanel(const std::string& texture, const float& posX, const float& posY, const float& sizeX, const float& sizeY)
	{
		return gui->addPanel(texture, Circe::Vec2(posX, posY), Circe::Vec2(sizeX, sizeY));
	}
	
	
	Button MedusaInterface::newButton(const std::string& texture, const float& posX, const float& posY, const float& sizeX, const float& sizeY, const std::function<void(void)>& action)
	{
		return gui->addButton(texture, Circe::Vec2(posX, posY), Circe::Vec2(sizeX, sizeY), action);
	}
	
	void MedusaInterface::listenTo(Mouse& mouse)
	{
		mouse.addListener(gui);
	}
}
