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
#include "EntryDialog.h"
#include "MainFrame.h"
#include "OnPairsEvent.h"
#include "View.h"

#include <cstddef>
#include <thread>

using namespace wxGUI;

class Utils {
private:
    static const int64_t oneVcash = 1000000;
public:
    static std::int64_t toJohnoshis(double amount) {
        return static_cast<std::int64_t >(amount*oneVcash);
    }

    static std::int64_t toJohnoshis(const std::string &amount) {
        return toJohnoshis(std::stod (amount, nullptr));
    }

    static double fromJohnoshis(std::int64_t amount) {
        return (double)amount/oneVcash;
    }

    static double fromJohnoshis(std::string &amount) {
        return fromJohnoshis(std::stol (amount, nullptr));
    }

    static std::string formatted(double amount, int decimals) {
        char buffer[256];
        std::string format = "%."+std::to_string(decimals)+"f";
        snprintf(buffer, sizeof(buffer), format.c_str(), amount);
        std::string res = buffer;
        return res;
    }
    
    static std::string find(const std::string &key, const std::map<std::string, std::string> &pairs) {
        auto it = pairs.find(key);
        if (it != pairs.end())
            return it->second;
        else
            return std::string("");
    }

    static bool isPrefix(const std::string &prefix, const std::string &str) {
        auto res = std::mismatch(prefix.begin(), prefix.end(), str.begin());
        return (res.first == prefix.end());
    }
};

wxStack::wxStack(View &view)
        : view(view)
        , coin::stack() { }

void wxStack::on_error(const std::map<std::string, std::string> &pairs) {
    std::string value = Utils::find("value", pairs);
    view.messageBox(value, "Fatal error", wxOK | wxICON_ERROR);
    return;

    // post event to GUI thread which will process the request 
    // in Controller::OnError
    OnErrorEvent onErrorEvent;
    onErrorEvent.SetPairs(pairs);
    wxPostEvent(view.mainFrame, onErrorEvent);
};

void wxStack::on_status(const std::map<std::string, std::string> &pairs) {
    // post event to GUI thread which will process the request 
    // in Controller::OnStatus
    OnStatusEvent onStatusEvent;
    onStatusEvent.SetPairs(pairs);
    wxPostEvent(view.mainFrame, onStatusEvent);
};

void wxStack::on_alert(const std::map<std::string, std::string> &pairs)  {
#if 0
    std::string txt = "";
    for(auto iterator = pairs.begin(); iterator != pairs.end(); iterator++) {
        txt += iterator->first + " -> " + iterator->second + "\n";
    }
    view.messageBox(txt, "Alert", wxOK | wxICON_ERROR);
#endif
    return;
};



Controller::Controller(View &view)
        : view(view)
        , stack(view)
        , walletLoaded(false) { }

bool Controller::onInit() {
    std::map<std::string,std::string> args;

    bool isClient = false;
    if(!stack.wallet_exists(isClient)) {
        auto pair = view.restoreHDSeed();
        if(pair.first) {
            args["wallet-seed"] = pair.second;
        }
    }

    stack.start(args);

    view.setMining(false);

    std::string zero = "0.000000";

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

void Controller::walletLock() {
    stack.wallet_lock();
    view.setWalletStatus(Locked);
}

bool Controller::onWalletWantUnlock(const std::string &password) {
    stack.wallet_unlock(password);
    for(int i=0; i<50; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!stack.wallet_is_locked()) {
            view.setWalletStatus(Unlocked);
            return true;
        }
    }
    return false;
}

void Controller::encryptWallet(const std::string &password) {
    stack.wallet_encrypt(password);
    view.setWalletStatus(Locked);
}

void Controller::walletChangePassword(
        const std::string &passphrase_old,
        const std::string &password_new) {
  stack.wallet_change_passphrase(passphrase_old, password_new);
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

// Called when user wants to lock a transaction
void Controller::onZerotimeLockTransaction(const std::string &txid) {
    stack.wallet_zerotime_lock(txid);
}

std::string Controller::getHDSeed() {
    std::cout << stack.wallet_hd_keychain_seed() << std::endl;
    return stack.wallet_hd_keychain_seed();
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

void Controller::OnError(const std::map<std::string, std::string> &pairs) {
    std::string value = Utils::find("value", pairs);
    view.messageBox(value, "Fatal error", wxOK | wxICON_ERROR);
    stack.stop();
}

// This method is called whenever the Vcash stack sends an on_status message. 
// This method is guaranteed to run on the GUI thread, which is required for 
// modifying elements in the GUI without crashing the application.
void Controller::OnStatus(const std::map<std::string, std::string> &pairs) {
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
            view.appendToConsole(result);
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
                if(code == "0" && stack.wallet_is_locked()) { // double check
                    view.setWalletStatus(Locked);
                    goto end;
                }
            } else if(value == "encrypt") {
                std::string code = Utils::find("error.code", pairs);
                if(code == "0" && stack.wallet_is_locked()) { // double check
                    view.setWalletStatus(Locked);
                    goto end;
                }
            } else if(value == "lock") {
                std::string code = Utils::find("error.code", pairs);
                if(code == "0" && stack.wallet_is_locked()) { // double check
                    view.setWalletStatus(Locked);
                    goto end;
                }
            } else if(value == "unlock") {
                std::string code = Utils::find("error.code", pairs);
                if(code == "0" && !stack.wallet_is_locked()) { // double check
                    view.setWalletStatus(Unlocked);
                    goto end;
                }
            } else if(value == "Loaded wallet") {
                // Now that wallet has been loaded, set locked/unlocked status in GUI
                walletLoaded = true;
                view.setWalletStatus(stack.wallet_is_crypted() ? Locked : Unencrypted);
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
                bool isConfirmed = confirmed == "1";
                int confirms = std::stoi(confirmations);
                bool isOutgoing = !net.empty() && net[0] == '-';
                bool isOnChain = mainChain == "1";
                bool isNew = value == "new";

                bool done = (confirms>0);

                bool isZeroTime = done && !isOnChain;

                std::string txMsg = done ? (isOutgoing ? "Sent" : "Received")
                                         : (isOutgoing ? "Sending(0/1)" : "Receiving(0/1)");

                if(isZeroTime)
                    txMsg += "(ZT)"; // is confirmed but off chain (ZeroTime)
                else if(confirms > 0 && confirms < 4)
                    txMsg += "("+std::to_string(confirms)+")";

                std::time_t txTime = (std::time_t) atoll(time.c_str());

                view.addTransaction(hash, txTime, txMsg, formated(net));
                view.setColour(hash, (isOutgoing && (isNew || confirms<0)) ? Red : (done ? Green : Yellow));

                if (!isOutgoing && isNew)
                    view.notificationBox("Amount: " + formated(net), "Incoming Vcash transaction");
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