/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DasaPanel.h
 *  Release    6.0
 *  Author     M. Pettau
 *  Copyright  2003-2010 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
****************************************************************************/

#ifndef DASAPANEL_H
#define DASAPANEL_H

#ifdef __GNUG__
	#pragma interface "DasaPanel.h"
#endif

// begin wxGlade: dependencies
// end wxGlade

#include <wx/panel.h>

class wxBitmapButton;
class wxStaticText;

enum { DASA_CREATE_ENTRY_CHART = wxID_HIGHEST + 2000 };

/*************************************************//**
*
*  \brief bottom panel in Dasa tree view
*
******************************************************/
class DasaPanel: public wxPanel
{
	friend class DasaView;
public:
    // begin wxGlade: DasaPanel::ids
    // end wxGlade

    DasaPanel(wxWindow* parent, int id );

private:
    // begin wxGlade: DasaPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
protected:
    // begin wxGlade: DasaPanel::attributes
    wxStaticText* label_t;
    wxBitmapButton* button_entry_chart;
    wxPanel* panel_header;
    wxStaticText* label_type;
    wxStaticText* label_startdate;
    wxStaticText* label_enddate;
    wxStaticText* label_duration;
    wxPanel* panel_details;
    // end wxGlade

private:
};


#endif // DASAPANEL_H
