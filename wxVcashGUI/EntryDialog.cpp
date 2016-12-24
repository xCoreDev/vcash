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
#include <wx/textdlg.h>
#include <wx/stattext.h>
#endif

#include "EntryDialog.h"

using namespace wxGUI;

EntryDialog::EntryDialog( wxWindow &parent, const wxString &title
                        , const std::vector<Entry> &entries
                        , std::function<bool (const std::vector<wxString> &)> validate)
        : textCtrls()
        , validate(validate)
        , wxDialog(&parent, wxID_ANY, title) {

    int cols = 2, vgap = 5, hgap = 10;
    wxFlexGridSizer *fgs = new wxFlexGridSizer(cols, vgap, hgap);

    for(auto const& entry: entries) {
        fgs->Add(new wxStaticText(this, wxID_ANY, entry.label+":"), wxSizerFlags().Right());
        wxTextCtrl *textCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, entry.size, entry.style | wxTE_PROCESS_ENTER );
        textCtrl->SetToolTip(entry.toolTip);
        fgs->Add(textCtrl, 1, wxRIGHT | wxEXPAND, 20);
        textCtrls.push_back(textCtrl);
    }

    wxButton *okButton = new wxButton(this, wxID_ANY, wxT("Ok"));
    wxButton *cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"));

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(okButton);
    hbox->AddSpacer(10);
    hbox->Add(cancelButton);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(fgs, 1, wxALIGN_CENTER  | wxALL, 20);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizerAndFit(vbox);

    okButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { onReturnKeyPressed(); });
    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { EndModal(wxID_CANCEL); });
    for(auto const& entry: textCtrls)
        entry->Bind(wxEVT_KEY_DOWN, &EntryDialog::onKeyPressed, this);
}

void EntryDialog::onReturnKeyPressed() {
    // BEWARE BEWARE BEWARE BEWARE BEWARE:
    //
    // According to wxWidgets documentation:
    //   ToStdString: Return the string as an std::string in current locale encoding.
    //   Note that if the conversion of (Unicode) string contents to the current
    //   locale fails, the return string will be empty. Be sure to check for this
    //   to avoid silent data loss.
    //
    // We only allow for ascii contents in these strings to avoid problems
    // on different machines with different namespaces.

    for(auto const& entry: textCtrls) {
        wxString value = entry->GetValue();
        if(!value.IsAscii() || value.empty()) {
            entry->SetFocus();
            return;
        }
    }

    values.clear();
    for(auto const& entry: textCtrls)
        values.push_back(entry->GetValue());
    if(validate(values))
        EndModal(wxID_OK);
}

void EntryDialog::onKeyPressed(wxKeyEvent &ev) {
    switch (ev.GetKeyCode()) {
        case WXK_ESCAPE:
            values.clear();
            EndModal(wxID_CANCEL);
            break;
        case WXK_RETURN:
            onReturnKeyPressed();
            break;
        default:
            ev.Skip();
    }
}

std::vector<wxString> EntryDialog::getValues() {
    return values;
}

std::pair<int, std::vector<wxString>>
    EntryDialog::run( wxWindow &parent
                    , const wxString &title
                    , const std::vector<EntryDialog::Entry> &entries
                    , std::function<bool(const std::vector<wxString> &)> validate) {

    EntryDialog entryDialog(parent, title, entries, validate);
    auto result = entryDialog.ShowModal();
    entryDialog.Destroy();

    return make_pair(result, entryDialog.getValues());
}
