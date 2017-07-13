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

ConsolePage::ConsolePage(VcashApp &vcashApp, wxWindow &parent)
        : consecutiveEnters(0)
        , wasDoubleClick(false)
        , focusFromCommand(false)
        , wxPanel(&parent) {
    vcashApp.view.consolePage = this;

    const int cols = 1, vgap = 10, hgap = 15;
    wxFlexGridSizer *fgs = new wxFlexGridSizer(cols, vgap, hgap);

    output = new wxRichTextCtrl(
            this, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxVSCROLL | wxBORDER_NONE | wxWANTS_CHARS);


#if defined (__WXMSW__)
    wxFont fixedFont = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas"));
#else
    wxFont fixedFont = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
#endif
    output->SetFont(fixedFont);

    command = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    wxButton *clear = new wxButton(this, wxID_ANY, wxT("Clear"));

    output->SetToolTip(wxT("Results of invoked RPC commands"));
    command->SetToolTip(wxT("RPC command to invoke"));
    clear->SetToolTip(wxT("Clear all previous output"));

    wxArrayString wxRpcCommands;
    for(auto rpc : rpcCommands)
        wxRpcCommands.Add(wxString(rpc));
    command->AutoComplete(wxRpcCommands);

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

    auto onEnter = [this, &vcashApp](wxEvent &ev) {
        std::string cmd = command->GetValue().ToStdString();

        bool execute = consecutiveEnters > 1;

        if(!execute) {
            // check if cmd is a prefix of any command
            int isPrefix = 0;
            for(auto rpc : rpcCommands) {
                auto res = std::mismatch(cmd.begin(), cmd.end(), rpc.begin());
                if(res.first == cmd.end()) {
                    isPrefix++;
                }
            }
#if defined(__WXMSW__)
            execute = isPrefix <= 1;
#else
            if((isPrefix==1) && (rpcCommands.find(cmd) != rpcCommands.end()))
                execute = true;
            else
                execute = isPrefix <= 0;
#endif
        }

        if(execute) {
            // execute rpc command
            vcashApp.controller.onConsoleCommandEntered(cmd);
            command->Clear();
            consecutiveEnters = 0;
        } else {
            // select word from list
#if defined(__WXMSW__)
            //MSVC
            command->SelectNone();
            command->SetInsertionPointEnd();
#endif
            ev.Skip();
        }
    };

    command->Bind(wxEVT_KEY_DOWN, [this, onEnter](wxKeyEvent &ev) {
        if(ev.GetKeyCode() == WXK_RETURN)
            consecutiveEnters++;
        else
            consecutiveEnters = 0;
        switch (ev.GetKeyCode()) {
            case WXK_RETURN:
                onEnter(ev);
                break;
            case WXK_ESCAPE:
                consecutiveEnters++;
                long from, to;
                command->GetSelection(&from, &to);
                if(from!=to)
                    command->Remove(from, to);
#if defined(__WXMSW__)
                else
                    command->Clear();
#endif
                ev.Skip();
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

    command->Bind(wxEVT_KILL_FOCUS, [this](wxFocusEvent &ev) {
       focusFromCommand = true;
        ev.Skip();
    });

    output->Bind(wxEVT_SET_FOCUS, [this, &parent](wxFocusEvent &ev) {
#if defined(__WXMSW__)
        command->SetFocus();
#else
        if(focusFromCommand)
            parent.SetFocus();
        else
            command->SetFocus();
#endif
        focusFromCommand = false;
    });

    auto onClick = [this, &vcashApp](wxMouseEvent &ev) {
        wxTextCoord col , row;
        auto result = output->HitTest(ev.GetPosition(), &col, &row);
        if(result != wxTE_HT_BELOW  && result != wxTE_HT_BEYOND) {
            wxString line = output->GetLineText(row);
            command->SetValue(line);
            command->SetInsertionPointEnd();
        }
        ev.Skip();
    };

    output->Bind(wxEVT_LEFT_DCLICK, [this, onClick, onEnter](wxMouseEvent &ev) {
        onClick(ev);
        onEnter(ev);
        wasDoubleClick = true;
    });

    output->Bind(wxEVT_LEFT_UP, [this, onClick](wxMouseEvent &ev) {
        if(!wasDoubleClick)
            onClick(ev);
        wasDoubleClick = false;
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

const std::set<std::string> ConsolePage::rpcCommands =
    { "backupwallet"
    , "chainblender", "checkwallet"
    , "databaseenv", "databasefind", "databasestore"
    , "dumpwalletseed", "dumpprivkey", "dumpwallet"
    , "encryptwallet"
    , "getaccount", "getaccountaddress", "getbalance", "getblock", "getblockcount"
    , "getblockhash", "getblocktemplate", "getdifficulty", "getincentiveinfo"
    , "getinfo"
    , "incentive", "importprivkey"
    , "listsinceblock", "listreceivedbyaccount", "listreceivedbyaddress", "listtransactions"
    , "getmininginfo", "getnetworkhashps", "getnetworkinfo"
    , "getnewaddress", "getpeerinfo", "getrawmempool", "getrawtransaction", "gettransaction"
    , "repairwallet"
    , "sendfrom", "sendmany", "sendtoaddress", "settxfee", "submitblock"
    , "validateaddress"
    , "walletdenominate", "walletlock", "walletpassphrase", "walletpassphrasechange"
    , "ztlock"
    };
