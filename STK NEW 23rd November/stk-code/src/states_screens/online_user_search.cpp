//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2010 Lucas Baudin, Joerg Henrichs
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

#include "states_screens/online_user_search.hpp"

#include <iostream>
#include <assert.h>

#include "guiengine/modaldialog.hpp"
#include "states_screens/dialogs/user_info_dialog.hpp"
#include "states_screens/state_manager.hpp"
#include "states_screens/dialogs/message_dialog.hpp"
#include "utils/translation.hpp"
#include "utils/string_utils.hpp"
#include "online/messages.hpp"
#include "online/current_user.hpp"
#include "audio/sfx_manager.hpp"

using namespace Online;

DEFINE_SCREEN_SINGLETON( OnlineUserSearch );

// ----------------------------------------------------------------------------

OnlineUserSearch::OnlineUserSearch() : Screen("online/user_search.stkgui")
{
    m_selected_index = -1;
    m_search_request = NULL;
    m_search_string = "";
    m_last_search_string = "";
}   // OnlineUserSearch

// ----------------------------------------------------------------------------

OnlineUserSearch::~OnlineUserSearch()
{
}   // OnlineUserSearch

// ----------------------------------------------------------------------------

void OnlineUserSearch::tearDown()
{
    delete m_search_request;
    m_search_request = NULL;
}   // tearDown


// ----------------------------------------------------------------------------

void OnlineUserSearch::parseResult(const XMLNode * input)
{
    m_users.clear();
    const XMLNode * users_xml = input->getNode("users");
    for (unsigned int i = 0; i < users_xml->getNumNodes(); i++)
    {
        Profile * profile = new Profile(users_xml->getNode(i));
        ProfileManager::get()->addToCache(profile);
        m_users.push_back(profile->getID());
    }
}

void OnlineUserSearch::showList()
{
    m_user_list_widget->clear();
    for(unsigned int i=0; i < m_users.size(); i++)
    {
        PtrVector<GUIEngine::ListWidget::ListCell> * row = new PtrVector<GUIEngine::ListWidget::ListCell>;
        Profile * profile = ProfileManager::get()->getProfileByID(m_users[i]);
        assert(profile != NULL);
        row->push_back(new GUIEngine::ListWidget::ListCell(profile->getUserName(),-1,3));
        m_user_list_widget->addItem("user", row);
    }
}

// ----------------------------------------------------------------------------

void OnlineUserSearch::search()
{
    if ( m_search_string != "" && m_last_search_string != m_search_string )
        m_search_request = CurrentUser::get()->requestUserSearch(m_search_string);
    else
        m_fake_refresh = true;
    m_user_list_widget->clear();
    m_user_list_widget->addItem("spacer", L"");
    m_user_list_widget->addItem("loading", Messages::searching());
    m_back_widget->setDeactivated();
    m_search_box_widget->setDeactivated();
    m_search_button_widget->setDeactivated();
}


// ----------------------------------------------------------------------------

void OnlineUserSearch::loadedFromFile()
{
    m_back_widget = getWidget<GUIEngine::IconButtonWidget>("back");
    assert(m_back_widget != NULL);
    m_search_button_widget = getWidget<GUIEngine::ButtonWidget>("search_button");
    assert(m_search_button_widget != NULL);
    m_search_box_widget = getWidget<GUIEngine::TextBoxWidget>("search_box");
    assert(m_search_box_widget != NULL);
    m_user_list_widget = getWidget<GUIEngine::ListWidget>("user_list");
    assert(m_user_list_widget != NULL);
}   // loadedFromFile


// ----------------------------------------------------------------------------

void OnlineUserSearch::beforeAddingWidget()
{
    m_user_list_widget->clearColumns();
    m_user_list_widget->addColumn( _("Username"), 3 );
}
// ----------------------------------------------------------------------------

void OnlineUserSearch::init()
{
    Screen::init();
    search();
    m_fake_refresh = false;
    m_search_box_widget->setText(m_search_string);
}   // init

// ----------------------------------------------------------------------------
void OnlineUserSearch::eventCallback( GUIEngine::Widget* widget, const std::string& name, const int playerID)
{
    if (name == m_back_widget->m_properties[GUIEngine::PROP_ID])
    {
        StateManager::get()->escapePressed();
    }
    else if (name == m_user_list_widget->m_properties[GUIEngine::PROP_ID])
    {
        m_selected_index = m_user_list_widget->getSelectionID();
        new UserInfoDialog(m_users[m_selected_index]);
    }
    else if (name == m_search_button_widget->m_properties[GUIEngine::PROP_ID])
    {
        m_last_search_string = m_search_string;
        m_search_string = m_search_box_widget->getText().trim();
        search();
    }

}   // eventCallback

// ----------------------------------------------------------------------------
/** Selects the last selected item on the list (which is the item that
 *  is just being installed) again. This function is used from the
 *  addons_loading screen: when it is closed, it will reset the
 *  select item so that people can keep on installing from that
 *  point on.
*/
void OnlineUserSearch::setLastSelected() //FIXME actually use this here and in server selection
{
    if(m_selected_index>-1)
    {
        m_user_list_widget->setFocusForPlayer(PLAYER_ID_GAME_MASTER);
        m_user_list_widget->setSelectionID(m_selected_index);
    }
}   // setLastSelected

// ----------------------------------------------------------------------------

void OnlineUserSearch::onUpdate(float dt)
{
    if(m_search_request != NULL)
    {
        if(m_search_request->isDone())
        {
            if(m_search_request->isSuccess())
            {
                parseResult(m_search_request->getXMLData());
                showList();
            }
            else
            {
                sfx_manager->quickSound( "anvil" );
                new MessageDialog(m_search_request->getInfo());
            }
            delete m_search_request;
            m_search_request = NULL;
            m_back_widget->setActivated();
            m_search_box_widget->setActivated();
            m_search_button_widget->setActivated();
        }
        else
        {
            m_user_list_widget->renameItem("loading", Messages::searching());
        }
    }
    else if(m_fake_refresh)
    {
        showList();
        m_fake_refresh = false;
        m_back_widget->setActivated();
        m_search_box_widget->setActivated();
        m_search_button_widget->setActivated();
    }
}   // onUpdate
