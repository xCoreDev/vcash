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

#ifndef RESOURCES_H
#define RESOURCES_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/bitmap.h>
#include <wx/icon.h>
#endif

namespace wxGUI {

    class Resources {
    public:
        static void init();

        static wxBitmap redR, greenR, yellowR;
        static wxBitmap empty, locked, settings, tools, unlocked;

        static wxIcon vcashIcon;
    };
}

#endif // RESOURCES_H
