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
 
#ifndef TOOLSPANEL_H
#define TOOLSPANEL_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/window.h>
#endif

namespace wxGUI {

    class VcashApp;

    class ToolsPanel : public wxPanel {
    public:
        ToolsPanel(VcashApp &vcashApp, wxWindow &parent);
        void showConsolePage();
    private:
        wxNotebook *notebook;
    };
}

#endif // TOOLSPANEL_H