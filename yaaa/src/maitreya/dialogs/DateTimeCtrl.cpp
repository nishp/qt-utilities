/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DateTimeCtrl.cpp
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
	#pragma implementation "DateTimeCtrl.h"
#endif

#include "DateTimeCtrl.h"

#include "constants.h"
#include "Calculator.h"
#include "Conf.h"
#include "DataSet.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"

#include <math.h>

#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/spinbutt.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

DEFINE_EVENT_TYPE( COMMAND_DATE_SPIN_CHANGED )
DEFINE_EVENT_TYPE( COMMAND_TIME_SPIN_CHANGED )
DEFINE_EVENT_TYPE( COMMAND_TIME_SPIN_WRAP )

enum { DATE_SPIN = wxID_HIGHEST + 650, DATE_TEXT, TIME_TEXT, TIME_SPIN, DATE_LABEL };

IMPLEMENT_CLASS( DateSpin, wxControl )
IMPLEMENT_CLASS( TimeSpin, wxControl )
IMPLEMENT_CLASS( DateInputField, wxTextCtrl )
IMPLEMENT_CLASS( DateValidator, wxTextValidator )

/*****************************************************
**
**   DateSpin   ---   Constructor 
**
******************************************************/
DateSpin::DateSpin( wxWindow *parent, int id, double startjd, const wxPoint& pos, const wxSize& size )
 : wxControl( parent, id, pos, size )
{
	SetToolTip( _( "Date" ));
	jd = startjd;

	theText = new DateInputField( this, DATE_TEXT, &date_value );
	wxSize tisize = theText->GetBestSize();

	spin = new wxSpinButton( this, DATE_SPIN, wxDefaultPosition, wxSize( 15, tisize.GetY() ), wxSP_ARROW_KEYS);
	spin->SetRange( -20000, 5000000 );

	theLabel = new wxStaticText( this, DATE_LABEL, wxT( "Day" ), wxPoint( 0, 0 ), wxSize( 30, tisize.GetY() ));

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( theText, 1, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 0 );
	sizer->Add( spin, 0, wxALIGN_CENTER_VERTICAL, 0 );
	sizer->Add( theLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );

	SetAutoLayout(true);
	SetSizer(sizer);
	sizer->Fit(this);
	sizer->SetSizeHints(this);
	Layout();

	jd ? writeContents() : setToCurrent();

  Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( DateSpin::OnMouseWheelEvent ));
  Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( DateSpin::OnUp ));
  Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( DateSpin::OnDown ));
	Connect( DATE_TEXT, wxEVT_COMMAND_TEXT_ENTER, wxTextEventHandler( DateSpin::OnTextChanged ));
}

/*****************************************************
**
**   DateSpin   ---   writeContents 
**
******************************************************/
void DateSpin::writeContents()
{
	Formatter *formatter = Formatter::get();
	Lang lang;
	wxString s;

	double jjd = jd + ( config->defaultLocation.getTimeZone() + config->defaultLocation.getDST() ) / 24;
	date_value = formatter->getDateStringFromJD( jjd );
	theText->SetValue( date_value );
	s = lang.getWeekdayName( ((int)(jjd + 1.5 )) % 7).Left( 2 );
	theLabel->SetLabel(  s );
	Refresh();
}

/*****************************************************
**
**   DateSpin   ---   OnMouseWheelEvent
**
******************************************************/
void DateSpin::OnMouseWheelEvent( wxMouseEvent &event )
{
	if ( event.GetWheelRotation() > 0 ) jd++;
	else jd--;
	writeContents();
	wxCommandEvent e( COMMAND_DATE_SPIN_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   DateSpin   ---   OnUp 
**
******************************************************/
void DateSpin::OnUp( wxSpinEvent &event )
{
	jd++;
	writeContents();
	wxCommandEvent e( COMMAND_DATE_SPIN_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   DateSpin   ---   OnDown 
**
******************************************************/
void DateSpin::OnDown( wxSpinEvent &event )
{
	jd--;
	writeContents();
	wxCommandEvent e( COMMAND_DATE_SPIN_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   DateSpin   ---   OnSetFocus 
**
******************************************************/
void DateSpin::OnSetFocus( wxFocusEvent& )
{
	theText->SetFocus();
}

/*****************************************************
**
**   DateSpin   ---   OnKillFocus 
**
******************************************************/
void DateSpin::OnKillFocus( wxFocusEvent &event ) 
{
}

/*****************************************************
**
**   DateSpin   ---   setToCurrent
**
******************************************************/
void DateSpin::setToCurrent()
{
	setDate( JDate().getJD() );
}

/*****************************************************
**
**   DateSpin   ---   setDate
**
******************************************************/
void DateSpin::setDate( const double &jjd )
{
	jd = floor( jjd + .5 );
	writeContents();
	//wxCommandEvent e( COMMAND_DATE_SPIN_CHANGED, GetId() );
	//wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   DateSpin   ---   OnTextChanged 
**
******************************************************/
void DateSpin::OnTextChanged( wxCommandEvent &event )
{
	Formatter *formatter = Formatter::get();
	int day, month, year;

  if ( ! formatter->getDateIntsFromString( (const wxChar*)theText->GetValue(), day, month, year ))
	{
		setToCurrent();
		return;
	}
	JDate date;
	date.setDate( day, month, year, 12 );
	jd = date.getJD();
	writeContents();
	wxCommandEvent e( COMMAND_DATE_SPIN_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   TimeSpin   ---   Constructor 
**
******************************************************/
TimeSpin::TimeSpin( wxWindow *parent, int id, double startjd, const wxPoint& pos, const wxSize& size )
 : wxControl( parent, id, pos, size, wxTAB_TRAVERSAL )
{
	SetToolTip( _( "Time" ));
	double jjd =  ( startjd ?  startjd :
		JDate().getJD() + ( config->defaultLocation.getTimeZone() + config->defaultLocation.getDST() ) / 24);
	time = jjd - (double)(int)jjd + .5;
	time *= 24;

	theText = new wxTextCtrl( this, TIME_TEXT, wxT( "" ), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	int ysize = theText->GetBestSize().GetY();

	spin = new wxSpinButton( this, TIME_SPIN, wxDefaultPosition, wxSize( 15, ysize ), wxSP_ARROW_KEYS);
	spin->SetRange( -20000, 5000000 );

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( theText, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0 );
	sizer->Add( spin, 0, wxALIGN_CENTER_VERTICAL, 0 );

	SetAutoLayout(true);
	SetSizer( sizer );
	sizer->Fit(this);
	sizer->SetSizeHints(this);
	Layout();
	writeContents();

	Connect( TIME_SPIN, wxEVT_SCROLL_LINEUP, wxSpinEventHandler( TimeSpin::OnUp ));
	Connect( TIME_SPIN, wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( TimeSpin::OnDown ));

	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( TimeSpin::OnMouseWheelEvent ));
	Connect( TIME_TEXT, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TimeSpin::OnTextChanged ));
}

/*****************************************************
**
**   TimeSpin   ---   writeContents 
**
******************************************************/
void TimeSpin::writeContents()
{
	Formatter *formatter = Formatter::get();
	time = a_red( time, 24 );
	theText->SetValue( formatter->getTimeFormatted( time ));
}

/*****************************************************
**
**   TimeSpin   ---   DoGetBestSize
**
******************************************************/
wxSize TimeSpin::DoGetBestSize() const
{
	wxSize ret( wxControl::DoGetBestSize() );
	wxSize best(95, ret.y);
	return best;
}

/*****************************************************
**
**   TimeSpin   ---   setDelta
**
******************************************************/
void TimeSpin::setDelta( const double &delta )
{
	assert( delta > -23 && delta < 23 );
	int eventid = COMMAND_TIME_SPIN_CHANGED;
	int extraint = 0;
	double oldtime = time;
	time += delta;
	writeContents();

	if ( delta > 0 && oldtime > time )
	{
		eventid = COMMAND_TIME_SPIN_WRAP;
		extraint = 1;
	}
	else if ( delta < 0 && oldtime < time )
	{
		eventid = COMMAND_TIME_SPIN_WRAP;
		extraint = -1;
	}
	else eventid = COMMAND_TIME_SPIN_CHANGED;
	wxCommandEvent e( eventid, GetId() );
	e.SetInt( extraint );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   TimeSpin   ---   OnMouseWheelEvent
**
******************************************************/
void TimeSpin::OnMouseWheelEvent( wxMouseEvent &event )
{
	setDelta( event.GetWheelRotation() > 0 ? 1 : -1 );
}

/*****************************************************
**
**   TimeSpin   ---   OnUp 
**
******************************************************/
void TimeSpin::OnUp( wxSpinEvent &event )
{
	setDelta( 1 );
}

/*****************************************************
**
**   TimeSpin   ---   OnDown 
**
******************************************************/
void TimeSpin::OnDown( wxSpinEvent &event )
{
	setDelta( -1 );
}

/*****************************************************
**
**   TimeSpin   ---   setToCurrent
**
******************************************************/
void TimeSpin::setToCurrent()
{
	setDate( JDate().getJD() );
}

/*****************************************************
**
**   TimeSpin   ---   setDate
**
******************************************************/
void TimeSpin::setDate( const double &d )
{
	double jjd = d + ( config->defaultLocation.getTimeZone() + config->defaultLocation.getDST() ) / 24;
	time = jjd - (double)(int)jjd + .5;
	time *= 24;
	writeContents();
}

/*****************************************************
**
**   TimeSpin   ---   OnTextChanged 
**
******************************************************/
void TimeSpin::OnTextChanged( wxCommandEvent &event )
{
	Formatter *formatter = Formatter::get();
	int deg, minute, second;

  if ( ! formatter->getDegreeIntsFromString( (const wxChar*)theText->GetValue(), deg, minute, second ))
	{
		setToCurrent();
		return;
	}
	time = second + 60 * ( minute + 60 * deg );
	time = a_red( time/3600, 24 );
	writeContents();
	wxCommandEvent e( COMMAND_TIME_SPIN_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   DateInputField   ---   Constructor 
**
******************************************************/
DateInputField::DateInputField( wxWindow *parent, int id, wxString *s, const wxPoint& pos, const wxSize& size )
	: wxTextCtrl( parent, id, wxT( "" ), pos, size, wxTE_PROCESS_ENTER )
{
	SetSize( 200, GetBestSize().GetY() );
	SetToolTip( _( "Date" ));
	SetValidator( DateValidator( s ));	

	Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( DateInputField::OnKillFocus ));
}

/*****************************************************
**
**   DateInputField   ---   OnKillFocus 
**
******************************************************/
void DateInputField::OnKillFocus( wxFocusEvent &event ) 
{
	event.Skip();
}

/*****************************************************
**
**   DateValidator   ---   Constructor 
**
******************************************************/
DateValidator::DateValidator( wxString *s )
: wxTextValidator( wxFILTER_INCLUDE_CHAR_LIST, s )
{
	wxArrayString a;
	for( int i = 0; i < 10; i++ )
	{
		a.Add( wxString::Format( wxT( "%d" ), i ));
	}
	a.Add( wxT( "." ));
	a.Add( wxT( "-" ));
	SetIncludes( a );
}

/*****************************************************
**
**   DateValidator   ---   Validate 
**
******************************************************/
bool DateValidator::Validate(wxWindow* parent)
{
	int day, month, year;
	Formatter *formatter = Formatter::get();

	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	if ( formatter->getDateIntsFromString( (const wxChar*)ctrl->GetValue(), day, month, year ) ) return true;

	doMessageBox( parent,
		wxString::Format( wxT( "Error in %s input field" ), (const wxChar*)ctrl->GetName() ),
		wxOK | wxCENTRE | wxICON_ERROR );
	return false;
}


