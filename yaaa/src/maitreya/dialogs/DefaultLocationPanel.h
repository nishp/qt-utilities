/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DefaultLocationPanel.h
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

#ifndef DEFAULTLOCATIONPANEL_H
#define DEFAULTLOCATIONPANEL_H

#ifdef __GNUG__
	#pragma interface "DefaultLocationPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class Location;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class wxTimer;
class wxTimerEvent;

/*************************************************//**
*
* \brief ConfigPanel for default location parameters
* 
******************************************************/
class DefaultLocationPanel: public ConfigPanel
{
	DECLARE_CLASS( DefaultLocationPanel )

public:
    // begin wxGlade: DefaultLocationPanel::ids
    // end wxGlade

  DefaultLocationPanel( wxWindow* parent );
	~DefaultLocationPanel();

	virtual void setData();
  virtual bool saveData();

private:
    // begin wxGlade: DefaultLocationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	void OnSearchLocation( wxCommandEvent& );
	void setDefaultLocation( Location* );
	void OnTimer( wxTimerEvent& );

	virtual void onActivate();
	virtual void onPassivate();

	wxTimer *timer;

protected:
    // begin wxGlade: DefaultLocationPanel::attributes
    wxStaticBox* sizer_tz_staticbox;
    wxStaticBox* sizer_deflocdata_staticbox;
    wxStaticBox* sizer_deflocname_staticbox;
    wxTextCtrl* text_deflocname;
    wxStaticText* label_defloc_long;
    wxTextCtrl* text_deflong;
    wxChoice* choice_ew;
    wxStaticText* label_defloc_lat;
    wxTextCtrl* text_deflat;
    wxChoice* choice_ns;
    wxStaticText* label_defloc_tz;
    wxTextCtrl* text_timezone;
    wxChoice* choice_tzew;
    wxStaticText* label_defloc_dst;
    wxTextCtrl* text_dst;
    wxStaticText* label_defloc_dst2;
    wxStaticText* label_systime;
    wxStaticText* text_systime;
    wxButton* searchloc_button;
    // end wxGlade
}; // wxGlade: end class


#endif // DEFAULTLOCATIONPANEL_H
