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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/window.h>
#endif

namespace wxGUI {
    class VcashApp;

    class DumpHDSeedDlg : public wxDialog {
    public:
        DumpHDSeedDlg(VcashApp &vcashApp, wxWindow &parent, const wxString &title);
    };

    class SettingsMenu : public wxMenu {
    public:
        SettingsMenu(VcashApp &vcashApp, wxWindow &parent);
    };


}
#endif // SETTINGS_H
