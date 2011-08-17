/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DataDialog.h
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

#ifndef DATADIALOG_H
#define DATADIALOG_H

#ifdef __GNUG__
	#pragma interface "DataDialog.h"
#endif

#include <wx/dialog.h>

// begin wxGlade: dependencies
// end wxGlade

class Document;
class wxEvent;
class wxTextCtrl;
class wxChoice;
class wxButton;
class wxCheckBox;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
* \brief contains data for birth data dialog
*
******************************************************/
class DataDialogModel
{
public:
	wxString name, date, time, locname, longitude, latitude, tz, dst, remark;
	int ew, ns, tzew;
};

/*************************************************//**
*
* \brief dialog for birth data
*
******************************************************/
class DataDialog: public wxDialog
{
 DECLARE_CLASS( DataDialog )

public:
    // begin wxGlade: DataDialog::ids
    // end wxGlade

	DataDialog(wxWindow* parent, Document* );

private:
    // begin wxGlade: DataDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	Document *doc;
	DataDialogModel model;
	void writeModel2Gui();
	void writeGui2Model();
	void writeModel2Doc();
	void writeDoc2Model();
	bool doApply();

	void OnActual( wxCommandEvent& );
	void OnDefaultLocation( wxCommandEvent& );
	void OnSearchLocation( wxCommandEvent& );
	void OnOK( wxCommandEvent& );
	void OnCancel( wxCommandEvent& );
	void OnApply( wxCommandEvent& ) { doApply(); }
	void OnRevert( wxCommandEvent& );
	void OnLocalTime( wxCommandEvent& );

protected:
    // begin wxGlade: DataDialog::attributes
    wxStaticBox* sizer_remark_staticbox;
    wxStaticBox* sizer_tz_staticbox;
    wxStaticBox* sizer_location_staticbox;
    wxStaticBox* sizer_datetime_staticbox;
    wxStaticBox* sizer_name_staticbox;
    wxTextCtrl* text_name;
    wxStaticText* label_date;
    wxTextCtrl* text_date;
    wxStaticText* label_datefmt;
    wxStaticText* label_time;
    wxTextCtrl* text_time;
    wxStaticText* label_timefmt;
    wxButton* button_now;
    wxStaticText* label_locname;
    wxTextCtrl* text_locname;
    wxStaticText* label_long;
    wxTextCtrl* text_longitude;
    wxStaticText* label_long_fmt;
    wxChoice* choice_ew;
    wxStaticText* label_lat;
    wxTextCtrl* text_latitude;
    wxStaticText* label_lat_fmt;
    wxChoice* choice_ns;
    wxButton* button_defloc;
    wxButton* button_searchloc;
    wxStaticText* label_tz;
    wxTextCtrl* text_tz;
    wxStaticText* label_tzfmt;
    wxChoice* choice_tzew;
    wxStaticText* label_dst;
    wxTextCtrl* text_dst;
    wxStaticText* label_dstfmt;
    wxButton* button_localtime;
    wxTextCtrl* text_remark;
    wxButton* button_ok;
    wxButton* button_cancel;
    wxButton* button_apply;
    wxButton* button_revert;
    // end wxGlade
};


#endif // DATADIALOG_H
