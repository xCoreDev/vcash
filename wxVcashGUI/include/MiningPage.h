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
 
#ifndef MININGPAGE_H
#define MININGPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/button.h>
#include <wx/panel.h>
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

    private:
        wxButton *mining;
        bool isMining;
    };
}

#endif // MININGPAGE_H