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
#include "TaskBarIcon.h"
#include "ToolsFrame.h"
#include "Utils.h"
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

const wxCmdLineEntryDesc VcashApp::cmdLineDesc[] =  {
    { wxCMD_LINE_SWITCH , "?", "help", "shows command line help",
                wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_SWITCH , "ib", "import-blockchain", "imports blockchain from blockchain.dat file",
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_SWITCH , "eb", "export-blockchain", "exports blockchain to blockchain.dat file",
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_SWITCH , "ewt", "erase-wallet-transactions", "erases wallet transactions",
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_OPTION, "ws", "wallet-seed", "restores wallet from hierarchical deterministic seed",
            wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
    { wxCMD_LINE_OPTION, "m", "mode", "use spv for light client (currenty unsupported)",
            wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
    { wxCMD_LINE_NONE }
};

// These two methods let us parse command line args
void VcashApp::OnInitCmdLine(wxCmdLineParser &parser) {
    parser.SetLogo(wxT("\nVcash version: "+controller.getVcashVersion()+"\nCopyright (C) The Vcash Developers\n"));
    parser.SetDesc(cmdLineDesc);
    parser.AddUsageText(wxT("\n"));
}

bool VcashApp::OnCmdLineParsed(wxCmdLineParser& parser) {
    wxString value;

    if (parser.Parse() == 0) {
        if(parser.Found("?")) {
            parser.Usage();
            return false;
        } else {
            if(parser.Found("ib"))
                args["import-blockchain"] = "1";
            if(parser.Found("eb"))
                args["export-blockchain"] = "1";
            if(parser.Found("ewt"))
                args["erase-wallet-transactions"] = "1";
            if(parser.Found("ws", &value))
                args["wallet-seed"] = value.ToStdString();
            if(parser.Found("m", &value))
                args["mode"] = value.ToStdString();
        }
        return true;
    } else
        return false;
}

bool VcashApp::OnInit() {
      if(singleInstanceChecker.IsAnotherRunning()) 
        return false;
    
    if (!wxApp::OnInit())
        return false;

    Resources::init();

    // Create and fill in view
    MainFrame *mainFrame = new MainFrame(*this);
    mainFrame->Show(true);
    SetTopWindow(mainFrame);

    // try to restore seed if no wallet exists
    bool isClient = Utils::find("mode", args) == "spv";
    if(!controller.walletExists(isClient)) {
        std::string seed = Utils::find("wallet-seed", args);
        if(seed.empty()) {
            auto pair = view.restoreHDSeed(*this);
            if (pair.first) {
                seed = pair.second;
                args["wallet-seed"] = seed;
            }
        }
    }

    // Create taskbar icon
    taskBarIconEnabled = TaskBarIcon::isEnabled();
    if(taskBarIconEnabled)
        view.taskBarIcon = new TaskBarIcon(*this);

    // Set global keys accelerators
    #define CTRL_T      (wxID_HIGHEST + 1)
    #define CTRL_M      (wxID_HIGHEST + 2)
    #define NUM_ACCELS  2

    wxAcceleratorEntry entries[NUM_ACCELS];
    entries[0].Set(wxACCEL_CTRL, 't', CTRL_T);
    entries[1].Set(wxACCEL_CTRL, 'm', CTRL_M);

    wxAcceleratorTable accel(NUM_ACCELS, entries);
    view.mainFrame->SetAcceleratorTable(accel);
    view.toolsFrame->SetAcceleratorTable(accel);

    Bind(wxEVT_MENU, [this](wxCommandEvent &ev) {
        view.showHideToolsFrame();
    }, CTRL_T);

    Bind(wxEVT_MENU, [this](wxCommandEvent &ev) {
        view.showContextMenu(*this, false);
    }, CTRL_M);

    return controller.onInit(args);
}

// This method is called before the main frame is destroyed. If
// false is returned, exit is aborted.
bool VcashApp::canExit() {
    if(!controller.isWalletLoaded())
        return false;
    else {
        if(taskBarIconEnabled)
            view.taskBarIcon->disable();
        return true;
    }
}

// This method is called once the main frame has been destroyed
int VcashApp::OnExit() {
    // Destroy taskbar icon (it isn't destroyed automatically)
    if(taskBarIconEnabled)
        view.taskBarIcon->~TaskBarIcon();
    // stop the stack
    return controller.onExit();
}

// Calling this method is the same as clicking close button on main frame.
void VcashApp::exit() {
    // Closes main frame. As a result, all wxWindows are destroyed.
    // Finally, OnExit is called and the stack is stopped.
    view.mainFrame->Close();
}


