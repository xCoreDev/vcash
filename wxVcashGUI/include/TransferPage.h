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
 
#ifndef TRANSFERPAGE_H
#define TRANSFERPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/panel.h>
#endif

namespace wxGUI {

    class VcashApp;

    class TransferPage : public wxPanel {
    public:
        TransferPage(VcashApp &vcashApp, wxWindow &parent);
    };
}

#endif // TRANSFERPAGE_H