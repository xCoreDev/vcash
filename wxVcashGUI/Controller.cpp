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
    std::string txt = "";
    for(auto iterator = pairs.begin(); iterator != pairs.end(); iterator++) {
        txt += iterator->first + " -> " + iterator->second + "\n";
    }
    view.messageBox(txt, "Alert", wxOK | wxICON_ERROR);
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

                goto end;
            }
        } else if(type == "database") {
            std::string blockchainMoneySupply = Utils::find("blockchain.money_supply", pairs);
            if(!blockchainMoneySupply.empty()) {
                view.setSupply(blockchainMoneySupply); //TODO: isn't working
                goto end;
            }

            std::string value = Utils::find("value", pairs);
            if(!value.empty()) {
                view.setStatusBarMessage(value);
                goto end;
            }
        } else if(type == "network") {
            std::string tcp = Utils::find("network.tcp.connections", pairs);
            if(!tcp.empty()) {
                view.setTCP(tcp);
                std::string value = Utils::find("value", pairs);
                view.setStatusBarMessage(value);
                goto end;
            } else {
                std::string udp = Utils::find("network.udp.connections", pairs);
                if(!udp.empty()) {
                    view.setUDP(udp);
                    std::string value = Utils::find("value", pairs);
                    view.setStatusBarMessage(value);
                    goto end;
                }
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
            if(value=="Loaded wallet") {
                // Now that wallet has been loaded, set locked/unlocked status in GUI
                walletLoaded = true;
                view.setWalletStatus(stack.wallet_is_crypted() ? Locked : Unencrypted);
                goto end;
            }

            std::string balance = Utils::find("wallet.balance", pairs);
            std::string unconfirmed = Utils::find("wallet.balance.unconfirmed", pairs);
            std::string stake = Utils::find("wallet.stake", pairs);
            if(!balance.empty())
                view.setBalance(formated(balance));
            if(!unconfirmed.empty())
                view.setUnconfirmed(formated(unconfirmed));
            if(!stake.empty())
                view.setStake(formated(stake));
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
            std::string confirmations = Utils::find("wallet.transaction.confirmations", pairs);
            std::string confirmed = Utils::find("wallet.transaction.confirmed", pairs);
            std::string credit = Utils::find("wallet.transaction.credit", pairs);
            std::string debit = Utils::find("wallet.transaction.debit", pairs);
            std::string hash = Utils::find("wallet.transaction.hash", pairs);
            std::string mainChain = Utils::find("wallet.transaction.in_main_chain", pairs);
            std::string fromMe = Utils::find("wallet.transaction.is_from_me", pairs);
            std::string net = Utils::find("wallet.transaction.net", pairs);
            std::string time = Utils::find("wallet.transaction.time", pairs);

            if(!confirmations.empty() && !hash.empty() && !net.empty() && !time.empty()) {
                bool outcoming = !net.empty() && net[0] == '-';

                bool isConfirmed = confirmations != "0";

                std::string txMsg = isConfirmed ? (outcoming ? "Sent" : "Received")
                                                : (outcoming ? "Sending(0/1)" : "Receiving(0/1)");

                std::time_t txTime = (std::time_t) atoll(time.c_str());

                view.addTransaction(hash, txTime, txMsg, formated(net));
                view.setColour(hash, isConfirmed ? Green : Yellow);

                if (!outcoming && !isConfirmed)
                    view.notificationBox("Amount: " + formated(net), "Incoming Vcash transaction");
                goto end;
            }
        }
    }

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
