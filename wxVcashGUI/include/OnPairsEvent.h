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

#ifndef ONPAIRSEVENT_H
#define ONPAIRSEVENT_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/event.h>
#endif

#include <map>

namespace wxGUI {
    class OnStatusEvent;

    class OnErrorEvent;

    wxDECLARE_EVENT(wxEVT_ONSTATUS, OnStatusEvent);
    wxDECLARE_EVENT(wxEVT_ONERROR, OnErrorEvent);

    typedef void (wxEvtHandler::*OnStatusEventFunction)(OnStatusEvent &);

    #define OnStatusEventHandler(func) wxEVENT_HANDLER_CAST(OnStatusEventFunction, func)

    class OnPairsEvent : public wxCommandEvent {
    protected:
        OnPairsEvent(wxEventType commandType, int id = 0);

        // You *must* copy here the data to be transported
        OnPairsEvent(const OnPairsEvent &event);

        // Required for sending with wxPostEvent()
        wxEvent *Clone() const;

    public:
        std::map<std::string, std::string> GetPairs() const;

        void SetPairs(const std::map<std::string, std::string> &pairs);

    private:
        std::map<std::string, std::string> pairs;
    };

    class OnStatusEvent : public OnPairsEvent {
    public:
        OnStatusEvent(wxEventType commandType = wxEVT_ONSTATUS, int id = 0) :
                OnPairsEvent(commandType, id) {};
    };

    class OnErrorEvent : public OnPairsEvent {
    public:
        OnErrorEvent(wxEventType commandType = wxEVT_ONERROR, int id = 0) :
                OnPairsEvent(commandType, id) {};
    };
}

#endif // ONSTATUSEVENT_H
