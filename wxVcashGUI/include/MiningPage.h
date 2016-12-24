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
 
#ifndef MININGPAGE_H
#define MININGPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/window.h>

#endif

namespace wxGUI {

    class VcashApp;

    class MiningPage : public wxPanel {
    public:
        MiningPage(VcashApp &vcashApp, wxWindow &parent);

        void setMining(bool b);

        bool getMining();

        void toogleMining();

        void setHash(const std::string &hashRate);

    private:
        wxButton *mining;
        wxStaticText *hash;
        bool isMining;
    };
}

#endif // MININGPAGE_H