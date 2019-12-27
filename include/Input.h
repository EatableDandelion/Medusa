#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include "Circe/Circe.h"
#include "Events.h"

namespace Medusa
{
	enum KEYS
	{
		KEY_A = 65,
		KEY_B = 66,
		KEY_C = 67,
		KEY_D = 68,
		KEY_E = 69,
		KEY_F = 70,
		KEY_G = 71,
		KEY_H = 72,
		KEY_I = 73,
		KEY_J = 74,
		KEY_K = 75,
		KEY_L = 76,
		KEY_M = 77,
		KEY_N = 78,
		KEY_O = 79,
		KEY_P = 80,
		KEY_Q = 81,
		KEY_R = 82,
		KEY_S = 83,
		KEY_T = 84,
		KEY_U = 85,
		KEY_V = 86,
		KEY_W = 87,
		KEY_X = 88,
		KEY_Y = 89,
		KEY_Z = 90,

		KEY_0 = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,	

		KEY_ENTER = 257,
		KEY_ESCAPE = 256,
		KEY_BACKSPACE = 259,
		KEY_TAB = 258,
		KEY_SPACE = 32,

		KEY_MINUS = 45,
		KEY_EQUALS = 61,
		
		LEFT_CLICK = 0,
		RIGHT_CLICK = 1
	};

	template<typename T, std::size_t NbChannels>
	class Input
	{
		public:		
			virtual ~Input(){};
			
			void addListener(const std::size_t& channel, const std::function<void(T, T)>& listener)
			{
				if(m_listeners.find(channel) == m_listeners.end())
				{
					std::vector<std::function<void(T, T)>> listeners;
					listeners.push_back(listener);
					m_listeners[channel] = listeners;
				}
				else
				{
					m_listeners[channel].push_back(listener);
					m_listeners[channel].push_back(listener);
				}
			}
			
			void onChange(const std::size_t& channel, const T& newValue)
			{
				if(m_listeners.find(channel) != m_listeners.end())
				{
					for(auto listener : m_listeners[channel])
					{
						listener(m_values[channel], newValue);
					}
				}
				m_values[channel] = newValue;
			}
			
			T getValue(const int& channel) const
			{
				return m_values[channel];
			}
			
			void setValue(const int& channel, const T& newValue)
			{
				m_values[channel] = newValue;
			}

		private:			
			std::unordered_map<std::size_t, std::vector<std::function<void(T, T)>>> m_listeners;
			T m_values[NbChannels];
	};
	
	class MouseListener;

	class Mouse
	{
		public:
			Mouse();
			void onPress(const std::size_t& channel);
			void onRelease(const std::size_t& channel);
			void onMoved(const Circe::Vec2& newValue);
			float getPosx();
			float getPosy();
			
			void addListener(const std::shared_ptr<MouseListener>& listener);
			
		private:
			bool LMB;
			bool RMB;
			bool WMB;
			float x;
			float y;
			bool dragged;
			float dragStartX;
			float dragStartY;
			std::vector<std::weak_ptr<MouseListener>> listeners;
			
		private:
			void onDrag();
	};
	
	class MouseListener
	{
		public:
			float getX() const;
			float getY() const;
			
			virtual void onLeftClick();
			virtual void onRightClick();
			virtual void onMove();
			virtual void onDrag(const float& startX, const float& startY);
			
		private:
			float x;
			float y;
			
			friend class Mouse;
	};
	
	typedef Input<bool, 260> Keyboard;
}