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
#include <wx/notebook.h>
#include <wx/sizer.h>
#endif

#include "AccountPage.h"
#include "HistoryPage.h"
#include "MainPanel.h"
#include "TransferPage.h"
#include "View.h"
#include "VcashApp.h"

using namespace wxGUI;

MainPanel::MainPanel(VcashApp &vcashApp, wxWindow &parent)
    : wxPanel(&parent, wxID_ANY) {

    wxNotebook *notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "MainPanel");

    notebook->AddPage(new AccountPage(vcashApp, *notebook), wxT("Account"));
    notebook->AddPage(new TransferPage(vcashApp, *notebook), wxT("Transfer"));
    notebook->AddPage(new HistoryPage(vcashApp, *notebook), wxT("History"));

    wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
    sizerV->Add(notebook, 1     // make horizontally stretchable
            , wxALL | wxEXPAND  // make border all around & fill parent
            , 10                // border size
    );

    SetSizerAndFit(sizerV);
}
