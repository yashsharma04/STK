#ifndef GAME_EVENTS_PROTOCOL_HPP
#define GAME_EVENTS_PROTOCOL_HPP

#include "network/protocol.hpp"

class AbstractKart;
class Item;

class GameEventsProtocol : public Protocol
{
    public:
        GameEventsProtocol();
        virtual ~GameEventsProtocol();

        virtual bool notifyEvent(Event* event);
        virtual bool notifyEventAsynchronous(Event* event) { return false; }
        virtual void setup();
        virtual void update();
        virtual void asynchronousUpdate() {}

        void collectedItem(Item* item, AbstractKart* kart);

    protected:
};

#endif // GAME_EVENTS_PROTOCOL_HPP
