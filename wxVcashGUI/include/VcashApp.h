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