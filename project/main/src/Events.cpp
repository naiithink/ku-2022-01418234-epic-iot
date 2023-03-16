#include <list>
#include <map>

#include "Events.h"


void Publisher::subscribe(ISubscriber *subscriber)
{
    subscriberList.push_back(subscriber);
}

void Publisher::unsubscribe(ISubscriber *subscriber)
{
    subscriberList.remove(subscriber);
}

void Publisher::notify(void *context)
{
    std::list<ISubscriber *>::iterator iterator = subscriberList.begin();
    
    while (iterator != subscriberList.end())
    {
        (*iterator)->update(context);
        iterator++;
    }
}
