#include <Circe/Circe.h>
#include <chrono>
#include <ratio>
#include <ctime>
#include <fstream>
#include <iostream>
#include "Medusa.h"


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
	std::cout << "fds" << std::endl;
	high_resolution_clock::time_point t0, t1, t2;
	t0 = high_resolution_clock::now();
	
	GLFWWindow window(600, 400, "Test");
	window.setCursorVisible(true);
	
	//World world;
	
	MedusaInterface medusa;
	medusa.load("../../Resource/");
	
	medusa.listenTo(window.getMouse());

	Circe::Transform3 transform1, transform2, transform3, transform4, transform5;
	
	transform2.setFrameRotation(Circe::Vec<3>(-1,0,0), Circe::Vec<3>(0,1,0));
	
	
	medusa.newDirectionalLight(0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 1.0f);
	medusa.newAmbientLight(0.1f, 1.0f, 1.0f, 1.0f);
	RenderingHandler entity1 = medusa.newWorldEntity("plane.obj", "Warframe0000.jpg");
	RenderingHandler entity2 = medusa.newWorldEntity("monkey.obj", "Warframe0002.jpg");
	RenderingHandler entity3 = medusa.newDebugEntity(Shape::SQUARE);
	RenderingHandler entity4 = medusa.newDebugEntity(Shape::HEXAGON);
	
	Label label = medusa.newLabel("Test", -0.8f, 0.9f, 0.05f);
	label.setText("Test");
	label.setTextColor(Circe::Vec3(1.0f, 0.8f, 0.7f));
	
	Button button = medusa.newButton("blank.png", 0.8f, 0.8f, 0.2f, 0.2f, [](){std::cout << "Button Pressed" << std::endl;});
	button.setBackgroundColor(Circe::Vec3(0.5f, 0.7f, 0.9f));
	
	
	transform1.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 6.0f));
	transform2.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 2.0f));
	transform3.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 1.0f, 1.0f, 1.5f));
	transform4.translate(Circe::Direction3(Circe::REF_FRAME::GLOBAL, 0.0f, 0.0f, 4.0f));
	transform2.resize(0.5f);
	transform3.resize(0.1f);
	transform4.resize(0.1f);
	
	entity1.setTransform(transform1);
	entity2.setTransform(transform2);
	entity3.setTransform(transform3);
	entity4.setTransform(transform4);
	
	/*World world;
	System system;
	system.update<TestComponent2>(world);*/

	while(!window.shouldClose())
	{	
		CIRCE_PROFILEBLOCK;
			t1 = high_resolution_clock::now();
			
			{CIRCE_PROFILEBLOCK;
				window.swapBuffers();
			}	 
			{CIRCE_PROFILEBLOCK;		
				medusa.draw(window.getWidth(), window.getHeight());
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