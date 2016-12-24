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

#include "OnPairsEvent.h"

using namespace wxGUI;

wxDEFINE_EVENT(wxGUI::wxEVT_ONSTATUS, OnStatusEvent);
wxDEFINE_EVENT(wxGUI::wxEVT_ONERROR, OnErrorEvent);

OnPairsEvent::OnPairsEvent(wxEventType commandType, int id)
        :  wxCommandEvent(commandType, id) { }

OnPairsEvent::OnPairsEvent(const OnPairsEvent &event)
        :  wxCommandEvent(event) {
    this->SetPairs(event.GetPairs());
}

wxEvent *OnPairsEvent::Clone() const {
    return new OnPairsEvent(*this);
}

std::map<std::string, std::string> OnPairsEvent::GetPairs() const {
    return pairs;
}

void OnPairsEvent::SetPairs(const std::map<std::string, std::string> &pairs) {
    this->pairs = pairs;
}
