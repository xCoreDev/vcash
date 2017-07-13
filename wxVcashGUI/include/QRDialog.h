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

#ifndef QRDIALOG_H
#define QRDIALOG_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/dialog.h>
#include <wx/frame.h>
#include <wx/window.h>
#endif

#include "ShowInfoDialog.h"
#include "qrencode.h"

namespace wxGUI {
    class QRDialog : public ShowInfoDialog {
    public:
        QRDialog(wxWindow &parent, const wxString &title, const wxString &str);
        wxSizer *contents();
    };
}

#endif //QRDIALOG_H
