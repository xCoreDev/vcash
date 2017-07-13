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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textdlg.h>
#include <wx/textctrl.h>
#include <VcashApp.h>
#endif

#include "Settings.h"

using namespace wxGUI;

DumpHDSeedDlg::DumpHDSeedDlg(VcashApp &vcashApp, wxWindow &parent, const wxString &title)
        : wxDialog(&parent, wxID_ANY, title) {

    wxTextCtrl *seedCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300, 100), wxTE_MULTILINE | wxTE_BESTWRAP);
    wxButton *closeButton = new wxButton(this, wxID_ANY, wxT("Close"));

    wxString seed = vcashApp.controller.getHDSeed();

    seedCtrl->SetValue(seed.empty() ? wxT("This wis not a hierarchical deterministic wallet!") : seed);
    seedCtrl->SetEditable(false);

    if(!seed.empty())
        seedCtrl->SetToolTip(wxT("This is your HD seed. Keep it safe!"));

    wxBoxSizer *hbox = new wxBoxSizer(wxVERTICAL);
    hbox->Add(seedCtrl);
    hbox->Add(closeButton);

    SetSizerAndFit(hbox);

    closeButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { EndModal(wxID_OK); });

    seedCtrl->Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent &ev) {
        switch (ev.GetKeyCode()) {
            case WXK_ESCAPE:
                EndModal(wxID_CANCEL);
                break;
            case WXK_RETURN:
                EndModal(wxID_OK);
                break;
            default:
                ev.Skip();
        }
    });
}

SettingsMenu::SettingsMenu(VcashApp &vcashApp, wxWindow &parent) : wxMenu() {
    enum PopupMenu {
        About, Seed
    };

    bool walletUnlocked = vcashApp.controller.isWalletLoaded() && !vcashApp.controller.isWalletLocked();

    if(walletUnlocked) {
        Append(Seed, wxT("&Show HD seed"));
    } else {
        Append(Seed, wxT("&Show HD seed (unlock first!)"));
        Enable(Seed, false);
    }
    AppendSeparator();
    Append(About, wxT("&About"));

    auto select = parent.GetPopupMenuSelectionFromUser(*this);

    switch(select) {
        case Seed: {
            DumpHDSeedDlg *dumpSeedDlg = new DumpHDSeedDlg(vcashApp, parent, wxT("Show seed"));
            dumpSeedDlg->ShowModal();
            dumpSeedDlg->Destroy();
            break;
        }
        case About: {
            wxMessageBox(wxT("A wxWidgets wallet for Vcash.\nCopyright (C) krypt.Richards@gmail.com."), wxT("About wxVcash"),
                         wxOK | wxICON_INFORMATION, &parent);
            break;
        }
        default:
            break;
    }
}
