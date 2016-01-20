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


#ifndef HEADER_SERVER_INFO_DIALOG_HPP
#define HEADER_SERVER_INFO_DIALOG_HPP

#include <irrString.h>

#include "guiengine/modaldialog.hpp"
#include "guiengine/widgets/icon_button_widget.hpp"
#include "guiengine/widgets/ribbon_widget.hpp"
#include "guiengine/widgets/label_widget.hpp"
#include "online/server.hpp"
#include "online/current_user.hpp"
#include "utils/types.hpp"


/**
 * \brief Dialog that allows a user to sign in
 * \ingroup states_screens
 */
class ServerInfoDialog : public GUIEngine::ModalDialog
{

private:

    bool m_self_destroy;
    bool m_enter_lobby;
    bool m_from_server_creation;
    const Online::CurrentUser::ServerJoinRequest * m_server_join_request;

    const uint32_t m_server_id;
    uint32_t m_host_id;

    GUIEngine::LabelWidget * m_name_widget;
    GUIEngine::LabelWidget * m_info_widget;

    GUIEngine::RibbonWidget * m_options_widget;
    GUIEngine::IconButtonWidget * m_join_widget;
    GUIEngine::IconButtonWidget * m_cancel_widget;

    void requestJoin();

public:
    ServerInfoDialog(uint32_t server_id, uint32_t host_id, bool just_created = false);
    ~ServerInfoDialog();

    void onEnterPressedInternal();
    GUIEngine::EventPropagation processEvent(const std::string& eventSource);

    virtual bool onEscapePressed();
    virtual void onUpdate(float dt);
};

#endif
