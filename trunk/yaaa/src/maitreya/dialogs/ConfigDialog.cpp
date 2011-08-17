/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigDialog.cpp
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
	#pragma implementation "ConfigDialog.h"
#endif

#include "ConfigDialog.h"

#include <wx/app.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/treebook.h>

#include "Conf.h"
#include "IconProvider.h"

#include "AnimationPanel.h"
#include "AspectPanel.h"
#include "AspectColorPanel.h"
#include "ColorPanel.h"
#include "DasaColorPanel.h"
#include "DefaultLocationPanel.h"
#include "EphemerisPanel.h"
#include "FontPanel.h"
#include "GeneralPanel.h"
#include "guibase.h"
#include "MultipleViewPanel.h"
#include "ObjectColorPanel.h"
#include "PrintoutPanel.h"
#include "ToolbarPanel.h"
#include "VargaPanel.h"
#include "VedicPanel.h"
#include "VedicCalculationPanel.h"
#include "VedicChartPanel.h"
#include "ViewPanel.h"
#include "WesternPanel.h"
#include "WesternCalculationPanel.h"
#include "WesternChartPanel.h"

enum { CONFIG_NOTEBOOK = wxID_HIGHEST + 3000 };

IMPLEMENT_CLASS( ConfigDialog, wxDialog )

DEFINE_EVENT_TYPE( CONFIG_CHANGED )
DEFINE_EVENT_TYPE( CONFIG_TOOLBAR_CHANGED )

extern Config *config;

// Hard wired for toolbar update on config change
#define TOOLBAR_PANEL_INDEX 12

/*****************************************************
**
**   ConfigDialog   ---   Constructor 
**
******************************************************/
ConfigDialog::ConfigDialog(wxWindow* parent )
  :  wxDialog( parent, -1, _( "Configuration" ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
	const static int page_types[NB_PANELS] = {
		1, 0, 0, 0, 0,	        // General
		1, 0, 0, 0, 0, 0, 0, 0, // View
		1, 0, 0, 0,             // Vedic
		1, 0, 0, 0              // Western
	};
	const wxChar *notebook_title[NB_PANELS] =
	{ 
		// General section
		_("General"),	_("Default Location"), _( "Print" ), _( "Ephemeris" ), _("Animation"),
		
		// View section
		_("View"), _( "Colors" ), _( "Object Colors" ), _( "Dasa Colors" ), _( "Aspect Display" ), _( "Fonts"), _( "Multiple Views" ), _( "Toolbar" ),
		
		// Vedic section
		_( "Vedic Astrology" ), _("Chart"), _("Calculation"), _( "Vargas and Dasas" ),

		// Western section
		_( "Western Astrology" ), _("Chart"), _("Calculation"),  _("Aspects")
	};

	notebook = new wxTreebook( this, CONFIG_NOTEBOOK );
	for( int i = 0; i < NB_PANELS; i++ )
	{
		configpanel[i] = 0;
		panel[i] = new wxPanel( notebook, -1 );
		page_types[i] ? 
			notebook->AddPage( panel[i], notebook_title[i] )
		: notebook->AddSubPage( panel[i], notebook_title[i] );
	}
	static_line_bottom = new wxStaticLine(this, -1);
	okbutton = new wxButton(this, wxID_OK, _("OK"));
	applybutton = new wxButton(this, wxID_APPLY, _("Apply"));
	cancelbutton = new wxButton(this, wxID_CANCEL, _("Cancel"));

	wxBoxSizer* sizer_main = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* buttonsizer = new wxBoxSizer( wxHORIZONTAL );

	sizer_main->Add(notebook, 1, wxALL|wxEXPAND, 3);
	sizer_main->Add(static_line_bottom, 0, wxEXPAND, 0);
	buttonsizer->Add(okbutton, 0, wxALL, 3);
	buttonsizer->Add(applybutton, 0, wxALL, 3);
	buttonsizer->Add(cancelbutton, 0, wxALL, 3);
	sizer_main->Add(buttonsizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxFIXED_MINSIZE , 3);
	SetAutoLayout( true );
	SetSizer( sizer_main );
	Layout();
	
	SetIcon( IconProvider::getIcon( BITMAP_CONFIG ));
	SetSize( config->xConfigDialog, config->yConfigDialog );
	moveWindow2Center( this );

  Connect( wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigDialog::OnApply ));
  Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigDialog::OnOK ));
  Connect( wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigDialog::OnCancel ));
#ifdef _WX29
  Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler( ConfigDialog::OnTreebook ));
#else
  Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED, wxTreebookEventHandler( ConfigDialog::OnTreebook ));
#endif
	notebook->SetSelection( config->configDialogActivePage );
}

/*****************************************************
**
**   ConfigDialog   ---   Destructor
**
******************************************************/
ConfigDialog::~ConfigDialog()
{
	config->configDialogActivePage = notebook->GetSelection();
	config->xConfigDialog = GetSize().GetWidth();
	config->yConfigDialog = GetSize().GetHeight();
}

/*****************************************************
**
**   ConfigDialog   ---   saveData 
**
******************************************************/
void ConfigDialog::saveData()
{
	for( int i = 0; i < NB_PANELS; i++ )
	{
		if ( configpanel[i] && configpanel[i]->saveData() &&  i == TOOLBAR_PANEL_INDEX )
		{
			wxCommandEvent e( CONFIG_TOOLBAR_CHANGED, GetId() );
			wxPostEvent( GetParent(), e );
		}
	}
}

/*****************************************************
**
**   ConfigDialog   ---   showPanel
**
******************************************************/
void ConfigDialog::showPanel( const int &sel )
{
	assert( sel >= 0 && sel < NB_PANELS );
	if ( ! configpanel[sel] )
	{
		panel[sel]->Show( false );
		switch( sel )
		{
			case 0:
				configpanel[sel] = new GeneralPanel( panel[sel] );
			break;
			case 1:
				configpanel[sel] = new DefaultLocationPanel( panel[sel] );
			break;
			case 2:
				configpanel[sel] = new PrintoutPanel( panel[sel] );
			break;
			case 3:
				configpanel[sel] = new EphemerisPanel( panel[sel] );
			break;
			case 4:
				configpanel[sel] = new AnimationPanel( panel[sel] );
			break;
			case 5:
				configpanel[sel] = new ViewPanel( panel[sel] );
			break;
			case 6:
				configpanel[sel] = new ColorPanel( panel[sel] );
			break;
			case 7:
				configpanel[sel] = new ObjectColorPanel( panel[sel] );
			break;
			case 8:
				configpanel[sel] = new DasaColorPanel( panel[sel] );
			break;
			case 9:
				configpanel[sel] = new AspectColorPanel( panel[sel] );
			break;
			case 10:
				configpanel[sel] = new FontPanel( panel[sel] );
			break;
			case 11:
				configpanel[sel] = new MultipleViewPanel( panel[sel] );
			break;
			case 12:
				configpanel[sel] = new ToolbarPanel( panel[sel] );
			break;
			case 13:
				configpanel[sel] = new VedicPanel( panel[sel] );
			break;
			case 14:
				configpanel[sel] = new VedicChartPanel( panel[sel] );
			break;
			case 15:
				configpanel[sel] = new VedicCalculationPanel( panel[sel] );
			break;
			case 16:
				configpanel[sel] = new VargaPanel( panel[sel] );
			break;
			case 17:
				configpanel[sel] = new WesternPanel( panel[sel] );
			break;
			case 18:
				configpanel[sel] = new WesternChartPanel( panel[sel] );
			break;
			case 19:
				configpanel[sel] = new WesternCalculationPanel( panel[sel] );
			break;
			case 20:
				configpanel[sel] = new AspectPanel( panel[sel] );
			break;
			default:
				assert( false );
			break;
		}
		configpanel[sel]->setData();
		//panel[sel]->SetBackgroundColour( *wxRED );
    wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 1, 0, 0 );
		sizer->AddGrowableCol( 0 );
		sizer->AddGrowableRow( 0 );
    sizer->Add( configpanel[sel], 1, wxEXPAND, 0 );
    panel[sel]->SetSizer(sizer);
		panel[sel]->Layout();
		panel[sel]->Show( true );
	}
	configpanel[sel]->onActivate();
	configpanel[sel]->SetFocus();
}

/*****************************************************
**
**   ConfigDialog   ---   OnApply 
**
******************************************************/
void ConfigDialog::OnApply( wxCommandEvent &event )
{
	saveData();
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   ConfigDialog   ---   OnTreebook
**
******************************************************/
/*#ifdef _WX29
void ConfigDialog::OnTreebook( wxBookCtrlEvent &event )
#else
void ConfigDialog::OnTreebook( wxTreebookEvent &event )
#endif
*/
void ConfigDialog::OnTreebook( wxTreebookEvent &event )
{
	showPanel( event.GetSelection());
	if ( event.GetOldSelection() != -1 && configpanel[event.GetOldSelection()] != 0 )
		configpanel[event.GetOldSelection()]->onPassivate();
}

/*****************************************************
**
**   ConfigDialog   ---   OnSize 
**
******************************************************/
void ConfigDialog::OnSize( wxSizeEvent &event )
{
	config->xConfigDialog = event.GetSize().GetWidth();
	config->yConfigDialog = event.GetSize().GetHeight();
	event.Skip();
}

/*****************************************************
**
**   ConfigDialog   ---   OnOK 
**
******************************************************/
void ConfigDialog::OnOK( wxCommandEvent &event )
{
	saveData();
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
	EndModal( wxID_OK );
}

/*****************************************************
**
**   ConfigDialog   ---   OnCancel 
**
******************************************************/
void ConfigDialog::OnCancel( wxCommandEvent &event )
{
	EndModal( wxID_CANCEL );
}


