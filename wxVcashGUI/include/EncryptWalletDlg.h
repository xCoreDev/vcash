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
 
#ifndef ENCRYPTWALLETDLG_H
#define ENCRYPTWALLETDLG_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/msgdlg.h>
#endif

namespace wxGUI {

    class EncryptWalletDlg : public wxDialog {
    public:
        EncryptWalletDlg(wxWindow &parent, const wxString &title);

        std::string getPassword();

    private:
        wxTextCtrl *passwordTC, *confirmTC;
        std::string password, confirm;

        void onKeyPressed(wxKeyEvent &ev);

        void onReturnKeyPressed();
    };
}

#endif // ENCRYPTWALLETDLG_H
