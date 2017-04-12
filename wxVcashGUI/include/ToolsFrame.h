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
    };
}

#endif // TOOLSFRAME_H