/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/LocationDialog.h
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

#ifndef LOCATIONDIALOG_H
#define LOCATIONDIALOG_H

#ifdef __GNUG__
	#pragma interface "LocationDialog.h"
#endif

#include "DataSet.h"
#include "guibase.h"

#include <wx/dialog.h>
#include <wx/statline.h>
#include <list>

// begin wxGlade: dependencies
// end wxGlade

class MyGrid;
class wxButton;
class wxChoice;
class wxGridEvent;
class wxProgressDialog;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

using namespace std;

/*************************************************//**
*
*  dialog for GUI access to location database
*
******************************************************/
class LocationDialog: public wxDialog
{
	DECLARE_CLASS( LocatinDialog )
public:
    // begin wxGlade: LocationDialog::ids
    // end wxGlade

	LocationDialog(wxWindow* parent, const int &callerId );
	~LocationDialog();
	Location* getSelectedLocation();

private:
    // begin wxGlade: LocationDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	wxProgressDialog *progressdialog;
	list<wxString> countries;
	list<wxString> fav_countries;
	list<LocationEntry*> entries;
	bool doSetup();
	void updateGrid();
	void OnSelectCell( wxGridEvent& );
	void OnSelectLabel( wxGridEvent& );
	void OnCellChange( wxGridEvent& );
	void OnCellDClick( wxGridEvent& );
	void OnApplyFilter( wxCommandEvent& );
	void OnResetFilter( wxCommandEvent& );
	void OnAddEntry( wxCommandEvent& );
	void OnDeleteEntry( wxCommandEvent& );
	void OnEditEntry( wxCommandEvent& );
	void OnFilterChanged( wxCommandEvent& );
	void OnCountryChoice( wxCommandEvent& );
	void OnAddCountry( wxCommandEvent& );
	void OnDelCountry( wxCommandEvent& );
	void updateLocationData();
	wxString filename;
	LocationEntry *selectedEntry;
	Location selectedLocation;
	bool dirty;
	bool calledFromDialog;
	bool saveDB();
	void OnOK( wxCommandEvent &event );
	void OnCancel( wxCommandEvent &event );
	void OnSaveDB( wxCommandEvent &event );
	void setDirty( const bool& );
	void updateCountryChoice();
	void saveFavouriteContries();
	bool editEntry();

protected:
    // begin wxGlade: LocationDialog::attributes
    wxStaticBox* sizer_entry_staticbox;
    wxStaticBox* sizer_datadetails_staticbox;
    wxStaticBox* sizer_country_edit_staticbox;
    wxStaticBox* sizer_actions_staticbox;
    wxStaticBox* sizer_filter_staticbox;
    wxStaticText* label_filtermatch;
    wxTextCtrl* filter_edit;
    wxStaticText* label_filtercountry;
    wxChoice* country_choice;
    wxButton* applyfilterbutton;
    wxButton* resetbutton;
    wxButton* button_addcountry;
    wxButton* button_delcountry;
    MyGrid* locgrid;
    wxStaticText* match_label;
    wxStaticLine* static_line_match;
    wxStaticText* label_showname;
    wxStaticText* label_name;
    wxStaticText* label_showcountry;
    wxStaticText* label_country;
    wxStaticText* label_showstate;
    wxStaticText* label_state;
    wxStaticText* label_showlong;
    wxStaticText* label_lon;
    wxStaticText* label_showlat;
    wxStaticText* label_lat;
    wxButton* addbutton;
    wxButton* button_mod;
    wxButton* delbutton;
    wxStaticLine* static_line_button;
    wxButton* button_accept;
    wxButton* button_close;
    wxButton* button_save;
    // end wxGlade
};


#endif // LOCATIONDIALOG_H
