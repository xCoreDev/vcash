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
#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#endif

#include "QRDialog.h"

namespace wxGUI {
    class QRImage : public wxWindow {
    private:
        QRcode *qrcode;
    public:
        QRImage(wxWindow *parent, const std::string str, const int dimension) : wxWindow(parent, wxID_ANY) {
            SetMinSize(wxSize(dimension, dimension));
            SetMaxSize(wxSize(dimension, dimension));
            qrcode = QRcode_encodeString(str.c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, 1);
            Bind(wxEVT_PAINT, [this](wxPaintEvent &event) {
                paintEvent(event);
            });
        }

        ~QRImage() {
            if(qrcode!=NULL)
                QRcode_free(qrcode);
        }

        bool AcceptsFocus() const {
            return false;
        }

        bool AcceptsFocusFromKeyboard() const {
            return false;
        }

        void paintEvent(wxPaintEvent &evt) {
            wxPaintDC dc(this);
            render(dc);
        }

        void render(wxDC &dc) {
            if(qrcode==NULL)
                return;

            wxSize sz = GetClientSize();
            int width = qrcode->width;
            double dim = wxMin(sz.x, sz.y) / width;
            double marginX = (sz.x - (dim * width)) / 2;
            double marginY = (sz.y - (dim * width)) / 2;

            dc.SetPen(*wxWHITE_PEN);
            dc.SetBrush(*wxWHITE_BRUSH);
            wxRect rectToDraw(marginX, marginY, dim*width, dim*width);
            if (IsExposed(rectToDraw))
                dc.DrawRectangle(rectToDraw);

            dc.SetPen(*wxBLACK_PEN);
            dc.SetBrush(*wxBLACK_BRUSH);
            for (int pos = 0, i = 0; i < width; i++) {
                for (int j = 0; j < width; j++, pos++) {
                    if (qrcode->data[pos] & 1) {
                        wxRect rectToDraw(marginX + dim*j, marginY + dim*i, dim, dim);
                        if (IsExposed(rectToDraw))
                            dc.DrawRectangle(rectToDraw);
                    }
                }
            }
        }
    };
}

using namespace wxGUI;

wxGUI::QRDialog::QRDialog(wxWindow &parent, const wxString &title, const wxString &str)
        : ShowInfoDialog(parent, title, [this, str]() {
            wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

            wxStaticText *header = new wxStaticText(this, wxID_ANY, wxString(str));
            const int dimension = 128;
            QRImage *qr = new QRImage(this, str.ToStdString(), dimension);
            vbox->Add(header, 0, wxALIGN_CENTER  | wxALL, 10);
            vbox->Add(qr, 0, wxALIGN_CENTER  | wxALL, 10);

            return vbox;
          }) { }
