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
		for(auto it = listeners.begin(); it != listeners.end(); ++it)
		{
			if(std::shared_ptr<MouseListener> listener = it->lock())
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
			else
			{
				listeners.erase(it);
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
		for(auto it = listeners.begin(); it != listeners.end(); ++it)
		{
			if(std::shared_ptr<MouseListener> listener = it->lock())
			{							
				listener->x = x;
				listener->y = y;
				listener->onMove();
			}
			else
			{
				listeners.erase(it);
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
		for(auto it = listeners.begin(); it != listeners.end(); ++it)
		{
			if(std::shared_ptr<MouseListener> listener = it->lock())
			{							
				listener->x = x;
				listener->y = y;
				listener->onDrag(dragStartX, dragStartY);
			}
			else
			{
				listeners.erase(it);
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
	
	void Mouse::addListener(const std::shared_ptr<MouseListener>& listener)
	{
		listeners.push_back(listener);
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