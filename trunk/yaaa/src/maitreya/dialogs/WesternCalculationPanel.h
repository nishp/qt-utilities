/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/WesternCalculationPanel.h
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

#ifndef WESTERNCALCULATIONPANEL_H
#define WESTERNCALCULATIONPANEL_H

#ifdef __GNUG__
	#pragma interface "WesternCalculationPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class AyanamsaChoice;
class wxChoice;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;
class YearLengthChoice;

/*************************************************//**
*
* \brief ConfigPanel for western calculation parameters
* 
******************************************************/
class WesternCalculationPanel: public ConfigPanel
{
	DECLARE_CLASS( WesternCalculationPanel )

public:
    // begin wxGlade: WesternCalculationPanel::ids
    // end wxGlade

  WesternCalculationPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: WesternCalculationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	void OnYlChoice( wxCommandEvent& );

protected:
    // begin wxGlade: WesternCalculationPanel::attributes
    wxStaticBox* sizer_yl_staticbox;
    wxStaticBox* sizer_wcalc_staticbox;
    wxStaticText* label_wcalc_aya;
    AyanamsaChoice* choice_waya;
    wxStaticText* label_wcalc_node;
    wxChoice* choice_wnode;
    wxStaticText* label_wcalc_house;
    wxChoice* choice_whouse;
    YearLengthChoice* choice_yl;
    wxTextCtrl* text_custom_yl;
    // end wxGlade
}; // wxGlade: end class


#endif // WESTERNCALCULATIONPANEL_H
