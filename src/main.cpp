#include <Circe/Circe.h>
#include <chrono>
#include "RenderingEngine.h"
#include "TestECS.h"




using namespace Medusa;
using namespace Circe;
using namespace std::chrono;
using namespace std;

int main(void)
{
	/**
	TODO:
	Integration with Rosie
	HUD renderer
	Input
	*/
	
	CIRCE_INITPROFILER;
	
	high_resolution_clock::time_point t0, t1, t2;
	t0 = high_resolution_clock::now(); 
	
	shared_ptr<Transform<3>> transform1(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform2(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform3(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform4(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform5(make_shared<Transform<3>>());
	
	transform2->setFrameRotation(Circe::Vec<3>(-1,0,0), Circe::Vec<3>(0,1,0));

	RenderingEngine engine(600, 400);
	engine.addWorldEntity("plane.obj", "Warframe0000.jpg", transform1);
	engine.addWorldEntity("monkey.obj", "Warframe0002.jpg", transform2);
	engine.addDebugEntity("sphere.obj", transform3);
	engine.addDebugEntity("sphere.obj", transform4);
	engine.addHUDEntity(transform5, "Warframe0002.jpg");
	
	Camera cam = engine.getCamera();
	
	engine.getMouse().addMoveListener([&cam](Circe::Vec2 oldValue, Circe::Vec2 newValue){cam.rotate(-0.5f*(newValue(1)-oldValue(1)), -0.5f*(newValue(0)-oldValue(0)), 0.0f);});
	engine.getKeyboard().addListener(KEYS::KEY_A, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(-0.1f, 0.0f, 0.0f);}});
	engine.getKeyboard().addListener(KEYS::KEY_D, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(0.1f, 0.0f, 0.0f);}});
	engine.getKeyboard().addListener(KEYS::KEY_S, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(0.0f, 0.0f, -0.1f);}});
	engine.getKeyboard().addListener(KEYS::KEY_W, [&cam](bool oldValue, bool newValue){if(oldValue == 0 && newValue==1){cam.translate(0.0f, 0.0f, 0.1f);}});


	EntityLoader test;
	test.load("../../Resource/Model/", "monkey.mod");
	
	
	transform1->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 6.0f));
	transform2->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 2.0f));
	transform3->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, 1.0f, 1.0f, 1.5f));
	transform4->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 4.0f));
	transform2->resize(0.5f);
	transform3->resize(0.1f);
	transform4->resize(0.1f);
	transform5->resize(0.1f);
	//transform5->translate(Circe::Direction<3>(Circe::REF_FRAME::GLOBAL, 0.50f, 0.50f, 0.0f));

	std::cout << std::endl;
	while(!engine.shouldCloseWindow())
	{	
		CIRCE_PROFILEBLOCK;
		
		t1 = high_resolution_clock::now(); 
		
		engine.draw();
		//transform2->rotate(0.01f, 0.01f, 0.0f);
		
		std::cout << "\r" << "Execution time: " << (duration_cast<milliseconds>(high_resolution_clock::now() - t0)).count()/1000.0 << " s           " << std::flush;
		
		t2 = high_resolution_clock::now();
		auto dt = duration_cast<milliseconds>(t2 - t1);
		while(dt.count()<16 && !engine.shouldCloseWindow()){
			t2 = high_resolution_clock::now();
			dt = duration_cast<milliseconds>(t2 - t1);
		}
		
    }
	std::cout << std::endl;
}