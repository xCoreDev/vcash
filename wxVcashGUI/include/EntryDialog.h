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

#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#endif

#include <functional>
#include <vector>

namespace wxGUI {

    class EntryDialog : public wxDialog {
    public:
        struct Entry {
            wxString label;
            long style;
            wxString toolTip;
            wxSize size;
        };

        EntryDialog( wxWindow &parent, const wxString &title
                   , const std::vector<Entry> &entries
                   , std::function<bool(EntryDialog &dlg)> validate);

        std::vector<wxString> getValues();

        static std::pair<int,std::vector<wxString>> run(wxWindow &parent
                , const wxString &title
                , const std::vector<Entry> &entries
                , std::function<bool(EntryDialog &dlg)> validate);

        bool validate(bool cond, wxString msg);

    private:
        std::vector<wxTextCtrl*> textCtrls;
        std::vector<wxString> values;
        wxString title;

        std::function<bool(EntryDialog &dlg)> validatef;

        void onKeyPressed(wxKeyEvent &ev);

        void onReturnKeyPressed();
    };
}

#endif //ENTRYDIALOG_H
