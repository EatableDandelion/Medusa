#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include "Circe/Circe.h"

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
		
		MOUSE_LEFT = 0,
		MOUSE_RIGHT = 1
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
	
	class Mouse
	{
		public:
			Mouse();
			void onPress(const std::size_t& channel);
			void onRelease(const std::size_t& channel);
			void onMoved(const Circe::Vec2& newValue);
			void addClickListener(const std::function<void(bool, bool)>& listener);
			void addMoveListener(const std::function<void(Circe::Vec2, Circe::Vec2)>& listener);
			void addDragListener(const std::function<void(Circe::Vec2, Circe::Vec2)>& listener);
			float getPosx();
			float getPosy();
			
		private:
			Input<bool, 3> buttons;
			Input<Circe::Vec2, 1> cursor;
			Input<Circe::Vec2, 1> dragObserver;
			bool dragged;
			Circe::Vec2 dragStart;
			void onDrag();
	};
	
	typedef Input<bool, 260> Keyboard;
}