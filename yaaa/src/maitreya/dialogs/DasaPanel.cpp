/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DasaPanel.cpp
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

#ifdef __GNUG__
	#pragma implementation "DasaPanel.h"
#endif

#include "DasaPanel.h"

#include <wx/bmpbuttn.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "guibase.h"
#include "IconProvider.h"

/*****************************************************
**
**   DasaPanel   ---   Constructor 
**
******************************************************/
DasaPanel::DasaPanel(wxWindow* parent, int id )
 : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    // begin wxGlade: DasaPanel::DasaPanel
    panel_details = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL);
    panel_header = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL);
    label_t = new wxStaticText(panel_header, -1, _("type"));
    button_entry_chart = new wxBitmapButton(panel_header, DASA_CREATE_ENTRY_CHART, wxNullBitmap);
    label_type = new wxStaticText(panel_details, -1, _("type"));
    label_startdate = new wxStaticText(panel_details, -1, _("start"));
    label_enddate = new wxStaticText(panel_details, -1, _("end"));
    label_duration = new wxStaticText(panel_details, -1, _("duration"));

    set_properties();
    do_layout();
    // end wxGlade
}

/*****************************************************
**
**   DasaPanel   ---   set_properties 
**
******************************************************/
void DasaPanel::set_properties()
{
	button_entry_chart->SetBitmapLabel( IconProvider::getBitmap( BITMAP_NEW ));
    // begin wxGlade: DasaPanel::set_properties
    button_entry_chart->SetMinSize(wxSize(32, 32));
    button_entry_chart->SetToolTip(_("Create Entry Chart"));
    label_type->SetMinSize(wxSize(120,22));
    label_startdate->SetMinSize(wxSize(120,22));
    label_enddate->SetMinSize(wxSize(120,22));
    label_duration->SetMinSize(wxSize(120,22));
    // end wxGlade
}

/*****************************************************
**
**   DasaPanel   ---   do_layout 
**
******************************************************/
void DasaPanel::do_layout()
{
    // begin wxGlade: DasaPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(2, 1, 0, 0);
    wxBoxSizer* sizer_details = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* grid_sizer_2 = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_label = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_label->Add(label_t, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_label->Add(button_entry_chart, 0, wxALL|wxALIGN_RIGHT|wxADJUST_MINSIZE, 3);
    panel_header->SetAutoLayout(true);
    panel_header->SetSizer(sizer_label);
    sizer_label->Fit(panel_header);
    sizer_label->SetSizeHints(panel_header);
    sizer_label->AddGrowableCol(0);
    grid_sizer_2->Add(panel_header, 1, wxALL|wxEXPAND, 3);
    grid_sizer_2->AddGrowableCol(0);
    sizer_main->Add(grid_sizer_2, 1, wxEXPAND, 0);
    sizer_details->Add(label_type, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_details->Add(label_startdate, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_details->Add(label_enddate, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_details->Add(label_duration, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    panel_details->SetAutoLayout(true);
    panel_details->SetSizer(sizer_details);
    sizer_details->Fit(panel_details);
    sizer_details->SetSizeHints(panel_details);
    sizer_main->Add(panel_details, 1, wxALL|wxEXPAND, 3);
    SetAutoLayout(true);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->SetSizeHints(this);
    sizer_main->AddGrowableRow(1);
    sizer_main->AddGrowableCol(0);
    // end wxGlade
}


