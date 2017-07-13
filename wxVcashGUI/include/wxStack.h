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

#ifndef WXSTACK_H
#define WXSTACK_H

#include <map>
#include <string>

#include "coin/stack.hpp"

namespace wxGUI {
    class View;

    class wxStack : public coin::stack {
    public:
        wxStack(View &view);
        static bool canZerotimeLock(const std::string &txid);
        static bool changePassword(const std::string &oldPassword, const std::string &newPassword);
        static bool encryptWallet(const std::string &password);
        static std::string getVersion();
        static bool lockWallet();
        static bool unlockWallet(const std::string &password);
        static bool validateHDSeed(std::string &seed);

        void on_error(const std::map<std::string, std::string> &pairs) override;
        void on_status(const std::map<std::string, std::string> &pairs) override;
        void on_alert(const std::map<std::string, std::string> &pairs) override;

        static const std::int64_t oneVcash;
        static const std::int64_t maxMoneySupply;
        static const std::int32_t updateTransactionMaxConfirmations;
        static const std::int32_t coinbaseMaturity;
        static const std::int32_t minConfirmations;
    private:
        View &view;
    };
}

#endif // WXSTACK_H
 
