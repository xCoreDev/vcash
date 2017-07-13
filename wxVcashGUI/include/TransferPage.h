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