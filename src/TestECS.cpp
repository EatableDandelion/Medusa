#include "TestECS.h"
#include <iostream>

namespace Manure{
	
	void Component1::print()
	{
		cout << "Component 1" << endl;	
	}
	
	void Component2::print()
	{
		cout << "Component 2" << endl;		
	}
	
	World::World():entity()
	{
		entity.addComponent<Component2>();
	}
	
}