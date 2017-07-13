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

#ifndef RESOURCES_H
#define RESOURCES_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#endif

namespace wxGUI {

    class Resources {
    public:
        static void init();

        static wxBitmap redR, greenR, yellowR;
        static wxBitmap empty, locked, settings, tools, unlocked;

        static wxIcon vcashIcon;

        static wxImage vcashImage64;
    };
}

#endif // RESOURCES_H
