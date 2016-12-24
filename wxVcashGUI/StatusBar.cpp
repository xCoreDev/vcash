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

#include "Resources.h"
#include "StatusBarImage.h"
#include "StatusBar.h"
#include "StatusBarWallet.h"
#include "View.h"
#include "VcashApp.h"
#include "WalletActions.h"

using namespace wxGUI;

StatusBar::StatusBar(VcashApp &vcashApp, wxWindow &parent, wxFrame &toolsFrame)
        : wxStatusBar(&parent) {

    int ws[numFields];
    for (int i = 0; i < numFields; i++)
        ws[i] = fieldsWidths[i].second;
    SetFieldsCount(numFields, ws);

    StatusBarImage *toolsImg = new StatusBarImage(*this, Resources::tools);
    StatusBarImage *settingsImg = new StatusBarImage(*this, Resources::settings);
    toolsImg->bindOnLeftClick([this, &toolsFrame, toolsImg](wxMouseEvent &event) {
        int x = event.GetX();
        int y = event.GetY();

        wxPoint point = toolsImg->ClientToScreen(wxPoint(x, y));
        toolsFrame.Move(point.x, point.y);
        toolsFrame.Show(true);
        toolsFrame.Raise();
        toolsFrame.SetFocus();
    });

    settingsImg->bindOnLeftClick([this, &vcashApp, &parent](wxMouseEvent &event) {
        new SettingsMenu(vcashApp, parent);
    });

    vcashApp.view.walletLock = new StatusBarWallet(vcashApp, *this);

    double iconSz = wxMax(toolsImg->GetBestSize().GetHeight(),
                          vcashApp.view.walletLock->GetBestSize().GetHeight());
    SetMinHeight(iconSz);
    SetSize(-1, iconSz+2);
    parent.SendSizeEvent();

    Bind(wxEVT_SIZE, [this, &vcashApp, toolsImg, settingsImg](wxSizeEvent &event) {

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
            static void move(StatusBar &statusBar, Pane pane, wxStaticBitmap &bitmap) {
                wxRect rect;
                statusBar.GetFieldRect(pane, rect);
                wxSize size = bitmap.GetSize();
                bitmap.Move(rect.x + (rect.width - size.x) / 2
                        , rect.y + (rect.height - size.y) / 2 - ICON_OFFSET);
            }
        };

        Local::move(*this, Tools, *toolsImg);
        Local::move(*this, Settings, *settingsImg);
        Local::move(*this, Locked, *view.walletLock);

        event.Skip();
    });
};

void StatusBar::SetMessage(wxString msg) {
    SetStatusText(msg, Msg);
}
