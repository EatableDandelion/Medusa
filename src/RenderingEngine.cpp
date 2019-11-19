#include "RenderingEngine.h"
#include <initializer_list>

namespace Medusa
{
	using namespace Circe;
	using namespace std;
	

	RenderingEngine::RenderingEngine(const std::shared_ptr<IRenderingPass> geoPass, const std::shared_ptr<IRenderingPass> ppPass, const int& windowWidth, const int& windowHeight):geometryPass(geoPass), postProcessPass(ppPass), framebuffer(windowWidth, windowHeight), camera(2.0f, 2.0f)
	{}
	
	RenderingEngine::~RenderingEngine()
	{
		CIRCE_INFO("Rendering engine terminated");
	}
	
	void RenderingEngine::draw(const int& width, const int& height)
	{		
		camera.update(width, height);
		
		/** Geometry pass */
		framebuffer.bindForWrite();	
		geometryPass->renderAll(camera);
	
		/** Post-process passes */
		framebuffer.bindForRead();
		postProcessPass->renderAll(camera);
	}
	
	Camera& RenderingEngine::getCamera()
	{
		return camera;
	}
}
