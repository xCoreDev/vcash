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

#include "MainFrame.h"
#include "Resources.h"
#include "ToolsFrame.h"
#include "VcashApp.h"

#ifdef __WXGTK__
#include <X11/Xlib.h>
#endif

#if defined(__WXMSW__) && (_MSC_VER >= 1900)
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
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