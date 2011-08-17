/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/LocationEditDialog.h
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

#ifndef LOCATIONEDITDIALOG_H
#define LOCATIONEDITDIALOG_H

#ifdef __GNUG__
	#pragma interface "LocationEditDialog.h"
#endif

#include "DataSet.h"

#include <wx/dialog.h>

class wxButton;
class wxChoice;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

// begin wxGlade: dependencies
// end wxGlade

/*************************************************//**
*
*  editor for the data of a single location
*
******************************************************/
class LocationEditDialog: public wxDialog
{
	DECLARE_CLASS( LocationEditDialog )
public:
    // begin wxGlade: LocationEditDialog::ids
    // end wxGlade

	LocationEditDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition,
		const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);
	void setEntry( LocationEntry *e );
	LocationEntry *getEntry() { return &entry; }

private:
    // begin wxGlade: LocationEditDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

		void OnOK( wxCommandEvent &event );
		LocationEntry entry;

protected:
    // begin wxGlade: LocationEditDialog::attributes
    wxStaticBox* sizer_data_staticbox;
    wxStaticText* label_name;
    wxTextCtrl* name_edit;
    wxStaticText* label_country;
    wxTextCtrl* country_edit;
    wxStaticText* label_state;
    wxTextCtrl* state_edit;
    wxStaticText* label_long;
    wxTextCtrl* long_edit;
    wxChoice* choice_ew;
    wxStaticText* label_lat;
    wxTextCtrl* lat_edit;
    wxChoice* choice_ns;
    wxStaticText* label_tz_copy;
    wxTextCtrl* text_tz;
    wxChoice* choice_tzew;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
};


#endif // LOCATIONEDITDIALOG_H
