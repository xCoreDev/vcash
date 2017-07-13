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

#include "Controller.h"
#include "HistoryPage.h"
#include "MainFrame.h"
#include "StatusBarWallet.h"
#include "Utils.h"
#include "View.h"

#ifndef _MSC_VER
using std::isspace;
#endif

using namespace wxGUI;

Controller::Controller(View &view)
        : view(view)
        , stack(view)
        , walletLoaded(false) { }

bool Controller::onInit(std::map<std::string, std::string> &args) {
    view.setStatusBarWorking(true);
    stack.start(args);

    view.setMining(false);

    std::string zero = Utils::formatted(0.0, 6);

    view.setBalance(zero);
    view.setUnconfirmed(zero);
    view.setStake(zero);
    view.setImmature(zero);

    return true;
}

int Controller::onExit() {
    stack.stop();
    return 0;
}

bool Controller::validateAddress(const std::string &address) {
    // not really needed in the end as the stack does the verification
    return true;
}

bool Controller::validateAmount(const std::string &amount) {
    // checks this string represents a valid amount of coins
    bool result;
    try {
        double value = std::stod(amount);
        result = value > 0;
    }
    catch(std::exception& e) {
        result = false;
    }
    return result;
}

// Only gets called after validation and confirmation
void Controller::onSendPressed(const std::string &pay, const std::string &to, const bool zeroTime,
                               const bool blendedOnly) {

    std::int64_t amount = Utils::toJohnoshis(pay);

    std::map<std::string,std::string> walletArgs;
    if(zeroTime)
        walletArgs["zerotime"] = "1";

    if(blendedOnly)
        walletArgs["blended"] = "1";

    stack.send_coins(amount, to, walletArgs);
}

bool Controller::isWalletCrypted() {
    return stack.wallet_is_crypted();
}

bool Controller::isWalletLocked() {
    return stack.wallet_is_locked();
}

bool Controller::isWalletLoaded() {
    return walletLoaded;
}

WalletStatus Controller::getWalletStatus() {
    if(!isWalletLoaded())
        return WalletStatus::Unknown;
    if(!isWalletCrypted())
        return WalletStatus::Unencrypted;
    else if(isWalletLocked())
        return WalletStatus::Locked;
    else
        return WalletStatus::Unlocked;
}

bool Controller::onWalletWantLock() {
    return wxStack::lockWallet();
}

bool Controller::onWalletWantUnlock(const std::string &password) {
    return wxStack::unlockWallet(password);
}

bool Controller::onWalletWantEncrypt(const std::string &password) {
    return wxStack::encryptWallet(password);
}

bool Controller::walletChangePassword(const std::string &oldPassword
                                     ,const std::string &newPassword) {
    return wxStack::changePassword(oldPassword, newPassword);
}

void Controller::onMiningPressed(bool isMining) {
    std::map<std::string,std::string> args;
    args["algorithm"] = "proof-of-work";
    if(isMining)
        stack.start_mining(args);
    else
        stack.stop_mining(args);
}

void Controller::onConsoleCommandEntered(const std::string &command) {
    view.appendToConsole(command, true);
    stack.rpc_send(command);
}

void Controller::onZerotimeLockTransaction(const std::string &txid) {
    stack.wallet_zerotime_lock(txid);
}

bool Controller::canZerotimeLock(const std::string &txid) {
    return wxStack::canZerotimeLock(txid);
}

std::string Controller::getHDSeed() {
    return stack.wallet_hd_keychain_seed();
}

void Controller::onError(const std::map<std::string, std::string> &pairs) {
    std::string txt = "";
    for(auto iterator = pairs.begin(); iterator != pairs.end(); iterator++) {
        txt += iterator->first + " -> " + iterator->second + "\n";
    }
    view.messageBox(txt, "Fatal error", wxOK | wxICON_ERROR);
    stack.stop();
}

void Controller::onAlert(const std::map<std::string, std::string> &pairs) {
    std::string txt = "";
    for(auto iterator = pairs.begin(); iterator != pairs.end(); iterator++) {
        txt += iterator->first + " -> " + iterator->second + "\n";
    }
    view.messageBox(txt, "Alert", wxOK | wxICON_EXCLAMATION);
}

// This method is called whenever the Vcash stack sends an on_status message. 
// This method is guaranteed to run on the GUI thread, which is required for 
// modifying elements in the GUI without crashing the application.
void Controller::onStatus(const std::map<std::string, std::string> &pairs) {
    //wxMutexGuiEnter();
    if (pairs.size() > 0) {
#if 0
        std::cout << "STATUS " << pairs.size() << std::endl;
        std::cout << "********************************************************" << std::endl;
        for(auto iterator = pairs.begin(); iterator != pairs.end(); iterator++) {
            std::cout << "* " << iterator->first << " -> " << iterator->second << std::endl;
        }
        std::cout << "********************************************************" << std::endl << std::endl;
#endif
        auto formated = [](std::string str) {
            return Utils::formatted(Utils::fromJohnoshis(str), 6);
        };

        std::string type = Utils::find("type", pairs);

        if(type == "block") {
            std::string value = Utils::find("value", pairs);
            if(value == "Downloading blocks") {
                std::string difficulty = Utils::find("block.difficulty", pairs);
                std::string local = Utils::find("block.count.local", pairs);
                std::string peer = Utils::find("block.count.peer", pairs);
                std::string percent = Utils::find("block.download.percent", pairs);

                view.setDifficulty(difficulty);
                view.setTotal(local);
                view.setEstimated(peer);

                double percentD ;
                try {
                    percentD = std::stod(percent);
                }
                catch(std::exception& e) {
                    percentD = 0.0;
                }

                if(percentD < 100) {
                    view.setStatusBarMessage("Downloading blocks: "+Utils::formatted(percentD, 2)+"%");
                }

                goto end;
            }
        } else if(type == "database") {
            std::string value = Utils::find("value", pairs);
            if(Utils::isPrefix("Verifying", value) ||
               Utils::isPrefix("Loaded", value) ||
               Utils::isPrefix("Loading", value) ||
               Utils::isPrefix("Opening", value)
              ) {
                view.setStatusBarMessage(value);
                goto end;
            } else if (value == "Importing blockchain...") {
                std::string blockchainImport = Utils::find("blockchain.import", pairs);
                if(!blockchainImport.empty()) {
                    view.setStatusBarMessage("Importing blockchain: "+blockchainImport+" blocks");
                    goto end;
                }
            } else {
                std::string blockchainMoneySupply = Utils::find("blockchain.money_supply", pairs);
                if(!blockchainMoneySupply.empty()) {
                    view.setSupply(blockchainMoneySupply);
                    goto end;
                }
            }
        } else if(type == "mining") {
            std::string value = Utils::find("value", pairs);
            if(value == "proof-of-work") {
                std::string hashRate = Utils::find("mining.hashes_per_second", pairs);
                view.setHash(hashRate);
                goto end;
            }
        } else if(type == "network") {
            std::string value = Utils::find("value", pairs);
            if(value == "Connecting" || value == "Connected") {
                view.setStatusBarMessage(value);
                if(value == "Connected")
                    view.setStatusBarWorking(false);
                std::string tcp = Utils::find("network.tcp.connections", pairs);
                if (!tcp.empty()) {
                    view.setTCP(tcp);
                }
                std::string udp = Utils::find("network.udp.connections", pairs);
                if (!udp.empty()) {
                    view.setUDP(udp);
                }
                goto end;
            } else if (value == "Loading network addresses" ||
                       value == "Loaded network addresses") {
                view.setStatusBarMessage(value);
                goto end;
            }
        } else if(type == "transaction") {
            std::string errorCode = Utils::find("error.code", pairs);
            if(!errorCode.empty() && (errorCode != "0")) {
                std::string errorMsg = Utils::find("error.message", pairs);
                view.messageBox("Error: " + errorMsg, "Transaction error", wxOK | wxICON_EXCLAMATION);
                goto end;
            }
        } else if(type == "rpc") {
            std::string result = Utils::find("value", pairs);
            result.erase(std::find_if(result.rbegin(), result.rend(),
                                 std::not1(std::ptr_fun<int, int>(isspace))).base(), result.end());
            view.appendToConsole(result+"\n");
            // rpc command may have modified the wallet status
            view.setWalletStatus(getWalletStatus());
            goto end;
        } else if(type == "wallet") {
            std::string value = Utils::find("value", pairs);

            if(value == "address") {
                // main wallet address
                std::string mainAddress = Utils::find("wallet.address", pairs);
                if(!mainAddress.empty()) {
                    view.emboldenAddress(mainAddress, true);
                    goto end;
                }
            } else if(value == "change_passphrase") {
                std::string code = Utils::find("error.code", pairs);
                if(code == "0") { // double check
                    view.setWalletStatus(getWalletStatus());
                    goto end;
                }
            } else if(value == "encrypt") {
                std::string code = Utils::find("error.code", pairs);
                if(code == "0") { // double check
                    view.setWalletStatus(getWalletStatus());
                    goto end;
                }
            } else if(value == "lock") {
                std::string code = Utils::find("error.code", pairs);
                if(code == "0") { // double check
                    view.setWalletStatus(getWalletStatus());
                    goto end;
                }
            } else if(value == "unlock") {
                std::string code = Utils::find("error.code", pairs);
                if(code == "0") { // double check
                    view.setWalletStatus(getWalletStatus());
                    goto end;
                }
            } else if(value == "Loaded wallet") {
                // Now that wallet has been loaded, set locked/unlocked status in GUI
                walletLoaded = true;
                view.setWalletStatus(isWalletCrypted() ? WalletStatus::Locked : WalletStatus::Unencrypted);
                goto end;
            } else if(value == "Loading wallet") {
                std::string status = Utils::find("wallet.status", pairs);
                view.setStatusBarMessage(status);
                goto end;
            } else if(value == "Rescanning wallet") {
                view.setStatusBarMessage(Utils::find("wallet.status", pairs));
                goto end;
            }

            std::string balance = Utils::find("wallet.balance", pairs);
            std::string unconfirmed = Utils::find("wallet.balance.unconfirmed", pairs);
            std::string stake = Utils::find("wallet.stake", pairs);
            std::string immature = Utils::find("wallet.balance.immature", pairs);

            if(!balance.empty())
                view.setBalance(formated(balance));
            if(!unconfirmed.empty())
                view.setUnconfirmed(formated(unconfirmed));
            if(!stake.empty())
                view.setStake(formated(stake));
            if(!immature.empty())
                view.setImmature(formated(immature));
            goto end;
        } else if(type == "wallet.address_book") {
            std::string value = Utils::find("value", pairs);
            if(value == "new") {
                std::string address = Utils::find("wallet.address_book.address", pairs);
                std::string name = Utils::find("wallet.address_book.name", pairs);
                if(!address.empty()) {
                    view.addAddress(name, address);
                    goto end;
                }
            }
        } else if(type == "wallet.transaction") {
            std::string value = Utils::find("value", pairs);
            std::string confirmations = Utils::find("wallet.transaction.confirmations", pairs);
            std::string confirmed = Utils::find("wallet.transaction.confirmed", pairs);
            std::string credit = Utils::find("wallet.transaction.credit", pairs);
            std::string debit = Utils::find("wallet.transaction.debit", pairs);
            std::string hash = Utils::find("wallet.transaction.hash", pairs);
            std::string mainChain = Utils::find("wallet.transaction.in_main_chain", pairs);
            std::string fromMe = Utils::find("wallet.transaction.is_from_me", pairs);
            std::string net = Utils::find("wallet.transaction.net", pairs);
            std::string time = Utils::find("wallet.transaction.time", pairs);
            std::string type = Utils::find("wallet.transaction.type", pairs);
            std::string coinBase = Utils::find("wallet.transaction.coin_base", pairs);
            std::string coinStake = Utils::find("wallet.transaction.coin_stake", pairs);
            std::string valueOut = Utils::find("wallet.transaction.value_out", pairs);
            std::string blended = Utils::find("blended", pairs);
            std::string denominated = Utils::find("denominated", pairs);

            /*************************************************************************
             * A ZeroTime incoming transaction goes through following states:
             * 1) Tx is received:
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 0
             *     wallet.transaction.in_main_chain -> 0
             * 2) Tx is confirmed off chain:
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 0
             * 3) Tx is confirmed on chain:
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 1
             *
             * If wallet is restarted before on chain confirmation, different states are:
             * 1) Tx is received from wallet:
             *     wallet.transaction.confirmations -> -1
             *     wallet.transaction.confirmed -> 0
             *     wallet.transaction.in_main_chain -> 0
             * 2) Tx is confirmed off chain:
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 0
             *     wallet.transaction.in_main_chain -> 0
             * 3) Tx is confirmed on chain:
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 1

             *************************************************************************
             * A normal incoming transaction goes through following states:
             * 1) Tx is received:
             *     value -> new
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 0
             *     wallet.transaction.in_main_chain -> 0
             * 2) Tx is updated:
             *     value -> updated
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 0
             *     wallet.transaction.in_main_chain -> 0
             * 3) Tx is confirmed on chain:
             *     value -> updated
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 1
             *
             * If wallet is restarted before confirmation, different states are:
             * 1) Tx is updated from our wallet:
             *     wallet.transaction.confirmations -> -1
             *     wallet.transaction.confirmed -> 0
             *     wallet.transaction.in_main_chain -> 0
             * 2) Tx is updated from network:
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 0
             *     wallet.transaction.in_main_chain -> 0
             * 3) Tx is confirmed on chain:
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 1

             *************************************************************************
             * A ZeroTime outgoing transaction goes through following states:
             * 1) Tx is created:
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 0
             *     wallet.transaction.is_from_me -> 1
             * 2) Tx is confirmed off chain:
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 0
             *     wallet.transaction.is_from_me -> 1
             * 3) Tx is confirmed on chain:
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 1
             *     wallet.transaction.is_from_me -> 1

             *************************************************************************
             * A normal outgoing transaction goes through following states:
             * 1) Tx is created:
             *     value -> new
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 0
             *     wallet.transaction.is_from_me -> 1
             * 2) Tx is updated from network:
             *     value -> updated
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 0
             *     wallet.transaction.is_from_me -> 1
             * 3) Tx is confirmed on chain:
             *     value -> updated
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 1
             *     wallet.transaction.is_from_me -> 1
             *
             * If wallet is restarted before confirmation, different states are:
             * 1) Tx is updated from our wallet::
             *     value -> updated
             *     wallet.transaction.confirmations -> -1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 0
             *     wallet.transaction.is_from_me -> 1
             * 2) Tx is updated from network:
             *     value -> updated
             *     wallet.transaction.confirmations -> 0
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 0
             *     wallet.transaction.is_from_me -> 1
             * 3) Tx is confirmed on chain:
             *     value -> updated
             *     wallet.transaction.confirmations -> 1
             *     wallet.transaction.confirmed -> 1
             *     wallet.transaction.in_main_chain -> 1
             *     wallet.transaction.is_from_me -> 1
             */

            if(!confirmations.empty() && !confirmed.empty() && !hash.empty() && !net.empty() && !time.empty() && !mainChain.empty()) {
                std::int32_t confirms = std::stoi(confirmations);
                bool isOnChain = mainChain == "1";
                bool isNew = value == "new";
                bool isCoinBase = coinBase == "1";
                bool isCoinStake = coinStake == "1";
                bool isBlended = blended == "1";
                bool isDenominated = denominated == "1";
                bool isOutgoing;
                bool isConfirmed = (confirms>=stack.minConfirmations);
                std::string amount;
                std::string txMsg;

                auto txtConfs = [confirms, this]() {
                    std::int32_t currentConfs = std::max(0, confirms); // confirms can be -1
                    return " (" + std::to_string(currentConfs) + "/" + std::to_string(stack.minConfirmations) + ")";
                };

                if(isBlended) {
                    amount = net;
                    isOutgoing = true;

                    if(isOnChain)
                        txMsg = "Blended";
                    else
                        txMsg = "Blending" + txtConfs();
                } else if(isDenominated) {
                    amount = net;
                    isOutgoing = true;

                    if(isOnChain)
                        txMsg = "Denominated";
                    else
                        txMsg = "Denominating" + txtConfs();
                } else if(isCoinBase) {
                    amount = credit;
                    isOutgoing = false;

                    if(isOnChain)
                        txMsg = "Reward";
                    else
                        txMsg = "Reward (Unaccepted)";
                } else if(isCoinStake) {
                    if(confirms < (stack.coinbaseMaturity + 20)) {
                        std::int64_t amountN = Utils::stringToJohnoshis(valueOut) - Utils::stringToJohnoshis(debit);
                        amount = std::to_string(amountN);
                    } else
                        amount = net;
                    isOutgoing = false;

                    if(isOnChain)
                        txMsg = "Interest";
                    else
                        txMsg = "Interest (Unaccepted)";
                } else {
                    amount = net;
                    isOutgoing = !amount.empty() && amount[0] == '-';

                    if(isConfirmed)
                        txMsg = isOutgoing ? "Sent" : "Received";
                    else
                        txMsg = (isOutgoing ? "Sending" : "Receiving") + txtConfs();

                    // toDo stack.updateTransactionMaxConfirmations corresponds to coin::transaction::confirmations.
                    // In any case, if wallet is shut down before this number of confirmations
                    // has been reached, next time we restart the wallet, the number of
                    // confirmations won't get properly updated :(
                    // line 4926 of stack_impl.cpp must be modified accordingly to fix this issue.

                    bool isZeroTime = isConfirmed && !isOnChain;

                    if(isZeroTime)
                        txMsg += " (ZT)"; // is confirmed but off chain (ZeroTime)
                    else if(confirms > 0 && confirms < (stack.updateTransactionMaxConfirmations + 1))
                        txMsg += " ("+std::to_string(confirms)+")";
                }

                std::time_t txTime = (std::time_t) atoll(time.c_str());

                view.addTransaction(hash, txTime, txMsg, formated(amount));
                view.setColour(hash, (isOutgoing && (isNew || confirms<0)) ? BulletColor::Red : (isConfirmed ? BulletColor::Green : BulletColor::Yellow));

                if (!isOutgoing && isNew)
                    view.notificationBox("Amount: " + formated(amount), "Incoming Vcash transaction");
                goto end;
            }
        }
    }
    goto end;
    std::cout << "STATUS " << pairs.size() << std::endl;
    std::cout << "********************************************************" << std::endl;
    for(auto iterator = pairs.begin(); iterator != pairs.end(); iterator++) {
        std::cout << "* " << iterator->first << " -> " << iterator->second << std::endl;
    }
    std::cout << "********************************************************" << std::endl << std::endl;
    end:
        ;
    //wxMutexGuiLeave();
}

void Controller::rescanWallet() {
    // TODO: this doesn't work well when rescanning a wallet restored from HD
    // It seems it only rescans main address in restored wallet
    stack.rescan_chain();
    view.mainFrame->Destroy();
}

std::string Controller::getVcashVersion() {
    return wxStack::getVersion();
}

bool Controller::validateHDSeed(std::string &seed) {
    return wxStack::validateHDSeed(seed);
}

bool Controller::walletExists(bool isClient) {
    return stack.wallet_exists(isClient);
}