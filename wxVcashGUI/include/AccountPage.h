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
 
#ifndef ACCOUNTPAGE_H
#define ACCOUNTPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/window.h>
#endif

namespace wxGUI {

    class VcashApp;

    class AccountPage : public wxPanel {
    public:
        AccountPage(VcashApp &vcashApp, wxWindow &parent);

        void setBalance(const std::string &balance);

        void setUnconfirmed(const std::string &unconfirmed);

        void setStake(const std::string &stake);

        void setImmature(const std::string &immature);

    private:
        wxStaticText *balance, *unconfirmed, *stake, *immature;
    };
}

#endif // ACCOUNTPAGE_H