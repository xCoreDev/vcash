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

#ifndef BLOCKEXPLORER_H
#define BLOCKEXPLORER_H

#include <string>

namespace wxGUI {
    class BlockExplorer {
    public:
        static std::string addressURL(const std::string &addr);
        static std::string transactionURL(const std::string &txid);
    };

    class BlockExperts : public BlockExplorer {
    public:
        static const std::string URL;
        static std::string addressURL(const std::string &addr);
        static std::string transactionURL(const std::string &txid);
    };

    class VcashExplorer : public BlockExplorer {
    public:
        static const std::string URL;
        static std::string addressURL(const std::string &addr);
        static std::string transactionURL(const std::string &txid);
    };
}

#endif // BLOCKEXPLORER_H
 