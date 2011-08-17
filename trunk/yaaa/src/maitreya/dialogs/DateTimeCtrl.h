/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DateTimeCtrl.h
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


#ifndef _DATETIMECTRL_H_
#define _DATETIMECTRL_H_

#ifdef __GNUG__
	#pragma interface "DateTimeCtrl.h"
#endif

#include <wx/app.h>
#include <wx/choice.h>
#include <wx/event.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>

class wxSpinButton;
class wxSpinEvent;
class wxStaticText;

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(COMMAND_DATE_SPIN_CHANGED, wxID_HIGHEST + 5000 )
	DECLARE_EVENT_TYPE(COMMAND_TIME_SPIN_CHANGED, wxID_HIGHEST + 5001 )
	DECLARE_EVENT_TYPE(COMMAND_TIME_SPIN_WRAP, wxID_HIGHEST + 5002 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
* \brief spin control for time input
*
******************************************************/
class TimeSpin : public wxControl
{
	DECLARE_CLASS( TimeSpin )

public:

	TimeSpin( wxWindow *parent, int id = -1, double startjd = 0, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );
	double getTime() { return time; }
	void setToCurrent();
	void setDate( const double& );
	virtual wxSize DoGetBestSize() const;

protected:

	void setDelta( const double &delta );
	double time;
	wxTextCtrl *theText;
	wxSpinButton *spin;
	void writeContents();
	void OnUp( wxSpinEvent &event );
	void OnDown( wxSpinEvent &event );
	void OnTextChanged( wxCommandEvent &event );
	void OnMouseWheelEvent( wxMouseEvent& );

};

/*************************************************//**
*
* \brief text field for date input
*
******************************************************/
class DateInputField : public wxTextCtrl
{
	DECLARE_CLASS( DateInputField )

public:
	DateInputField( wxWindow *parent, int id, wxString*, const wxPoint& pos = wxDefaultPosition,
	    const wxSize& size = wxDefaultSize );

protected:
	void OnKillFocus( wxFocusEvent& );
};

/*************************************************//**
*
* \brief validator for date fields
*
******************************************************/
class DateValidator : public wxTextValidator
{
	DECLARE_CLASS( DateValidator )
public:
	DateValidator( wxString* );

protected:

	virtual bool Validate( wxWindow* parent );
	virtual wxObject *Clone() const { return new DateValidator(*this); }
};

/*************************************************//**
*
* \brief control for date input
*
******************************************************/
class DateSpin : public wxControl
{
	DECLARE_CLASS( DateSpin )

public:
	DateSpin( wxWindow *parent, int id = -1, double startjd = 0, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );
	double getJD() { return jd; }
	void setToCurrent();
	void setDate( const double& );

protected:
	double jd;
	wxString date_value, time_value;
	DateInputField *theText;
	wxStaticText *theLabel;
	wxSpinButton *spin;
	void writeContents();
	void OnUp( wxSpinEvent& );
	void OnDown( wxSpinEvent& );
	void OnTextChanged( wxCommandEvent& );
	void OnKillFocus( wxFocusEvent& );
	virtual void OnSetFocus( wxFocusEvent& );
	void OnMouseWheelEvent( wxMouseEvent& );

};

#endif


