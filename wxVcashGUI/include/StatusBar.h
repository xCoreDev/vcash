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
 
#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/statusbr.h>
#endif

#include <map>

namespace wxGUI {

    class VcashApp;

    class StatusBar : public wxStatusBar {
    public:
        enum Pane {
            Tools, Msg, Settings, Locked
        };
    private:
        const std::map<Pane, int> fieldsWidths{{Tools,    40},
                                               {Msg,      -1},
                                               {Settings, 25},
                                               {Locked,   40}};
    public:
        StatusBar(VcashApp &vcashApp, wxWindow &parent, wxFrame &toolsFrame);

        void SetMessage(wxString msg);
    };
}

#endif // STATUSBAR_H