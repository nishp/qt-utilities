/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DefaultLocationPanel.cpp
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
	#pragma implementation "DefaultLocationPanel.h"
#endif

#include "DefaultLocationPanel.h"

#include "LocationDialog.h"

#include "astrobase.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"

#include <math.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>

using namespace std;

extern Config *config;
enum { CD_SEARCHLOCATION = wxID_HIGHEST + 1, CD_TIMER };

IMPLEMENT_CLASS( DefaultLocationPanel, ConfigPanel )

/*****************************************************
**
**   DefaultLocationPanel   ---   Constructor 
**
******************************************************/
DefaultLocationPanel::DefaultLocationPanel( wxWindow* parent )
 : ConfigPanel( parent )
{
    // begin wxGlade: DefaultLocationPanel::DefaultLocationPanel
    sizer_deflocdata_staticbox = new wxStaticBox(this, -1, _("Geographical Data"));
    sizer_tz_staticbox = new wxStaticBox(this, -1, _("Time Zone Options"));
    sizer_deflocname_staticbox = new wxStaticBox(this, -1, _("Default Location"));
    text_deflocname = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_defloc_long = new wxStaticText(this, wxID_ANY, _("Longitude"));
    text_deflong = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    const wxString choice_ew_choices[] = {
        _("East"),
        _("West")
    };
    choice_ew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ew_choices, 0);
    label_defloc_lat = new wxStaticText(this, wxID_ANY, _("Latitude"));
    text_deflat = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    const wxString choice_ns_choices[] = {
        _("North"),
        _("South")
    };
    choice_ns = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ns_choices, 0);
    label_defloc_tz = new wxStaticText(this, wxID_ANY, _("Time Zone"));
    text_timezone = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    const wxString choice_tzew_choices[] = {
        _("East"),
        _("West")
    };
    choice_tzew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_tzew_choices, 0);
    label_defloc_dst = new wxStaticText(this, wxID_ANY, _("Daylight Saving"));
    text_dst = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_defloc_dst2 = new wxStaticText(this, wxID_ANY, _("Hours"));
    label_systime = new wxStaticText(this, wxID_ANY, _("Current Time"));
    text_systime = new wxStaticText(this, wxID_ANY, _("00:00:00"));
    searchloc_button = new wxButton(this, CD_SEARCHLOCATION, _("Search Location ..."));

    set_properties();
    do_layout();
    // end wxGlade
	timer = new wxTimer( this, CD_TIMER );
	text_deflocname->SetFocus();

  Connect( CD_SEARCHLOCATION, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DefaultLocationPanel::OnSearchLocation ));
  Connect( CD_TIMER, wxEVT_TIMER, wxTimerEventHandler( DefaultLocationPanel::OnTimer ));
}

/*****************************************************
**
**   DefaultLocationPanel   ---   Destructor
**
******************************************************/
DefaultLocationPanel::~DefaultLocationPanel()
{
	if ( timer->IsRunning()) timer->Stop();
	delete timer;
}

/*****************************************************
**
**   DefaultLocationPanel   ---   onActivate
**
******************************************************/
void DefaultLocationPanel::onActivate()
{
	if ( ! timer->IsRunning() ) timer->Start( 1000 );
}

/*****************************************************
**
**   DefaultLocationPanel   ---   onPassivate
**
******************************************************/
void DefaultLocationPanel::onPassivate()
{
	if ( timer->IsRunning()) timer->Stop();
}

/*****************************************************
**
**   DefaultLocationPanel   ---   OnTimer 
**
******************************************************/
void DefaultLocationPanel::OnTimer( wxTimerEvent &event )
{
	time_t tt;
	time( &tt );
	double mytime, diff, dst;
	Formatter *formatter = Formatter::get();

	mytime = 2440587.5 + (double)tt / 86400.0; // 1.1.1970 + seconds elapsed / number of seconds per day

	double tz = myatof( text_timezone->GetValue());
	if ( choice_tzew->GetSelection()) tz *= -1;
	text_dst->GetValue().ToDouble( &dst );
	diff = tz + dst;

	//if ( check_systemgt->GetValue() ) mytime += diff/24;
	mytime += diff/24;
	text_systime->SetLabel( formatter->getTimeFormatted( getTimeFromJD( mytime )));
}

/*****************************************************
**
**   DefaultLocationPanel   ---   set_properties 
**
******************************************************/
void DefaultLocationPanel::set_properties()
{
    // begin wxGlade: DefaultLocationPanel::set_properties
    text_deflong->SetMinSize(wxSize(70, 21));
    choice_ew->SetSelection(0);
    text_deflat->SetMinSize(wxSize(70, 21));
    choice_ns->SetSelection(0);
    text_timezone->SetMinSize(wxSize(50, 21));
    choice_tzew->SetSelection(0);
    text_dst->SetMinSize(wxSize(50, 21));
    // end wxGlade
}

/*****************************************************
**
**   DefaultLocationPanel   ---   do_layout 
**
******************************************************/
void DefaultLocationPanel::do_layout()
{
    // begin wxGlade: DefaultLocationPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* sizer_defloc = new wxFlexGridSizer(4, 1, 0, 0);
    wxStaticBoxSizer* sizer_tz = new wxStaticBoxSizer(sizer_tz_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_tz = new wxFlexGridSizer(3, 3, 3, 3);
    wxStaticBoxSizer* sizer_deflocdata = new wxStaticBoxSizer(sizer_deflocdata_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_geographicaldata = new wxFlexGridSizer(2, 3, 3, 3);
    wxStaticBoxSizer* sizer_deflocname = new wxStaticBoxSizer(sizer_deflocname_staticbox, wxVERTICAL);
    sizer_deflocname->Add(text_deflocname, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_defloc->Add(sizer_deflocname, 1, wxALL|wxEXPAND, 3);
    grid_sizer_geographicaldata->Add(label_defloc_long, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(text_deflong, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(choice_ew, 0, wxALL|wxEXPAND, 3);
    grid_sizer_geographicaldata->Add(label_defloc_lat, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(text_deflat, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(choice_ns, 0, wxALL|wxEXPAND, 3);
    grid_sizer_geographicaldata->AddGrowableCol(1);
    sizer_deflocdata->Add(grid_sizer_geographicaldata, 1, wxALL, 3);
    sizer_defloc->Add(sizer_deflocdata, 1, wxALL|wxEXPAND, 3);
    grid_sizer_tz->Add(label_defloc_tz, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_tz->Add(text_timezone, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_tz->Add(choice_tzew, 0, wxALL, 3);
    grid_sizer_tz->Add(label_defloc_dst, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_tz->Add(text_dst, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_tz->Add(label_defloc_dst2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_tz->Add(label_systime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_tz->Add(text_systime, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_tz->Add(20, 20, 0, wxALL, 3);
    sizer_tz->Add(grid_sizer_tz, 1, wxALL, 3);
    sizer_defloc->Add(sizer_tz, 1, wxALL|wxEXPAND, 3);
    sizer_defloc->Add(searchloc_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_defloc->AddGrowableCol(0);
    sizer_main->Add(sizer_defloc, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   DefaultLocationPanel   ---   setData 
**
******************************************************/
void DefaultLocationPanel::setData()
{
	setDefaultLocation( &config->defaultLocation );
  text_dst->SetValue( printfDouble( config->defaultLocation.getDST()));
}

/*****************************************************
**
**   DefaultLocationPanel   ---   saveData 
**
******************************************************/
bool DefaultLocationPanel::saveData()
{
  Formatter *formatter = Formatter::get();
  double longitude, latitude, tz;
  int ldeg, lmin, lsec, bdeg, bmin, bsec;
  wxColour color;
   
  if ( ! formatter->getDegreeIntsFromString( (const wxChar*)text_deflong->GetValue(), ldeg, lmin, lsec ))
  {
		doMessageBox( this, _("Longitude field invalid"), wxOK | wxICON_ERROR | wxCENTRE );
    return false;
  }
 
  if ( ! formatter->getDegreeIntsFromString( (const wxChar*)text_deflat->GetValue(), bdeg, bmin, bsec ))
  {
		doMessageBox( this, _("Latitude field invalid"), wxOK | wxICON_ERROR | wxCENTRE );
    return false;
  }

  longitude = 3600 * ldeg + 60 * lmin + lsec;
  longitude /= 3600;
  if ( choice_ew->GetSelection() ) longitude *= -1;
 
  latitude = 3600 * bdeg + 60 * bmin + bsec;
  latitude /= 3600;
  if ( choice_ns->GetSelection() ) latitude *= -1;

	tz = myatof( text_timezone->GetValue());
	if ( choice_tzew->GetSelection()) tz *= -1;


	double d;
	if ( ! text_dst->GetValue().ToDouble( &d ) ) wxLogError( wxT( "Cannot convert time zone to double" ));
  config->defaultLocation.setLocation( text_deflocname->GetValue(), longitude, latitude, tz, d );

	return true;
}

/*****************************************************
**
**   DefaultLocationPanel   ---   setDefaultLocation 
**
******************************************************/
void DefaultLocationPanel::setDefaultLocation( Location *loc )
{
	Formatter *formatter = Formatter::get();
  double longitude, latitude;

  text_deflocname->SetValue( loc->getLocName() );
  // update longitude
  longitude = loc->getLongitude();
  text_deflong->SetValue( formatter->getDegreesFormatted( fabs( longitude )));
  choice_ew->SetSelection( longitude < 0 );

  // update latitude
  latitude = loc->getLatitude();
  text_deflat->SetValue( formatter->getDegreesFormatted( fabs( latitude )));
  if ( latitude >= 0 ) choice_ns->SetSelection( 0 );
  else choice_ns->SetSelection( 1 );

  text_timezone->SetValue( printfDouble( fabs( loc->getTimeZone() )));
  choice_tzew->SetSelection( loc->getTimeZone() < 0 );
}

/*****************************************************
**
**   DefaultLocationPanel   ---   OnSearchLocation 
**
******************************************************/
void DefaultLocationPanel::OnSearchLocation( wxCommandEvent &event )
{
  Location *l;
  LocationDialog dialog( this, 2 );
  if ( dialog.ShowModal() == wxID_OK )
  {
    l = dialog.getSelectedLocation();
    if ( ! l ) return;
    setDefaultLocation( l );
  }
}

