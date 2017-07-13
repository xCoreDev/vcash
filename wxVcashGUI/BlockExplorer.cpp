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

#include "BlockExplorer.h"

using namespace wxGUI;

const std::string BlockExperts::URL = "https://www.blockexperts.com/xvc";

std::string BlockExperts::addressURL(const std::string &addr) {
    return (URL + "/address/" + addr);
}

std::string BlockExperts::transactionURL(const std::string &txid) {
    return (URL + "/tx/" + txid);
}

const std::string VcashExplorer::URL = "https://explorer.vchain.info/";

std::string VcashExplorer::addressURL(const std::string &addr) {
    return (URL + "/address/" + addr);
}

std::string VcashExplorer::transactionURL(const std::string &txid) {
    return (URL + "/tx/" + txid);
}

 