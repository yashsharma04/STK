//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "network/protocols/request_connection.hpp"

#include "network/protocol_manager.hpp"
#include "online/request_manager.hpp"
#include "online/current_user.hpp"
#include "config/user_config.hpp"

RequestConnection::RequestConnection(uint32_t server_id) : Protocol(NULL, PROTOCOL_SILENT)
{
    m_server_id = server_id;
}

RequestConnection::~RequestConnection()
{
}

void RequestConnection::setup()
{
    m_state = NONE;
}

void RequestConnection::asynchronousUpdate()
{
    switch (m_state)
    {
        case NONE:
        {
            m_request = new Online::CurrentUser::ServerJoinRequest();
            m_request->setServerURL("address-management.php");
            m_request->addParameter("id",Online::CurrentUser::get()->getID());
            m_request->addParameter("token",Online::CurrentUser::get()->getToken());
            m_request->addParameter("server_id",m_server_id);
            m_request->addParameter("action","request-connection");

            Online::RequestManager::get()->addRequest(m_request);
            m_state = REQUEST_PENDING;
            break;
        }
        case REQUEST_PENDING:
        {
            if (!m_request->isDone())
                return;
            const XMLNode * result = m_request->getXMLData();
            std::string rec_success;

            if(result->get("success", &rec_success))
            {
                if (rec_success == "yes")
                {
                    Log::debug("RequestConnection", "Connection Request made successfully.");
                }
                else
                {
                    Log::error("RequestConnection", "Fail to make a request to connecto to server %d", m_server_id);
                }
            }
            else
            {
                Log::error("RequestConnection", "Fail to make a request.");
            }
            m_state = DONE;

            break;
        }
        case DONE:
            m_state = EXITING;
            delete m_request;
            m_request = NULL;
            m_listener->requestTerminate(this);
            break;
        case EXITING:
            break;
    }
}

