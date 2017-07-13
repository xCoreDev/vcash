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

#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>

namespace wxGUI {
    class Utils {
    public:
        static std::int64_t stringToJohnoshis(const std::string &amount);

        static std::int64_t toJohnoshis(double amount);

        static std::int64_t toJohnoshis(const std::string &amount);

        static double fromJohnoshis(std::int64_t amount);

        static double fromJohnoshis(std::string &amount);

        static std::string formatted(double amount, int decimals);

        static std::string find(const std::string &key, const std::map<std::string, std::string> &pairs);

        static bool isPrefix(const std::string &prefix, const std::string &str);
    };
}

#endif // UTILS_H
 
