#ifndef SERVER_LOBBY_ROOM_PROTOCOL_HPP
#define SERVER_LOBBY_ROOM_PROTOCOL_HPP

#include "network/protocols/lobby_room_protocol.hpp"

class ServerLobbyRoomProtocol : public LobbyRoomProtocol
{
    public:
        ServerLobbyRoomProtocol();
        virtual ~ServerLobbyRoomProtocol();

        virtual bool notifyEventAsynchronous(Event* event);
        virtual void setup();
        virtual void update();
        virtual void asynchronousUpdate() {};

        void startGame();
        void startSelection();
        void checkIncomingConnectionRequests();
        void checkRaceFinished();

    protected:
        // connection management
        void kartDisconnected(Event* event);
        void connectionRequested(Event* event);
        // kart selection
        void kartSelectionRequested(Event* event);
        // race votes
        void playerMajorVote(Event* event);
        void playerRaceCountVote(Event* event);
        void playerMinorVote(Event* event);
        void playerTrackVote(Event* event);
        void playerReversedVote(Event* event);
        void playerLapsVote(Event* event);

        uint8_t m_next_id; //!< Next id to assign to a peer.
        std::vector<TransportAddress> m_peers;
        std::vector<uint32_t> m_incoming_peers_ids;
        uint32_t m_current_protocol_id;
        TransportAddress m_public_address;
        bool m_selection_enabled;
        bool m_in_race;

        enum STATE
        {
            NONE,
            GETTING_PUBLIC_ADDRESS,
            LAUNCHING_SERVER,
            WORKING,
            DONE,
            EXITING
        };
        STATE m_state;
};
#endif // SERVER_LOBBY_ROOM_PROTOCOL_HPP
