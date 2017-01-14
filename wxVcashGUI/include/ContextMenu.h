/******************************************************************************
 * wxVcashGUI: a GUI for Vcash, a decentralized currency
 *             for the internet (https://vcash.info).
 *
 * Copyright (c) The Vcash Developers
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 ******************************************************************************/

#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/menu.h>
#include <wx/window.h>
#endif

namespace wxGUI {
    class VcashApp;

    class ContextMenu : public wxMenu {
    public:
        ContextMenu(VcashApp &vcashApp, wxWindow &parent, wxPoint pos = wxDefaultPosition);
    };
}

#endif // CONTEXTMENU_H
 
