//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2012-2013 SuperTuxKart-Team
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

#include <stdlib.h>

#include "config/player.hpp"
#include "utils/string_utils.hpp"
#include <sstream>


//------------------------------------------------------------------------------
PlayerProfile::PlayerProfile(const core::stringw& name) :
    m_player_group("Player", "Represents one human player"),
    m_name(name, "name", &m_player_group),
    m_is_guest_account(false, "guest", &m_player_group),
    m_use_frequency(0, "use_frequency", &m_player_group),
    m_unique_id("", "unique_id", &m_player_group)
{
#ifdef DEBUG
    m_magic_number = 0xABCD1234;
#endif
    int64_t unique_id = generateUniqueId(core::stringc(name.c_str()).c_str());

    std::ostringstream to_string;
    to_string << std::hex << unique_id;
    m_unique_id = to_string.str();
}

//------------------------------------------------------------------------------
PlayerProfile::PlayerProfile(const XMLNode* node) :
    m_player_group("Player", "Represents one human player"),
    m_name("-", "name", &m_player_group),
    m_is_guest_account(false, "guest", &m_player_group),
    m_use_frequency(0, "use_frequency", &m_player_group),
    m_unique_id("", "unique_id", &m_player_group)
{
    //m_player_group.findYourDataInAChildOf(node);
    m_name.findYourDataInAnAttributeOf(node);
    m_is_guest_account.findYourDataInAnAttributeOf(node);
    m_use_frequency.findYourDataInAnAttributeOf(node);
    m_unique_id.findYourDataInAnAttributeOf(node);

    if ((std::string)m_unique_id == "")
    {
        fprintf(stderr, "** WARNING: Player has no unique ID, probably it is from an older STK version\n");
        int64_t unique_id = generateUniqueId(core::stringc(m_name.c_str()).c_str());
        std::ostringstream tostring;
        tostring << std::hex << unique_id;
        m_unique_id = tostring.str();
    }

    #ifdef DEBUG
    m_magic_number = 0xABCD1234;
    #endif
}   // PlayerProfile

//------------------------------------------------------------------------------
void PlayerProfile::incrementUseFrequency()
{
    if (m_is_guest_account) m_use_frequency = -1;
    else m_use_frequency++;
}   // incrementUseFrequency

//------------------------------------------------------------------------------
int64_t PlayerProfile::generateUniqueId(const char* player_name)
{
    // First create a simple hash based on he player name
    int hash = 0;
    for (int n=0; player_name[n] != 0; n++)
    {
        hash += (hash << (hash & 0xF)) ^ player_name[n];
    }

    return ((int64_t)(StkTime::getTimeSinceEpoch()) << 32) |
           ((rand() << 16) & 0xFFFF0000)                   | 
           hash;
}   // generateUniqueId
