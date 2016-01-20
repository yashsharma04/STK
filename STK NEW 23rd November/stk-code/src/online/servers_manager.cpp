//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013 Glenn De Jonghe
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


#include "online/servers_manager.hpp"

#include <string>
#include <irrString.h>
#include <assert.h>
#include "config/user_config.hpp"
#include "utils/translation.hpp"
#include "utils/time.hpp"

#define SERVER_REFRESH_INTERVAL 5.0f

namespace Online{

    static ServersManager* manager_singleton(NULL);

    ServersManager* ServersManager::get()
    {
        if (manager_singleton == NULL)
            manager_singleton = new ServersManager();
        return manager_singleton;
    }

    void ServersManager::deallocate()
    {
        delete manager_singleton;
        manager_singleton = NULL;
    }   // deallocate

    // ============================================================================
    ServersManager::ServersManager(){
        m_last_load_time.setAtomic(0.0f);
        m_joined_server.setAtomic(NULL);
    }

    ServersManager::~ServersManager(){
        cleanUpServers();
        MutexLocker(m_joined_server);
        delete m_joined_server.getData();
    }

    // ============================================================================
    void ServersManager::cleanUpServers()
    {
        m_sorted_servers.lock();
        m_sorted_servers.getData().clearAndDeleteAll();
        m_sorted_servers.unlock();
        m_mapped_servers.lock();
        m_mapped_servers.getData().clear();
        m_mapped_servers.unlock();
    }

    // ============================================================================
    ServersManager::RefreshRequest * ServersManager::refreshRequest(bool request_now) const
    {
        RefreshRequest * request = NULL;
        if(StkTime::getRealTime() - m_last_load_time.getAtomic() > SERVER_REFRESH_INTERVAL)
        {
            request = new RefreshRequest();
            request->setServerURL("client-user.php");
            request->addParameter("action","get_server_list");
            if (request_now)
                RequestManager::get()->addRequest(request);
        }
        return request;
    }

    void ServersManager::refresh(bool success, const XMLNode * input)
    {
        if (success)
        {
            const XMLNode * servers_xml = input->getNode("servers");
            cleanUpServers();
            for (unsigned int i = 0; i < servers_xml->getNumNodes(); i++)
            {
                addServer(new Server(*servers_xml->getNode(i)));
            }
            m_last_load_time.setAtomic((float)StkTime::getRealTime());
        }
        //FIXME error message
    }

    void ServersManager::RefreshRequest::callback()
    {
        ServersManager::get()->refresh(isSuccess(), getXMLData());
    }

    // ============================================================================
    const Server * ServersManager::getQuickPlay() const
    {
        if(m_sorted_servers.getData().size() > 0)
            return getServerBySort(0);

        return NULL;
    }

    // ============================================================================
    void ServersManager::setJoinedServer(uint32_t id)
    {
        MutexLocker(m_joined_server);
        delete m_joined_server.getData();
        //It's a copy!
        m_joined_server.getData() = new Server(*getServerByID(id));
    }

    // ============================================================================
    void ServersManager::unsetJoinedServer()
    {
        MutexLocker(m_joined_server);
        delete m_joined_server.getData();
        m_joined_server.getData() = NULL;
    }

    // ============================================================================
    void ServersManager::addServer(Server * server)
    {
        m_sorted_servers.lock();
        m_sorted_servers.getData().push_back(server);
        m_sorted_servers.unlock();
        m_mapped_servers.lock();
        m_mapped_servers.getData()[server->getServerId()] = server;
        m_mapped_servers.unlock();
    }

    // ============================================================================
    int ServersManager::getNumServers () const
    {
        MutexLocker(m_sorted_servers);
        return m_sorted_servers.getData().size();
    }

    // ============================================================================
    const Server * ServersManager::getServerBySort (int index) const
    {
        MutexLocker(m_sorted_servers);
        return m_sorted_servers.getData().get(index);
    }

    // ============================================================================
    const Server * ServersManager::getServerByID (uint32_t id) const
    {
        MutexLocker(m_mapped_servers);
        return m_mapped_servers.getData().at(id);
    }

    // ============================================================================
    Server * ServersManager::getJoinedServer() const
    {
        return m_joined_server.getAtomic();
    }

    // ============================================================================
    void ServersManager::sort(bool sort_desc){
        MutexLocker(m_sorted_servers);
        m_sorted_servers.getData().insertionSort(0, sort_desc);
    }

} // namespace Online
