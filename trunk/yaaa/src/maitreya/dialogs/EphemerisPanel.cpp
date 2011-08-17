/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/EphemerisPanel.cpp
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
	#pragma implementation "EphemerisPanel.h"
#endif

#include "EphemerisPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dir.h>
#include <wx/dirdlg.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
    
#include "Conf.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"

extern Config *config;
enum { CD_SEPATH = wxID_HIGHEST + 1, CD_DEFAULT_CALC, CD_TIMEMODEL, CD_COORDINATES, CD_POSITIONS,
	CD_DEFAULTAYA, CD_AYA_CONSTANT, CD_T0, CD_AYAN_T0 };

IMPLEMENT_CLASS( EphemerisPanel, ConfigPanel )
   
/*****************************************************
**
**   EphemerisPanel   ---   Constructor 
**
******************************************************/
EphemerisPanel::EphemerisPanel( wxWindow* parent )
 : ConfigPanel( parent )
{
    // begin wxGlade: EphemerisPanel::EphemerisPanel
    sizer_ephem_path_staticbox = new wxStaticBox(this, -1, _("Path for Ephemeris Data Files"));
    sizer_calc_staticbox = new wxStaticBox(this, -1, _("Calculation"));
    sizer_sunrise_staticbox = new wxStaticBox(this, -1, _("Sunrise and Sunset"));
    sizer_aya_staticbox = new wxStaticBox(this, -1, _("Custom Ayanamsa"));
    label_custom_aya_t0 = new wxStaticText(this, wxID_ANY, _("Date t0"));
    text_t0 = new wxTextCtrl(this, CD_T0, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    text_t0_formatted = new wxStaticText(this, wxID_ANY, _("0000-00-00 00:00:00"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    label_custom_aya_ayat0 = new wxStaticText(this, wxID_ANY, _("Ayanamsa at t0"));
    text_ayan_t0 = new wxTextCtrl(this, CD_AYAN_T0, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    text_ayan_t0_formatted = new wxStaticText(this, wxID_ANY, _("00:00:00.000"));
    check_period = new wxCheckBox(this, CD_AYA_CONSTANT, _("Constant Period"));
    text_period = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    defaya_button = new wxButton(this, CD_DEFAULTAYA, _("Restore Defaults"));
    button_sepath = new wxButton(this, CD_SEPATH, _("Choose ..."));
    text_sepath = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_timemodel = new wxStaticText(this, wxID_ANY, _("Time Model"));
    const wxString choice_timemodel_choices[] = {
        _("Universal"),
        _("Ephemeris")
    };
    choice_timemodel = new wxChoice(this, CD_TIMEMODEL, wxDefaultPosition, wxDefaultSize, 2, choice_timemodel_choices, 0);
    label_coord = new wxStaticText(this, wxID_ANY, _("Coordinates"));
    const wxString choice_coord_choices[] = {
        _("Geocentric"),
        _("Topocentric"),
        _("Equatorial"),
        _("Barycentric"),
        _("Heliocentric")
    };
    choice_coord = new wxChoice(this, CD_COORDINATES, wxDefaultPosition, wxDefaultSize, 5, choice_coord_choices, 0);
    label_pos = new wxStaticText(this, wxID_ANY, _("Positions"));
    const wxString choice_pos_choices[] = {
        _("Apparent"),
        _("True")
    };
    choice_pos = new wxChoice(this, CD_POSITIONS, wxDefaultPosition, wxDefaultSize, 2, choice_pos_choices, 0);
    button_defcalc = new wxButton(this, CD_DEFAULT_CALC, _("Restore Defaults"));
    label_sunrise_disk = new wxStaticText(this, wxID_ANY, _("Reference Point"));
    const wxString choice_sunrise_disk_choices[] = {
        _("Center of Disk"),
        _("Edge of Disk")
    };
    choice_sunrise_disk = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_sunrise_disk_choices, 0);
    check_sunrise_refrac = new wxCheckBox(this, wxID_ANY, _("Use Refraction"));

    set_properties();
    do_layout();
    // end wxGlade

  Connect( CD_SEPATH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EphemerisPanel::OnSelectSEPath ));
  Connect( CD_DEFAULT_CALC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EphemerisPanel::OnDefaultCalculation ));
  Connect( CD_DEFAULTAYA, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EphemerisPanel::OnDefaultAyanamsa ));

  Connect( CD_TIMEMODEL, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemerisPanel::OnChangeConfigParams ));
  Connect( CD_COORDINATES, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemerisPanel::OnChangeConfigParams ));
  Connect( CD_POSITIONS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemerisPanel::OnChangeConfigParams ));

  Connect( CD_AYA_CONSTANT, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EphemerisPanel::OnCheckConstantAyanamsa ));

  Connect( CD_T0, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( EphemerisPanel::OnTextChanged ));
  Connect( CD_AYAN_T0, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( EphemerisPanel::OnTextChanged ));
}

/*****************************************************
**
**   EphemerisPanel   ---   set_properties 
**
******************************************************/
void EphemerisPanel::set_properties()
{
    // begin wxGlade: EphemerisPanel::set_properties
    text_t0->SetMinSize(wxSize(-1, -1));
    text_ayan_t0->SetMinSize(wxSize(100, -1));
    text_period->SetMinSize(wxSize(100, 23));
    text_sepath->SetToolTip(_("Only required for high precision calculation"));
    choice_timemodel->SetSelection(0);
    choice_coord->SetSelection(0);
    choice_pos->SetSelection(0);
    button_defcalc->SetToolTip(_("Restore default settings"));
    choice_sunrise_disk->SetSelection(0);
    check_sunrise_refrac->SetToolTip(_("Checked: apparent position will be used"));
    // end wxGlade
}

/*****************************************************
**
**   EphemerisPanel   ---   do_layout 
**
******************************************************/
void EphemerisPanel::do_layout()
{
    // begin wxGlade: EphemerisPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_sunrise = new wxStaticBoxSizer(sizer_sunrise_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_sunrise_disk = new wxFlexGridSizer(1, 2, 3, 3);
    wxStaticBoxSizer* sizer_calc = new wxStaticBoxSizer(sizer_calc_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_calc = new wxFlexGridSizer(3, 2, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_ephem_path = new wxStaticBoxSizer(sizer_ephem_path_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_aya = new wxStaticBoxSizer(sizer_aya_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(5, 2, 3, 3);
    grid_sizer_1->Add(label_custom_aya_t0, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(text_t0, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(20, 20, 0, wxEXPAND, 0);
    grid_sizer_1->Add(text_t0_formatted, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(label_custom_aya_ayat0, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(text_ayan_t0, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(20, 20, 0, wxEXPAND, 0);
    grid_sizer_1->Add(text_ayan_t0_formatted, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(check_period, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(text_period, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_aya->Add(grid_sizer_1, 1, wxEXPAND, 0);
    sizer_aya->Add(defaya_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_left->Add(sizer_aya, 1, wxALL, 3);
    sizer_ephem_path->Add(button_sepath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sizer_ephem_path->Add(text_sepath, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
    sizer_left->Add(sizer_ephem_path, 1, wxALL|wxEXPAND, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_sizer_calc->Add(label_timemodel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_calc->Add(choice_timemodel, 0, wxALL|wxEXPAND, 3);
    grid_sizer_calc->Add(label_coord, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_calc->Add(choice_coord, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_calc->Add(label_pos, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_calc->Add(choice_pos, 0, wxALL|wxEXPAND, 3);
    grid_sizer_calc->AddGrowableCol(1);
    sizer_calc->Add(grid_sizer_calc, 1, wxALL|wxEXPAND, 3);
    sizer_calc->Add(button_defcalc, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_right->Add(sizer_calc, 1, wxALL|wxEXPAND, 3);
    sizer_sunrise_disk->Add(label_sunrise_disk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_sunrise_disk->Add(choice_sunrise_disk, 0, wxEXPAND, 0);
    sizer_sunrise_disk->AddGrowableCol(0);
    sizer_sunrise->Add(sizer_sunrise_disk, 1, 0, 3);
    sizer_sunrise->Add(check_sunrise_refrac, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_sunrise, 1, wxALL, 3);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   EphemerisPanel   ---   setData 
**
******************************************************/
void EphemerisPanel::setData()
{
  text_t0->SetValue( printfDouble( config->custom_t0 ) );
  text_ayan_t0->SetValue( printfDouble( config->custom_ayan_t0 ) );

  check_period->SetValue( config->custom_aya_constant );
  text_period->SetValue( printfDouble( config->custom_aya_period ) );
  text_period->Enable( config->custom_aya_constant );

  choice_sunrise_disk->SetSelection( config->sunrise_def );
  check_sunrise_refrac->SetValue( config->sunrise_refrac );
  text_sepath->SetValue( config->sePath );
  choice_timemodel->SetSelection( config->useEphemerisTime );

	choice_coord->SetSelection( config->ephemCoordinateSystem );
  choice_pos->SetSelection( config->useTruePositions );

	updateFormattedDateFields();
}

/*****************************************************
**
**   EphemerisPanel   ---   saveData 
**
******************************************************/
bool EphemerisPanel::saveData()
{
	config->custom_t0 = myatof( text_t0->GetValue() );
  config->custom_ayan_t0 = myatof( text_ayan_t0->GetValue() );
  config->custom_aya_constant = check_period->GetValue();
  config->custom_aya_period = myatof( text_period->GetValue() );

  config->sunrise_def = choice_sunrise_disk->GetSelection();
  config->sunrise_refrac = check_sunrise_refrac->GetValue();

  config->sePath = text_sepath->GetValue();
	config->useEphemerisTime = choice_timemodel->GetSelection();
	config->ephemCoordinateSystem = choice_coord->GetSelection();
	assert( config->ephemCoordinateSystem >= 0 && config->ephemCoordinateSystem <= EPHEM_COORDINATES_HELIOCENTRIC );

	config->useTruePositions = choice_pos->GetSelection();

	return true;
}

/*****************************************************
**
**   EphemerisPanel   ---   OnSelectSEPath 
**
******************************************************/
void EphemerisPanel::OnSelectSEPath( wxCommandEvent &event )
{
  wxDirDialog dialog( this, _( "Choose a directory" ), text_sepath->GetValue());
  if ( dialog.ShowModal() == wxID_OK )
  {
    text_sepath->SetValue( dialog.GetPath());
  }
}  

/*****************************************************
**
**   EphemerisPanel   ---   OnTextChanged 
**
******************************************************/
void EphemerisPanel::OnTextChanged( wxCommandEvent &event )
{
	updateFormattedDateFields();
}

/*****************************************************
**
**   EphemerisPanel   ---   updateFormattedDateFields 
**
******************************************************/
void EphemerisPanel::updateFormattedDateFields()
{
	Formatter *formatter = Formatter::get();
	if ( text_t0 != 0 && ! text_t0->GetValue().IsEmpty())
	{
		double custom_t0 = myatof( text_t0->GetValue() );
		wxString t0 = formatter->getFullDateStringFromJD( custom_t0 );
		if ( text_t0_formatted ) text_t0_formatted->SetLabel( t0 );
	}
	if ( text_ayan_t0  != 0 && ! text_ayan_t0->GetValue().IsEmpty())
	{
		double custom_ayan_t0 = myatof( text_ayan_t0->GetValue() );
		wxString ayan_t0 = formatter->getLenFormatted( custom_ayan_t0, DEG_PRECISION_MORE );
		if ( text_ayan_t0_formatted ) text_ayan_t0_formatted->SetLabel( ayan_t0 );
	}
}

/*****************************************************
**
**   EphemerisPanel   ---   OnDefaultAyanamsa 
**
******************************************************/
void EphemerisPanel::OnDefaultAyanamsa( wxCommandEvent &event )
{
  text_t0->SetValue( printfDouble( DEFAULT_T0 ) );
  text_ayan_t0->SetValue( printfDouble( DEFAULT_AYAN_T0 ));

  check_period->SetValue( false );
  text_period->SetValue( printfDouble( DEFAULT_AYA_PERIOD ));
  text_period->Enable( false );
}
    
/*****************************************************
**
**   EphemerisPanel   ---   OnCheckConstantAyanamsa 
**
******************************************************/
void EphemerisPanel::OnCheckConstantAyanamsa( wxCommandEvent &event )
{
  text_period->Enable( check_period->GetValue() );
}

/*****************************************************
**
**   EphemerisPanel   ---   OnDefaultCalculation 
**
******************************************************/
void EphemerisPanel::OnDefaultCalculation( wxCommandEvent &event )
{
	choice_timemodel->SetSelection( 0 );
	choice_coord->SetSelection( 0 );
	choice_pos->SetSelection(  0 );
}

/*****************************************************
**
**   EphemerisPanel   ---   OnChangeConfigParams
**
******************************************************/
void EphemerisPanel::OnChangeConfigParams( wxCommandEvent &event )
{
  static bool alreadySayThat = false;
	if ( alreadySayThat == false )
	{
		doMessageBox( this,
			_( "Warning: changes in calculation configuration may result in wrong calculation results!"), wxICON_EXCLAMATION );
		alreadySayThat = true;
	}
}

