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
#include <wx/sizer.h>
#include <wx/stattext.h>
#endif

#include "AccountPage.h"
#include "VcashApp.h"

using namespace wxGUI;

AccountPage::AccountPage(VcashApp &vcashApp, wxWindow &parent) : wxPanel(&parent) {
    vcashApp.view.accountPage = this;

    balance = new wxStaticText(this, wxID_ANY, wxT("0"));
    unconfirmed = new wxStaticText(this, wxID_ANY, wxT("0"));
    stake = new wxStaticText(this, wxID_ANY, wxT("0"));
    immature = new wxStaticText(this, wxID_ANY, wxT("0"));

    int cols = 2, vgap = 10, hgap = 20;
    wxSizer *gridSizer = new wxGridSizer(cols, vgap, hgap);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Balance:")), wxSizerFlags().Right());
    gridSizer->Add(balance, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Unconfirmed:")), wxSizerFlags().Right());
    gridSizer->Add(unconfirmed, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Stake:")), wxSizerFlags().Right());
    gridSizer->Add(stake, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Immature:")), wxSizerFlags().Right());
    gridSizer->Add(immature, wxSizerFlags().Left());

    wxSizer *sizerH = new wxBoxSizer(wxVERTICAL);
    sizerH->AddSpacer(vgap);
    sizerH->Add(gridSizer, wxSizerFlags().Border(wxALL, hgap).Center());

    SetSizerAndFit(sizerH);
}

void AccountPage::setBalance(const std::string &balance) {
    this->balance->SetLabel(wxString(balance));
}

void AccountPage::setUnconfirmed(const std::string &unconfirmed) {
    this->unconfirmed->SetLabel(wxString(unconfirmed));
}

void AccountPage::setStake(const std::string &stake) {
    this->stake->SetLabel(wxString(stake));
}

void AccountPage::setImmature(const std::string &immature) {
    this->immature->SetLabel(wxString(immature));
}
