/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/WesternChartPanel.h
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

#ifndef WESTERNCHARTPANEL_H
#define WESTERNCHARTPANEL_H

#ifdef __GNUG__
	#pragma interface "WesternChartPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class ChartProperties;
class Horoscope;
class WesternChart;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief preview panel for western chart
* 
******************************************************/
class WesternChartPreviewPanel: public wxPanel
{
	DECLARE_CLASS( WesternChartPreviewPanel )

public:
	WesternChartPreviewPanel( wxWindow* parent, ChartProperties*, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL );
	~WesternChartPreviewPanel();

private:
	void OnPaint( wxPaintEvent& );
	Horoscope *horoscope;
	WesternChart *chart;
	ChartProperties *chartprops;

};

/*************************************************//**
*
* \brief Config panel for western chart parameters
* 
******************************************************/
class WesternChartPanel: public ConfigPanel
{
	DECLARE_CLASS( ConfWesternChartPanel )

public:
    // begin wxGlade: WesternChartPanel::ids
    // end wxGlade

	WesternChartPanel( wxWindow* parent );
	~WesternChartPanel();

	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: WesternChartPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	ChartProperties *chartprops;
	void OnCommand( wxCommandEvent& );
	void setGraphicDescription();
	int calculateStyle();

protected:
    // begin wxGlade: WesternChartPanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_colors_staticbox;
    wxStaticBox* sizer_chartoptions_staticbox;
    wxStaticBox* sizer_defaultview_staticbox;
    wxChoice* choice_defaultskin;
    wxStaticText* label_leftside;
    wxChoice* choice_leftside;
    wxStaticLine* static_line_1;
    wxCheckBox* check_aspects;
    wxCheckBox* check_aspectsymbols;
    wxCheckBox* check_retro;
    wxCheckBox* check_houses;
    wxCheckBox* check_use_planet_colors;
    wxCheckBox* check_use_sign_colors;
    wxCheckBox* check_use_house_colors;
    wxCheckBox* check_use_aspect_colors;
    WesternChartPreviewPanel* window_preview;
    wxPanel* panel_preview;
    // end wxGlade
}; // wxGlade: end class


#endif // WESTERNCHARTPANEL_H
