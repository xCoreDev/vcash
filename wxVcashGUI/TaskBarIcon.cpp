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

#include "ContextMenu.h"
#include "MainFrame.h"
#include "Resources.h"
#include "TaskBarIcon.h"
#include "VcashApp.h"

using namespace wxGUI;

TaskBarIcon::TaskBarIcon(VcashApp &vcashApp) : wxTaskBarIcon() {
    SetIcon(Resources::vcashIcon, wxT("Vcash"));

    Bind(wxEVT_TASKBAR_LEFT_DOWN, [this, &vcashApp](wxTaskBarIconEvent &ev) {
        bool wasShown = vcashApp.view.mainFrame->IsShown();
        if(wasShown)
            vcashApp.view.mainFrame->minimizeToTray();
        else
            vcashApp.view.mainFrame->restoreFromTray();
    });

    Bind(wxEVT_TASKBAR_RIGHT_DOWN, [&vcashApp] (wxTaskBarIconEvent &ev) {
        vcashApp.view.showContextMenu(vcashApp);
    });
}

bool TaskBarIcon::isEnabled() {
    return wxTaskBarIcon::IsAvailable();
}

void TaskBarIcon::disable() {
    wxImage image = Resources::vcashImage64.ConvertToDisabled();
    wxIcon icon;
    icon.CopyFromBitmap(wxBitmap(image));
    SetIcon(icon);
    //Unbind(wxEVT_TASKBAR_RIGHT_DOWN);
}
