#include "RenderingEngine.h"
#include <initializer_list>

namespace Medusa
{
	using namespace Circe;
	using namespace std;
	
	RenderingEngine::RenderingEngine(const int& windowWidth, const int& windowHeight, const VertexSpecs& specs):m_window(windowWidth, windowHeight, "test"),camera(2.0f, 2.0f), shaderResources("../../Resource/Shader/", specs), meshResources("../../Resource/Mesh/", specs), textureResources("../../Resource/Texture/"), framebuffer(windowWidth, windowHeight)
	{
		pass.setShader(shaderResources.getResource("Basic"));
		initScreen();
	}
	
	RenderingEngine::~RenderingEngine()
	{
		shaderResources.unloadAll();
		meshResources.unloadAll();
		CIRCE_INFO("Rendering engine terminated");
	}
	
	void RenderingEngine::draw()
	{		
		m_window.update(camera);
		
		framebuffer.write();
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		pass.bind();
		
		for(shared_ptr<RenderingEntity> entity : entities)
		{
			//TODO remove entities based on bool result of pass.render(...)
			pass.render(camera, *entity);
		}
		
		pass.unbind();
		
		renderScreen();
		
		
		m_window.swapBuffers();
	}
	
	void RenderingEngine::addEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<ITransform>& transform, const int& faceOrientation)
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>(meshResources.getResource(meshName, Medusa::TRIANGLE_RENDERING, faceOrientation));
		shared_ptr<RenderingEntity> entity = make_shared<RenderingEntity>(mesh, transform);
		entity->setTexture(textureResources.getTexture(textureName));
		entities.push_back(entity);
	}
	
	bool RenderingEngine::shouldCloseWindow() const
	{
		return m_window.shouldClose();
	}
	
	void RenderingEngine::initScreen()
	{
		screenShader=make_shared<Shader>(shaderResources.getResource("HUD"));
		screenMesh=make_shared<Mesh>(meshResources.getResource("plane.obj", Medusa::TRIANGLE_RENDERING));
	}
			
	void RenderingEngine::renderScreen()
	{
		framebuffer.read();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader->bind();
		glDisable(GL_DEPTH_TEST);
		screenMesh->draw(1);
	}
	
	Mouse& RenderingEngine::getMouse()
	{
		return m_window.getMouse();
	}
					
	Keyboard& RenderingEngine::getKeyboard()
	{
		return m_window.getKeyboard();
	}
}
