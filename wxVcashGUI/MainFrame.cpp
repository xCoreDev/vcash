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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/sizer.h>
#endif

#include "MainFrame.h"
#include "MainPanel.h"
#include "OnPairsEvent.h"
#include "Resources.h"
#include "StatusBar.h"
#include "ToolsFrame.h"
#include "VcashApp.h"

#ifdef __WXGTK__
#define STYLE (wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
#else
#define STYLE wxDEFAULT_FRAME_STYLE
#endif

using namespace wxGUI;

MainFrame::MainFrame(VcashApp &vcashApp)
        : wxFrame(NULL, wxID_ANY, wxT("Vcash"), wxDefaultPosition, wxDefaultSize,STYLE) {

    SetIcon(Resources::vcashIcon);

    MainPanel *vcashPanel = new MainPanel(vcashApp, *this);

    ToolsFrame *toolsFrame = new ToolsFrame(vcashApp, *this);
    toolsFrame->Show(false);

    vcashApp.view.statusBar = new StatusBar(vcashApp, *this, *toolsFrame);
    SetStatusBar(vcashApp.view.statusBar);

    vcashApp.view.mainFrame = this;

    wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
    sizerV->Add(vcashPanel
            , 1        // make horizontally stretchable
            , wxEXPAND // make it fill parent
    );

    SetSizerAndFit(sizerV);

    Bind(wxEVT_ACTIVATE, [this, toolsFrame](wxActivateEvent &event) {
        if (event.GetActive()) {
            toolsFrame->Show(false);
        }
    });

    Bind(wxEVT_ONSTATUS, [&vcashApp](OnStatusEvent &event) {
        vcashApp.controller.OnStatus(event.GetPairs());
    });

    Bind(wxEVT_ONERROR, [&vcashApp](OnErrorEvent &event) {
        vcashApp.controller.OnError(event.GetPairs());
    });

    auto moveToolsFrame = [this, &vcashApp, toolsFrame]() {
        wxRect rect;
        vcashApp.view.statusBar->GetFieldRect(StatusBar::Tools, rect);
        toolsFrame->Move(vcashApp.view.statusBar->ClientToScreen(
                wxPoint(rect.x+rect.width/2, rect.y+rect.height/2)));
    };

    Bind(wxEVT_MOVE, [moveToolsFrame](wxMoveEvent &event) {
        moveToolsFrame();
        event.Skip();
    });

    Bind(wxEVT_SIZE, [moveToolsFrame](wxSizeEvent &event) {
        moveToolsFrame();
        event.Skip();
    });
};


