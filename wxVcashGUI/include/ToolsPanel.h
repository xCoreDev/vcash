/******************************************************************************
 * wxVcashGUI: a GUI for Vcash, the decentralized currency 
 *             for the internet (https://v.cash/).
 *
 * Copyright (c) kryptRichards (krypt.Richards@gmail.com)
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