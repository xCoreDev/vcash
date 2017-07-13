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
 
#ifndef TOOLSFRAME_H
#define TOOLSFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/frame.h>
#include <wx/window.h>
#endif

namespace wxGUI {

    class VcashApp;

    class ToolsFrame : public wxFrame {
    public:
        ToolsFrame(VcashApp &vcashApp, wxWindow &parent);
        void updatePosition();
    private:
        wxWindow &parent;
        VcashApp &vcashApp;
    };
}

#endif // TOOLSFRAME_H