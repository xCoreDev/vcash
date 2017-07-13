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

#include "Utils.h"
#include "wxStack.h"

#include <algorithm>

using namespace wxGUI;

std::int64_t Utils::stringToJohnoshis(const std::string &amount) {
    return std::stoll(amount, nullptr);
}

std::int64_t Utils::toJohnoshis(double amount) {
    return static_cast<std::int64_t>(amount*wxStack::oneVcash);
}

std::int64_t Utils::toJohnoshis(const std::string &amount) {
    return toJohnoshis(std::stod(amount, nullptr));
}

double Utils::fromJohnoshis(std::int64_t amount) {
    return (double)amount/wxStack::oneVcash;
}

double Utils::fromJohnoshis(std::string &amount) {
    return fromJohnoshis(stringToJohnoshis(amount));
}

std::string Utils::formatted(double amount, int decimals) {
    char buffer[256];
    std::string format = "%."+std::to_string(decimals)+"f";
    snprintf(buffer, sizeof(buffer), format.c_str(), amount);
    std::string res = buffer;
    return res;
}

std::string Utils::find(const std::string &key, const std::map<std::string, std::string> &pairs) {
    auto it = pairs.find(key);
    if (it != pairs.end())
        return it->second;
    else
        return std::string("");
}

bool Utils::isPrefix(const std::string &prefix, const std::string &str) {
    auto res = std::mismatch(prefix.begin(), prefix.end(), str.begin());
    return (res.first == prefix.end());
}
