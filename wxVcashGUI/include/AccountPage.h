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