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
#include <wx/msgdlg.h>
#endif

#include "ContextMenu.h"
#include "Resources.h"
#include "StatusBarImage.h"
#include "StatusBar.h"
#include "StatusBarWallet.h"
#include "View.h"
#include "VcashApp.h"

using namespace wxGUI;

StatusBar::StatusBar(VcashApp &vcashApp, wxWindow &parent, wxFrame &toolsFrame)
        : wxStatusBar(&parent) {

    int ws[numFields];
    for (int i = 0; i < numFields; i++)
        ws[i] = fieldsWidths[i].second;
    SetFieldsCount(numFields, ws);

    StatusBarImage *toolsImg = new StatusBarImage(*this, Resources::tools);
    StatusBarImage *settingsImg = new StatusBarImage(*this, Resources::settings);

    Bind(wxEVT_RIGHT_DOWN, [&vcashApp](wxMouseEvent &ev) {
        vcashApp.view.showHideToolsFrame(false);
        ev.Skip();
    });

    Bind(wxEVT_LEFT_DOWN, [&vcashApp](wxMouseEvent &ev) {
        vcashApp.view.showHideToolsFrame(false);
        ev.Skip();
    });

    toolsImg->bindOnClick([&vcashApp](wxMouseEvent &ev) {
        vcashApp.view.showHideToolsFrame(true);
        ev.Skip();
    });

    settingsImg->bindOnClick([&vcashApp](wxMouseEvent &ev) {
        vcashApp.view.showContextMenu(vcashApp);
    });

    statusBarWallet = new StatusBarWallet(vcashApp, *this);
    vcashApp.view.walletLock = statusBarWallet;

    toolsImg->SetToolTip(wxT("Click to open/close tools window"));
    settingsImg->SetToolTip(wxT("Click to open menu"));

    double iconSz = wxMax(toolsImg->GetBestSize().GetHeight(),
                          vcashApp.view.walletLock->GetBestSize().GetHeight());
    SetMinHeight(iconSz);
    SetSize(-1, iconSz+2);
    parent.SendSizeEvent();

    activityIndicator = new wxActivityIndicator(this, wxID_ANY, wxDefaultPosition, wxSize(iconSz,iconSz));

    Bind(wxEVT_SIZE, [this, &vcashApp, toolsImg, settingsImg](wxSizeEvent &ev) {
        View &view = vcashApp.view;

#if defined(__WXGTK__)
#define ICON_OFFSET 2
#elif defined(__WXMSW__)
#define ICON_OFFSET 0
#elif defined(__WXOSX__)
#define ICON_OFFSET 2.5
#else
#error "You must define one of: __WXGTK__, __WXMSW__ or __WXOSX__"
#endif
        struct Local {
            static void move(StatusBar &statusBar, Pane pane, wxWindow &bitmap) {
                wxRect rect;
                statusBar.GetFieldRect(pane, rect);
                wxSize size = bitmap.GetSize();
                bitmap.Move(rect.x + (rect.width - size.x) / 2
                           , rect.y + (rect.height - size.y) / 2 - ICON_OFFSET);
            }
        };

        Local::move(*this, Tools, *toolsImg);
        Local::move(*this, Settings, *settingsImg);
        Local::move(*this, Locked, *statusBarWallet);
        Local::move(*this, Locked, *activityIndicator);

        ev.Skip();
    });
};

void StatusBar::setMessage(wxString msg) {
    SetStatusText(msg, Msg);
}

void StatusBar::setWorking(bool working) {
    bool oldWorking = activityIndicator->IsRunning();
    if(oldWorking==working)
        return;
    if(working) {
        statusBarWallet->Hide();
        activityIndicator->Show();
        activityIndicator->Start();
    } else {
        activityIndicator->Stop();
        activityIndicator->Hide();
        statusBarWallet->Show();
    }
}