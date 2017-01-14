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
 
#ifndef CONSOLEPAGE_H
#define CONSOLEPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/panel.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/textctrl.h>
#include <wx/window.h>
#endif

#include <set>

namespace wxGUI {

    class VcashApp;

    class ConsolePage : public wxPanel {
    public:
        ConsolePage(VcashApp &vcashApp, wxWindow &parent);

        void appendToConsole(const std::string &text, bool bold = false);

    private:
        wxRichTextCtrl *output;
        wxTextCtrl *command;
        bool wasDoubleClick, focusFromCommand;
        int consecutiveEnters;

        // Commands for autocompleting
        static const std::set<std::string> rpcCommands;
    };
}

#endif // CONSOLEPAGE_H