#pragma once

#include <stack>
#include <functional>
#include <memory>

namespace Medusa
{
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
	
	struct Msg
	{};
	
	class Subscriber
	{
		public:
			void post(std::shared_ptr<Msg>& msg);
		
			std::stack<std::shared_ptr<Msg>> collect();
			
		private:
			std::stack<std::shared_ptr<Msg>> msgs;
	};
	
	class Messenger
	{
		public:
			template<typename T, typename... Args>
			void publish(Args... args)
			{
				std::shared_ptr<T> msg = std::make_shared<msg>(std::forward<Args>(args)...);
				for(std::shared_ptr<Subscriber> subscriber : subscribers)
				{
					subscriber->post(msg);
				}
			}
			
			void addSubscriber(const Subscriber& subscriber);
		
		private:
			std::vector<std::shared_ptr<Subscriber>> subscribers; 			
	};
}