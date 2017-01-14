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
#include <wx/stattext.h>
#include <wx/sizer.h>
#endif

#include "StatisticsPage.h"
#include "VcashApp.h"

using namespace wxGUI;

StatisticsPage::StatisticsPage(VcashApp &vcashApp, wxWindow &parent) : wxPanel(&parent) {
    vcashApp.view.statisticsPage = this;

    TCP = new wxStaticText(this, wxID_ANY, wxT("0"));
    UDP = new wxStaticText(this, wxID_ANY, wxT("0"));
    estimated = new wxStaticText(this, wxID_ANY, wxT("0"));
    total = new wxStaticText(this, wxID_ANY, wxT("0"));
    difficulty = new wxStaticText(this, wxID_ANY, wxT("0"));
    supply = new wxStaticText(this, wxID_ANY, wxT("0"));

    TCP->SetToolTip(wxT("Number of TCP connections"));
    UDP->SetToolTip(wxT("Number of UDP connections"));
    estimated->SetToolTip(wxT("Estimated total number of blocks"));
    total->SetToolTip(wxT("Number of blocks for your node"));
    difficulty->SetToolTip(wxT("Network difficulty"));
    supply->SetToolTip(wxT("Current money supply"));

    const int cols = 2, vgap = 5, hgap = 20;
    wxSizer *gridSizer = new wxGridSizer(cols, vgap, hgap);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Network TCP:")), wxSizerFlags().Right());
    gridSizer->Add(TCP, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Network UDP:")), wxSizerFlags().Right());
    gridSizer->Add(UDP, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Blocks Estimated:")), wxSizerFlags().Right());
    gridSizer->Add(estimated, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Blocks Total:")), wxSizerFlags().Right());
    gridSizer->Add(total, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Difficulty:")), wxSizerFlags().Right());
    gridSizer->Add(difficulty, wxSizerFlags().Left());
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Money Supply:")), wxSizerFlags().Right());
    gridSizer->Add(supply, wxSizerFlags().Left());

    wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
    sizerV->AddSpacer(vgap);
    sizerV->Add(gridSizer, wxSizerFlags().Border(wxALL, hgap).Center());

    SetSizerAndFit(sizerV);

    // This windows doesn't accept focus as it only shows information
    Bind(wxEVT_SET_FOCUS, [&parent](wxFocusEvent &ev) {
        parent.SetFocus();
        ev.Skip();
    });
}

void StatisticsPage::setTCP(const std::string &tcp) {
    this->TCP->SetLabel(wxString(tcp));
}

void StatisticsPage::setUDP(const std::string &udp) {
    this->UDP->SetLabel(wxString(udp));
}

void StatisticsPage::setEstimated(const std::string &estimated) {
    this->estimated->SetLabel(wxString(estimated));
}

void StatisticsPage::setTotal(const std::string &total) {
    this->total->SetLabel(wxString(total));
}

void StatisticsPage::setDifficulty(const std::string &difficulty) {
    this->difficulty->SetLabel(wxString(difficulty));
}

void StatisticsPage::setSupply(const std::string &supply) {
    this->supply->SetLabel(wxString(supply));
}
