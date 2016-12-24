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
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textdlg.h>
#endif

#include "EntryDialog.h"
#include "VcashApp.h"
#include "WalletActions.h"

using namespace wxGUI;

bool WalletActions::encrypt(VcashApp &vcashApp, wxWindow &parent) {
    wxString title = wxT("Encrypt wallet");
    int result = wxMessageBox(
            wxT("Encrypting your wallet keeps it safe in case it is lost or stolen.\n"
                "Do you want to encrypt your wallet?"),
            title, wxYES_NO | wxICON_QUESTION | wxYES_DEFAULT, &parent);
    if(result == wxYES) {
        wxMessageBox(
                wxT("Do a safe back up of your password. Once encrypted, you \n"
                    "will not be able to use your funds without the password."),
                title, wxOK | wxICON_INFORMATION, &parent);

        auto pair = EntryDialog::run( parent, title
                , { { wxT("Password"), wxTE_PASSWORD, wxT("Enter your password"), wxDefaultSize }
                  , { wxT("Confirm"), wxTE_PASSWORD, wxT("Reenter your password"), wxDefaultSize }
                  }
                , [](std::vector<wxString> values){ return values[0] == values[1]; }
                );

        switch(pair.first) {
            case wxID_CANCEL:
                break;
            case wxID_OK:
                vcashApp.controller.encryptWallet(
                        pair.second[0].ToStdString());
                break;
            default:
                break;
        }
    }
    return false;
}

void WalletActions::dumpHDSeed(VcashApp &vcashApp, wxWindow &parent) {
    DumpHDSeedDlg *dumpSeedDlg = new DumpHDSeedDlg(vcashApp, parent);
    dumpSeedDlg->ShowModal();
    dumpSeedDlg->Destroy();
}


std::pair<bool, std::string> WalletActions::restoreHDSeed(wxWindow &parent) {
    // toDo check that deterministic wallets are set on config.dat
    wxString title = wxT("Create wallet");

    int result = wxMessageBox(
            wxT("Do you want to restore your wallet from\na backed up hierarchical deterministic seed?"),
            wxT("Restore wallet"), wxYES_NO | wxICON_QUESTION | wxYES_DEFAULT, &parent);

    if(result==wxYES) {

        std::vector<EntryDialog::Entry> entries = {
                {wxT("HD seed"), wxTE_MULTILINE, wxT("Enter your hierarchical deterministic seed"), wxSize(300, 100)},
        };

        auto pair = EntryDialog::run( parent, wxT("Restore wallet"), entries
                                    , [](std::vector<wxString> values) {
                                         return !values[0].empty();
                                      }
                                    );
        if(pair.first == wxID_OK) {
            std::string seed = pair.second[0].ToStdString();
            // toDo check that this is a valied HD seed
            return std::make_pair(true, seed);
        } else {
            wxMessageBox(
                    wxT("You did not provide a hierarchical deterministic seed."),
                    title, wxOK | wxICON_INFORMATION, &parent);
        }
    }
    wxMessageBox(
            wxT("A new wallet will be generated.\n"
                        "Wait until wallet is loaded to do a safe\n"
                        "back up of your hierarchical deterministic seed."),
            title, wxOK | wxICON_INFORMATION, &parent);
    return std::make_pair(false, "");
}

bool WalletActions::changePassword(VcashApp &vcashApp, wxWindow &parent) {
    wxString title = wxT("Change password");
    int result = wxMessageBox(
            wxT("Do you want to change your wallet password?"),
            title, wxYES_NO | wxICON_QUESTION | wxYES_DEFAULT, &parent);
    if(result == wxYES) {
        wxMessageBox(
                wxT("In order to change your password, you need to provide your\n"
                     "old password firstly.\n"
                     "Do a safe back up of your new password. Once encrypted, you\n"
                     "will not be able to use your funds without the password."),
                title, wxOK | wxICON_INFORMATION, &parent);

        // lock wallet to be able to check old password
        vcashApp.controller.walletLock();

        auto pair = EntryDialog::run( parent, title
                , { { wxT("Old password"), wxTE_PASSWORD, wxT("Enter your old password"), wxDefaultSize }
                  , { wxT("New password"), wxTE_PASSWORD, wxT("Enter your new password"), wxDefaultSize }
                  , { wxT("Confirm new password"), wxTE_PASSWORD, wxT("Reenter your new password"), wxDefaultSize }
                  }
                , [&vcashApp](std::vector<wxString> values){
                    return vcashApp.controller.onWalletWantUnlock(values[0].ToStdString())
                            && (values[1] == values[2]);
                }
        );

        switch(pair.first) {
            case wxID_CANCEL:
                break;
            case wxID_OK:
                vcashApp.controller.walletChangePassword(
                        pair.second[0].ToStdString(),
                        pair.second[1].ToStdString());

                // let wallet locked after password change
                vcashApp.controller.walletLock();

                break;
            default:
                break;
        }
    }
    return false;
}

bool WalletActions::unlock(VcashApp &vcashApp, wxWindow &parent) {
    auto pair = EntryDialog::run( parent
                    , wxT("Unlock wallet")
                    , { { wxT("Password"), wxTE_PASSWORD, wxT("Enter your password"), wxDefaultSize } }
                    , [&vcashApp](std::vector<wxString> values) {
                        bool unlocked = vcashApp.controller.onWalletWantUnlock(values[0].ToStdString());
                        if(unlocked)
                            vcashApp.view.setWalletStatus(Unlocked);
                        return unlocked;
                    }
    );
    return pair.first == wxID_OK;
}

void WalletActions::rescan(VcashApp &vcashApp, wxWindow &parent) {
    wxString title = wxT("Rescan wallet");
    int result = wxMessageBox(
            wxT("Do you want to rescan your wallet?"),
            title, wxYES_NO | wxICON_QUESTION | wxYES_DEFAULT, &parent);
    if(result == wxYES) {
        result = wxMessageBox(
                wxT("The wallet will shutdown. You will have to\n"
                    "restart the wallet in order to complete the rescan."
                ),
                title, wxOK | wxCANCEL | wxOK_DEFAULT | wxICON_INFORMATION, &parent);
        if(result == wxOK)
            vcashApp.controller.rescanWallet();
    }
}

DumpHDSeedDlg::DumpHDSeedDlg(VcashApp &vcashApp, wxWindow &parent)
        : wxDialog(&parent, wxID_ANY, wxT("Show HD seed")) {

    wxString warning = wxT("This is your hierarchical deterministic seed.\n"
                           "Anyone knowing this seed can access your\n"
                           "funds. Keep it safe!");
    wxTextCtrl *seedCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300, 100), wxTE_MULTILINE | wxTE_BESTWRAP);
    wxButton *closeButton = new wxButton(this, wxID_ANY, wxT("Close"));

    wxString seed = vcashApp.controller.getHDSeed();

    seedCtrl->SetValue(seed.empty() ? wxT("This is not a hierarchical deterministic wallet!") : seed);
    seedCtrl->SetEditable(false);


    wxBoxSizer *hbox = new wxBoxSizer(wxVERTICAL);
    int border = 10;
    if(!seed.empty()) {
        hbox->Add(new wxStaticText(this, wxID_ANY, warning), 0, wxALL, border);
        seedCtrl->SetToolTip(warning);
    }

    hbox->Add(seedCtrl, 1, wxALL, border);
    hbox->Add(closeButton, wxSizerFlags().Center());
    hbox->AddSpacer(border);

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
        About, ChangePass, Encrypt, Lock, Seed, Unlock, Rescan
    };

    bool loaded = vcashApp.controller.isWalletLoaded();
    if(loaded) {
        wxMenu *submenu = new wxMenu();
        if(!vcashApp.controller.isWalletLocked()) {
            submenu->Append(Seed, wxT("&Show HD seed"));
        } else {
            submenu->Append(Seed, wxT("&Show HD seed (unlock first!)"));
            submenu->Enable(Seed, false);
        }
        if(vcashApp.controller.isWalletCrypted())
            submenu->Append(ChangePass, wxT("&Change password"));
        if(!vcashApp.controller.isWalletCrypted())
            submenu->Append(Encrypt, wxT("&Encrypt"));
        if(vcashApp.controller.isWalletLocked())
           submenu->Append(Unlock, wxT("&Unlock"));
        else if(vcashApp.controller.isWalletCrypted()) {
            submenu->Append(Lock, wxT("&Lock"));
        }
        submenu->Append(Rescan, wxT("&Rescan"));
        AppendSubMenu(submenu, wxT("Wallet"));
        AppendSeparator();
    }

    Append(About, wxT("&About"));

    auto select = parent.GetPopupMenuSelectionFromUser(*this);

    switch(select) {
        case Encrypt: {
            WalletActions::encrypt(vcashApp, parent);
            break;
        }
        case ChangePass: {
            WalletActions::changePassword(vcashApp, parent);
            break;
        }
        case Seed: {
            WalletActions::dumpHDSeed(vcashApp, parent);
            break;
        }
        case Lock: {
            vcashApp.controller.walletLock();
            break;
        }
        case Unlock: {
            WalletActions::unlock(vcashApp, parent);
            break;
        }
        case Rescan: {
            WalletActions::rescan(vcashApp, parent);
            break;
        }
        case About: {
            wxMessageBox(wxT("A wxWidgets wallet for Vcash.\nCopyright (C) The Vcash Developers."), wxT("About wxVcash"),
                         wxOK | wxICON_INFORMATION, &parent);
            break;
        }
        default:
            break;
    }
}