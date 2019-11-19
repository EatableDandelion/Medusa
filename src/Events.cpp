#include "Events.h"

namespace Medusa
{
	void Subscriber::post(std::shared_ptr<Msg>& msg)
	{
		msgs.push(msg);
	}
		
	std::stack<std::shared_ptr<Msg>> Subscriber::collect()
	{
		return msgs;
	}
	
	void Messenger::addSubscriber(const Subscriber& subscriber)
	{
		subscribers.push_back(std::make_shared<Subscriber>(subscriber));
	}
}
