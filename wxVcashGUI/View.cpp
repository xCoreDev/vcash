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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/msgdlg.h>
#include <wx/notifmsg.h>
#endif

#include "AccountPage.h"
#include "AddressesPage.h"
#include "ConsolePage.h"
#include "ContextMenu.h"
#include "HistoryPage.h"
#include "MainFrame.h"
#include "MiningPage.h"
#include "Resources.h"
#include "StatusBarWallet.h"
#include "StatusBar.h"
#include "TaskBarIcon.h"
#include "ToolsFrame.h"
#include "ToolsPanel.h"
#include "VcashApp.h"
#include "View.h"
#include "WalletActions.h"

using namespace wxGUI;

// Account

void View::setBalance(const std::string &balance) {
    accountPage->setBalance(balance);
}

void View::setUnconfirmed(const std::string &unconfirmed) {
    accountPage->setUnconfirmed(unconfirmed);
}

void View::setStake(const std::string &stake) {
    accountPage->setStake(stake);
}

void View::setImmature(const std::string &immature) {
    accountPage->setImmature(immature);
}

// Wallet state

// sets wallet state shown in view. Calls controller too
void View::setWalletStatus(WalletStatus walletStatus) {
    walletLock->setWalletStatus(walletStatus);
}

// gets wallet state shown in view
WalletStatus View::getWalletStatus() {
    return walletLock->getWalletStatus();
}

// Status bar

void View::setStatusBarMessage(const std::string &msg) {
    statusBar->setMessage(wxString(msg));
}

void View::setStatusBarWorking(bool working) {
    statusBar->setWorking(working);
}

// Mining state

// sets mining state shown in view. Calls controller too
void View::setMining(bool mining) {
    miningPage->setMining(mining);
}

//sets mining hash rate
void View::setHash(const std::string &hashRate) {
    miningPage->setHash(hashRate);
}

// sets mining state shown in view
bool View::getMining() {
    return miningPage->getMining();
}

// Address book

void View::addAddress(const std::string &account, const std::string &address) {
    addressesPage->addAddress(account, address);
}

void View::emboldenAddress(const std::string &address, bool bold) {
    addressesPage->emboldenAddress(address, bold);
}

// Console

// apends a new line of text to console output
void View::appendToConsole(const std::string &text, bool bold) {
    toolsPanel->showConsolePage();
    consolePage->appendToConsole(text, bold);
}

// History

void View::addTransaction(const std::string &txid, const std::time_t &date, const std::string &status,
                               const std::string &amount) {
    historyPage->addTransaction(txid, date, status, amount);
}

void View::setColour(const std::string &txid, BulletColor color) {
    historyPage->setColour(txid, color);
}

void View::setStatus(const std::string &txid, const std::string &status) {
    historyPage->setStatus(txid, status);
}

// Statistics

void View::setTCP(const std::string &tcp) {
    statisticsPage->setTCP(tcp);
}

void View::setUDP(const std::string &udp) {
    statisticsPage->setUDP(udp);
}

void View::setEstimated(const std::string &estimated) {
    statisticsPage->setEstimated(estimated);
}

void View::setTotal(const std::string &total) {
    statisticsPage->setTotal(total);
}

void View::setDifficulty(const std::string &difficulty) {
    statisticsPage->setDifficulty(difficulty);
}

void View::setSupply(const std::string &supply) {
    statisticsPage->setSupply(supply);
}

void View::messageBox(const std::string &msg, const std::string &title, long style) {
    wxMessageBox(msg, title, style, mainFrame);
}

void View::notificationBox(const std::string &msg, const std::string &title) {
    wxNotificationMessage *notificationMessage = new wxNotificationMessage(wxString(title), wxString(msg), mainFrame);
#if (wxMAJOR_VERSION >= 3) && (wxMINOR_VERSION >= 1)
    wxIcon icon;
    icon.CopyFromBitmap(wxBitmap(Resources::vcashImage64));
    notificationMessage->SetIcon(icon);
    notificationMessage->SetParent(mainFrame);
#if defined (__WXMSW__)
    wxNotificationMessage::UseTaskBarIcon(taskBarIcon);
#endif
#endif
    notificationMessage->Show(2);
}

std::pair<bool, std::string> View::restoreHDSeed(VcashApp &vcashApp) {
    return WalletActions::restoreHDSeed(vcashApp.controller, *mainFrame);
}

void View::showContextMenu(VcashApp &vcashApp, bool atClickPosition) {
    wxPoint p = atClickPosition ? wxDefaultPosition : walletLock->GetPosition() +  statusBar->GetPosition();
    new ContextMenu(vcashApp, *mainFrame, p);
}

void View::showHideToolsFrame(bool showAlso) {
    if(toolsFrame->IsShown())
        toolsFrame->Hide();
    else if (showAlso) {
        toolsFrame->updatePosition();
        toolsFrame->Iconize(false); // restore the window if minimized
        toolsFrame->Restore();      // restore the window if minimized
        toolsFrame->Show();
        // toolsFrame->updatePosition();
        toolsFrame->SetFocus();
    }
}