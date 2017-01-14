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
 
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/frame.h>
#endif

namespace wxGUI {

    class VcashApp;

    class MainFrame : public wxFrame {
    public:
        MainFrame(VcashApp &vcashApp);
        void minimizeToTray();
        void restoreFromTray();
    private:
        VcashApp &vcashApp;
    };
}

#endif // MAINFRAME_H