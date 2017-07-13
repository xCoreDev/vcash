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
 
#ifndef VIEW_H
#define VIEW_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/app.h>
#include <wx/checkbox.h>
#include "wx/listctrl.h"
#include <wx/stattext.h>
#include <wx/statusbr.h>
#endif

#include <ctime>

namespace wxGUI {

    class AddressesPage;
    class AccountPage;
    enum class BulletColor;
    class ConsolePage;
    class HistoryPage;
    class MainFrame;
    class MiningPage;
    class StatisticsPage;
    class StatusBarWallet;
    class StatusBar;
    class TaskBarIcon;
    class ToolsFrame;
    class ToolsPanel;
    class VcashApp;
    enum class WalletStatus;

    class View {
    public:
        // Status bar
        void setWalletStatus(WalletStatus walletStatus);

        WalletStatus getWalletStatus();

        void setStatusBarMessage(const std::string &msg);

        void setStatusBarWorking(bool working);

        // Account
        void setBalance(const std::string &balance);

        void setUnconfirmed(const std::string &unconfirmed);

        void setStake(const std::string &stake);

        void setImmature(const std::string &immature);

        // History of transactions
        void addTransaction(const std::string &txid, const std::time_t &date, const std::string &status,
                            const std::string &amount);

        void setColour(const std::string &txid, BulletColor color);

        void setStatus(const std::string &txid, const std::string &status);

        // Statistics
        void setTCP(const std::string &tcp);

        void setUDP(const std::string &udp);

        void setEstimated(const std::string &estimated);

        void setTotal(const std::string &total);

        void setDifficulty(const std::string &difficulty);

        void setSupply(const std::string &supply);

        // Mining
        void setMining(bool mining);
        void setHash(const std::string &hashRate);

        bool getMining();

        // Console
        void appendToConsole(const std::string &text, bool bold = false);

        // Address book
        void addAddress(const std::string &account, const std::string &address);
        void emboldenAddress(const std::string &address, bool bold = true);

        // Messages and notifications
        void messageBox(const std::string &msg, const std::string &title, long style);

        void notificationBox(const std::string &msg, const std::string &title);

        std::pair<bool, std::string> restoreHDSeed(VcashApp &vcashApp);

        void showContextMenu(VcashApp &vcashApp, bool atClickPosition = true);

        void showHideToolsFrame(bool showAlso = true);

        MainFrame *mainFrame;
        TaskBarIcon *taskBarIcon;
        ToolsFrame *toolsFrame;
        StatusBar *statusBar;
        ToolsPanel *toolsPanel;
        StatusBarWallet *walletLock;

        AddressesPage *addressesPage;
        AccountPage *accountPage;
        ConsolePage *consolePage;
        HistoryPage *historyPage;
        MiningPage *miningPage;
        StatisticsPage *statisticsPage;
    };
}

#endif // VIEW_H