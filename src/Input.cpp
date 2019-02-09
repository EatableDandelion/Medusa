#include "Input.h"
#include <iostream>

namespace Medusa
{
	Mouse::Mouse():dragStart(0.0f, 0.0f)
	{
		dragged = false;
	}
	
	void Mouse::onPress(const std::size_t& channel)
	{
		buttons.onChange(channel, 1);
		
		if(channel == 0)
		{
			dragStart = cursor.getValue(0);
		}		
	}
	
	void Mouse::onRelease(const std::size_t& channel)
	{
		buttons.onChange(channel, 0);
		
		if(channel == 0)
		{
			dragged = false;
		}
	}
	
	void Mouse::onMoved(const Circe::Vec2& newValue)
	{
		cursor.onChange(0, newValue);

		if(buttons.getValue(0)==0)return;
		if(!dragged && Circe::distanceSquare(dragStart, cursor.getValue(0)) > 0.01f*0.01f)
		{
			dragged = true;
			dragObserver.setValue(0, cursor.getValue(0));
		}
		if(dragged)
		{
			onDrag();
		}
			
	}
	
	void Mouse::addClickListener(const std::function<void(bool, bool)>& listener)
	{
		buttons.addListener(1, listener);
	}
	
	void Mouse::addMoveListener(const std::function<void(Circe::Vec2, Circe::Vec2)>& listener)
	{
		cursor.addListener(0, listener);
	}
	
	void Mouse::addDragListener(const std::function<void(Circe::Vec2, Circe::Vec2)>& listener)
	{
		dragObserver.addListener(0, listener);
	}
	
	void Mouse::onDrag()
	{
		dragObserver.onChange(0, cursor.getValue(0));
	}
	
	float Mouse::getPosx()
	{
		return cursor.getValue(0)(0);
	}
	
	float Mouse::getPosy()
	{
		return cursor.getValue(0)(1);
	}
}