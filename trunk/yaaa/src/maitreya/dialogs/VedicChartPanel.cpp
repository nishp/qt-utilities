/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VedicChartPanel.cpp
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
	#pragma implementation "VedicChartPanel.h"
#endif

#include "VedicChartPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/cmndata.h>
#include <wx/choice.h>
#include <wx/colour.h>
#include <wx/colordlg.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>


#include "ChartProperties.h"
#include "Conf.h"
#include "guibase.h"
#include "Horoscope.h"
#include "Lang.h"
#include "Painter.h"
#include "SarvatobhadraChart.h"
#include "Varga.h"
#include "VedicChart.h"
#include "VedicChartConfig.h"
#include "Writer.h"

extern Config *config;

enum { CD_SKIN_SELECT = wxID_HIGHEST + 1, CD_USE_COLORS, CD_SELECT_SNE, CD_SHOW_ARUDHAS, CD_SHOW_RETRO, CD_SHOW_CENTER, CD_SELECT_NORTH,
	CD_SHOW_SANSKRIT_SYMBOLS, CD_SHOW_AFFLICTIONS };

IMPLEMENT_CLASS( VedicChartPanel, wxPanel )
IMPLEMENT_CLASS( VedicChartPreviewPanel, wxPanel )

/*****************************************************
**
**   VedicChartPreviewPanel   ---   Constructor 
**
******************************************************/
VedicChartPreviewPanel::VedicChartPreviewPanel( wxWindow* parent, const ChartProperties *chartprops, const wxPoint& pos, const wxSize& size, long style )
	: wxPanel( parent, -1, pos, size, wxFULL_REPAINT_ON_RESIZE ),
	chartprops( chartprops )
{
	mode = 0;
	horoscope = new Horoscope();
	horoscope->update();
	chart = 0;
	recreateChart( mode );

  Connect( wxEVT_PAINT, wxPaintEventHandler( VedicChartPreviewPanel::OnPaint ));
}

/*****************************************************
**
**   VedicChartPreviewPanel   ---   Destructor 
**
******************************************************/
VedicChartPreviewPanel::~VedicChartPreviewPanel()
{
	if ( chart ) delete chart;
	delete horoscope;
}

/*****************************************************
**
**   VedicChartPreviewPanel   ---   recreateChart
**
******************************************************/
void VedicChartPreviewPanel::recreateChart( const int &newmode )
{
	mode = newmode;
	if ( chart ) delete chart;
	
	if ( mode == 0 )
	{
		chart = new VedicVargaChart( horoscope, VARGA_RASI, chartprops );
	}
	else
	{
		chart = new SarvatobhadraChart( horoscope, chartprops );
	}
}

/*****************************************************
**
**   VedicChartPreviewPanel   ---   OnPaint
**
******************************************************/
void VedicChartPreviewPanel::OnPaint( wxPaintEvent &event )
{
	wxCoord h, w;

	wxPaintDC context( this );
	PrepareDC( context );

	GetSize( &w, &h );
	horoscope->update();
	DcPainter painter( &context );
	
  VedicChartConfig *conf = VedicChartConfigLoader::get()->getConfig( chartprops->getVedicSkin());

	painter.doFloodFill( conf->doFloodFill ? (const wxChar*)conf->bgImage : wxEmptyString, conf->fgColor, conf->bgColor, wxRect( 0, 0, w, h ));
		
	chart->OnDataChanged();
	chart->doPaint( &painter, wxRect( 0, 0, w, h ));
}

/*****************************************************
**
**   VedicChartPanel   ---   Constructor 
**
******************************************************/
VedicChartPanel::VedicChartPanel( wxWindow* parent )
 : ConfigPanel( parent )
{
	chartprops = new ChartProperties;
	mode = 0;

    // begin wxGlade: VedicChartPanel::VedicChartPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_general_staticbox = new wxStaticBox(this, -1, _("General Options"));
    sizer_varga_chart_staticbox = new wxStaticBox(this, -1, _("Varga Chart"));
    sizer_sbc_staticbox = new wxStaticBox(this, -1, _("Show in Sarvatobhadra Chart"));
    sizer_preview_staticbox = new wxStaticBox(this, -1, _("Preview"));
    sizer_defaultview_staticbox = new wxStaticBox(this, -1, _("Default View"));
    const wxString choice_defaultskin_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_defaultskin = new wxChoice(this, CD_SKIN_SELECT, wxDefaultPosition, wxDefaultSize, 5, choice_defaultskin_choices, 0);
    check_retro = new wxCheckBox(this, CD_SHOW_RETRO, _("Show Retrogression (R)"));
    check_use_colors = new wxCheckBox(this, CD_USE_COLORS, _("Use Colors for Planet Symbols"));
    label_chart_type = new wxStaticText(this, wxID_ANY, _("Chart Type"));
    const wxString choice_chart_type_choices[] = {
        _("Varga Chart"),
        _("Sarvatobhadra")
    };
    choice_chart_type = new wxChoice(this, CD_SELECT_NORTH, wxDefaultPosition, wxDefaultSize, 2, choice_chart_type_choices, 0);
    label_chartstyle = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_chart_choices[] = {
        _("South Indian"),
        _("North Indian"),
        _("East Indian")
    };
    choice_chart = new wxChoice(this, CD_SELECT_NORTH, wxDefaultPosition, wxDefaultSize, 3, choice_chart_choices, 0);
    label_north_style = new wxStaticText(this, wxID_ANY, _("Signs (North Indian)"));
    const wxString choice_north_chart_choices[] = {
        _("Ascendant Only"),
        _("Number"),
        _("Short String"),
        _("Symbol")
    };
    choice_north_chart = new wxChoice(this, CD_SELECT_SNE, wxDefaultPosition, wxDefaultSize, 4, choice_north_chart_choices, 0);
    label_chartcenter = new wxStaticText(this, wxID_ANY, _("Center of Chart"));
    const wxString choice_chart_center_choices[] = {
        _("Nothing"),
        _("Varga Name"),
        _("Division D-N")
    };
    choice_chart_center = new wxChoice(this, CD_SHOW_CENTER, wxDefaultPosition, wxDefaultSize, 3, choice_chart_center_choices, 0);
    check_arudhas = new wxCheckBox(this, CD_SHOW_ARUDHAS, _("Show Arudhas"));
    check_sanskrit_symbols = new wxCheckBox(this, CD_SHOW_SANSKRIT_SYMBOLS, _("Sanskrit Symbols"));
    check_afflictions = new wxCheckBox(this, CD_SHOW_AFFLICTIONS, _("Afflictions"));
    window_preview = new VedicChartPreviewPanel(panel_preview, chartprops);

    set_properties();
    do_layout();
    // end wxGlade

  Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicChartPanel::OnCommand ));
  Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( VedicChartPanel::OnCommand ));
}

/*****************************************************
**
**   VedicChartPanel   ---   Destructor 
**
******************************************************/
VedicChartPanel::~VedicChartPanel()
{
	delete chartprops;
}

/*****************************************************
**
**   VedicChartPanel   ---   setData 
**
******************************************************/
void VedicChartPanel::setData()
{
	choice_defaultskin->Clear();
	vector<VedicChartConfig*> v = VedicChartConfigLoader::get()->getConfigs();
	for( unsigned int i = 0; i < v.size(); i++ ) choice_defaultskin->Append( v[i]->name );
	choice_defaultskin->SetSelection( chartprops->getVedicSkin() );

	int charttype = 0;
	if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN ) charttype = 1;
	else if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_EAST_INDIAN ) charttype = 2;
	choice_chart->SetSelection( charttype );

	int northchart = 0;
	if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_NUMBER ) charttype = 1;
	else if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_SHORT ) charttype = 2;
	else if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_SYMBOL ) charttype = 3;
	choice_north_chart->SetSelection( northchart );

	check_use_colors->SetValue( chartprops->getVedicGraphicStyle() & VGRAPHIC_SHOW_PLANET_COLORS );
	check_arudhas->SetValue( chartprops->getVedicGraphicStyle() & VGRAPHIC_SHOW_ARUDHAS );
  check_retro->SetValue( chartprops->getVedicGraphicStyle() & VGRAPHIC_SHOW_RETRO );

	int chartcenter = 0;
	if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_CHART_CENTER_VARGA ) chartcenter = 1;
	else if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_CHART_CENTER_DIVISION ) chartcenter = 2;
  choice_chart_center->SetSelection( chartcenter );

  check_afflictions->SetValue( chartprops->getVedicGraphicStyle() & VGRAPHIC_SHOW_AFFLICTIONS );
  check_sanskrit_symbols->SetValue( chartprops->getVedicGraphicStyle() & VGRAPHIC_SHOW_SANSKRIT_SYMBOLS );

	updateUI();
}

/*****************************************************
**
**   VedicChartPanel   ---   calculateStyle
**
******************************************************/
void VedicChartPanel::calculateStyle()
{
	style = 0;
  if ( check_arudhas->GetValue()) style |= VGRAPHIC_SHOW_ARUDHAS;
  if ( check_retro->GetValue()) style |= VGRAPHIC_SHOW_RETRO;
  if ( check_use_colors->GetValue()) style |= VGRAPHIC_SHOW_PLANET_COLORS;
  if ( check_sanskrit_symbols->GetValue()) style |= VGRAPHIC_SHOW_SANSKRIT_SYMBOLS;
  if ( check_afflictions->GetValue()) style |= VGRAPHIC_SHOW_AFFLICTIONS;

	switch( choice_chart->GetSelection())
	{
		case 1:
			style |= VGRAPHIC_NORTH_INDIAN;
		break;
		case 2:
			style |= VGRAPHIC_EAST_INDIAN;
		break;
		case 0:
		default:
			style |= VGRAPHIC_SOUTH_INDIAN;
		break;
	}

	switch( choice_north_chart->GetSelection())
	{
		case 0:
			style |= VGRAPHIC_NORTH_INDIAN_ASC;
		break;
		case 2:
			style |= VGRAPHIC_NORTH_INDIAN_SHORT;
		break;
		case 3:
			style |= VGRAPHIC_NORTH_INDIAN_SYMBOL;
		break;
		case 1:
		default:
			style |= VGRAPHIC_NORTH_INDIAN_NUMBER;
		break;
	}

  switch( choice_chart_center->GetSelection())
	{
		case 0:
			style |= VGRAPHIC_CHART_CENTER_NOTHING;
		break;
		case 1:
			style |= VGRAPHIC_CHART_CENTER_VARGA;
		break;
		case 2:
		default:
			style |= VGRAPHIC_CHART_CENTER_DIVISION;
		break;
	}
}

/*****************************************************
**
**   VedicChartPanel   ---   saveData 
**
******************************************************/
bool VedicChartPanel::saveData()
{
	config->vGraphicStyle = style;
	config->vGraphicSkin = choice_defaultskin->GetSelection();
	return true;
}

/*****************************************************
**
**   VedicChartPanel   ---   OnCommand
**
******************************************************/
void VedicChartPanel::OnCommand( wxCommandEvent& )
{
	updateUI();
}

/*****************************************************
**
**   VedicChartPanel   ---   updateUI
**
******************************************************/
void VedicChartPanel::updateUI()
{
	calculateStyle();
	int oldmode = mode;

	mode = choice_chart_type->GetSelection();

	// Varga Chart
	sizer_varga_chart_staticbox->Enable( mode == 0 );
	label_chartstyle->Enable( mode == 0 );
	choice_chart->Enable( mode == 0 );
	label_north_style->Enable( mode == 0 && choice_chart->GetSelection() == 1 );
	choice_north_chart->Enable( mode == 0 && choice_chart->GetSelection() == 1 );
	label_chartcenter->Enable( mode == 0 );
	choice_chart_center->Enable( mode == 0 );
	check_arudhas->Enable( mode == 0 );

	// Sarvatobhadra
	sizer_sbc_staticbox->Enable( mode == 1 );
	check_sanskrit_symbols->Enable( mode == 1 );
	check_afflictions->Enable( mode == 1 );
	//button_benefics->Enable( mode == 1 && check_afflictions->GetValue() );

	chartprops->setVedicSkin( choice_defaultskin->GetSelection());
	chartprops->setVedicGraphicStyle( style );

	if ( mode != oldmode ) window_preview->recreateChart( mode );
	window_preview->Refresh();
}

/*****************************************************
**
**   VedicChartPanel   ---   set_properties 
**
******************************************************/
void VedicChartPanel::set_properties()
{
    // begin wxGlade: VedicChartPanel::set_properties
    choice_defaultskin->SetSelection(0);
    choice_chart_type->SetSelection(0);
    choice_chart->SetSelection(0);
    choice_north_chart->SetSelection(0);
    choice_chart_center->SetSelection(0);
    panel_preview->SetMinSize(wxSize(100, 100));
    // end wxGlade
}

/*****************************************************
**
**   VedicChartPanel   ---   do_layout 
**
******************************************************/
void VedicChartPanel::do_layout()
{
    // begin wxGlade: VedicChartPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_preview = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(4, 1, 0, 0);
    wxStaticBoxSizer* sizer_sbc = new wxStaticBoxSizer(sizer_sbc_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_varga_chart = new wxStaticBoxSizer(sizer_varga_chart_staticbox, wxVERTICAL);
    wxGridSizer* grid_option_choices = new wxGridSizer(4, 2, 0, 0);
    wxFlexGridSizer* sizer_chart_type = new wxFlexGridSizer(1, 2, 3, 3);
    wxStaticBoxSizer* sizer_general = new wxStaticBoxSizer(sizer_general_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_defaultview = new wxStaticBoxSizer(sizer_defaultview_staticbox, wxVERTICAL);
    sizer_defaultview->Add(choice_defaultskin, 0, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_defaultview, 1, wxALL|wxEXPAND, 3);
    sizer_general->Add(check_retro, 0, wxALL|wxEXPAND, 3);
    sizer_general->Add(check_use_colors, 0, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_general, 1, wxEXPAND, 0);
    sizer_chart_type->Add(label_chart_type, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_chart_type->Add(choice_chart_type, 0, wxALL|wxEXPAND, 3);
    sizer_chart_type->AddGrowableCol(1);
    sizer_left->Add(sizer_chart_type, 1, wxALL|wxEXPAND, 3);
    grid_option_choices->Add(label_chartstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_option_choices->Add(choice_chart, 0, wxALL|wxEXPAND, 3);
    grid_option_choices->Add(label_north_style, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_option_choices->Add(choice_north_chart, 0, wxALL|wxEXPAND, 3);
    grid_option_choices->Add(label_chartcenter, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_option_choices->Add(choice_chart_center, 0, wxALL|wxEXPAND, 3);
    grid_option_choices->Add(check_arudhas, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_option_choices->Add(20, 20, 0, 0, 0);
    sizer_varga_chart->Add(grid_option_choices, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_varga_chart, 1, wxALL, 3);
    sizer_sbc->Add(check_sanskrit_symbols, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_sbc->Add(check_afflictions, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_left->Add(sizer_sbc, 1, wxEXPAND, 0);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_preview->Add(window_preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_preview);
    grid_preview->AddGrowableRow(0);
    grid_preview->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableRow(0);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(1);
    // end wxGlade
}

