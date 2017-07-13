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
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/textentry.h>
#include <wx/valnum.h>
#include <wx/valtext.h>
#endif

#include "TransferPage.h"
#include "VcashApp.h"
#include "wxStack.h"

using namespace wxGUI;

TransferPage::TransferPage(VcashApp &vcashApp, wxWindow &parent) : wxPanel(&parent) {
    double vcashMaxSupply = wxStack::maxMoneySupply / wxStack::oneVcash;
    int decimals = 6;
    wxFloatingPointValidator<double>
            payValidator(decimals, nullptr, wxNUM_VAL_ZERO_AS_BLANK | wxNUM_VAL_NO_TRAILING_ZEROES);
    payValidator.SetRange(0, vcashMaxSupply);

    wxTextValidator toValidator(wxFILTER_ALPHANUMERIC);

    wxTextCtrl *payCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, payValidator);
    wxTextCtrl *toCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0 /*, toValidator*/);

    wxCheckBox *zeroTimeCtrl = new wxCheckBox(this, wxID_ANY, wxT("ZeroTime"));
    zeroTimeCtrl->SetValue(false);
    wxCheckBox *blendedOnlyCtrl = new wxCheckBox(this, wxID_ANY, wxT("Blended Only"));
    blendedOnlyCtrl->SetValue(false);

    wxButton *send = new wxButton(this, wxID_ANY, wxT("Send"));

    payCtrl->SetToolTip(wxT("Amount of coins to send"));
    toCtrl->SetToolTip(wxT("Destination address to send coins to"));
    zeroTimeCtrl->SetToolTip(wxT("Enable ZeroTime for this transaction"));
    blendedOnlyCtrl->SetToolTip(wxT("Use only blended coins for this transaction"));
    send->SetToolTip(wxT("Send coins now"));

    payCtrl->SetHint(wxT("0.0"));
    toCtrl->SetHint(wxT("destination address"));

    const int cols = 2, vgap = 5, hgap = 10, border = 20;
    wxFlexGridSizer *fgs = new wxFlexGridSizer(cols, vgap, hgap);
    fgs->Add(new wxStaticText(this, wxID_ANY, wxT("Pay:")), wxSizerFlags().Right());
    fgs->Add(payCtrl, 1, wxRIGHT | wxEXPAND, 165);
    fgs->Add(new wxStaticText(this, wxID_ANY, wxT("To:")), wxSizerFlags().Right());
    fgs->Add(toCtrl, 1, wxEXPAND);
    fgs->AddGrowableCol(1, 1);

    wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
    sizerV->Add(zeroTimeCtrl);
    sizerV->Add(blendedOnlyCtrl);

    wxSizer *sizerH = new wxBoxSizer(wxHORIZONTAL);
    sizerH->Add(sizerV, 1, wxALL | wxEXPAND, 0);
    sizerH->Add(send, wxSizerFlags());

    fgs->Add(0, border);
    fgs->Add(0, 0);

    fgs->Add(0, 0);
    fgs->Add(sizerH, wxSizerFlags().Expand());

    // Add a border
    wxSizer *pageSizer = new wxBoxSizer(wxVERTICAL);
    pageSizer->Add(fgs, 1, wxALL | wxEXPAND, border);

    SetSizerAndFit(pageSizer);

    send->Bind(wxEVT_BUTTON,
       [&parent, &vcashApp, payCtrl, toCtrl, zeroTimeCtrl, blendedOnlyCtrl](wxCommandEvent &) {
           Controller &controller = vcashApp.controller;

           std::string pay = payCtrl->GetValue().ToStdString();

           if (!controller.validateAmount(pay)) {
               wxMessageBox(pay + wxT(" is not a valid amount of coins."), wxT("Error"),
                            wxOK | wxICON_EXCLAMATION, &parent
               );
               return;
           }

           std::string to = toCtrl->GetValue().ToStdString();
           if (!controller.validateAddress(to)) {
               wxMessageBox(to + wxT(" is not a valid address."), wxT("Error"), wxOK | wxICON_EXCLAMATION,
                            &parent
               );
               return;
           }

           wxString msg =
                   wxT("Send ") + pay + wxT(" coins to ") + to + wxT("?\nA transaction fee will be added.");
           auto response = wxMessageBox(msg, wxT("Send Coins"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION,
                                        &parent
           );

           if (response == wxYES) {
               bool zeroTime = zeroTimeCtrl->GetValue();
               bool blendedOnly = blendedOnlyCtrl->GetValue();
               controller.onSendPressed(pay, to, zeroTime, blendedOnly);
           }
    });
}


