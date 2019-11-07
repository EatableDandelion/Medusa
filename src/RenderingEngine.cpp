#include "RenderingEngine.h"
#include <initializer_list>

namespace Medusa
{
	using namespace Circe;
	using namespace std;
	

	RenderingEngine::RenderingEngine(const int& windowWidth, const int& windowHeight, const VertexSpecs& specs):m_window(windowWidth, windowHeight, "test"), assets(std::make_shared<Assets>("../../Resource/Mesh/", "../../Resource/Texture/", "../../Resource/Shader/", specs)),camera(2.0f, 2.0f), framebuffer(windowWidth, windowHeight)//, geometryPass(assets), debugPass(assets), ambientLights(assets), directionalLights(assets), huds(assets)
	{
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
		
		geometryPass.init(assets);
		debugPass.init(assets);
		directionalLights.init(assets);
		ambientLights.init(assets);
		huds.init(assets);

		directionalLights.addEntity(0.5f, Circe::Vec3(1.0f,1.0f,1.0f), Circe::Vec3(1.0f,0.8f,1.0f));
		ambientLights.addEntity(0.1f, Circe::Vec3(1.0f,1.0f,1.0f));
	}
	
	RenderingEngine::~RenderingEngine()
	{
		CIRCE_INFO("Rendering engine terminated");
	}
	
	void RenderingEngine::draw()
	{		
		m_window.update(camera);
		
		/** Geometry pass */
		framebuffer.bindForWrite();	
		geometryPass.render(camera);
		
		/** Lighting passes */
		framebuffer.bindForRead();
		ambientLights.render(camera);
		directionalLights.render(camera);

		/** Debug pass */
		debugPass.render(camera);
		
		/** HUD */
		huds.render(camera);
		
		m_window.swapBuffers();
	}
	
	void RenderingEngine::addWorldEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<Transform<3>>& transform)
	{
		geometryPass.addEntity(assets->getMesh(meshName, Medusa::TRIANGLE_RENDERING), assets->getTexture(textureName), transform);
	}
	
	void RenderingEngine::addDebugEntity(const std::string& meshName, const std::shared_ptr<Transform<3>>& transform)
	{
		debugPass.addEntity(assets->getMesh(meshName, Medusa::WIRE_RENDERING), transform);
	}
	
	void RenderingEngine::addHUDPanel(const std::shared_ptr<Transform<3>> transform, const std::string& texture)
	{
		huds.addPanel(assets->getTexture(texture), transform);
	}
	
	Label RenderingEngine::addHUDLabel(const std::shared_ptr<Transform<3>> transform, const std::string& texture, const int& length)
	{
		return huds.addLabel(assets->getTexture(texture), transform, length);
	}
	
	bool RenderingEngine::shouldCloseWindow() const
	{
		return m_window.shouldClose();
	}
	
	Mouse& RenderingEngine::getMouse()
	{
		return m_window.getMouse();
	}
					
	Keyboard& RenderingEngine::getKeyboard()
	{
		return m_window.getKeyboard();
	}
	
	Camera& RenderingEngine::getCamera()
	{
		return camera;
	}
}
