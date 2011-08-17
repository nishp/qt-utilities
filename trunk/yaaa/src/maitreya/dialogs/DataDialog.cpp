/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DataDialog.cpp
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
	#pragma implementation "DataDialog.h"
#endif

#include "DataDialog.h"

#include <math.h>

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "constants.h"
#include "Calculator.h"
#include "Conf.h"
#include "DataSet.h"
#include "Lang.h"
#include "mathbase.h"

#include "DateTimeCtrl.h"
#include "DialogElements.h"
#include "Document.h"
#include "guibase.h"
#include "IconProvider.h"
#include "LocationDialog.h"

extern Config *config;

enum { DD_NOW = wxID_HIGHEST + 300, DD_DEFLOC, DD_SEARCHLOC, DD_LOCALTIME, DD_REVERT };

IMPLEMENT_CLASS( DataDialog, wxDialog )

/*****************************************************
**
**   DataDialog   ---   Constructor 
**
******************************************************/
DataDialog::DataDialog( wxWindow* parent, Document *d )
 : wxDialog( parent, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
	doc = d;
	writeDoc2Model();
	//SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY ); -> dialog doesnt close under linux if closed by right upper corner button

    // begin wxGlade: DataDialog::DataDialog
    sizer_datetime_staticbox = new wxStaticBox(this, -1, _("Date and Time"));
    sizer_location_staticbox = new wxStaticBox(this, -1, _("Location"));
    sizer_tz_staticbox = new wxStaticBox(this, -1, _("Time Zone"));
    sizer_remark_staticbox = new wxStaticBox(this, -1, _("Remark"));
    sizer_name_staticbox = new wxStaticBox(this, -1, _("Name"));
    text_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_date = new wxStaticText(this, wxID_ANY, _("Date"));
    text_date = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_datefmt = new wxStaticText(this, wxID_ANY, _("YYYY-MM-DD"));
    label_time = new wxStaticText(this, wxID_ANY, _("Time"));
    text_time = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_timefmt = new wxStaticText(this, wxID_ANY, _("HH:MM:SS"));
    button_now = new wxButton(this, DD_NOW, _("Now"));
    label_locname = new wxStaticText(this, wxID_ANY, _("Name"));
    text_locname = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_long = new wxStaticText(this, wxID_ANY, _("Longitude"));
    text_longitude = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_long_fmt = new wxStaticText(this, wxID_ANY, _("DD:MM:SS"));
    const wxString choice_ew_choices[] = {
        _("East"),
        _("West")
    };
    choice_ew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ew_choices, 0);
    label_lat = new wxStaticText(this, wxID_ANY, _("Latitude"));
    text_latitude = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_lat_fmt = new wxStaticText(this, wxID_ANY, _("DD:MM:SS"));
    const wxString choice_ns_choices[] = {
        _("North"),
        _("South")
    };
    choice_ns = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ns_choices, 0);
    button_defloc = new wxButton(this, DD_DEFLOC, _("Default Location"));
    button_searchloc = new wxButton(this, DD_SEARCHLOC, _("Search Location"));
    label_tz = new wxStaticText(this, wxID_ANY, _("Time Zone"));
    text_tz = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_tzfmt = new wxStaticText(this, wxID_ANY, _("HH"));
    const wxString choice_tzew_choices[] = {
        _("East"),
        _("West")
    };
    choice_tzew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_tzew_choices, 0);
    label_dst = new wxStaticText(this, wxID_ANY, _("DST"));
    text_dst = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_dstfmt = new wxStaticText(this, wxID_ANY, _("HH"));
    button_localtime = new wxButton(this, DD_LOCALTIME, _("Local Time"));
    text_remark = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
    button_apply = new wxButton(this, wxID_APPLY, _("Apply"));
    button_revert = new wxButton(this, DD_REVERT, _("Revert"));

    set_properties();
    do_layout();
    // end wxGlade

	writeModel2Gui();
	SetIcon( IconProvider::getIcon( BITMAP_EDIT ));
	moveWindow2Center( this );
	text_name->SetFocus();

  Connect( DD_NOW, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnActual ));
  Connect( DD_DEFLOC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnDefaultLocation ));
  Connect( DD_SEARCHLOC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnSearchLocation ));
  Connect( DD_REVERT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnRevert ));
  Connect( wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnApply ));
  Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnOK ));
  Connect( wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnCancel ));
  Connect( DD_LOCALTIME, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnLocalTime ));
}

/*****************************************************
**
**   DataDialog   ---   writeDoc2Model 
**
******************************************************/
void DataDialog::writeDoc2Model()
{
	Formatter *formatter = Formatter::get();
	Location *loc = doc->getLocation();
	double tmpjd, time;

	model.name = doc->getHName();

	tmpjd = doc->getJD() + loc->getTimeZone() / 24 + loc->getDST() / 24;
	model.date = formatter->getDateStringFromJD( floor( tmpjd + .5 ));

	time = ( tmpjd - floor( tmpjd ) + .5 ) * 24;
	if ( time > 24 ) time -= 24;
	model.time = formatter->getTimeFormatted( time );

	model.locname = loc->getLocName();
  model.longitude = formatter->getDegreesFormatted( fabs( loc->getLongitude() ));
  model.ew = ( loc->getLongitude() < 0 );

  model.latitude = formatter->getDegreesFormatted( fabs( loc->getLatitude() ));
  model.ns = ( loc->getLatitude() < 0 );

	model.tz = printfDouble( ::fabs( loc->getTimeZone()));
	model.tzew = ( loc->getTimeZone() < 0 );
	model.dst = printfDouble( ::fabs( loc->getDST() ));

	model.remark = doc->getRemark();
}

/*****************************************************
**
**   DataDialog   ---   writeModel2Gui 
**
******************************************************/
void DataDialog::writeModel2Gui()
{
	text_name->SetValue( model.name );
	text_date->SetValue( model.date );
	text_time->SetValue( model.time );

	text_locname->SetValue( model.locname );
	text_longitude->SetValue( model.longitude );
	choice_ew->SetSelection( model.ew );
	text_latitude->SetValue( model.latitude );
	choice_ns->SetSelection( model.ns );

	text_tz->SetValue( model.tz );
	text_dst->SetValue( model.dst );
	choice_tzew->SetSelection( model.tzew );

	text_remark->SetValue( model.remark );
}


/*****************************************************
**
**   DataDialog   ---   writeGui2Model 
**
******************************************************/
void DataDialog::writeGui2Model()
{
	model.name = text_name->GetValue();
	model.date = text_date->GetValue();
	model.time = text_time->GetValue();

	model.locname = text_locname->GetValue();
	model.longitude = text_longitude->GetValue();
	model.ew = choice_ew->GetSelection();
	model.latitude = text_latitude->GetValue();
	model.ns = choice_ns->GetSelection();

	model.tz = text_tz->GetValue();
	model.tzew = choice_tzew->GetSelection();
	model.dst = text_dst->GetValue();
	model.remark = text_remark->GetValue();
}


/*****************************************************
**
**   DataDialog   ---   writeModel2Doc 
**
******************************************************/
void DataDialog::writeModel2Doc()
{
	int day, month, year;
	int hour, minute, second, deg;
	double jd, time, tz, dst, longitude, latitude;
  Formatter *formatter = Formatter::get();
	Calculator *calculator = CalculatorFactory().getCalculator();

	doc->setHName( (const wxChar*)model.name );

  formatter->getDateIntsFromString( (const wxChar*)model.date, day, month, year );

  formatter->getDegreeIntsFromString( (const wxChar*)model.time, hour, minute, second );
  time = 3600 * hour + 60 * minute + second;
  time /= 3600;
  tz = myatof( model.tz );
	if ( model.tzew ) tz *= -1;
  dst = myatof( model.dst );
	time -= tz + dst;
	jd = calculator->calc_jd( year, month, day, time );
	doc->setDate( jd );

  formatter->getDegreeIntsFromString( (const wxChar*)model.longitude, deg, minute, second );
  longitude = 3600 * deg + 60 * minute + second;
  longitude /= 3600;
	if ( model.ew ) longitude *= -1;

  formatter->getDegreeIntsFromString( (const wxChar*)model.latitude, deg, minute, second );
  latitude = 3600 * deg + 60 * minute + second;
  latitude /= 3600;
	if ( model.ns ) latitude *= -1;

	doc->setLocation( (const wxChar*)model.locname, longitude, latitude, tz, dst );

	doc->setRemark( (const wxChar*)model.remark );
}

/*****************************************************
**
**   DataDialog   ---   OnCancel 
**
******************************************************/
void DataDialog::OnCancel( wxCommandEvent &event )
{
	EndModal( wxID_CANCEL );
}

/*****************************************************
**
**   DataDialog   ---   OnOK 
**
******************************************************/
void DataDialog::OnOK( wxCommandEvent &event )
{
	if ( doApply() )
	{
		EndModal( wxID_OK );
	}
}

/*****************************************************
**
**   DataDialog   ---   OnRevert 
**
******************************************************/
void DataDialog::OnRevert( wxCommandEvent &event )
{
	writeDoc2Model();
	writeModel2Gui();
}

/*****************************************************
**
**   DataDialog   ---   OnActual 
**
******************************************************/
void DataDialog::OnActual( wxCommandEvent &event )
{
	double jd, tmpjd, time;
	Formatter *formatter = Formatter::get();
	Location *loc = &config->defaultLocation;

	jd = JDate().getJD();
	tmpjd = jd + loc->getTimeZone() / 24 + loc->getDST() / 24;

	model.date = formatter->getDateStringFromJD( tmpjd );

	time = ( tmpjd - floor( tmpjd ) + .5 ) * 24;
	if ( time > 24 ) time -= 24;
	model.time = formatter->getTimeFormatted( time );

	writeModel2Gui();
}

/*****************************************************
**
**   DataDialog   ---   doApply 
**
******************************************************/
bool DataDialog::doApply()
{
	if ( ! Validate() ) return false;
	writeGui2Model();
	writeModel2Doc();
	doc->update();
	doc->updateAllChildWindows();
	doc->setDirty( true );
	return true;
}

/*****************************************************
**
**   DataDialog   ---   OnDefaultLocation 
**
******************************************************/
void DataDialog::OnDefaultLocation( wxCommandEvent &event )
{
	Formatter *formatter = Formatter::get();

	writeGui2Model();
	Location *loc = &config->defaultLocation;
	model.locname = loc->getLocName();

	model.longitude = formatter->getDegreesFormatted( fabs( loc->getLongitude() ));
	model.ew = ( loc->getLongitude() < 0 );
	model.latitude = formatter->getDegreesFormatted( fabs( loc->getLatitude() ));
	model.ns = ( loc->getLatitude() < 0 );
	model.tz = wxString::Format( wxT( "%1.1f" ), fabs( loc->getTimeZone() ));
	model.tzew = ( loc->getTimeZone() < 0 );
	model.dst = wxString::Format( wxT( "%1.1f" ), loc->getDST() );

	writeModel2Gui();
}

/*****************************************************
**
**   DataDialog   ---   OnSearchLocation 
**
******************************************************/
void DataDialog::OnSearchLocation( wxCommandEvent &event )
{
	Formatter *formatter = Formatter::get();

  LocationDialog dialog( this, 1 );
	if ( dialog.ShowModal() == wxID_OK )
	{
		writeGui2Model();

		Location *loc = dialog.getSelectedLocation();
		model.locname = loc->getLocName();
		model.longitude = formatter->getDegreesFormatted( fabs( loc->getLongitude() ));
		model.ew = ( loc->getLongitude() < 0 );
		model.latitude = formatter->getDegreesFormatted( fabs( loc->getLatitude() ));
		model.ns = ( loc->getLatitude() < 0 );
		model.tz = wxString::Format( wxT( "%1.1f" ), fabs( loc->getTimeZone() ));
		model.tzew = ( loc->getTimeZone() < 0 );
		model.dst = wxString::Format( wxT( "%1.1f" ), loc->getDST() );

		writeModel2Gui();
	}
}

/*****************************************************
**
**   DataDialog   ---   OnLocalTime 
**
******************************************************/
void DataDialog::OnLocalTime( wxCommandEvent &event )
{
	Formatter *formatter = Formatter::get();
	int deg, minute, second;
	double longitude;

	if ( !Validate() ) return;
	writeGui2Model();
  formatter->getDegreeIntsFromString( (const wxChar*)model.longitude, deg, minute, second );
  longitude = 3600 * deg + 60 * minute + second;
  longitude /= 3600;
	if ( model.ew ) longitude *= -1;
	model.tz = wxString::Format( wxT( "%1.6f" ), fabs( longitude/15 ));
	model.tzew = ( longitude < 0 );
	model.dst = wxString::Format( wxT( "%1.1f" ), 0.0 );
	writeModel2Gui();
}

/*****************************************************
**
**   DataDialog   ---   set_properties 
**
******************************************************/
void DataDialog::set_properties()
{
		text_date->SetValidator( DateValidator( &model.date ));
		//text_date->SetName( "Date" );
		text_time->SetValidator( DegreeValidator( &model.time ));
		//text_time->SetName( "Time" );
		text_longitude->SetValidator( DegreeValidator( &model.longitude ));
		//text_longitude->SetName( "Longitude" );
		text_latitude->SetValidator( DegreeValidator( &model.latitude ));
		//text_latitude->SetName( "Latitude" );
		text_tz->SetValidator( DoubleValidator( &model.tz ));
		//text_tz->SetName( "Time zone" );
		text_dst->SetValidator( DoubleValidator( &model.dst ));
		//text_dst->SetName( "DST" );

    // begin wxGlade: DataDialog::set_properties
    SetTitle(_("Birth Data"));
    text_name->SetMinSize(wxSize(300, 22));
    label_date->SetMinSize(wxSize(70, 20));
    text_date->SetMinSize(wxSize(120, -1));
    label_time->SetMinSize(wxSize(70, 20));
    text_time->SetMinSize(wxSize(120, -1));
    button_now->SetToolTip(_("Set current date"));
    label_locname->SetMinSize(wxSize(70, 20));
    label_long->SetMinSize(wxSize(70, 20));
    choice_ew->SetSelection(0);
    label_lat->SetMinSize(wxSize(70, 20));
    choice_ns->SetSelection(0);
    button_defloc->SetToolTip(_("Set default location"));
    button_searchloc->SetToolTip(_("Search location in database"));
    label_tz->SetMinSize(wxSize(70, 20));
    text_tz->SetMinSize(wxSize(100, 22));
    choice_tzew->SetSelection(0);
    label_dst->SetMinSize(wxSize(70, 20));
    text_dst->SetMinSize(wxSize(100, 22));
    button_localtime->SetToolTip(_("Set time zone for local time"));
    text_remark->SetMinSize(wxSize(300,80));
    button_ok->SetToolTip(_("Apply data and close dialog"));
    button_ok->SetDefault();
    button_cancel->SetToolTip(_("Cancel changes and close dialog"));
    button_apply->SetToolTip(_("Apply changes and recalculate chart with new data"));
    button_revert->SetToolTip(_("Revert changes"));
    // end wxGlade
}


/*****************************************************
**
**   DataDialog   ---   do_layout 
**
******************************************************/
void DataDialog::do_layout()
{
    // begin wxGlade: DataDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(4, 1, 0, 0);
    wxFlexGridSizer* sizer_buttons = new wxFlexGridSizer(1, 9, 0, 0);
    wxStaticBoxSizer* sizer_remark = new wxStaticBoxSizer(sizer_remark_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sizer_remark = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* grid_sizer_dt = new wxFlexGridSizer(3, 2, 3, 3);
    wxStaticBoxSizer* sizer_tz = new wxStaticBoxSizer(sizer_tz_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_tz = new wxFlexGridSizer(2, 4, 3, 3);
    wxBoxSizer* sizer_loc_action_copy = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_location = new wxStaticBoxSizer(sizer_location_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_lat = new wxFlexGridSizer(1, 4, 0, 0);
    wxFlexGridSizer* sizer_long = new wxFlexGridSizer(1, 4, 0, 0);
    wxFlexGridSizer* sizer_locname = new wxFlexGridSizer(1, 2, 0, 0);
    wxStaticBoxSizer* sizer_datetime = new wxStaticBoxSizer(sizer_datetime_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_datetime = new wxFlexGridSizer(2, 3, 3, 3);
    wxStaticBoxSizer* sizer_name = new wxStaticBoxSizer(sizer_name_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sizer_name = new wxFlexGridSizer(1, 1, 0, 0);
    grid_sizer_name->Add(text_name, 0, wxALL|wxEXPAND, 3);
    grid_sizer_name->AddGrowableCol(0);
    sizer_name->Add(grid_sizer_name, 1, wxEXPAND, 0);
    sizer_main->Add(sizer_name, 1, wxALL|wxEXPAND, 3);
    grid_datetime->Add(label_date, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(text_date, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(label_datefmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(label_time, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(text_time, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(label_timefmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_datetime->Add(grid_datetime, 1, wxALL|wxEXPAND, 3);
    grid_sizer_dt->Add(sizer_datetime, 1, wxALL|wxEXPAND, 3);
    grid_sizer_dt->Add(button_now, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_locname->Add(label_locname, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_locname->Add(text_locname, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_locname->AddGrowableCol(1);
    sizer_location->Add(sizer_locname, 1, wxALL|wxEXPAND, 3);
    sizer_long->Add(label_long, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_long->Add(text_longitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_long->Add(label_long_fmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_long->Add(choice_ew, 0, wxALL, 3);
    sizer_location->Add(sizer_long, 1, wxALL|wxEXPAND, 3);
    sizer_lat->Add(label_lat, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_lat->Add(text_latitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_lat->Add(label_lat_fmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_lat->Add(choice_ns, 0, wxALL, 3);
    sizer_location->Add(sizer_lat, 1, wxALL|wxEXPAND, 3);
    grid_sizer_dt->Add(sizer_location, 1, wxALL|wxEXPAND, 3);
    sizer_loc_action_copy->Add(button_defloc, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_loc_action_copy->Add(button_searchloc, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    grid_sizer_dt->Add(sizer_loc_action_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
    grid_tz->Add(label_tz, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(text_tz, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(label_tzfmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(choice_tzew, 0, wxALL, 3);
    grid_tz->Add(label_dst, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(text_dst, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(label_dstfmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(20, 20, 0, 0, 0);
    grid_tz->AddGrowableCol(3);
    sizer_tz->Add(grid_tz, 1, wxALL|wxEXPAND, 3);
    grid_sizer_dt->Add(sizer_tz, 1, wxALL|wxEXPAND, 3);
    grid_sizer_dt->Add(button_localtime, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_dt->AddGrowableCol(0);
    sizer_main->Add(grid_sizer_dt, 1, wxEXPAND, 0);
    grid_sizer_remark->Add(text_remark, 0, wxALL|wxEXPAND, 3);
    grid_sizer_remark->AddGrowableRow(0);
    grid_sizer_remark->AddGrowableCol(0);
    sizer_remark->Add(grid_sizer_remark, 1, 0, 0);
    sizer_main->Add(sizer_remark, 1, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_ok, 0, wxALL, 5);
    sizer_buttons->Add(button_cancel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sizer_buttons->Add(button_apply, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sizer_buttons->Add(button_revert, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sizer_buttons->AddGrowableCol(0);
    sizer_buttons->AddGrowableCol(1);
    sizer_buttons->AddGrowableCol(2);
    sizer_buttons->AddGrowableCol(3);
    sizer_main->Add(sizer_buttons, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(2);
    sizer_main->AddGrowableCol(0);
    Layout();
    Centre();
    // end wxGlade
}



