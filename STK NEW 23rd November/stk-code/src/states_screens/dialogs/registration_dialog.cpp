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

#include "states_screens/dialogs/registration_dialog.hpp"

#include <IGUIEnvironment.h>

#include "audio/sfx_manager.hpp"
#include "config/player.hpp"
#include "guiengine/engine.hpp"
#include "states_screens/state_manager.hpp"
#include "states_screens/register_screen.hpp"
#include "utils/translation.hpp"
#include "utils/string_utils.hpp"
#include "online/messages.hpp"


using namespace GUIEngine;
using namespace irr;
using namespace irr::gui;
using namespace Online;

// -----------------------------------------------------------------------------

RegistrationDialog::RegistrationDialog() :
        ModalDialog(0.8f,0.9f)
{
    loadFromFile("online/registration_terms.stkgui");
    ListWidget * terms_widget = getWidget<ListWidget>("terms");


    terms_widget->addItem("title", "=== STK Terms and Conditions ===", -1 , true );
    terms_widget->addItem("par1", "You must agree to these terms in order to register an account for STK.", -1 , false );
    terms_widget->addItem("par2",
        "Still needs actual content. Preferably in an XML document which can then be parsed to be put here."
        , -1 , false );
    terms_widget->addItem("par3",
        "By checking the box below, you are confirming that you understand these terms."
        "If you have any questions or comments regarding these terms,"
        "one of the members of the development team would gladly assist you."
        , -1 , false );

   // showRegistrationTerms();
}

// -----------------------------------------------------------------------------

RegistrationDialog::~RegistrationDialog()
{
}

// -----------------------------------------------------------------------------
/** Process input events.
 *  \event_source name of the widget that triggered the event.
 */
EventPropagation RegistrationDialog::processEvent(const std::string& event_source)
{
    if (event_source == "options")
    {
        RibbonWidget *rib = getWidget<RibbonWidget>("options");
        std::string s = rib->getSelectionIDString(PLAYER_ID_GAME_MASTER);
        if(s=="accept")
        {
            Screen *s = GUIEngine::getCurrentScreen();
            RegisterScreen *r = dynamic_cast<RegisterScreen*>(s);
            assert(r);
            r->acceptTerms();
        }
        // If it's not accept, it's cancel - anyway, close dialog
        ModalDialog::dismiss();
        return EVENT_BLOCK;
    }
    return EVENT_LET;
}   // processEvent
