/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/LocationEditDialog.cpp
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
	#pragma implementation "LocationEditDialog.h"
#endif

#include "LocationEditDialog.h"

#include <math.h>

#include <wx/button.h>  
#include <wx/choice.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "IconProvider.h"

#include "Conf.h"
#include "constants.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"

IMPLEMENT_CLASS( LocationEditDialog, wxDialog )

extern Config *config;

/*****************************************************
**
**   LocationEditDialog   ---   Constructor 
**
******************************************************/
LocationEditDialog::LocationEditDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos,
	const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, DEFAULT_DIALOG_STYLE )
{
    // begin wxGlade: LocationEditDialog::LocationEditDialog
    sizer_data_staticbox = new wxStaticBox(this, -1, _("Data"));
    label_name = new wxStaticText(this, wxID_ANY, _("Name"));
    name_edit = new wxTextCtrl(this, -1, wxEmptyString);
    label_country = new wxStaticText(this, wxID_ANY, _("Country"));
    country_edit = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_state = new wxStaticText(this, wxID_ANY, _("State"));
    state_edit = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_long = new wxStaticText(this, wxID_ANY, _("Longitude"));
    long_edit = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    const wxString choice_ew_choices[] = {
        _("East"),
        _("West")
    };
    choice_ew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ew_choices, 0);
    label_lat = new wxStaticText(this, wxID_ANY, _("Latitude"));
    lat_edit = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    const wxString choice_ns_choices[] = {
        _("North"),
        _("South")
    };
    choice_ns = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ns_choices, 0);
    label_tz_copy = new wxStaticText(this, wxID_ANY, _("Time Zone"));
    text_tz = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    const wxString choice_tzew_choices[] = {
        _("East"),
        _("West")
    };
    choice_tzew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_tzew_choices, 0);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade
		moveWindow2Center( this );
		SetIcon( IconProvider::getIcon( BITMAP_WORLD ));
		name_edit->SetFocus();

	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationEditDialog::OnOK ));
}

/*****************************************************
**
**   LocationEditDialog   ---   setEntry 
**
******************************************************/
void LocationEditDialog::setEntry( LocationEntry *e )
{
	wxString s;
  Formatter *formatter = Formatter::get();

	if ( e )
	{
		entry = *e;
		name_edit->SetValue( entry.name );
		country_edit->SetValue( entry.country );
		state_edit->SetValue( entry.state );

		long_edit->SetValue( formatter->getDegreesFormatted( fabs( entry.longitude ) ) );
		choice_ew->SetSelection( ( entry.longitude > 0 ? 0 : 1 ));

		lat_edit->SetValue( formatter->getDegreesFormatted( fabs( entry.latitude ) ) );
		choice_ns->SetSelection( ( entry.latitude > 0 ? 0 : 1 ));

		s.Printf( wxT( "%2.1f" ),  ::fabs( entry.timeZone ));
		text_tz->SetValue( s );
		choice_tzew->SetSelection( entry.timeZone < 0 );
	}
	else
	{
		name_edit->SetValue( config->defaultLocation.getLocName() );
		country_edit->SetValue( wxT( "" ));
		state_edit->SetValue( wxT( "" ));

		long_edit->SetValue( formatter->getDegreesFormatted( fabs( config->defaultLocation.getLongitude() )));
		choice_ew->SetSelection( config->defaultLocation.getLongitude() > 0 ? 0 : 1 );

		lat_edit->SetValue( formatter->getDegreesFormatted( fabs( config->defaultLocation.getLatitude() )));
		choice_ns->SetSelection( config->defaultLocation.getLatitude() > 0 ? 0 : 1 );

		text_tz->SetValue( printfDouble( ::fabs( config->defaultLocation.getTimeZone() )) );
		choice_tzew->SetSelection( config->defaultLocation.getTimeZone() < 0 );
	}
}

/*****************************************************
**
**   LocationEditDialog   ---   OnOK 
**
******************************************************/
void LocationEditDialog::OnOK( wxCommandEvent &event )
{
	Formatter *formatter = Formatter::get();
	int ldeg, lmin, lsec, bdeg, bmin, bsec;

	entry.name = name_edit->GetValue();
	entry.country = country_edit->GetValue();
	entry.state = state_edit->GetValue();

	if ( ! formatter->getDegreeIntsFromString( (const wxChar*)long_edit->GetValue(), ldeg, lmin, lsec ))
	{
		doMessageBox( this, _("Longitude field invalid"), wxOK | wxICON_ERROR | wxCENTRE );
		return;
	}

	if ( ! formatter->getDegreeIntsFromString( (const wxChar*)lat_edit->GetValue(), bdeg, bmin, bsec ))
	{
		doMessageBox( this, _("Latitude field invalid"), wxOK | wxICON_ERROR | wxCENTRE );
		return;
	}

  entry.longitude = 3600 * ldeg + 60 * lmin + lsec;
	entry.longitude /= 3600;
	if ( choice_ew->GetSelection() ) entry.longitude *= -1;

  entry.latitude = 3600 * bdeg + 60 * bmin + bsec;
	entry.latitude /= 3600;
	if ( choice_ns->GetSelection() ) entry.latitude *= -1;

	entry.timeZone = myatof( text_tz->GetValue() );
	if ( choice_tzew->GetSelection() ) entry.timeZone *= -1;

	EndModal( wxID_OK );
}

/*****************************************************
**
**   LocationEditDialog   ---   set_properties 
**
******************************************************/
void LocationEditDialog::set_properties()
{
    // begin wxGlade: LocationEditDialog::set_properties
    SetTitle(_("Edit Location"));
    label_name->SetMinSize(wxSize(80, 16));
    name_edit->SetMinSize(wxSize(200, 22));
    label_country->SetMinSize(wxSize(80, 16));
    country_edit->SetMinSize(wxSize(200, 22));
    label_state->SetMinSize(wxSize(80, 16));
    state_edit->SetMinSize(wxSize(200, 22));
    label_long->SetMinSize(wxSize(80, 16));
    long_edit->SetMinSize(wxSize(105, 22));
    choice_ew->SetSelection(0);
    label_lat->SetMinSize(wxSize(80, 16));
    lat_edit->SetMinSize(wxSize(105, 22));
    choice_ns->SetSelection(0);
    label_tz_copy->SetMinSize(wxSize(80, 16));
    text_tz->SetMinSize(wxSize(105, 22));
    choice_tzew->SetSelection(0);
    // end wxGlade
}


/*****************************************************
**
**   LocationEditDialog   ---   do_layout 
**
******************************************************/
void LocationEditDialog::do_layout()
{
    // begin wxGlade: LocationEditDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 3, 0, 0);
    wxBoxSizer* sizer_button = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_data = new wxStaticBoxSizer(sizer_data_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_data = new wxFlexGridSizer(6, 2, 3, 3);
    wxFlexGridSizer* grid_tz = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* grid_lat = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* grid_long = new wxFlexGridSizer(1, 2, 0, 0);
    grid_data->Add(label_name, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(name_edit, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(label_country, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(country_edit, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(label_state, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(state_edit, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(label_long, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_long->Add(long_edit, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_long->Add(choice_ew, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(grid_long, 1, wxEXPAND, 0);
    grid_data->Add(label_lat, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_lat->Add(lat_edit, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_lat->Add(choice_ns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(grid_lat, 1, wxEXPAND, 0);
    grid_data->Add(label_tz_copy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(text_tz, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(choice_tzew, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_data->Add(grid_tz, 1, wxEXPAND, 0);
    sizer_data->Add(grid_data, 1, wxEXPAND, 0);
    sizer_main->Add(sizer_data, 1, wxALL|wxEXPAND, 3);
    wxStaticLine* static_line_button = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    sizer_main->Add(static_line_button, 0, wxLEFT|wxRIGHT|wxEXPAND, 8);
    sizer_button->Add(button_ok, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_button->Add(button_cancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_main->Add(sizer_button, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableCol(0);
    Layout();
    // end wxGlade
}

