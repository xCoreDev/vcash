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
 
#ifndef ADDRESSESPAGE_H
#define ADDRESSESPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/window.h>
#endif

#include <map>
#include <vector>

namespace wxGUI {

    class VcashApp;

    class AddressesPage : public wxPanel {
    public:
        AddressesPage(VcashApp &vcashApp, wxWindow &parent);

        void addAddress(const std::string &account, const std::string &address);
        void emboldenAddress(const std::string &address, bool bold = true);

    private:
        wxListCtrl *listCtrl;
        enum Column { Account, Address };

        std::map<std::string, int> addresses; // from address to id

        struct SortData {
            AddressesPage *addressesPage;
            std::vector<std::pair<Column,bool>> order;
        };

        SortData sortData;

        // needs access to transactions map and sortData
        friend int wxCALLBACK cmpAddresses(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortDt);
    };
}

#endif // ADDRESSESPAGE_H