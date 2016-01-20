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

#ifndef HEADER_NETWORKING_LOBBY_HPP
#define HEADER_NETWORKING_LOBBY_HPP

#include "guiengine/screen.hpp"
#include "guiengine/widgets/label_widget.hpp"
#include "guiengine/widgets/ribbon_widget.hpp"
#include "guiengine/widgets/icon_button_widget.hpp"
#include "online/server.hpp"

namespace GUIEngine { class Widget; class ListWidget; }

/**
  * \brief Handles the main menu
  * \ingroup states_screens
  */
class NetworkingLobby :     public GUIEngine::Screen,
                            public GUIEngine::ScreenSingleton<NetworkingLobby>
{
private:
    friend class GUIEngine::ScreenSingleton<NetworkingLobby>;

    Online::Server * m_server;

    NetworkingLobby();

    GUIEngine::IconButtonWidget * m_back_widget;

    GUIEngine::LabelWidget * m_server_name_widget;

    GUIEngine::LabelWidget * m_online_status_widget;

    GUIEngine::RibbonWidget * m_bottom_menu_widget;
    GUIEngine::IconButtonWidget * m_exit_widget;

    /** \brief Sets which widget has to be focused. Depends on the user state. */
    void setInitialFocus();

public:

    virtual void onUpdate(float delta) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget, const std::string& name,
                               const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void beforeAddingWidget() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void tearDown() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual bool onEscapePressed() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void onDisabledItemClicked(const std::string& item) OVERRIDE;

    /** \brief Implements the callback when a dialog gets closed. */
    virtual void onDialogClose() OVERRIDE;
};

#endif
