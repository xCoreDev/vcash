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
 
#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/panel.h>
#include <wx/window.h>
#endif

namespace wxGUI {

    class VcashApp;

    class MainPanel : public wxPanel {
    public:
        MainPanel(VcashApp &vcashApp, wxWindow &parent);
    };
}

#endif // MAINPANEL_H