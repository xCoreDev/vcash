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
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/textdlg.h>
#include <wx/stattext.h>
#endif

#include "EncryptWalletDlg.h"

using namespace wxGUI;

EncryptWalletDlg::EncryptWalletDlg(wxWindow &parent, const wxString &title)
        : wxDialog(&parent, wxID_ANY, title) {

    int cols = 2, vgap = 5, hgap = 10;
    wxFlexGridSizer *fgs = new wxFlexGridSizer(cols, vgap, hgap);
    fgs->Add(new wxStaticText(this, wxID_ANY, wxT("Password:")), wxSizerFlags().Right());
    passwordTC = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER );
    fgs->Add(passwordTC, 1, wxRIGHT | wxEXPAND, 20);
    fgs->Add(new wxStaticText(this, wxID_ANY, wxT("Confirm:")), wxSizerFlags().Right());
    confirmTC = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER );
    fgs->Add(confirmTC, 1, wxRIGHT | wxEXPAND, 20);

    wxButton *okButton = new wxButton(this, wxID_ANY, wxT("Ok"));
    wxButton *cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"));

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(okButton);
    hbox->Add(cancelButton);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(fgs, 1, wxALIGN_CENTER | wxALL, 20);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizerAndFit(vbox);

    okButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { onReturnKeyPressed(); });
    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { EndModal(wxID_CANCEL); });
    passwordTC->Bind(wxEVT_KEY_DOWN, &EncryptWalletDlg::onKeyPressed, this);
    confirmTC->Bind(wxEVT_KEY_DOWN, &EncryptWalletDlg::onKeyPressed, this);
}

void EncryptWalletDlg::onReturnKeyPressed() {
    password = passwordTC->GetValue().ToStdString();
    confirm = confirmTC->GetValue().ToStdString();

    // BEWARE BEWARE BEWARE BEWARE BEWARE:
    //
    // According to wxWidgets documentation:
    //   ToStdString: Return the string as an std::string in current locale encoding.
    //   Note that if the conversion of (Unicode) string contents to the current
    //   locale fails, the return string will be empty. Be sure to check for this
    //   to avoid silent data loss.
    //
    // empty() tests should be enough to detect and avoid `non-standard chars' in
    // text wxTextCtrls, but different passwords can be accepted on different machines
    // using different locales.

    if (password.empty())
        passwordTC->SetFocus();
    else if (confirm.empty())
        confirmTC->SetFocus();
    else if (password == confirm)
        EndModal(wxID_OK);
}

void EncryptWalletDlg::onKeyPressed(wxKeyEvent &ev) {
    switch (ev.GetKeyCode()) {
        case WXK_ESCAPE:
            EndModal(wxID_CANCEL);
            break;
        case WXK_RETURN:
            onReturnKeyPressed();
            break;
        default:
            ev.Skip();
    }
}

std::string EncryptWalletDlg::getPassword() {
    return password;
}
