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

#include "MainFrame.h"
#include "Resources.h"
#include "ToolsFrame.h"
#include "VcashApp.h"

#ifdef __WXGTK__
#include <X11/Xlib.h>
#endif

IMPLEMENT_APP(wxGUI::VcashApp)

using namespace wxGUI;

VcashApp::VcashApp() : view(), controller(view) {
#ifdef __WXGTK__
    // This App uses multiple threads.
    // Must be called here in the constructor of the GUI App
    XInitThreads();
#endif
}

bool VcashApp::OnInit() {
    if (!wxApp::OnInit())
        return false;

    Resources::init();

    // Create and fill in view
    MainFrame *vcashFrame = new MainFrame(*this);
    vcashFrame->Show(true);

    return controller.onInit();
}

int VcashApp::OnExit() {
    return controller.onExit();
}
