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
 
#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/window.h>
#endif

namespace wxGUI {

    class VcashApp;

    class StatisticsPage : public wxPanel {
    public:
        StatisticsPage(VcashApp &vcashApp, wxWindow &parent);

    void setTCP(const std::string &tcp);
    void setUDP(const std::string &udp);
    void setEstimated(const std::string &estimated);
    void setTotal(const std::string &total);
    void setDifficulty(const std::string &difficulty);
    void setSupply(const std::string &supply);

    private:
        wxStaticText *TCP, *UDP, *estimated, *total, *difficulty, *supply;
    };
}

#endif // STATISTICSPAGE_H