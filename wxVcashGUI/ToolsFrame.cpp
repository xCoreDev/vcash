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

#include "AddressesPage.h"
#include "MainFrame.h"
#include "Resources.h"
#include "StatusBar.h"
#include "ToolsFrame.h"
#include "ToolsPanel.h"
#include "VcashApp.h"

#ifdef __WXGTK__
#define STYLE (wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR)
#else
#define STYLE (wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR | wxRESIZE_BORDER)
#endif

using namespace wxGUI;

ToolsFrame::ToolsFrame(VcashApp &vcashApp, wxWindow &parent)
    : parent(parent)
    , vcashApp(vcashApp)
    , wxFrame(&parent, wxID_ANY, wxT("ToolsFrame"), wxDefaultPosition, wxDefaultSize, STYLE) {

    SetIcon(Resources::vcashIcon);

    ToolsPanel *toolsPanel = new ToolsPanel(vcashApp, *this);

    vcashApp.view.toolsFrame = this;

    wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
    sizerV->Add(toolsPanel
               , 1        // make horizontally stretchable
               , wxEXPAND // make it fill parent
    );
    SetSizerAndFit(sizerV);

    Bind(wxEVT_SET_FOCUS, [toolsPanel](wxFocusEvent &ev) {
       toolsPanel->SetFocus();
    });
}

void ToolsFrame::updatePosition() {
    if(parent.IsShown()) {
        wxRect rect;
        auto statusBar = vcashApp.view.statusBar;
        statusBar->GetFieldRect(StatusBar::Tools, rect);
        Move(statusBar->ClientToScreen(
                wxPoint(rect.x + rect.width / 2, rect.y + rect.height)));
    }
}