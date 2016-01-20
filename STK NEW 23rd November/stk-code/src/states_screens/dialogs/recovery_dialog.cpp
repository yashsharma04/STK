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

#include "states_screens/dialogs/recovery_dialog.hpp"

#include <IGUIEnvironment.h>

#include "audio/sfx_manager.hpp"
#include "config/player.hpp"
#include "guiengine/engine.hpp"
#include "states_screens/state_manager.hpp"
#include "utils/translation.hpp"
#include "utils/string_utils.hpp"
#include "online/messages.hpp"


using namespace GUIEngine;
using namespace irr;
using namespace irr::gui;
using namespace Online;

// -----------------------------------------------------------------------------

RecoveryDialog::RecoveryDialog() :
        ModalDialog(0.8f,0.8f)
{
    m_recovery_request = NULL;
    m_self_destroy = false;
    m_show_recovery_input = true;
}

// -----------------------------------------------------------------------------

RecoveryDialog::~RecoveryDialog()
{
    delete m_recovery_request;
}

// -----------------------------------------------------------------------------

void RecoveryDialog::showRecoveryInput(){
    m_show_recovery_input = false;
    clearWindow();
    m_phase = Input;
    loadFromFile("online/recovery_input.stkgui");

    m_username_widget = getWidget<TextBoxWidget>("username");
    assert(m_username_widget != NULL);
    m_username_widget->setFocusForPlayer(PLAYER_ID_GAME_MASTER);

    m_email_widget = getWidget<TextBoxWidget>("email");
    assert(m_email_widget != NULL);

    m_info_widget = getWidget<LabelWidget>("info");
    assert(m_info_widget != NULL);

    m_options_widget = getWidget<RibbonWidget>("options");
    assert(m_options_widget != NULL);
    m_submit_widget = getWidget<IconButtonWidget>("submit");
    assert(m_submit_widget != NULL);
    m_cancel_widget = getWidget<IconButtonWidget>("cancel");
    assert(m_cancel_widget != NULL);
}

// -----------------------------------------------------------------------------

void RecoveryDialog::showRecoveryInfo(){
    m_show_recovery_info = false;
    clearWindow();
    m_phase = Info;
    loadFromFile("online/recovery_info.stkgui");

    m_info_widget = getWidget<LabelWidget>("info");
    assert(m_info_widget != NULL);

    m_options_widget = getWidget<RibbonWidget>("options");
    assert(m_options_widget != NULL);
    m_cancel_widget = getWidget<IconButtonWidget>("cancel");
    assert(m_cancel_widget != NULL);
}

// -----------------------------------------------------------------------------

bool RecoveryDialog::onEscapePressed()
{
    return m_cancel_widget->isActivated();
}

// -----------------------------------------------------------------------------

void RecoveryDialog::processInput()
{
    const stringw username = m_username_widget->getText().trim();
    const stringw email = m_email_widget->getText().trim();
    if (username.size() < 4 || username.size() > 30 || email.size() < 4 || email.size() > 50)
    {
        sfx_manager->quickSound("anvil");
        m_info_widget->setErrorColor();
        m_info_widget->setText(_("Username and/or email address invalid."), false);
    }
    else
    {
        m_info_widget->setDefaultColor();
        m_options_widget->setDeactivated();
        m_recovery_request = CurrentUser::get()->requestRecovery(username, email);
    }
}

// -----------------------------------------------------------------------------

GUIEngine::EventPropagation RecoveryDialog::processEvent(const std::string& eventSource)
{
    std::string selection;
    if (eventSource == m_options_widget->m_properties[PROP_ID])
        selection = m_options_widget->getSelectionIDString(PLAYER_ID_GAME_MASTER);
    else
        selection = eventSource;
    if (selection == m_cancel_widget->m_properties[PROP_ID])
    {
        m_self_destroy = true;
        return GUIEngine::EVENT_BLOCK;
    }
    else if (selection == m_submit_widget->m_properties[PROP_ID])
    {
        processInput();
        return GUIEngine::EVENT_BLOCK;
    }
    return GUIEngine::EVENT_LET;
}

// -----------------------------------------------------------------------------

void RecoveryDialog::onEnterPressedInternal()
{

    if (GUIEngine::isFocusedForPlayer(m_options_widget, PLAYER_ID_GAME_MASTER))
        return;
    if (m_submit_widget->isActivated())
        processInput();
}

// -----------------------------------------------------------------------------

void RecoveryDialog::onUpdate(float dt)
{
    if(m_recovery_request  != NULL)
    {
        if(m_recovery_request->isDone())
        {
            if(m_recovery_request->isSuccess())
            {
                m_show_recovery_info = true;
            }
            else
            {
                sfx_manager->quickSound( "anvil" );
                m_info_widget->setErrorColor();
                m_info_widget->setText(m_recovery_request->getInfo(), false);
                m_options_widget->setActivated();
            }
            delete m_recovery_request;
            m_recovery_request = NULL;
        }
        else
        {
            m_info_widget->setText(Messages::validatingInfo(), false);
        }
    }
    // It's unsafe to delete from inside the event handler so we do it here
    if (m_self_destroy)
        ModalDialog::dismiss();
    else if (m_show_recovery_input)
        showRecoveryInput();
    else if (m_show_recovery_info)
        showRecoveryInfo();
}
