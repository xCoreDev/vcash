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
 
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

#include "coin/stack.hpp"

namespace wxGUI {

    class Controller;
    class View;

    class wxStack : public coin::stack {
    private:
        View &view;

    public:
        wxStack(View &view);
        void on_error(const std::map<std::string, std::string> &pairs) override;
        void on_status(const std::map<std::string, std::string> &pairs) override;
        void on_alert(const std::map<std::string, std::string> &pairs) override;
    };

    class Controller {
    private:
        View &view;
        wxStack stack;
        bool walletLoaded;
    public:
        Controller(View &view);

        // Called when application is initialized
        bool onInit();

        // Called when application is about to exit
        int onExit();

        // Should return true if address is valid one
        bool validateAddress(const std::string &address);

        // Should return true id amount is valid for a send transaction
        bool validateAmount(const std::string &amount);

        // Called after `send' is pressed in transfer dialog and input has been validated and confirmed
        void onSendPressed(const std::string &pay, const std::string &to, const bool zeroTime, const bool blendedOnly);

        // Called when wallet has to be locked.
        void walletLock();

        // Called when wallet has to be encrypted. Should return true if successful.
        void encryptWallet(const std::string &password);

        // Called when wallet password must be changed. Should return true if successful.
        void walletChangePassword(
                const std::string &passphrase_old,
                const std::string &password_new);

        // Called when wallet has to be unlocked with provided password.
        // Should unlock wallet using password and return true if successful.
        bool onWalletWantUnlock(const std::string &password);

        // Called after mining button is pressed
        void onMiningPressed(bool isMining);

        // Called after return key is pressed on console input.
        // Gets command introduced.
        void onConsoleCommandEntered(const std::string &command);

        // Called when user wants to lock a transaction
        void onZerotimeLockTransaction(const std::string &txid);

        // This method will be executed by GUI thread when we get an on_status
        // notification from Vcash stack. This is needed as all GUI operations
        // have to be performed by GUI thread,
        void OnStatus(const std::map<std::string, std::string> &pairs);

        // This method will be executed by GUI thread when we get an on_error
        // notification from Vcash stack. This is needed as all GUI operations
        // have to be performed by GUI thread,
        void OnError(const std::map<std::string, std::string> &pairs);

        // Returns the wallet HD keychain seed
        std::string getHDSeed();

        // True is wallet is encrypted
        bool isWalletCrypted();

        // True is wallet is locked
        bool isWalletLocked();

        // True is wallet has been loaded
        bool isWalletLoaded();

        // Called when wallet has to be rescanned
        void rescanWallet();      
    };
}

#endif // CONTROLLER_H
