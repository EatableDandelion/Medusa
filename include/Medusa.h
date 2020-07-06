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
			MedusaInterface();
			
			void load(const char* resourceDirectory);
			
			void listenToInput(Mouse& mouse);
			
			void draw(const int& width, const int& height);
			
			RenderingHandler newWorldEntity(const std::string& mesh, const std::string& texture);
			
			void removeWorldEntity(const RenderingHandler& entity);

			DirectionalLight newDirectionalLight(const float& intensity, const float& r, const float& g, const float& b, const float& directionX, const float& directionY, const float& directionZ);
			
			AmbientLight newAmbientLight(const float& intensity, const float& r, const float& g, const float& b);
			
			RenderingHandler newDebugEntity(const Shape& shape);
			
			void removeDebugEntity(const RenderingHandler& entity);
			
			void setDebugLineThickness(const float& thickness);
			
			Camera getCamera();
			
			Label newLabel(const std::string& text, const float& posX, const float& posY, const float& size);
			
			Panel newPanel(const std::string& texture, const float& posX, const float& posY, const float& sizeX, const float& sizeY);
			
			Button newButton(const std::string& texture, const float& posX, const float& posY, const float& sizeX, const float& sizeY, const std::function<void(void)>& action);
			
			void listenTo(Mouse& mouse);
			
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
