/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/NonDocTextView.cpp
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
	#pragma implementation "NonDocTextView.h"
#endif

#include "NonDocTextView.h"

#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/toolbar.h>

#include "Conf.h"
#include "DateTimeCtrl.h"
#include "Eclipse.h"
#include "guibase.h"
#include "Hora.h"
#include "Lang.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( EclipseView, TextViewBase )
IMPLEMENT_CLASS( HoraView, TextViewBase )

/*************************************************//**
*
*  
*
******************************************************/
class EclipseViewTextWidget : public TextWidget 
{
	DECLARE_CLASS( EclipseViewTextWidget )
	
public:
	EclipseViewTextWidget( wxWindow *parent, BasicView *v, 	EclipseExpert *expert )
		: TextWidget( parent, v )
	{
		this->expert = expert;
	}

	virtual int write()
	{
		expert->write( writer, tz );
		return 0;
	}

	int tz;
	
private:
	EclipseExpert *expert;
};

IMPLEMENT_CLASS( EclipseViewTextWidget, TextWidget )

/*****************************************************
**
**   EclipseView   ---   Constructor 
**
******************************************************/
EclipseView::EclipseView( wxWindow *parent, ChildWindow *frame, const bool &vedic )
 : TextViewBase( parent, frame, 0, VIEW_ECLIPSE, true, vedic )
{
	supportEWToggle = true;
  double jd = JDate().getJD();
  int day, month;
  Formatter *formatter = Formatter::get();
  formatter->getDateIntsFromJD( jd, day, month, startyear );
	years = 3;
	type = 0;
	tz = config->ephemTimezone;
	expert = new EclipseExpert;

	theText = new EclipseViewTextWidget( this, this, 	expert );
	widget = theText;

	initToolItems();
	updateView = true;

	Connect( wxEVT_IDLE, wxIdleEventHandler( EclipseView::OnIdle ));
}

/*****************************************************
**
**   EclipseView   ---   Destructor 
**
******************************************************/
EclipseView::~EclipseView()
{
	delete expert;
}

/**************************************************************
***
**   EclipseView   ---   initToolItems
***
***************************************************************/
void EclipseView::initToolItems()
{
	if ( toolbar )
	{
		wxSpinCtrl *spin_start = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if( spin_start ) spin_start->SetValue( startyear );

		wxSpinCtrl *spin_years = (wxSpinCtrl*)toolbar->FindControl( TBS_NBYEARS );
		if( spin_years ) spin_years->SetValue( years );

		wxChoice *choice_type = (wxChoice*)toolbar->FindControl( TBS_ECL_TYPE );
		if( choice_type ) choice_type->SetSelection( type );

		wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_EPHEMTZ );
		if( choice_ephemtz ) choice_ephemtz->SetSelection( tz );
	}
}

/*****************************************************
**
**   EclipseView   ---   OnToolbarCommand 
**
******************************************************/
void EclipseView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxSpinCtrl *spin_start = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if( spin_start ) startyear = spin_start->GetValue();

		wxSpinCtrl *spin_years = (wxSpinCtrl*)toolbar->FindControl( TBS_NBYEARS );
		if( spin_years ) years = spin_years->GetValue();

		wxChoice *choice_type = (wxChoice*)toolbar->FindControl( TBS_ECL_TYPE );
		if( choice_type ) type = choice_type->GetSelection();

		wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_EPHEMTZ );
		if ( choice_ephemtz ) tz = choice_ephemtz->GetSelection();
		config->ephemTimezone = tz;
	}
	updateView = true;
}

/**************************************************************
***
**   EclipseView   ---   OnIdle
***
***************************************************************/
void EclipseView::OnIdle( wxIdleEvent &event )
{
	if ( ! updateView ) return;
	write();
	updateView = false;
}

/**************************************************************
***
**   EclipseView   ---   write
***
***************************************************************/
void EclipseView::write()
{
	double startjd, endjd;
	JDate d;
	d.setDate( 1, 1, startyear, 0 );
	startjd = d.getJD();
	d.setDate( 1, 1, startyear + years, 0 );
	endjd = d.getJD();

	expert->update( startjd, endjd, type, 0, isVedic() );

	EclipseViewTextWidget *w = (EclipseViewTextWidget*)theText;
	w->tz = tz;
	theText->writeContents();
}

/**************************************************************
***
**   EclipseView   ---   getWindowLabel
***
***************************************************************/
wxString EclipseView::getWindowLabel( const bool shortname ) { return _( "Eclipse" ); }

/**************************************************************
***
**   ViewFactory   ---   createEclipseView
***
***************************************************************/
BasicView *ViewFactory::createEclipseView( wxWindow *parent, ChildWindow *frame, const bool &vedic )
{
	return new EclipseView( parent, frame, vedic );
}

/*************************************************//**
*
*  
*
******************************************************/
class HoraViewTextWidget : public TextWidget 
{
	DECLARE_CLASS( HoraViewTextWidget )
	
public:
	HoraViewTextWidget( wxWindow *parent, BasicView *v, 	HoraExpert *expert )
		: TextWidget( parent, v )
	{
		this->expert = expert;
	}

	virtual int write()
	{
		expert->write( writer );
		return 0;
	}

private:
	HoraExpert *expert;
};

IMPLEMENT_CLASS( HoraViewTextWidget, TextWidget )

/*****************************************************
**
**   HoraView   ---   Constructor 
**
******************************************************/
HoraView::HoraView( wxWindow *parent, ChildWindow *frame )
 : TextViewBase( parent, frame, 0, VIEW_HORA, true, true )
{
	expert = new HoraExpert();
	theText = new HoraViewTextWidget( this, this, expert );
	widget = theText;
	
	jd = floor( JDate().getJD() + .5 );
	OnDataChanged();

	Connect( TBB_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( HoraView::OnNow ));
	Connect( TBS_DATE, COMMAND_DATE_SPIN_CHANGED, wxCommandEventHandler( HoraView::OnDateCtrlChanged ));
}

/*****************************************************
**
**   HoraView   ---   Destructor 
**
******************************************************/
HoraView::~HoraView()
{
	delete expert;
}

/*****************************************************
**
**   HoraView   ---   OnNow 
**
******************************************************/
void HoraView::OnNow( wxCommandEvent &event )
{
	if ( toolbar )
	{
		DateSpin *ds = (DateSpin*)toolbar->FindControl( TBS_DATE );
		if( ds )
		{
			ds->setToCurrent();
			jd = ds->getJD();
		}
	}
	OnDataChanged();
}

/*****************************************************
**
**   HoraView   ---   OnDateCtrlChanged
**
******************************************************/
void HoraView::OnDateCtrlChanged( wxCommandEvent &event )
{
  double tz = config->defaultLocation.getTimeZone() + config->defaultLocation.getDST();
  if ( toolbar )
  {
    DateSpin *spin_date = (DateSpin*)toolbar->FindControl( TBS_DATE );
    if ( spin_date )
    {
      jd = spin_date->getJD() - tz / 24;
			//printf( "JD1 %f TIME %f JD %f\n",  spin_date->getJD(), spin_time->getTime(), jd  );
    }
  }
  OnDataChanged();
}

/*****************************************************
**
**   HoraView   ---   write
**
******************************************************/
void HoraView::write()
{
	expert->update( jd - .5 );
	theText->writeContents();
}

/**************************************************************
***
**   HoraView   ---   getWindowLabel
***
***************************************************************/
wxString HoraView::getWindowLabel( const bool shortname ) { return _( "Hora" ); }

/**************************************************************
***
**   ViewFactory   ---   createHoraView
***
***************************************************************/
BasicView *ViewFactory::createHoraView( wxWindow *parent, ChildWindow *frame )
{
	return new HoraView( parent, frame );
}




