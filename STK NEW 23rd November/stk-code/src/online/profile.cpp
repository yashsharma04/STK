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


#include "online/profile.hpp"

#include "online/profile_manager.hpp"
#include "online/request_manager.hpp"
#include "config/user_config.hpp"
#include "online/current_user.hpp"
#include "utils/log.hpp"
#include "utils/translation.hpp"

#include <sstream>
#include <stdlib.h>
#include <assert.h>

using namespace Online;

namespace Online{



    Profile::RelationInfo::RelationInfo(const irr::core::stringw & date, bool is_online, bool is_pending, bool is_asker)
    {
        m_date = date;
        m_is_online = is_online;
        m_is_pending = is_pending;
        m_is_asker = is_asker;
    }

    // ============================================================================
    void Profile::RelationInfo::setOnline(bool online)
    {
        m_is_online = online;
        if(m_is_online)
            m_is_pending = false;
    }

    // ============================================================================
    Profile::Profile( const uint32_t           & userid,
                      const irr::core::stringw & username,
                      bool is_current_user)
    {
        m_state = S_READY;
        m_cache_bit = true;
        m_id = userid;
        m_is_current_user = is_current_user;
        m_username = username;
        m_has_fetched_friends = false;
        m_has_fetched_achievements = false;
        m_relation_info = NULL;
        m_is_friend = false;
    }

    Profile::Profile(const XMLNode * xml, ConstructorType type)
    {
        m_relation_info = NULL;
        m_is_friend = false;
        if(type == C_RELATION_INFO){
            irr::core::stringw date("");
            xml->get("date", &date);
            std::string is_pending_string("");
            xml->get("is_pending", &is_pending_string);
            bool is_pending = is_pending_string == "yes";
            bool is_asker(false);
            bool is_online(false);
            if(is_pending)
            {
                std::string is_asker_string("");
                xml->get("is_asker", &is_asker_string);
                is_asker = is_asker_string == "yes";
            }
            else
            {
                std::string is_online_string("");
                xml->get("online", &is_online_string);
                is_online = is_online_string == "yes";
                m_is_friend = true;
            }
            m_relation_info = new RelationInfo(date, is_online, is_pending, is_asker);
            xml = xml->getNode("user");
        }

        xml->get("id", &m_id);
        xml->get("user_name", &m_username);
        m_cache_bit = true;
        m_has_fetched_friends = false;
        m_has_fetched_achievements = false;
        m_is_current_user = (m_id == CurrentUser::get()->getID());
        m_state = S_READY;
    }
    // ============================================================================
    Profile::~Profile()
    {
        delete m_relation_info;
    }

    // ============================================================================
    void Profile::fetchAchievements()
    {
        assert(CurrentUser::get()->isRegisteredUser());
        if(m_has_fetched_achievements || m_is_current_user)
            return;
        m_state = S_FETCHING;
        requestAchievements();
    }

    // ============================================================================
    void Profile::achievementsCallback(const XMLNode * input)
    {
        m_achievements.clear();
        std::string achieved_string("");
        if(input->get("achieved", &achieved_string) == 1)
        {
            m_achievements = StringUtils::splitToUInt(achieved_string, ' ');
        }
        m_has_fetched_achievements = true;
        m_state = S_READY;
        Log::info("test","tit");
    }

    // ============================================================================

    void Profile::requestAchievements()
    {
        assert(CurrentUser::get()->isRegisteredUser() && !m_is_current_user);
        AchievementsRequest * request = new AchievementsRequest();
        request->setServerURL("client-user.php");
        request->addParameter("action","get-achievements");
        request->addParameter("token", CurrentUser::get()->getToken());
        request->addParameter("userid", CurrentUser::get()->getID());
        request->addParameter("visitingid", m_id);
        RequestManager::get()->addRequest(request);
    }

    void Profile::AchievementsRequest::callback()
    {
        uint32_t user_id(0);
        getXMLData()->get("visitingid", &user_id);
        if( ProfileManager::get()->getProfileByID(user_id) != NULL )
            ProfileManager::get()->getProfileByID(user_id)
                                 ->achievementsCallback(getXMLData());
    }

    // ============================================================================
    void Profile::fetchFriends()
    {
        assert(CurrentUser::get()->isRegisteredUser());
        if(m_has_fetched_friends)
            return;
        m_state = S_FETCHING;
        requestFriendsList();
    }
    // ============================================================================
    void Profile::friendsListCallback(const XMLNode * input)
    {
        const XMLNode * friends_xml = input->getNode("friends");
        m_friends.clear();
        for (unsigned int i = 0; i < friends_xml->getNumNodes(); i++)
        {
            Profile * profile;
            if(m_is_current_user)
            {
                profile = new Profile(friends_xml->getNode(i) , C_RELATION_INFO);
                ProfileManager::get()->addPersistent(profile);
            }
            else
            {
                profile = new Profile(friends_xml->getNode(i)->getNode("user"), C_DEFAULT);
                ProfileManager::get()->addToCache(profile);
            }
            m_friends.push_back(profile->getID());
        }
        m_has_fetched_friends = true;
        m_state = S_READY;
    }

    // ============================================================================

    void Profile::requestFriendsList()
    {
        assert(CurrentUser::get()->isRegisteredUser());
        FriendsListRequest * request = new FriendsListRequest();
        request->setServerURL("client-user.php");
        request->addParameter("action","get-friends-list");
        request->addParameter("token", CurrentUser::get()->getToken());
        request->addParameter("userid", CurrentUser::get()->getID());
        request->addParameter("visitingid", m_id);
        RequestManager::get()->addRequest(request);
    }

    void Profile::FriendsListRequest::callback()
    {
        uint32_t user_id(0);
        getXMLData()->get("visitingid", &user_id);
        if( ProfileManager::get()->getProfileByID(user_id) != NULL )
            ProfileManager::get()->getProfileByID(user_id)
                                ->friendsListCallback(getXMLData());
    }

    // ============================================================================

    void Profile::removeFriend( const uint32_t id)
    {
        assert (m_has_fetched_friends);
        std::vector<uint32_t>::iterator iter;
        for (iter = m_friends.begin(); iter != m_friends.end();)
        {
           if (*iter == id)
           {
               m_friends.erase(iter++);
               break;
           }
           else
               ++iter;
        }
    }
    // ============================================================================

    void Profile::addFriend( const uint32_t id)
    {
        assert (m_has_fetched_friends);
        for(unsigned int i=0; i< m_friends.size(); i++)
            if(m_friends[i] == id)
                return;
        m_friends.push_back(id);
    }

    // ============================================================================

    void Profile::deleteRelationalInfo()
    {
        delete m_relation_info;
        m_relation_info = NULL;
    }

    // ============================================================================
    const std::vector<uint32_t> & Profile::getFriends()
    {
        assert (m_has_fetched_friends && m_state == S_READY);
        return m_friends;
    }

    // ============================================================================
    const std::vector<uint32_t> & Profile::getAchievements()
    {
        assert (m_has_fetched_achievements && m_state == S_READY && !m_is_current_user);
        return m_achievements;
    }

    //=============================================================================
    void Profile::merge(Profile * profile)
    {
        assert (profile != NULL);
        if(!this->m_has_fetched_friends && profile->m_has_fetched_friends)
            this->m_friends = profile->m_friends;
        if(!this->m_has_fetched_achievements && profile->m_has_fetched_friends)
            this->m_achievements = profile->m_achievements;
        if(this->m_relation_info == NULL && profile->m_relation_info != NULL)
        {
            this->m_relation_info = profile->m_relation_info;
            profile->m_relation_info = NULL; //We don't want the destructor of the profile instance to destroy the relation info
        }
        delete profile;
    }

} // namespace Online
