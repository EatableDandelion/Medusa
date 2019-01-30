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
	
	shared_ptr<Transform<3>> transform1(make_shared<Transform<3>>());
	shared_ptr<Transform<3>> transform2(make_shared<Transform<3>>());
	
	/*CIRCE_INITPROFILER;
	CIRCE_PROFILEBLOCK;*/
	

	RenderingEngine engine;
	engine.addEntity("plane.obj", "Warframe0000.jpg", transform1);
	engine.addEntity("monkey.obj", "Warframe0002.jpg", transform2);
	
	high_resolution_clock::time_point t0, t1;

   
	transform2->resize(0.8f);
	
	while(!engine.shouldCloseWindow())
	{	
		
		//CIRCE_PROFILEBLOCK;	
		t0 = high_resolution_clock::now(); 
			
		engine.draw();
		transform2->rotate(0.0f, 0.0f, 0.01f);
		t1 = high_resolution_clock::now();
		auto dt = duration_cast<milliseconds>(t1 - t0);
		while(dt.count()<16 && !engine.shouldCloseWindow()){
			t1 = high_resolution_clock::now();
			dt = duration_cast<milliseconds>(t1 - t0);
		}
    }
}