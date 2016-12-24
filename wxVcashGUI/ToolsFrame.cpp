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
#include "Resources.h"
#include "ToolsFrame.h"
#include "ToolsPanel.h"
#include <VcashApp.h>

#ifdef __WXGTK__
#define STYLE wxFRAME_FLOAT_ON_PARENT
#else
#define STYLE (wxFRAME_FLOAT_ON_PARENT | wxRESIZE_BORDER)
#endif

using namespace wxGUI;

ToolsFrame::ToolsFrame(VcashApp &vcashApp, wxWindow &parent)
    : wxFrame(&parent, wxID_ANY, wxT("ToolsFrame"), wxDefaultPosition, wxDefaultSize, STYLE) {

SetIcon(Resources::vcashIcon);

ToolsPanel *toolsPanel = new ToolsPanel(vcashApp, *this);

vcashApp.view.toolsFrame = this;

wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
sizerV->Add(toolsPanel
           , 1        // make horizontally stretchable
           , wxEXPAND // make it fill parent
);
SetSizerAndFit(sizerV);
}
