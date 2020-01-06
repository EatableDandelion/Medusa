#include <Circe/Circe.h>
#include <chrono>
#include <ratio>
#include <ctime>
#include "TestECS.h"
#include "Window.h"

using namespace Medusa;
using namespace Circe;
using namespace std::chrono;
using namespace std;

int main(void)
{
	/**
	TODO:
	Integration with Rosie
	*/
	
	CIRCE_INITPROFILER;
	
	high_resolution_clock::time_point t0, t1, t2;
	t0 = high_resolution_clock::now();
	
	
	GLFWWindow window(600, 400, "Test");
	window.setCursorVisible(true);
	
	std::shared_ptr<Assets> assets = std::make_shared<Assets>("../../Resource/Mesh/", "../../Resource/Texture/", "../../Resource/Shader/");
	assets->loadMesh("plane.obj", -1);
	assets->loadMesh("triangle.obj", -1);
	assets->loadMesh("monkey.obj", 1);
	assets->loadMesh("sphere.obj", 1);
	assets->loadMesh("cube.obj", 1);
	assets->loadMesh("circle.obj", 1);
	assets->loadMesh("square.obj", 1);
	assets->loadMesh("hexagon.obj", 1);
	
	assets->loadShader("GeometryPass");
	assets->loadShader("DirectionalLight");
	assets->loadShader("AmbientLight");
	assets->loadShader("DebugDisplay");
	assets->loadShader("HUD");
	
	assets->loadTexture("Warframe0000.jpg");
	assets->loadTexture("Warframe0002.jpg");
	assets->loadTexture("font.png");
	assets->loadTexture("blockIso.png");
	assets->loadTexture("blank.png");
	
	std::shared_ptr<GeometryPass> geometryPass = std::make_shared<GeometryPass>();
	std::shared_ptr<AmbientLightPass> ambientLights = std::make_shared<AmbientLightPass>();
	std::shared_ptr<DirectionalLightPass> directionalLights = std::make_shared<DirectionalLightPass>();	
	std::shared_ptr<DebugPass> debugPass = std::make_shared<DebugPass>();
	std::shared_ptr<HUDPass> hudPass = std::make_shared<HUDPass>();
	
	debugPass->setLineThickness(2.0f);
	
	debugPass->addNext(hudPass);
	directionalLights->addNext(debugPass);
	ambientLights->addNext(directionalLights);
	
	
	RenderingEngine engine(window.getWidth(), window.getHeight());
	engine.init(geometryPass, ambientLights, assets);
	
	std::shared_ptr<Messenger> messenger = std::make_shared<Messenger>();
	std::shared_ptr<GUI> gui = std::make_shared<GUI>(hudPass);
	window.getMouse().addListener(gui);
	
	
	/*shared_ptr<Transform<3>> transform1(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform2(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform3(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform4(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform5(make_shared<Transform<3>>());*/
	
	Circe::Transform3 transform1, transform2, transform3, transform4, transform5;
	
	transform2.setFrameRotation(Circe::Vec<3>(-1,0,0), Circe::Vec<3>(0,1,0));
	
	
	DirectionalLight directionalLight = directionalLights->addEntity(0.5f, Circe::Vec3(1.0f,1.0f,1.0f), Circe::Vec3(1.0f,0.8f,1.0f));
	
	AmbientLight ambient = ambientLights->addEntity(0.1f, Circe::Vec3(1.0f,1.0f,1.0f));	
	
	RenderingEntity entity1 = geometryPass->addEntity("plane.obj", "Warframe0000.jpg");
	RenderingEntity entity2 = geometryPass->addEntity("monkey.obj", "Warframe0002.jpg");
	RenderingEntity entity3 = debugPass->addEntity("square.obj");
	RenderingEntity entity4 = debugPass->addEntity("hexagon.obj");
	
	Label label = gui->addLabel(4, "font.png", Circe::Vec2(-0.8f, 0.9f), Circe::Vec2(0.05f, 0.05f));
	label.setText("Test");
	label.setTextColor(Circe::Vec3(1.0f, 0.8f, 0.7f));
	
	Button button = gui->addButton("blank.png", Circe::Vec2(0.8f, 0.8f), Circe::Vec2(0.2f, 0.2f), [](){std::cout << "Button Pressed" << std::endl;});
	button.setBackgroundColor(Circe::Vec3(0.5f, 0.7f, 0.9f));
	
	Camera cam = engine.getCamera();
	
	
	
	window.getKeyboard().addListener(KEYS::KEY_E, [&messenger](bool oldValue, bool newValue){messenger->publish(Event("mouseclick"));});
	window.getKeyboard().addListener(KEYS::KEY_A, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(-0.1f, 0.0f, 0.0f);}});
	window.getKeyboard().addListener(KEYS::KEY_D, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(0.1f, 0.0f, 0.0f);}});
	window.getKeyboard().addListener(KEYS::KEY_S, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(0.0f, 0.0f, -0.1f);}});
	window.getKeyboard().addListener(KEYS::KEY_W, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(0.0f, 0.0f, 0.1f);}});


	/*Messenger messenger;
	std::shared_ptr<Subscriber<MouseClickedMsg>> subscriber = std::make_shared<Subscriber<MouseClickedMsg>>();
	messenger.addSubscriber(subscriber);
	messenger.publish<MouseClickedMsg>();
	*/
	EntityLoader test;
	test.load("../../Resource/Model/", "monkey.mod");
	
	
	transform1.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 6.0f));
	transform2.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 2.0f));
	transform3.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 1.0f, 1.0f, 1.5f));
	transform4.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 4.0f));
	transform2.resize(0.5f);
	transform3.resize(0.1f);
	transform4.resize(0.1f);
	//transform5.resize(0.1f);
	
	entity1.setTransform(transform1);
	entity2.setTransform(transform2);
	entity3.setTransform(transform3);
	entity4.setTransform(transform4);
	//entity5.setTransform(transform5);
	

	while(!window.shouldClose())
	{	
		CIRCE_PROFILEBLOCK;
			t1 = high_resolution_clock::now();
			
			{CIRCE_PROFILEBLOCK;
				window.swapBuffers();
			}	 
			{CIRCE_PROFILEBLOCK;		
				engine.draw(window.getWidth(), window.getHeight());
			}
			{CIRCE_PROFILEBLOCK;
				window.update();
			}
			
			{CIRCE_PROFILEBLOCK;		
				double execTime = (duration_cast<duration<double>>(high_resolution_clock::now() - t0)).count();
				std::cout << "\r" << "Execution time: " << execTime << " s           " << std::flush;
				//label.setText(std::to_string(execTime));
			}	
				
			{CIRCE_PROFILEBLOCK;
				t2 = high_resolution_clock::now();
				duration<double> dt = duration_cast<duration<double>>(t2 - t1);
				int t = 0;
				while(dt.count()<0.016 && !window.shouldClose() && t<200){
					t2 = high_resolution_clock::now();
					dt = duration_cast<duration<double>>(t2 - t1);
					t++;
				}
			}
				
    }
	std::cout << std::endl;
}