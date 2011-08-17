/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/WesternChartPanel.cpp
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
	#pragma implementation "WesternChartPanel.h"
#endif

#include "WesternChartPanel.h"

#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Painter.h"
#include "WesternChart.h"
#include "WesternChartConfig.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

enum { CD_SKIN_SELECT = wxID_HIGHEST + 1, CD_ASCENDANT, CD_SHOW_ASPECTS, CD_SHOW_RETRO, CD_SHOW_HOUSES, CD_USE_ASPECT_COLORS,
	CD_USE_SIGN_COLORS, CD_USE_PLANET_COLORS, CD_USE_HOUSE_COLORS, CD_SHOW_ASPECT_SYMBOLS };

IMPLEMENT_CLASS( WesternChartPanel, ConfigPanel )

IMPLEMENT_CLASS( WesternChartPreviewPanel, wxPanel )

/*****************************************************
**
**   WesternChartPreviewPanel   ---   Constructor 
**
******************************************************/
WesternChartPreviewPanel::WesternChartPreviewPanel( wxWindow* parent, ChartProperties *props, const wxPoint& pos, const wxSize& size, long style )
	: wxPanel( parent, -1, pos, size, wxFULL_REPAINT_ON_RESIZE )
{
	chartprops = props;
	horoscope = new Horoscope();
	horoscope->update();
	chart = new WesternChart( horoscope, horoscope, chartprops );
	chart->OnDataChanged();

  Connect( wxEVT_PAINT, wxPaintEventHandler( WesternChartPreviewPanel::OnPaint ));
}

/*****************************************************
**
**   WesternChartPreviewPanel   ---   Destructor 
**
******************************************************/
WesternChartPreviewPanel::~WesternChartPreviewPanel()
{
	delete chart;
	delete horoscope;
}

/*****************************************************
**
**   WesternChartPreviewPanel   ---   OnPaint
**
******************************************************/
void WesternChartPreviewPanel::OnPaint( wxPaintEvent &event )
{
	wxCoord h, w;
	wxPaintDC context( this );
	PrepareDC( context );

	GetSize( &w, &h );
	horoscope->update();
	DcPainter painter( &context );

  WesternChartConfig *conf = WesternChartConfigLoader::get()->getConfig( chartprops->getWesternSkin());

	painter.doFloodFill( conf->doFloodFill ? (const wxChar*)conf->bgImage : wxEmptyString, conf->fgColor, conf->bgColor, wxRect( 0, 0, w, h ));

	chart->paint( &painter, wxRect( 0, 0, w, h ));
}

/*****************************************************
**
**   WesternChartPanel   ---   Constructor 
**
******************************************************/
WesternChartPanel::WesternChartPanel( wxWindow* parent )
 : ConfigPanel( parent )
{
	chartprops = new ChartProperties;
    // begin wxGlade: WesternChartPanel::WesternChartPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_chartoptions_staticbox = new wxStaticBox(this, -1, _("Chart Options"));
    sizer_colors_staticbox = new wxStaticBox(this, -1, _("Use Colors"));
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
    label_leftside = new wxStaticText(this, wxID_ANY, _("Left Side"));
    const wxString choice_leftside_choices[] = {
        _("Ascendant"),
        _("Aries"),
        _("Cancer"),
        _("Libra"),
        _("Capricorn")
    };
    choice_leftside = new wxChoice(this, CD_ASCENDANT, wxDefaultPosition, wxDefaultSize, 5, choice_leftside_choices, 0);
    static_line_1 = new wxStaticLine(this, wxID_ANY);
    check_aspects = new wxCheckBox(this, CD_SHOW_ASPECTS, _("Show Aspects"));
    check_aspectsymbols = new wxCheckBox(this, CD_SHOW_ASPECT_SYMBOLS, _("Show Aspect Symbols"));
    check_retro = new wxCheckBox(this, CD_SHOW_RETRO, _("Show Retrogression (R)"));
    check_houses = new wxCheckBox(this, CD_SHOW_HOUSES, _("Show Houses"));
    check_use_planet_colors = new wxCheckBox(this, CD_USE_PLANET_COLORS, _("Planets"));
    check_use_sign_colors = new wxCheckBox(this, CD_USE_SIGN_COLORS, _("Signs"));
    check_use_house_colors = new wxCheckBox(this, CD_USE_HOUSE_COLORS, _("Houses"));
    check_use_aspect_colors = new wxCheckBox(this, CD_USE_ASPECT_COLORS, _("Aspects"));
    window_preview = new WesternChartPreviewPanel(panel_preview, chartprops);

    set_properties();
    do_layout();
    // end wxGlade

  Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WesternChartPanel::OnCommand ));
  Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WesternChartPanel::OnCommand ));
}

/*****************************************************
**
**   WesternChartPanel   ---   Destructor
**
******************************************************/
WesternChartPanel::~WesternChartPanel()
{
	delete chartprops;
}

/*****************************************************
**
**   WesternChartPanel   ---   setData 
**
******************************************************/
void WesternChartPanel::setData()
{
	choice_defaultskin->Clear();
	vector<WesternChartConfig*> v = WesternChartConfigLoader::get()->getConfigs();
	for( unsigned int i = 0; i < v.size(); i++ ) choice_defaultskin->Append( v[i]->name );
	choice_defaultskin->SetSelection( chartprops->getWesternSkin() );

	//choice_leftside->SetSelection( config->wChartLeftSide );
	check_aspects->SetValue( config->wGraphicStyle& WGRAPHIC_SHOW_ASPECTS );
	check_aspectsymbols->SetValue( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_ASPECT_SYMBOLS );
  check_retro->SetValue( config->wGraphicStyle& WGRAPHIC_SHOW_RETRO );
  check_houses->SetValue( config->wGraphicStyle& WGRAPHIC_SHOW_HOUSES );

	check_use_sign_colors->SetValue( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_SIGN_COLORS );
	check_use_planet_colors->SetValue( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_PLANET_COLORS );
	check_use_house_colors->SetValue( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_HOUSE_COLORS );
	check_use_aspect_colors->SetValue( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_ASPECT_COLORS );

	int leftside = 0;
	if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_ARIES ) leftside = 1;
	else if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_CANCER ) leftside = 1;
	else if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_LIBRA ) leftside = 1;
	else if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_CAPRICORN ) leftside = 1;
	choice_leftside->SetSelection( leftside );

	window_preview->Refresh();
}

/*****************************************************
**
**   WesternChartPanel   ---   calculateStyle
**
******************************************************/
int WesternChartPanel::calculateStyle()
{
	int style = config->wObjects;
	if ( check_aspects->GetValue() ) style |= WGRAPHIC_SHOW_ASPECTS;
	if ( check_aspectsymbols->GetValue() ) style |= WGRAPHIC_SHOW_ASPECT_SYMBOLS;
  if ( check_retro->GetValue() ) style |= WGRAPHIC_SHOW_RETRO;
  if ( check_houses->GetValue() ) style |= WGRAPHIC_SHOW_HOUSES;

  if ( check_use_sign_colors->GetValue() ) style |= WGRAPHIC_SHOW_SIGN_COLORS;
  if ( check_use_planet_colors->GetValue() ) style |= WGRAPHIC_SHOW_PLANET_COLORS;
  if ( check_use_house_colors->GetValue() ) style |= WGRAPHIC_SHOW_HOUSE_COLORS;
  if ( check_use_aspect_colors->GetValue() ) style |= WGRAPHIC_SHOW_ASPECT_COLORS;
	switch( choice_leftside->GetSelection())
	{
		case 1:
			 style |= WGRAPHIC_SHOW_LEFT_ARIES;
		break;
		case 2:
			 style |= WGRAPHIC_SHOW_LEFT_CANCER;
		break;
		case 3:
			 style |= WGRAPHIC_SHOW_LEFT_LIBRA;
		break;
		case 4:
			 style |= WGRAPHIC_SHOW_LEFT_CAPRICORN;
		break;
		case 0:
		default:
			 style |= WGRAPHIC_SHOW_LEFT_ASC;
		break;
	}
	return style;
}

/*****************************************************
**
**   WesternChartPanel   ---   saveData 
**
******************************************************/
bool WesternChartPanel::saveData()
{
	config->wGraphicSkin = choice_defaultskin->GetSelection();
  config->wGraphicStyle = calculateStyle();

	return true;
}

/*****************************************************
**
**   WesternChartPanel   ---   OnCommand
**
******************************************************/
void WesternChartPanel::OnCommand( wxCommandEvent& )
{
	setGraphicDescription();

	chartprops->setWesternSkin( choice_defaultskin->GetSelection());
  chartprops->setWesternGraphicStyle( calculateStyle());

	window_preview->Refresh();
}

/*****************************************************
**
**   WesternChartPanel   ---   setGraphicDescription
**
******************************************************/
void WesternChartPanel::setGraphicDescription()
{
	//int selskin = choice_defaultskin->GetSelection();
	//WesternChartConfig *cfg = WesternChartConfigLoader::get()->getConfig( selskin );
}

/*****************************************************
**
**   WesternChartPanel   ---   set_properties 
**
******************************************************/
void WesternChartPanel::set_properties()
{
    // begin wxGlade: WesternChartPanel::set_properties
    choice_defaultskin->SetSelection(0);
    choice_leftside->SetSelection(0);
    panel_preview->SetMinSize(wxSize(100, 100));
    // end wxGlade
}

/*****************************************************
**
**   WesternChartPanel   ---   do_layout 
**
******************************************************/
void WesternChartPanel::do_layout()
{
    // begin wxGlade: WesternChartPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_colors = new wxStaticBoxSizer(sizer_colors_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_chartoptions = new wxStaticBoxSizer(sizer_chartoptions_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_defaultview = new wxStaticBoxSizer(sizer_defaultview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_defaultview = new wxFlexGridSizer(4, 1, 3, 3);
    grid_defaultview->Add(choice_defaultskin, 0, wxALL|wxEXPAND, 3);
    grid_defaultview->AddGrowableCol(0);
    sizer_defaultview->Add(grid_defaultview, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_defaultview, 0, wxALL|wxEXPAND, 3);
    sizer_4->Add(label_leftside, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_4->Add(choice_leftside, 0, wxALL|wxEXPAND, 3);
    sizer_chartoptions->Add(sizer_4, 1, wxEXPAND, 0);
    sizer_chartoptions->Add(static_line_1, 0, wxEXPAND, 0);
    sizer_chartoptions->Add(check_aspects, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_chartoptions->Add(check_aspectsymbols, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_chartoptions->Add(check_retro, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_chartoptions->Add(check_houses, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_left->Add(sizer_chartoptions, 1, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_planet_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_sign_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_house_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_aspect_colors, 0, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_colors, 1, wxALL|wxEXPAND, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_previewpanel->Add(window_preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_previewpanel);
    grid_previewpanel->AddGrowableRow(0);
    grid_previewpanel->AddGrowableCol(0);
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

