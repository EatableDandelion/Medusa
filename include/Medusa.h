#pragma once

#include <vector>
#include <memory>
#include "Window.h"

namespace Medusa
{	
	enum Shape
	{
		TRIANGLE,
		CIRCLE,
		SQUARE,
		HEXAGON
	};

	class MedusaInterface
	{
		public:
			MedusaInterface(const int& windowWidth, const int& windowHeight);
			
			void load(const std::string& meshFolder, const std::string& textureFolder, const std::string& shaderFolder);
			
			void listenToInput(Mouse& mouse);
			
			void draw(const int& width, const int& height);
			
			RenderingEntity addWorldEntity(const std::string& mesh, const std::string& texture, const std::shared_ptr<Transform3> transform);
			
			RenderingEntity addDebugEntity(const Shape& shape, std::shared_ptr<Transform3> transform);
			
			std::shared_ptr<GUI> getGUI() const;
			
			void setDebugLineThickness(const float& thickness);
			
		private:
			RenderingEngine engine;
			std::shared_ptr<Assets> assets;
			std::shared_ptr<GeometryPass> geometryPass;
			std::shared_ptr<AmbientLightPass> ambientLights;
			std::shared_ptr<DirectionalLightPass> directionalLights;
			std::shared_ptr<DebugPass> debugPass;
			std::shared_ptr<HUDPass> hudPass;
			std::shared_ptr<GUI> gui;
	};
}
