/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ObjectColorPanel.cpp
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
	#pragma implementation "ObjectColorPanel.h"
#endif

#include "ObjectColorPanel.h"

#include <wx/button.h>
#include <wx/colour.h>
#include <wx/colordlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "ColorConfig.h"
#include "Conf.h"
#include "FontConfig.h"
#include "Lang.h"

extern Config *config;

IMPLEMENT_CLASS( ObjectColorPanel, ConfigPanel )

enum { CD_SET_DEFAULTS = wxID_HIGHEST, CD_BUTTON_OBJECT = wxID_HIGHEST + 100, CD_BUTTON_SIGN = wxID_HIGHEST + 200,
	CD_BUTTON_HOUSE = wxID_HIGHEST + 300 };

/*****************************************************
**
**   ObjectColorPanel   ---   Constructor 
**
******************************************************/
ObjectColorPanel::ObjectColorPanel( wxWindow* parent )
 : ConfigPanel( parent )
{
	int i;
	wxString s, tooltip;
	Lang lang;
	FontConfig *f = FontConfig::get();

	sizer_signcolors_staticbox = new wxStaticBox( this, -1, _( "Sign Colors" ));
	sizer_housecolors_staticbox = new wxStaticBox( this, -1, _( "House Colors" ));
	sizer_objectcolors_staticbox = new wxStaticBox( this, -1, _( "Object Colors" ));
	
	for( i = 0; i < 12; i++ )
	{
		button_signs[i] = new wxButton(this, CD_BUTTON_SIGN + i, lang.getSignSymbolCode( i ));
		button_signs[i]->SetFont( *f->getSymbolFont());
		button_signs[i]->SetToolTip( lang.getSignName( i ));
	  button_signs[i]->SetMinSize(wxSize( 32, 32 ));
		button_signs[i]->SetBackgroundColour( config->colors->bgColor );
		button_signs[i]->SetForegroundColour( config->colors->chart.signFgColor[i] );

		s.Printf( wxT( "%d" ), i+1 );
		button_houses[i] = new wxButton(this, CD_BUTTON_HOUSE + i, s );
		button_houses[i]->SetFont( *f->getGraphicFont());
		button_houses[i]->SetMinSize(wxSize( 32, 32 ));
		button_houses[i]->SetBackgroundColour( config->colors->bgColor );
		button_houses[i]->SetForegroundColour( config->colors->chart.houseFgColor[i] );
		
		s.Printf( wxT( "%s %d" ), _( "House" ), i+1 );
		button_houses[i]->SetToolTip( s );
	}
	for( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		button_objects[i] = new wxButton(this, CD_BUTTON_OBJECT + i );
		s = lang.getPlanetSymbolCode( i );
		if ( s.IsEmpty())
		{
			button_objects[i]->SetFont( *f->getGraphicFont());
			s = lang.getObjectName( i, TSHORT );
		}
		else
		{
			button_objects[i]->SetFont( *f->getSymbolFont());
		}
		button_objects[i]->SetLabel( s );

		if ( i == OMEANNODE || i == OMEANDESCNODE ) tooltip = _( "Mean Node" );
		else if ( i == OTRUENODE || i == OTRUEDESCNODE ) tooltip = _( "True Node" );
		else tooltip = lang.getObjectName( i, TLARGE );
		button_objects[i]->SetToolTip( tooltip );
		button_objects[i]->SetMinSize( wxSize(32, 32 ));
		button_objects[i]->SetBackgroundColour( config->colors->bgColor );
		button_objects[i]->SetForegroundColour( config->colors->chart.planetFgColor[i] );
	}
	button_default = new wxButton(this, CD_SET_DEFAULTS, _("Restore Defaults"));
  button_default->SetToolTip(_("Restore default settings"));
  do_layout();

  Connect( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectColorPanel::OnButton ));
}

/*****************************************************
**
**   ObjectColorPanel   ---   setData 
**
******************************************************/
void ObjectColorPanel::setData()
{
	// Nothing to do
}

/*****************************************************
**
**   ObjectColorPanel   ---   saveData 
***
******************************************************/
bool ObjectColorPanel::saveData()
{
	int i;
	for( i = 0; i < 12; i++ )
	{
		config->colors->chart.signFgColor[i] =	button_signs[i]->GetForegroundColour();
		config->colors->chart.houseFgColor[i] =	button_houses[i]->GetForegroundColour();
	}
	for( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) config->colors->chart.planetFgColor[i] =	button_objects[i]->GetForegroundColour();
	return true;
}

/*****************************************************
**
**   ObjectColorPanel   ---   setDefaultColors
**
******************************************************/
void ObjectColorPanel::setDefaultColors()
{
	int i;
	ColorConfig cfg;
	for( i = 0; i < 12; i++ )
	{
		button_signs[i]->SetForegroundColour( cfg.chart.signFgColor[i] );
		button_houses[i]->SetForegroundColour( cfg.chart.houseFgColor[i] );
	}
	for( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) button_objects[i]->SetForegroundColour( cfg.chart.planetFgColor[i] );

}

/*****************************************************
**
**   ObjectColorPanel   ---   OnButton
**
******************************************************/
void ObjectColorPanel::OnButton( wxCommandEvent &event )
{
	if ( event.GetId() == CD_SET_DEFAULTS ) { setDefaultColors(); }
	else
	{
		wxColourData data;
		wxButton *theButton = (wxButton*)event.GetEventObject();
		data.SetColour( theButton->GetForegroundColour());
		wxColourDialog dialog( this, &data );
		dialog.ShowModal() == wxID_OK && theButton->SetForegroundColour( dialog.GetColourData().GetColour());
	}
}

/*****************************************************
**
**   ObjectColorPanel   ---   do_layout
**
******************************************************/
void ObjectColorPanel::do_layout()
{
	int i;
	wxFlexGridSizer* sizer_main = new wxFlexGridSizer( 4, 1, 3, 3 );
	wxStaticBoxSizer* sizer_housecolors = new wxStaticBoxSizer( sizer_housecolors_staticbox, wxVERTICAL );
	wxFlexGridSizer* grid_housecolors = new wxFlexGridSizer( 1, 12, 3, 3 );
	wxStaticBoxSizer* sizer_signcolors = new wxStaticBoxSizer( sizer_signcolors_staticbox, wxVERTICAL );
	wxFlexGridSizer* grid_signcolors = new wxFlexGridSizer( 1, 12, 3, 3 );
	wxStaticBoxSizer* sizer_objectcolors = new wxStaticBoxSizer( sizer_objectcolors_staticbox, wxVERTICAL );

	wxFlexGridSizer* grid_objectcolors = new wxFlexGridSizer( 4, 12, 3, 3);
	for( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) grid_objectcolors->Add( button_objects[i], 0, wxALL, 3 );

	sizer_objectcolors->Add(grid_objectcolors, 1, wxEXPAND, 0 );
	sizer_main->Add( sizer_objectcolors, 1, wxALL, 3 );
	for( i = 0; i < 12; i++ ) grid_signcolors->Add( button_signs[i], 0, wxALL, 3 );
	sizer_signcolors->Add( grid_signcolors, 1, wxEXPAND, 0 );
	sizer_main->Add( sizer_signcolors, 1, wxALL, 3 );

	for( i = 0; i < 12; i++ ) grid_housecolors->Add( button_houses[i], 0, wxALL, 3 );
	sizer_housecolors->Add( grid_housecolors, 1, wxEXPAND, 0 );
	sizer_main->Add( sizer_housecolors, 1, wxALL, 3 );
  sizer_main->Add(button_default, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
	SetSizer( sizer_main );
	sizer_main->Fit( this );
	sizer_main->AddGrowableRow( 2 );
	sizer_main->AddGrowableCol( 0 );
}

