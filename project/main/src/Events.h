#include <list>
#include <map>

// typedef enum
// {
//     UPDATE
// }
// eventType;


class ISubscriber
{
public:
    virtual ~ISubscriber(){};
    virtual void update(void *) = 0;
};


class IPublisher
{
public:
    virtual ~IPublisher(){};
    virtual void subscribe(ISubscriber *) = 0;
    virtual void unsubscribe(ISubscriber *) = 0;
    virtual void notify(void *) = 0;
};


class Publisher : public IPublisher
{
public:
    virtual ~Publisher() {};
    void subscribe(ISubscriber *) override;
    void unsubscribe(ISubscriber *) override;
    void notify(void *) override;

private:
    std::list<ISubscriber *> subscriberList;
};
