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
#include "MainFrame.h"
#include "VcashApp.h"
#include "WalletActions.h"

using namespace wxGUI;

bool WalletActions::encrypt(VcashApp &vcashApp, wxWindow &parent) {
    wxString title = wxT("Encrypt wallet");

    bool result = false;
    if(vcashApp.controller.isWalletCrypted()) {
        wxMessageBox(
                wxT("Cannot encrypt wallet. Wallet is already encrypted.\n"
                    "Please, try to change password intead."),
                title, wxOK | wxICON_EXCLAMATION, &parent);
    } else {
        int res = wxMessageBox(
                wxT("Encrypting your wallet keeps it safe in case it is lost or stolen.\n"
                    "Do you want to encrypt your wallet?"),
                title, wxYES_NO | wxICON_QUESTION | wxYES_DEFAULT, &parent);
        if(res == wxYES) {
            wxMessageBox(
                    wxT("Do a safe back up of your password. Once encrypted, you\n"
                        "will not be able to use your funds without the password."),
                    title, wxOK | wxICON_INFORMATION, &parent);

            auto pair = EntryDialog::run( parent, title
                    , { { wxT("Password"), wxTE_PASSWORD, wxT("Enter your password"), wxDefaultSize }
                      , { wxT("Confirm"), wxTE_PASSWORD, wxT("Reenter your password"), wxDefaultSize }
                      }
                    , [&vcashApp](EntryDialog &dlg){
                        std::vector<wxString> values = dlg.getValues();
                        // check that password and confirmation are the same
                        if(!dlg.validate(values[0] == values[1]
                                        ,wxT("Password and confirmation are different")))
                            return false;
                        else if(!dlg.validate(vcashApp.controller.onWalletWantEncrypt(values[0].ToStdString())
                                             ,wxT("Encryption failed")))
                            return false;
                        else
                            return true;
                    }
            );
            result = pair.first == wxID_OK;
        }
    }
    vcashApp.view.setWalletStatus(vcashApp.controller.getWalletStatus());
    return result;
}

void WalletActions::dumpHDSeed(VcashApp &vcashApp, wxWindow &parent) {
    new DumpHDSeedDlg(vcashApp, parent);
}

std::pair<bool, std::string> WalletActions::restoreHDSeed(Controller &controller, wxWindow &parent) {
    // toDo check that deterministic wallets are set on config.dat. Seems like stack will check it
    wxString title = wxT("Create wallet");

    int result = wxMessageBox(
            wxT("Do you want to restore your wallet from\na backed up hierarchical deterministic seed?"),
            wxT("Restore wallet"), wxYES_NO | wxICON_QUESTION | wxYES_DEFAULT, &parent);

    if(result==wxYES) {

        std::vector<EntryDialog::Entry> entries = {
                {wxT("HD seed"), wxTE_MULTILINE, wxT("Enter your hierarchical deterministic seed"), wxSize(300, 100)},
        };

        auto pair = EntryDialog::run(parent, wxT("Restore wallet"), entries
                , [&controller](EntryDialog &dlg) {
                    std::vector<wxString> values = dlg.getValues();

                    std::string seed = dlg.getValues()[0].ToStdString();
                    bool ok = !(seed.empty())
                              && controller.validateHDSeed(seed);
                    return dlg.validate(ok, wxT("This is not a valid hierarchical deterministic seed."));
                }
        );
        if(pair.first == wxID_OK) {
            std::string seed = pair.second[0].ToStdString();
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

    bool result = false;
    if(!vcashApp.controller.isWalletCrypted()) {
        wxMessageBox(
                wxT("Cannot change password. Wallet is not currently encrypted"),
                title, wxOK | wxICON_EXCLAMATION, &parent);
    } else {
        int res = wxMessageBox(
                wxT("Do you want to change your wallet password?"),
                title, wxYES_NO | wxICON_QUESTION | wxYES_DEFAULT, &parent);
        if (res == wxYES) {
            wxMessageBox(
                    wxT("In order to change your password, you need\n"
                        "to provide your old password firstly.\n"
                        "Do a safe back up of your new password. Once\n"
                        "encrypted, you will not be able to use your\n"
                        "funds without this password."),
                    title, wxOK | wxICON_INFORMATION, &parent);

            auto pair = EntryDialog::run(parent, title,
                                         { {wxT("Old password"),         wxTE_PASSWORD, wxT("Enter your old password"),   wxDefaultSize}
                                         , {wxT("New password"),         wxTE_PASSWORD, wxT("Enter your new password"),   wxDefaultSize}
                                         , {wxT("Confirm new password"), wxTE_PASSWORD, wxT("Reenter your new password"), wxDefaultSize}
                                         }, [&vcashApp](EntryDialog &dlg) {
                        std::vector<wxString> values = dlg.getValues();
                        // confirm new password and confirmation are the same
                        if(!dlg.validate(values[1] == values[2]
                                ,wxT("New password and confirmation are different")))
                            return false;
                        else if(!dlg.validate(vcashApp.controller.walletChangePassword(values[0].ToStdString(), values[1].ToStdString())
                                ,wxT("Change of password failed. Maybe old password was incorrect")))
                            return false;
                        else
                            return true;
                    }
            );
            result = pair.first == wxID_OK;
        }
    }
    vcashApp.view.setWalletStatus(vcashApp.controller.getWalletStatus());
    return result;
}

bool WalletActions::lock(VcashApp &vcashApp, wxWindow &parent) {
    wxString title = wxT("Lock wallet");

    bool result = false;
    if(vcashApp.controller.isWalletLocked()) {
        wxMessageBox(
                wxT("Cannot lock wallet. Wallet is already locked"),
                title, wxOK | wxICON_EXCLAMATION, &parent);
    } else {
        result = vcashApp.controller.onWalletWantLock();
    }
    vcashApp.view.setWalletStatus(vcashApp.controller.getWalletStatus());
    return result;
}

bool WalletActions::unlock(VcashApp &vcashApp, wxWindow &parent) {
    wxString title = wxT("Unlock wallet");

    bool result = false;
    if(!vcashApp.controller.isWalletLocked()) {
        wxMessageBox(
                wxT("Cannot unlock wallet. Wallet is not currently locked"),
                title, wxOK | wxICON_EXCLAMATION, &parent);
    } else {
        auto pair = EntryDialog::run(parent
                , title
                , { { wxT("Password"), wxTE_PASSWORD, wxT("Enter your password"), wxDefaultSize } }
                , [&vcashApp](EntryDialog &dlg) {
                    std::vector<wxString> values = dlg.getValues();
                    if(!dlg.validate(vcashApp.controller.onWalletWantUnlock(values[0].ToStdString())
                                    ,wxT("Wallet could not be unlocked. Maybe password is incorrect")))
                        return false;
                    else
                        return true;
                }
        );
        result = pair.first == wxID_OK;
    }
    vcashApp.view.setWalletStatus(vcashApp.controller.getWalletStatus());
    return result;
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
        : ShowInfoDialog(parent, wxT("Show HD seed"), [this, &vcashApp]() {
    wxString warning = wxT("This is your hierarchical deterministic seed.\n"
                           "Anyone knowing this seed can access your\n"
                           "funds. Keep it safe!");
    wxTextCtrl *seedCtrl = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition
            , wxSize(260, 80), wxTE_MULTILINE | wxTE_BESTWRAP | wxTE_READONLY);

    wxString seed = vcashApp.controller.getHDSeed();

    seedCtrl->SetValue(seed.empty() ? wxT("This is not a hierarchical deterministic wallet!") : seed);
    seedCtrl->SetEditable(false);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    int border = 10;
    if(!seed.empty()) {
        vbox->Add(new wxStaticText(this, wxID_ANY, warning), 0, wxALL | wxALIGN_CENTER, border);
        seedCtrl->SetToolTip(warning);
    }

    vbox->Add(seedCtrl, 1, wxALL | wxALIGN_CENTER, border);
    return vbox;
}) {}

