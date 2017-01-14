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

#include "MainFrame.h"
#include "OnPairsEvent.h"
#include "Utils.h"
#include "View.h"
#include "wxStack.h"

#include "coin/constants.hpp"
#include "coin/globals.hpp"
#include "coin/key.hpp"
#include "coin/transaction_pool.hpp"
#include "coin/utility.hpp"
#include "coin/wallet.hpp"
#include "coin/zerotime.hpp"

using namespace wxGUI;

wxStack::wxStack(View &view)
        : view(view)
        , coin::stack() { }

void wxStack::on_error(const std::map<std::string, std::string> &pairs) {
    coin::stack::on_error(pairs);
    std::string type = Utils::find("type", pairs);
    std::string value = Utils::find("value", pairs);

    // toDo: This is troublesome as, the stack sometimes calls exit(0)
    // right after on_error hence, view.mainFrame won't be able to process
    // a posted event, but we shouldn't open the messageBox here as this
    // is not the GUI thread

    view.messageBox(type+" error.\n"+value, "Fatal error", wxOK | wxICON_ERROR);
}

void wxStack::on_status(const std::map<std::string, std::string> &pairs) {
    coin::stack::on_status(pairs);
    // post event to GUI thread which will process the request
    // in Controller::onStatus
    OnStatusEvent onStatusEvent;
    onStatusEvent.SetPairs(pairs);
    wxPostEvent(view.mainFrame, onStatusEvent);
}

void wxStack::on_alert(const std::map<std::string, std::string> &pairs) {
    coin::stack::on_alert(pairs);
    // post event to GUI thread which will process the request
    // in Controller::OnAlert
    OnAlertEvent onAlertEvent;
    onAlertEvent.SetPairs(pairs);
    wxPostEvent(view.mainFrame, onAlertEvent);
}

bool wxStack::changePassword(const std::string &oldPassword, const std::string &newPassword) {
    return coin::globals::instance().wallet_main()->change_passphrase(oldPassword, newPassword);
}

bool wxStack::encryptWallet(const std::string &password) {
    return coin::globals::instance().wallet_main()->encrypt(password);
}

bool wxStack::lockWallet() {
    return coin::globals::instance().wallet_main()->lock();
}

bool wxStack::unlockWallet(const std::string &password) {
    return coin::globals::instance().wallet_main()->unlock(password);
}

bool wxStack::canZerotimeLock(const std::string &txid) {
    return coin::globals::instance().is_zerotime_enabled()
           && coin::transaction_pool::instance().exists(txid)
           && (coin::zerotime::instance().locks().count(txid) == 0);
}

std::string wxStack::getVersion() {
    return coin::utility::format_version(coin::constants::version_client);
}

const std::int64_t wxStack::oneVcash = coin::constants::coin;

const std::int64_t wxStack::maxMoneySupply = coin::constants::max_money_supply;

const std::int32_t wxStack::updateTransactionMaxConfirmations = coin::transaction::confirmations;

const std::int32_t wxStack::coinbaseMaturity =
        coin::constants::test_net ? coin::constants::coinbase_maturity_test_network
                                  : coin::constants::coinbase_maturity;

const std::int32_t wxStack::minConfirmations = coin::transaction_wallet::confirmations;

bool wxStack::validateHDSeed(std::string &seed) {
    coin::key key;
    coin::key::secret_t secret;

    secret = coin::utility::from_hex(seed);

    try {
        key.set_secret(secret, true);
    } catch (std::exception &e) {
        return false;
    }

    if(!key.is_valid()) {
        return false;
    }
    return true;
}
 
