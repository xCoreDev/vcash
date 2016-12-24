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
#include <wx/textdlg.h>
#endif

#include "Resources.h"
#include "StatusBarWallet.h"
#include "VcashApp.h"
#include "WalletActions.h"

using namespace wxGUI;

StatusBarWallet::StatusBarWallet(VcashApp &vcashApp, wxWindow &parent)
        : walletStatus(Unknown)
        , bitmapLocked(Resources::locked)
        , bitmapUnlocked(Resources::unlocked)
        , bitmapUnknown (Resources::empty)
        , StatusBarImage(parent, *new wxBitmap(), [this, &parent, &vcashApp](wxMouseEvent &ev) {
              switch (walletStatus) {
                  case Locked: {
                      WalletActions::unlock(vcashApp, parent);
                      break;
                  }

                  case Unlocked: {
                      vcashApp.controller.walletLock();
                      break;
                  }

                  case Unencrypted: {
                      WalletActions::encrypt(vcashApp, parent);
                      break;
                  }

                  case Unknown: {
                      break;
                  }
              }
          }) {
    setWalletStatus(walletStatus);
};

WalletStatus StatusBarWallet::getWalletStatus() {
    return walletStatus;
}

void StatusBarWallet::setWalletStatus(WalletStatus st) {
    walletStatus = st;
    switch (walletStatus) {
        case Locked:
            SetBitmap(bitmapLocked);
            break;
        case Unlocked:
        case Unencrypted:
            SetBitmap(bitmapUnlocked);
            break;
        case Unknown:
            SetBitmap(bitmapUnknown);
            break;
    }
}

