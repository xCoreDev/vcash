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
 
#include "StatusBarImage.h"

using namespace wxGUI;

StatusBarImage::StatusBarImage(wxWindow &parent, wxBitmap &bitmap)
        : wxStaticBitmap(&parent, wxID_ANY, bitmap) {
}

StatusBarImage::StatusBarImage(wxWindow &parent, wxBitmap &bitmap,
                               std::function<void(wxMouseEvent &)> onClickCode)
        : StatusBarImage(parent, bitmap) {
    Bind(wxEVT_LEFT_UP, onClickCode);
}

void StatusBarImage::bindOnLeftClick(std::function<void(wxMouseEvent &)> onClickCode) {
    Bind(wxEVT_LEFT_UP, onClickCode);
}
