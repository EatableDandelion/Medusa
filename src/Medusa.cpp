#include "Medusa.h"
#include <initializer_list>

namespace Medusa
{
	MedusaInterface::MedusaInterface(const int& windowWidth, const int& windowHeight):engine(windowWidth, windowHeight)
	{
		geometryPass = std::make_shared<GeometryPass>();
		ambientLights = std::make_shared<AmbientLightPass>();
		directionalLights = std::make_shared<DirectionalLightPass>();	
		debugPass = std::make_shared<DebugPass>();
		hudPass = std::make_shared<HUDPass>();
		
		debugPass->addNext(hudPass);
		directionalLights->addNext(debugPass);
		ambientLights->addNext(directionalLights);
		
		gui = std::make_shared<GUI>(hudPass);
	}
	
	void MedusaInterface::load(const std::string& meshFolder, const std::string& textureFolder, const std::string& shaderFolder)
	{
		assets = std::make_shared<Assets>(meshFolder, textureFolder, shaderFolder);
		assets->loadMesh("plane.obj", -1);
		assets->loadMesh("monkey.obj", 1);
		assets->loadMesh("sphere.obj", 1);
		assets->loadMesh("cube.obj", 1);
		
		assets->loadShader("GeometryPass");
		assets->loadShader("DirectionalLight");
		assets->loadShader("AmbientLight");
		assets->loadShader("DebugDisplay");
		assets->loadShader("HUD");
		
		assets->loadTexture("Warframe0000.jpg");
		assets->loadTexture("Warframe0002.jpg");
		assets->loadTexture("font.png");
		assets->loadTexture("blockIso.png");
		
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
	
	RenderingEntity MedusaInterface::addWorldEntity(const std::string& mesh, const std::string& texture, const std::shared_ptr<Transform3> transform)
	{
		return geometryPass->addEntity(mesh, texture);
	}
	
	std::shared_ptr<GUI> MedusaInterface::getGUI() const
	{
		return gui;
	}
	
	RenderingEntity MedusaInterface::addDebugEntity(const Shape& shape, std::shared_ptr<Transform3> transform)
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
	
	void MedusaInterface::setDebugLineThickness(const float& thickness)
	{
		debugPass->setLineThickness(thickness);
	}
}
