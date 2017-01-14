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

#ifndef SHOWINFODIALOG_H
#define SHOWINFODIALOG_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include <wx/window.h>
#endif

#include <functional>

namespace wxGUI {
    class ShowInfoDialog : public wxDialog {
    public:
        ShowInfoDialog(wxWindow &parent, const wxString &title, std::function<wxSizer *(void)> contents, int timeout = -1);
    private:
        class CloseTimer : public wxTimer {
        private:
            ShowInfoDialog *dlg;
        public:
            CloseTimer(ShowInfoDialog *dlg) : dlg(dlg), wxTimer() { }
            void Notify() {
                dlg->EndModal(wxID_CLOSE);
            }
        };

        CloseTimer closeTimer;
    };
}

#endif // SHOWINFODIALOG_H
