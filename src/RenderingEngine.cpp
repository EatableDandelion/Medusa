#include "RenderingEngine.h"
#include <initializer_list>

namespace Medusa
{
	using namespace Circe;
	using namespace std;
	
	RenderingEngine::RenderingEngine(const int& windowWidth, const int& windowHeight, const VertexSpecs& specs):m_window(windowWidth, windowHeight, "test"),camera(2.0f, 2.0f), shaderResources("../../Resource/Shader/"), meshResources("../../Resource/Mesh/"), textureResources("../../Resource/Texture/"), framebuffer(windowWidth, windowHeight)
	{
		meshResources.load("plane.obj", -1);
		meshResources.load("monkey.obj", 1);
		meshResources.load("sphere.obj", 1);
		meshResources.load("cube.obj", 1);
		
		shaderResources.load("GeometryPass", specs);
		shaderResources.load("DirectionalLight", specs);
		shaderResources.load("AmbientLight", specs);
		shaderResources.load("DebugDisplay", specs);
		
		textureResources.load("Warframe0000.jpg");
		textureResources.load("Warframe0002.jpg");
		
		pass.setShader(shaderResources.getResource("GeometryPass"));
		debugPass.setShader(shaderResources.getResource("DebugDisplay"));
		directionalLights.setShader(shaderResources.getResource("DirectionalLight"));
		ambientLights.setShader(shaderResources.getResource("AmbientLight"));
		
		initScreen();
		
		directionalLights.addLight(0.5f, Circe::Vec3(1.0f,1.0f,1.0f), Circe::Vec3(1.0f,0.8f,1.0f));
		ambientLights.addLight(0.1f, Circe::Vec3(1.0f,1.0f,1.0f));
		
	}
	
	RenderingEngine::~RenderingEngine()
	{
		textureResources.unloadAll();
		shaderResources.unloadAll();
		meshResources.unloadAll();
		CIRCE_INFO("Rendering engine terminated");
	}
	
	void RenderingEngine::draw()
	{		
		m_window.update(camera);
		
		framebuffer.bindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0f);
		
		geometryPass();
		
		lightingPass();

		playDebugPass();
		
		m_window.swapBuffers();
	}
	
	void RenderingEngine::addWorldEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<ITransform>& transform)
	{
		entities.push_back(createEntity(meshName, textureName, transform));
	}
	
	void RenderingEngine::addDebugEntity(const std::string& meshName, const std::shared_ptr<ITransform>& transform)
	{
		debugEntities.push_back(createEntity(meshName, transform));
	}
	
	bool RenderingEngine::shouldCloseWindow() const
	{
		return m_window.shouldClose();
	}
	
	void RenderingEngine::initScreen()
	{
		std::shared_ptr<Mesh> screenMesh = make_shared<Mesh>(meshResources.getResource("plane.obj", Medusa::TRIANGLE_RENDERING));
		std::shared_ptr<Circe::ITransform> screenTransform = std::make_shared<Circe::Transform<3>>();
		screenEntity = RenderingEntity(screenMesh, screenTransform);

		int textureSlot =0;
		for(const Texture& gBufferTexture : framebuffer.getTextures())
		{
			screenEntity.setTexture(TextureType(textureSlot), gBufferTexture);
			textureSlot++;
		}
	}
	
	void RenderingEngine::geometryPass()
	{
		glDepthMask(true);
		pass.bind();
		
		for(shared_ptr<RenderingEntity> entity : entities)
		{
			//TODO remove entities based on bool result of pass.render(...)
			if(entity->update(camera.getProjectionMatrix(), camera.getViewMatrix())){
				pass.render(*entity);
			}
		}
		
		pass.unbind();
		glDepthMask(false);
	}

	void RenderingEngine::lightingPass()
	{
		framebuffer.read();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glDepthMask(false);
		glDisable(GL_DEPTH_TEST);
		
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		ambientLights.render(screenEntity);
		directionalLights.render(screenEntity);
		
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		
		glDisable(GL_BLEND);	
		
	}
	
	void RenderingEngine::playDebugPass()
	{
		framebuffer.copyDepth();
		debugPass.bind();
		glDepthMask(false);
		
		for(shared_ptr<RenderingEntity> entity : debugEntities)
		{
			if(entity->update(camera.getProjectionMatrix(), camera.getViewMatrix())){
				entity->setUniform<Circe::Vec3>("objectColor", Circe::Vec3(0.5f, 1.0f, 1.0f));
				debugPass.render(*entity);
			}
		}
		glDepthMask(true);
		
		debugPass.unbind();
	}
	
	std::shared_ptr<RenderingEntity> RenderingEngine::createEntity(const std::string& meshName, const std::shared_ptr<ITransform>& transform)
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>(meshResources.getResource(meshName, Medusa::TRIANGLE_RENDERING));
		shared_ptr<RenderingEntity> entity = make_shared<RenderingEntity>(mesh, transform);
		return entity;
	}
	
	std::shared_ptr<RenderingEntity> RenderingEngine::createEntity(const std::string& meshName, const std::string& textureName, const std::shared_ptr<ITransform>& transform)
	{
		shared_ptr<RenderingEntity> entity = createEntity(meshName, transform);
		entity->setTexture(TextureType::COLOR, textureResources.getResource(textureName));
		return entity;
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
