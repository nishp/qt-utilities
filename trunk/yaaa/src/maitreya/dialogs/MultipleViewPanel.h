/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/MultipleViewPanel.h
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

#ifndef MULTIPLEVIEWPANEL_H
#define MULTIPLEVIEWPANEL_H

#ifdef __GNUG__
	#pragma interface "MultipleViewPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxChoice;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for multiple view parameters
* 
******************************************************/
class MultipleViewPanel: public ConfigPanel
{
	DECLARE_CLASS( MultipleViewPanel )

public:
    // begin wxGlade: MultipleViewPanel::ids
    // end wxGlade

  MultipleViewPanel( wxWindow* parent );
  virtual void setData();
  virtual bool saveData();

private:
    // begin wxGlade: MultipleViewPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	void OnMViewChoice( wxCommandEvent& );
	void updateMViewDescription();

protected:
    // begin wxGlade: MultipleViewPanel::attributes
    wxStaticBox* sizer_nbstyle_staticbox;
    wxStaticBox* sizer_mview_staticbox;
    wxStaticBox* sizer_usemview_staticbox;
    wxStaticText* label_usemview;
    wxChoice* choice_usemview;
    wxStaticText* label_defview;
    wxChoice* choice_viewlist;
    wxTextCtrl* text_view_description;
    wxStaticText* label_nbstyle;
    wxChoice* choice_nbstyle;
    wxStaticText* label_orientation;
    wxChoice* choice_orientation;
    // end wxGlade
}; // wxGlade: end class


#endif // MULTIPLEVIEWPANEL_H
