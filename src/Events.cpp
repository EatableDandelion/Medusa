#include "Events.h"

namespace Medusa
{	
	Event::Event(const std::string& type):m_type(type)
	{}
	
	bool Event::isType(const std::string& type) const
	{
		return m_type == type;
	}
	
	void Event::addArgument(const std::string& name, const int& value)
	{
		arguments.insert(std::pair<std::string, var>(name, var(value)));
	}
	
	
	MessageSubscriber::MessageSubscriber(const std::string& msgType, const bool& presort):msgType(msgType), presort(presort)
	{}
	
	std::stack<Event>& MessageSubscriber::collect()
	{
		return msgs;				
	}
	
	void MessageSubscriber::post(const Event& msg)
	{						
		if(presort && msg.isType(msgType))
		{
			msgs.push(msg);
		}
	}
	
	EventListener::EventListener(const std::function<void(Event)>& reaction):reaction(reaction)
	{}
		
	void EventListener::post(const Event& msg)
	{
		reaction(msg);
	}
	
	void Messenger::publish(const Event& msg)
	{
		for(auto it = subscribers.begin(); it != subscribers.end(); ++it)
		{
			if(std::shared_ptr<Subscriber> subscriber = it->lock())
			{
				subscriber->post(msg);
			}
			else
			{
				subscribers.erase(it);
			}
			
		}
	}

	void Messenger::addSubscriber(const std::shared_ptr<Subscriber>& subscriber)
	{
		subscribers.push_back(subscriber);
	}
}
