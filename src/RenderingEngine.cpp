#include "RenderingEngine.h"
#include <initializer_list>

namespace Medusa
{
	using namespace Circe;
	using namespace std;
	
	RenderingEngine::RenderingEngine(const VertexSpecs& specs):m_window(500, 300, "test"),camera(2.0f, 2.0f), shaderResources("../../Resource/Shader/", specs), meshResources("../../Resource/Mesh/", specs), textureResources("../../Resource/Texture/")
	{
		m_window.init();
		pass.setShader(shaderResources.getResource("Basic"));
		/*framebuffer = std::make_shared<FrameBuffer>();
		framebuffer->init(500, 300);*/
		//initScreen();
	}
	
	RenderingEngine::~RenderingEngine()
	{
		shaderResources.unloadAll();
		meshResources.unloadAll();
		//textureResources.unloadAll();
		CIRCE_INFO("Rendering engine terminated");
	}
	
	void RenderingEngine::draw()
	{		
		m_window.update(camera);
		
		//framebuffer->bindForWrite();
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		pass.bind();
		
		for(shared_ptr<RenderingEntity> entity : entities)
		{
			pass.render(camera, *entity);
		}
		
		//renderScreen();
		
		
		m_window.swapBuffers();
	}
	
	void RenderingEngine::addEntity(const std::string& meshName, const std::string& textureName, ITransform* transform)
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>(meshResources.getResource(meshName, Medusa::TRIANGLE_RENDERING));
		shared_ptr<RenderingEntity> entity = make_shared<RenderingEntity>(mesh, transform);
		//entity->setUniform("diffuse", textureResources.getResource(textureName, Medusa::DIFFUSE));
		entities.push_back(entity);
	}
	
	bool RenderingEngine::shouldCloseWindow() const
	{
		return m_window.shouldClose();
	}
	
	/*void RenderingEngine::initScreen()
	{
		screenShader=make_shared<Shader>(shaderResources.getResource("HUD"));
		screenMesh=make_shared<Mesh>(meshResources.getResource("plane.obj", Medusa::TRIANGLE_RENDERING));
	}
			
	void RenderingEngine::renderScreen()
	{
		framebuffer->bindForRead();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader->bind();
		glDisable(GL_DEPTH_TEST);
		screenMesh->draw();
	}*/
}
