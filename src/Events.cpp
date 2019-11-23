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
	
	std::stack<Msg> Subscriber::collect()
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
		for(Subscriber subscriber : subscribers)
		{
			subscriber.post(msg);
		}
	}

	void Messenger::addSubscriber(const Subscriber& subscriber)
	{
		subscribers.push_back(subscriber);
	}
}
