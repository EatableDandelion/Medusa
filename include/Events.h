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
	/*template<typename T>
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
	};*/
	
	
	template<typename T> 
	class WeakCollection;
	
	template<typename T>
	class WeakIterator
	{
		public:
			WeakIterator(WeakCollection<T>& collection, const int& startIndex):collection(collection), index(startIndex)
			{}
			
			WeakIterator<T> operator++()
			{
				index++;
				return *this;
			}
			
			bool operator!=(const WeakIterator& other) const
			{
				return index != other.index;
			}
			
			std::shared_ptr<T> operator*()
			{
				if(!collection.listeners[index].lock())
				{
					collection.listeners.erase(collection.listeners.begin()+index);
				}
				return collection.listeners[index].lock();
			}
			
			int getIndex() const
			{
				return index;
			}
			
		private:
			int index;
			WeakCollection<T>& collection;
	};
	
	
	template<typename T>
	class WeakCollection
	{
		public:
			int add(std::shared_ptr<T>& listener)
			{
				listeners.push_back(listener);
				return listeners.size() - 1;
			}
			
			std::shared_ptr<T> get(const int& id)
			{
				if(!listeners[id].lock())
				{
					listeners.erase(listeners.begin()+id);
				}
				return listeners[id].lock();
			}
			
			WeakIterator<T> begin()
			{
				return WeakIterator<T>(*this, 0);
			}
			
			WeakIterator<T> end()
			{
				return WeakIterator<T>(*this, listeners.size());
			}	
			
			std::shared_ptr<T> get(const WeakIterator<T>& iterator)
			{
				if(!listeners[iterator.getIndex()].lock())
				{
					listeners.erase(listeners.begin()+iterator.getIndex());
				}
				return listeners[iterator.getIndex()].lock();
			}
		
		private:
			std::vector<std::weak_ptr<T>> listeners;
			friend class WeakIterator<T>;
	};	
	
	
	template<typename T>
	class Observer
	{
		public:
			void set(const T& newValue)
			{
				oldValue = value;
				value = newValue;
			}
			
			T get() const
			{
				return value;
			}
			
			T getPreviousValue() const
			{
				return oldValue;
			}
			
		private:
			T value;
			T oldValue;
	};
	
	
	template<typename T>
	class Observable
	{
		public:
			Observable(const T& value):value(value)
			{}
		
			void set(const T& newValue)
			{
				for(auto it = observers.begin(); it != observers.end(); ++it)
				{
					if(std::shared_ptr<Observer<T>> observer = it->lock())
					{
						//observer->onChange(value, newValue);
						observer->set(newValue);
					}
					else
					{
						observers.erase(it);
					}					
				}
				value = newValue;
			}
			
			void operator=(const T& newValue)
			{
				set(newValue);
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