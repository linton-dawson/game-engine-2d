#ifndef EVENTBUS_HPP
#define EVENTBUS_HPP

#include <map>
#include <list>
#include <typeindex>
#include <memory>
#include <functional>
#include "../Logger/Logger.hpp"
#include "Event.hpp"

class IEventCallback {
    private:
        virtual void call(Event& e) = 0;
    public:
        virtual ~IEventCallback() = default;
        void execute(Event& e) {
            call(e);
        }
};

template <typename TOwner, typename TEvent>
class EventCallback : IEventCallback {
    private:
        typedef void (TOwner::*CallbackFunction)(TEvent&);

        TOwner* ownerInstance;
        CallbackFunction cbFunc;
        virtual void call(Event& e) override{
            std::invoke(cbFunc, ownerInstance, static_cast<TEvent&>(e));
        }
    public:
        EventCallback(TOwner* ownerInstance, CallbackFunction cbFunc) {
            this->ownerInstance = ownerInstance;
            this->cbFunc = cbFunc;
        }
        virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;
class EventBus {
    private:
        //key= any event
        //value= list of functions that have subscribed to this event
        std::map<std::type_index,std::unique_ptr<HandlerList>> subscribers;
    public:
        EventBus() {
            Logger::Log("eventbus constructor");
        }

        ~EventBus() {
            Logger::Log("eventbus destructor");
        }
        
        void reset() {
            subscribers.clear();
        }
        //listener subscribes to event type <T>
        template <typename TEvent, typename TOwner>
        void subscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
            if(!subscribers[typeid(TEvent)].get()) {
                subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto subscriber = std::make_unique<EventCallback<TOwner,TEvent>>(ownerInstance, callbackFunction);
            subscribers[typeid(TEvent)]->push_back(std::move(subscriber));

        }
        
        //we execute all listener callbacks after emitting event type <T>
        template <typename TEvent,typename ...TArgs>
        void emitEvent(TArgs&& ...args) {
            auto handlers = subscribers[typeid(TEvent)].get();
            if(handlers) {
                for(auto it = handlers->begin(); it != handlers->end(); it++) {
                    auto handler = it->get();
                    TEvent event(std::forward<TArgs>(args)...);
                    handler->execute(event);
                }
            }
        }
};
#endif
