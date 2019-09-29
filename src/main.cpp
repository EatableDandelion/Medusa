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
	Deferred renderer	
	Integration with Rosie
	Lights
	HUD renderer
	Input
	*/
	
	CIRCE_INITPROFILER;
	
	high_resolution_clock::time_point t0, t1, t2;
	t0 = high_resolution_clock::now(); 
	
	shared_ptr<Transform<3>> transform1(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform2(make_shared<Transform<3>>());
	
	

	RenderingEngine engine(600, 400);
	engine.addEntity("plane.obj", "Warframe0000.jpg", transform1);
	engine.addEntity("monkey.obj", "Warframe0002.jpg", transform2);
	
	
	
	engine.getMouse().addDragListener([&transform2](Circe::Vec2 oldValue, Circe::Vec2 newValue){transform2->rotate(-2.0f*(newValue(1)-oldValue(1)), 2.0f*(newValue(0)-oldValue(0)), 0.0f);});
	
	
	EntityLoader test;
	test.load("../../Resource/Model/", "monkey.mod");
	
	
	transform1->translate(Circe::Vec3(0.0f, 0.0f, 10.0f));
	transform2->translate(Circe::Vec3(0.0f, 0.0f, 2.0f));
	transform2->resize(0.5f);
	//transform2->rotate(0.0f, 10.0f, 0.0f);
	
	std::cout << std::endl;
	while(!engine.shouldCloseWindow())
	{	
		CIRCE_PROFILEBLOCK;
		
		t1 = high_resolution_clock::now(); 
		
		engine.draw();
		transform2->rotate(0.0f, 0.0f, 0.0f);
		
		
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