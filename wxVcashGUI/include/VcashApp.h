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
 
#ifndef VCASHAPP_H
#define VCASHAPP_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/app.h>
#endif

#include "Controller.h"
#include "View.h"

namespace wxGUI {

    class VcashApp : public wxApp {
    public:
        VcashApp();

        Controller controller;
        View view;

    private:
        bool OnInit();
        int OnExit();
    };
}

#endif // VCASHAPP_H