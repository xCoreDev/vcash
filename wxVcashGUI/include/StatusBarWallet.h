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
 
#ifndef STATUSBARWALLET_H
#define STATUSBARWALLET_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/bitmap.h>
#endif

#include "StatusBarImage.h"

namespace wxGUI {

    class VcashApp;

    enum WalletStatus {
        Unknown, Unencrypted, Locked, Unlocked
    };

    class StatusBarWallet : public StatusBarImage {
    public:
        StatusBarWallet(VcashApp &vcashApp, wxWindow &parent);

        WalletStatus getWalletStatus();

        void setWalletStatus(WalletStatus st);

    private:
        WalletStatus walletStatus;

        wxBitmap bitmapLocked, bitmapUnlocked, bitmapUnknown;
    };
}

#endif // STATUSBARWALLET
