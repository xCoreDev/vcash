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

#ifndef WALLETACTIONS_H
#define WALLETACTIONS_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/window.h>
#endif

#include "ShowInfoDialog.h"

namespace wxGUI {
    class VcashApp;

    class DumpHDSeedDlg : public ShowInfoDialog {
    public:
        DumpHDSeedDlg(VcashApp &vcashApp, wxWindow &parent);
    };

    class WalletActions {
    public:
        static bool changePassword(VcashApp &vcashApp, wxWindow &parent);
        static void dumpHDSeed(VcashApp &vcashApp, wxWindow &parent);
        static bool encrypt(VcashApp &vcashApp, wxWindow &parent);
        static std::pair<bool, std::string> restoreHDSeed(Controller &controller, wxWindow &parent);
        static bool lock(VcashApp &vcashApp, wxWindow &parent);
        static bool unlock(VcashApp &vcashApp, wxWindow &parent);
        static void rescan(VcashApp &vcashApp, wxWindow &parent);
    };

}
#endif // WALLETACTIONS_H
