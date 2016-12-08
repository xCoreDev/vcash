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
 
#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/window.h>
#endif

#include <ctime>
#include <map>
#include <vector>

namespace wxGUI {

    class VcashApp;

    enum BulletColor {
        Red, Green, Yellow
    };

    class HistoryPage : public wxPanel {
    public:
        HistoryPage(VcashApp &vcashApp, wxWindow &parent);

        void addTransaction(const std::string &txid, const std::time_t &time, const std::string &status,
                            const std::string &amount);

        void setColour(const std::string &txid, BulletColor color);

        void setStatus(const std::string &txid, const std::string &status);

    private:
        enum Column {
            Icon, Date, Status, Amount
        };;

        wxListCtrl *listCtrl;

        struct TxData {
            std::time_t time;  // time of the transaction
        };

        std::map<std::string, TxData> transactions; // from txid (hash) to TxData

        struct SortData {
            HistoryPage *historyPage;
            std::vector<std::pair<Column,bool>> order;
        };

        SortData sortData;

        // needs access to transactions map and sortData
        friend int wxCALLBACK cmpHistory(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortDt);
    };
}

#endif // HISTORYPAGE_H