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
 
#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/activityindicator.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/statusbr.h>
#endif

#include <map>

namespace wxGUI {

    class VcashApp;
    class StatusBarWallet;

    class StatusBar : public wxStatusBar {
    public:
        enum Pane {
            Tools = 0, Msg = 1, Settings = 2, Locked = 3
        };
    private:
        static const int numFields = 4;
        const std::pair<Pane, int> fieldsWidths[numFields] = {{Tools,    40},
                                                              {Msg,      -1},
                                                              {Settings, 25},
                                                              {Locked,   40}};
        StatusBarWallet *statusBarWallet;
        wxActivityIndicator *activityIndicator;
    public:
        StatusBar(VcashApp &vcashApp, wxWindow &parent, wxFrame &toolsFrame);

        void setMessage(wxString msg);

        void setWorking(bool working);
    };
}

#endif // STATUSBAR_H
