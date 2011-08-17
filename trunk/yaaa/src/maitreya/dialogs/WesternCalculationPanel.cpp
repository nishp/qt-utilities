/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/WesternCalculationPanel.cpp
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
	#pragma implementation "WesternCalculationPanel.h"
#endif

#include "WesternCalculationPanel.h"

#include "Conf.h"
#include "DialogElements.h"
#include "mathbase.h"

#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

enum { CD_YL_CHOICE = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( WesternCalculationPanel, ConfigPanel )

/*****************************************************
**
**   WesternCalculationPanel   ---   Constructor 
**
******************************************************/
WesternCalculationPanel::WesternCalculationPanel( wxWindow* parent )
 : ConfigPanel( parent )
{
    // begin wxGlade: WesternCalculationPanel::WesternCalculationPanel
    sizer_yl_staticbox = new wxStaticBox(this, -1, _("Year Length"));
    sizer_wcalc_staticbox = new wxStaticBox(this, -1, _("Calculation Options"));
    label_wcalc_aya = new wxStaticText(this, wxID_ANY, _("Ayanamsa"));
    choice_waya = new AyanamsaChoice(this, wxID_ANY, config->wAyanamsa);
    label_wcalc_node = new wxStaticText(this, wxID_ANY, _("Lunar Node"));
    const wxString choice_wnode_choices[] = {
        _("True"),
        _("Mean")
    };
    choice_wnode = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_wnode_choices, 0);
    label_wcalc_house = new wxStaticText(this, wxID_ANY, _("House System"));
    const wxString choice_whouse_choices[] = {
        _("Placidus"),
        _("Koch"),
        _("Regiomontanus"),
        _("Campanus"),
        _("Porphyry"),
        _("Equal"),
        _("Vehlow Equal"),
        _("Axial Rotation"),
        _("Azimutal/Horizontal"),
        _("Polich/Page"),
        _("Alcabitus"),
        _("Morinus"),
        _("Krusinski")
    };
    choice_whouse = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 13, choice_whouse_choices, 0);
    choice_yl = new YearLengthChoice(this, CD_YL_CHOICE, false);
    text_custom_yl = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

  Connect( CD_YL_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WesternCalculationPanel::OnYlChoice ));
}

/*****************************************************
**
**   WesternCalculationPanel   ---   setData 
**
******************************************************/
void WesternCalculationPanel::setData()
{
  choice_whouse->SetSelection( config->wHouseSystem ? config->wHouseSystem -1 : 0 );
  choice_wnode->SetSelection( config->wLunarNodeMode );
	// Ayanamsa not needed

	choice_yl->SetSelection( config->wYearLength );
	text_custom_yl->SetValue( printfDouble( config->wCustomYearLength ));
	text_custom_yl->Enable( choice_yl->GetSelection() == 4 );
}

/*****************************************************
**
**   WesternCalculationPanel   ---   saveData 
**
******************************************************/
bool WesternCalculationPanel::saveData()
{
  config->wHouseSystem = choice_whouse->GetSelection() + 1;
  config->wLunarNodeMode = choice_wnode->GetSelection();
	config->wAyanamsa = choice_waya->getConfigIndex();

  config->wYearLength = choice_yl->GetSelection();
	config->wCustomYearLength = myatof( text_custom_yl->GetValue() );
	if ( config->wCustomYearLength < 0 ) config->wCustomYearLength = WCUSTOM_YEAR_LEN;

	return true;
}
/*****************************************************
**
**   WesternCalculationPanel   ---   OnYlChoice
**
******************************************************/
void WesternCalculationPanel::OnYlChoice( wxCommandEvent &event )
{
	text_custom_yl->Enable( choice_yl->GetSelection() == 4 );
}

/*****************************************************
**
**   WesternCalculationPanel   ---   set_properties 
**
******************************************************/
void WesternCalculationPanel::set_properties()
{
    // begin wxGlade: WesternCalculationPanel::set_properties
    choice_waya->SetToolTip(_("Custom Ayanamsa parameters can be configured under General->Ephemeris"));
    choice_wnode->SetToolTip(_("Western astrologers most likely use True Node"));
    choice_wnode->SetSelection(0);
    choice_whouse->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   WesternCalculationPanel   ---   do_layout 
**
******************************************************/
void WesternCalculationPanel::do_layout()
{
    // begin wxGlade: WesternCalculationPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_yl = new wxStaticBoxSizer(sizer_yl_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_yl = new wxFlexGridSizer(5, 2, 3, 3);
    wxStaticBoxSizer* sizer_wcalc = new wxStaticBoxSizer(sizer_wcalc_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_calcoptions = new wxFlexGridSizer(3, 2, 3, 3);
    grid_calcoptions->Add(label_wcalc_aya, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_calcoptions->Add(choice_waya, 1, wxEXPAND, 0);
    grid_calcoptions->Add(label_wcalc_node, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_calcoptions->Add(choice_wnode, 0, wxEXPAND, 0);
    grid_calcoptions->Add(label_wcalc_house, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_calcoptions->Add(choice_whouse, 0, wxEXPAND, 0);
    grid_calcoptions->AddGrowableCol(1);
    sizer_wcalc->Add(grid_calcoptions, 1, wxALL, 3);
    sizer_right->Add(sizer_wcalc, 1, wxALL, 3);
    wxStaticText* label_yl = new wxStaticText(this, wxID_ANY, _("Year Length"));
    grid_yl->Add(label_yl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(choice_yl, 1, wxALL, 3);
    wxStaticText* label_custom_yl = new wxStaticText(this, wxID_ANY, _("Custom Length"));
    grid_yl->Add(label_custom_yl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(text_custom_yl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->AddGrowableCol(1);
    sizer_yl->Add(grid_yl, 1, 0, 0);
    sizer_right->Add(sizer_yl, 1, wxALL, 3);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

