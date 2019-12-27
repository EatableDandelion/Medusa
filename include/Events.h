#pragma once

#include <stack>
#include <functional>
#include <memory>
#include <variant>
#include <vector>
#include <Circe/Circe.h>

namespace Medusa
{
	//Observer is the simplest subscriber, reacts to the value.
	//For more complex subscriber, use EventListener or MessageSubscriber
	template<typename T>
	class Observer
	{
		public:
			Observer(const std::function<void(T, T)>& reaction):reaction(reaction)
			{}
		
			void onChange(const T& oldValue, const T& newValue)
			{
				reaction(oldValue, newValue);
			}
			
		private:
			std::function<void(T, T)> reaction;
	};
	
	template<typename T>
	class Observable
	{
		public:
			void set(const T& newValue)
			{
				for(std::weak_ptr<Observer<T>> weakObserver : observers)
				{
					if(std::shared_ptr<Observer<T>> observer = weakObserver.lock())
					{
						observer->onChange(value, newValue);
					}
					else
					{
						observers.remove(weakObserver);
					}
				}
				value = newValue;
			}
			
			T get() const
			{
				return value;
			}
			
			void addObserver(const std::shared_ptr<Observer<T>>& observer)
			{
				observers.push_back(observer);
			}
		
		private:
			T value;
			std::vector<std::weak_ptr<Observer<T>>> observers;
	};
	
	struct Event
	{
		typedef std::variant<int, float, bool, std::string, Circe::Vec2, Circe::Vec3> var;
		public:
			Event(const std::string& type);
			
			bool isType(const std::string& type) const;
			
			void addArgument(const std::string& name, const int& value);
			
		private:
			const std::string m_type;
			std::unordered_map<std::string, var> arguments;
	};
	
	class Messenger;
	
	class Subscriber
	{
		public:			
			//Subscriber(const Subscriber& other);
					
			virtual void post(const Event& msg) = 0;	
		
		private:
			friend class Messenger;
			
			std::string msgType;
			
					
	};
	
	//MessageSubscriber is a delayed listener
	class MessageSubscriber : public Subscriber
	{
		public:
			MessageSubscriber(const std::string& msgType, const bool& presort = true);
			
			virtual void post(const Event& msg);
			
			std::stack<Event>& collect();
		
		private:
			const std::string msgType;
			bool presort;
			std::stack<Event> msgs;
	};
	
	//EventListener is a sync subscriber
	class EventListener : public Subscriber
	{
		public:
			EventListener(const std::function<void(Event)>& reaction);
		
			virtual void post(const Event& msg);
			
		private:
			std::function<void(Event)> reaction;
	};

	class Messenger
	{
		public:
			void publish(const Event& msg);

			void addSubscriber(const std::shared_ptr<Subscriber>& subscriber);
		
		private:
			std::vector<std::weak_ptr<Subscriber>> subscribers; 			
	};
}