#include "Input.h"
#include <iostream>

namespace Medusa
{
	Mouse::Mouse()
	{
		dragStartX = 0.0f;
		dragStartY = 0.0f;
		dragged = false;
	}
	
	void Mouse::onPress(const std::size_t& channel)
	{
		for(std::shared_ptr<MouseListener> listener : listeners)
		{
			if(listener)
			{
				if(channel == KEYS::LEFT_CLICK)
				{
					listener->onLeftClick();
				}
				else if(channel == KEYS::RIGHT_CLICK)
				{
					listener->onRightClick();
				}
			}				
		}
		
		if(channel == 0)
		{
			dragStartX = x;
			dragStartY = y;
		}		
	}
	
	void Mouse::onRelease(const std::size_t& channel)
	{
		if(channel == 0)
		{
			dragged = false;
		}
	}
	
	void Mouse::onMoved(const Circe::Vec2& newValue)
	{
		x = newValue(0);
		y = newValue(1);
		for(std::shared_ptr<MouseListener> listener : listeners)
		{
			if(listener)
			{			
				listener->x = x;
				listener->y = y;
				listener->onMove();
			}		
		}

		if(LMB==0)return;
		if(!dragged && ((x-dragStartX)*(x-dragStartX)+(y-dragStartY)*(y-dragStartY)) > 0.01f*0.01f)
		{
			dragged = true;
		}
		if(dragged)
		{
			onDrag();
		}		
	}
	
	void Mouse::onDrag()
	{
		for(std::shared_ptr<MouseListener> listener : listeners)
		{
			if(listener)
			{				
				listener->x = x;
				listener->y = y;
				listener->onDrag(dragStartX, dragStartY);
			}			
		}
	}
	
	float Mouse::getPosx()
	{
		return x;
	}
	
	float Mouse::getPosy()
	{
		return y;
	}
	
	void Mouse::addListener(std::shared_ptr<MouseListener> listener)
	{
		listeners.add(listener);
	}
	
	float MouseListener::getX() const
	{
		return x;
	}
	
	float MouseListener::getY() const
	{
		return y;
	}
	
	void MouseListener::onLeftClick()
	{}
	
	void MouseListener::onRightClick()
	{}
	
	void MouseListener::onMove()
	{}
	
	void MouseListener::onDrag(const float& startX, const float& startY)
	{}
}