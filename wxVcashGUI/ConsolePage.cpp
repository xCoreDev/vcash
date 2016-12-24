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
#endif

#include "ConsolePage.h"
#include "VcashApp.h"

using namespace wxGUI;

ConsolePage::ConsolePage(VcashApp &vcashApp, wxWindow &parent) : wxPanel(&parent) {
    vcashApp.view.consolePage = this;

    int cols = 1, vgap = 10, hgap = 15;
    wxFlexGridSizer *fgs = new wxFlexGridSizer(cols, vgap, hgap);

    output = new wxRichTextCtrl(
            this, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxVSCROLL | wxBORDER_NONE | wxWANTS_CHARS);


    wxFont fixedFont = wxFont(10, wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL);
    output->SetFont(fixedFont);

    command = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    wxButton *clear = new wxButton(this, wxID_ANY, wxT("Clear"));

    output->SetToolTip(wxT("Results of invoked RPC commands"));
    command->SetToolTip(wxT("RPC command to invoke"));
    clear->SetToolTip(wxT("Clear all previous output"));

    wxBoxSizer *row = new wxBoxSizer(wxHORIZONTAL);
    row->Add(command, 1, wxALL | wxEXPAND);
    row->Add(clear);

    fgs->Add(output, 1, wxEXPAND);
    fgs->Add(row, 1, wxALL | wxEXPAND);

    fgs->AddGrowableRow(0, 1);
    fgs->AddGrowableCol(0, 1);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(fgs, 1, wxALL | wxEXPAND, 10);
    SetSizerAndFit(hbox);
    Centre();

    command->Bind(wxEVT_TEXT_ENTER, [this, &vcashApp](wxCommandEvent &) {
        std::string cmd = command->GetValue().ToStdString();
        vcashApp.controller.onConsoleCommandEntered(cmd);
    });


    command->Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent &ev) {
        switch (ev.GetKeyCode()) {
            case WXK_UP:
                output->MoveUp();
                output->ScrollIntoView(output->GetCaretPosition(),WXK_UP);
                break;
            case WXK_DOWN:
                output->MoveDown();
                output->ScrollIntoView(output->GetCaretPosition(),WXK_UP);
                break;
            case WXK_HOME:
                output->MoveHome();
                output->ScrollIntoView(output->GetCaretPosition(),WXK_HOME);
                break;
            case WXK_END:
                output->MoveEnd();
                output->ScrollIntoView(output->GetCaretPosition(),WXK_END);
                break;
            case WXK_PAGEUP:
                output->PageUp();
                output->ScrollIntoView(output->GetCaretPosition(),WXK_PAGEUP);
                break;
            case WXK_PAGEDOWN:
                output->PageDown();
                output->ScrollIntoView(output->GetCaretPosition(),WXK_PAGEUP);
                break;
            default:
                ev.Skip();
        }
    });

    output->Bind(wxEVT_SET_FOCUS, [this](wxFocusEvent &) {
        command->SetFocus();
    });

    clear->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) {
        output->Clear();
    });
}

void ConsolePage::appendToConsole(const std::string &text, bool bold) {
    output->MoveEnd();
    output->ScrollIntoView(output->GetCaretPosition(),WXK_END);
    if(bold) {
        output->BeginBold();
        output->BeginFontSize(output->GetFont().GetPointSize()+2);
    }
    output->WriteText(wxString(text));
    if(bold) {
        output->EndFont();
        output->EndBold();
    }
    output->Newline();

    output->ShowPosition(output->GetLastPosition());
}
