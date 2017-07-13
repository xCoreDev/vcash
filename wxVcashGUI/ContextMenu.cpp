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
#include <wx/statbmp.h>
#endif

#include "ContextMenu.h"
#include "Resources.h"
#include "VcashApp.h"
#include "WalletActions.h"

using namespace wxGUI;

ContextMenu::ContextMenu(VcashApp &vcashApp, wxWindow &parent, wxPoint pos) : wxMenu() {
    enum PopupMenu {
        About, ChangePass, Encrypt, Exit, Lock, Rescan, Seed, Unlock
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
    }
    Append(About, wxT("&About"));

    Append(Exit, wxT("&Exit"));
    Enable(Exit, loaded);

    auto select = parent.GetPopupMenuSelectionFromUser(*this, pos);

    switch(select) {
        case About: {
            class AboutDlg : public ShowInfoDialog {
            public:
                AboutDlg(VcashApp &vcashApp, wxWindow &parent) : ShowInfoDialog(parent, wxT("About wxVcash"), [this, &vcashApp]() {
                    wxStaticBitmap *bm= new wxStaticBitmap(this, wxID_ANY, wxBitmap(Resources::vcashImage64));
                    wxStaticText *text =
                            new wxStaticText(this, wxID_ANY,
                                             wxT("A wxWidgets wallet for Vcash.\n"
                                                         "Copyright (C) The Vcash Developers.\n\n"
                                                         "Vcash version: "+vcashApp.controller.getVcashVersion()));
                    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

                    hbox->Add(bm, 0, wxALL | wxALIGN_CENTER, 10);
                    hbox->Add(text, 0, wxALL | wxALIGN_CENTER, 10);
                    return hbox;
                }) {}
            };

            new AboutDlg(vcashApp, parent);
            break;
        }
        case ChangePass: {
            WalletActions::changePassword(vcashApp, parent);
            break;
        }
        case Exit: {
            vcashApp.exit();
            break;
        }
        case Encrypt: {
            WalletActions::encrypt(vcashApp, parent);
            break;
        }
        case Lock: {
            WalletActions::lock(vcashApp, parent);
            break;
        }
        case Rescan: {
            WalletActions::rescan(vcashApp, parent);
            break;
        }
        case Seed: {
            WalletActions::dumpHDSeed(vcashApp, parent);
            break;
        }
        case Unlock: {
            WalletActions::unlock(vcashApp, parent);
            break;
        }
        default:
            break;
    }
}
