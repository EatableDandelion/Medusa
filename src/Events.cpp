#include "Events.h"

namespace Medusa
{	
	Msg::Msg(const std::string& type):m_type(type)
	{}
	
	bool Msg::isType(const std::string& type) const
	{
		return m_type == type;
	}
	
	void Msg::addArgument(const std::string& name, const int& value)
	{
		arguments.insert(std::pair<std::string, var>(name, var(value)));
	}
	
	
	Subscriber::Subscriber(const std::string& msgType, const bool& presort):msgType(msgType), presort(presort)
	{}
	
	Subscriber::Subscriber(const Subscriber& other):msgType(other.msgType), presort(other.presort),msgs(other.msgs)
	{
		
	}
	
	std::stack<Msg>& Subscriber::collect()
	{
		return msgs;				
	}
	
	void Subscriber::post(const Msg& msg)
	{						
		if(presort && msg.isType(msgType))
		{
			msgs.push(msg);
		}
	}
	
	void Messenger::publish(const Msg& msg)
	{
		//std::vector<std::weak_ptr<Subscriber>>::iterator it; 
		for(auto it = subscribers.begin(); it != subscribers.end(); ++it)
		{
			if(std::shared_ptr<Subscriber> subscriber = it->lock())
			{
				subscriber->post(msg);
			}
			else
			{//subscribers.remove(weakSubscriber);
				subscribers.erase(it);
			}
			
		}
	}

	void Messenger::addSubscriber(const std::shared_ptr<Subscriber>& subscriber)
	{
		subscribers.push_back(subscriber);
	}

	/*Subscriber Messenger::newSubscriber(const std::string& msgType, const bool& presort)
	{
		Subscriber subscriber(msgType, presort);
		subscribers.push_back(subscriber);
		return subscriber;
	}*/
}
