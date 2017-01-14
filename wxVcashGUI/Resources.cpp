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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/graphics.h>
#endif

#include "Resources.h"

#include "resources/greenR16.cpp"
#include "resources/redR16.cpp"
#include "resources/yellowR16.cpp"

#if defined (__WXGTK__)
#include "resources/empty22.cpp"
#include "resources/locked22.cpp"
#include "resources/tools22.cpp"
#include "resources/unlocked22.cpp"
#include "resources/settings22.cpp"
#elif defined (__WXMSW__)
#include "resources/empty20.cpp"
#include "resources/locked20.cpp"
#include "resources/tools20.cpp"
#include "resources/unlocked20.cpp"
#include "resources/settings20.cpp"
#elif defined (__WXOSX__)
#include "resources/empty15.cpp"
#include "resources/locked15.cpp"
#include "resources/tools15.cpp"
#include "resources/unlocked15.cpp"
#include "resources/settings15.cpp"
#else
#error "You must define one of: __WXGTK__, __WXMSW__ or __WXOSX__"
#endif


#if defined ( __WXGTK__) || defined (__WXMSW__) || defined (__WXOSX__)
#include "resources/vcash.cpp"
#endif

using namespace wxGUI;

void Resources::init() {
    wxInitAllImageHandlers();

    redR = wxBITMAP_PNG_FROM_DATA(redR16);
    greenR = wxBITMAP_PNG_FROM_DATA(greenR16);
    yellowR = wxBITMAP_PNG_FROM_DATA(yellowR16);

    #if defined (__WXGTK__)
    empty = wxBITMAP_PNG_FROM_DATA(empty22);
    locked = wxBITMAP_PNG_FROM_DATA(locked22);
    tools = wxBITMAP_PNG_FROM_DATA(tools22);
    unlocked = wxBITMAP_PNG_FROM_DATA(unlocked22);
    settings = wxBITMAP_PNG_FROM_DATA(settings22);
    #elif defined (__WXMSW__)
    empty = wxBITMAP_PNG_FROM_DATA(empty20);
    locked = wxBITMAP_PNG_FROM_DATA(locked20);
    tools = wxBITMAP_PNG_FROM_DATA(tools20);
    unlocked = wxBITMAP_PNG_FROM_DATA(unlocked20);
    settings = wxBITMAP_PNG_FROM_DATA(settings20);
    #elif defined (__WXOSX__)
    empty = wxBITMAP_PNG_FROM_DATA(empty15);
    locked = wxBITMAP_PNG_FROM_DATA(locked15);
    tools = wxBITMAP_PNG_FROM_DATA(tools15);
    unlocked = wxBITMAP_PNG_FROM_DATA(unlocked15);
    settings = wxBITMAP_PNG_FROM_DATA(settings15);
    #endif

    wxBitmap bm = wxBITMAP_PNG_FROM_DATA(vcash);
    vcashIcon.CopyFromBitmap(bm);

    // Make a 64x64 copy of vcashIcon
    int dimension = 64;
    vcashImage64.Create(vcashIcon.GetWidth(), vcashIcon.GetHeight());
    vcashImage64.InitAlpha();
    unsigned char *alpha = vcashImage64.GetAlpha();
    memset(alpha, wxIMAGE_ALPHA_TRANSPARENT, vcashImage64.GetWidth()*vcashImage64.GetHeight());

    wxGraphicsContext *gc = wxGraphicsContext::Create(vcashImage64);
    gc->DrawIcon(vcashIcon, 0, 0, vcashIcon.GetWidth(), vcashIcon.GetHeight());
    delete gc;
    vcashImage64.Rescale(dimension, dimension);
}

wxBitmap Resources::redR;

wxBitmap Resources::greenR;

wxBitmap Resources::yellowR;

wxBitmap Resources::empty;

wxBitmap Resources::locked;

wxBitmap Resources::settings;

wxBitmap Resources::tools;

wxBitmap Resources::unlocked;

wxIcon Resources::vcashIcon;

wxImage Resources::vcashImage64;