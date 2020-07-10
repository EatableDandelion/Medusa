#include "RenderingEngine.h"
#include <initializer_list>

namespace Medusa
{
	using namespace Circe;
	using namespace std;

	RenderingEngine::RenderingEngine(const int& windowWidth, const int& windowHeight):framebuffer(windowWidth, windowHeight), camera(2.0f, 2.0f)
	{}
	
	RenderingEngine::~RenderingEngine()
	{
		CIRCE_INFO("Rendering engine terminated");
	}
	
	void RenderingEngine::init(const std::shared_ptr<RenderingPass> geoPass, const std::shared_ptr<RenderingPass> ppPass, const std::shared_ptr<Assets> assets)
	{
		geometryPass = geoPass;
		postProcessPass = ppPass;
		geometryPass->init(assets);
		postProcessPass->init(assets);
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
