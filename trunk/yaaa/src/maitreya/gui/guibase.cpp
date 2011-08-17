/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/guibase.cpp
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
	#pragma implementation "guibase.h"
#endif

#include "guibase.h"

#include <wx/msgdlg.h>
#include <wx/settings.h>

#include "Conf.h"

extern Config *config;


/**************************************************************
***
**    mapCommand2ViewId
***
***************************************************************/
int mapCommand2ViewId( const int &command, int &vedic, int &subtype )
{
	switch( command )
	{
		case CMD_CHILD_NEW_TEXT:            
			subtype = config->textWindowPage;
			return VIEW_TEXT;
		break;
		case CMD_CHILD_NEW_TRANSIT:       return VIEW_TRANSIT; break;
		case CMD_CHILD_NEW_SBC:           return VIEW_SBC; break;
		case CMD_CHILD_NEW_SOLAR:         return VIEW_SOLAR; break;
		case CMD_CHILD_NEW_DASA:          return VIEW_DASA; break;
		case CMD_CHILD_NEW_GRAPHICALDASA: return VIEW_GRAPHICALDASA; break;
		case CMD_CHILD_NEW_URANIAN:       return VIEW_URANIAN; break;
		case CMD_CHILD_NEW_MAINVIEW:      return VIEW_PLANETLIST; break;
		case CMD_CHILD_NEW_YOGA:          return VIEW_YOGA; break;
		case CMD_CHILD_NEW_GRAPHICGRID:   return VIEW_GRAPHICGRID; break;
		case CMD_CHILD_NEW_VARGA:         return VIEW_VARGA; break;
		case CMD_CHILD_NEW_SHADBALA:      return VIEW_SHADBALA; break;
		case CMD_CHILD_NEW_ASHTAKAVARGA:  return VIEW_ASHTAKAVARGA; break;
		case CMD_CHILD_NEW_VIMSOPAKABALA: return VIEW_VIMSOPAKABALA; break;
		case APP_SHOWHORA:                return VIEW_HORA; break;
		case APP_SHOWECLIPSE:             return VIEW_ECLIPSE; break;
		case APP_NEW_PARTNER:             return VIEW_PARTNER; break;
		case APP_SHOWEPHEM:               return VIEW_EPHEM; break;
		case APP_YOGAEDITOR:              return VIEW_YOGA_EDITOR; break;
		case CMD_CHILD_NEW_WCHART:
			vedic = false;
			subtype = 0;
			return VIEW_GRAPHIC;
		break;
		default:
			if ( command >= CMD_CHILD_NEW_RASI && command <= CMD_CHILD_NEW_RASI+30 )
			{
				vedic = true;
				subtype = command - CMD_CHILD_NEW_RASI;
				return VIEW_GRAPHIC;
			}
		break;
	}
	return 0;
}

/**************************************************************
***
**    move a window to the centre of the screen
***
***************************************************************/
void moveWindow2Center( wxWindow *w )
{
	int x = wxSystemSettings::GetMetric( wxSYS_SCREEN_X );
	int y = wxSystemSettings::GetMetric( wxSYS_SCREEN_Y );
	wxSize sz = w->GetSize();
	w->Move( ( x - sz.x ) / 2, ( y - sz.y ) / 2 );
}

/**************************************************************
***
**    doMessageBox
***
***************************************************************/
int doMessageBox( wxWindow *parent, const wxString message, const long style )
{
	return wxMessageDialog( parent, message, GUI_APP_NAME, style | wxCENTRE ).ShowModal();
}

/**************************************************************
***
**    getSizeForViewId
***
***************************************************************/
wxSize getSizeForViewId( const int &viewId, const bool vedic )
{
	wxSize size;
	switch( viewId )
	{
		case VIEW_PLANETLIST:
			size = wxSize( config->xMainWindow, config->yMainWindow );
		break;
		case VIEW_GRAPHIC:
			if ( vedic )
				size = wxSize( config->xVedicGraphicWindow, config->yVedicGraphicWindow );
			else
				size = wxSize( config->xWesternGraphicWindow, config->yWesternGraphicWindow );
		break;
		case VIEW_TEXT:
			size = wxSize( config->xTextWindow, config->yTextWindow );
		break;
		case VIEW_TRANSIT:
			size = wxSize( config->xTransitWindow, config->yTransitWindow );
		break;
		case VIEW_SBC:
			size = wxSize( config->xSbcWindow, config->ySbcWindow );
		break;
		case VIEW_SOLAR:
			size = wxSize( config->xSolarWindow, config->ySolarWindow );
		break;
		case VIEW_GRAPHICALDASA:
			size = wxSize( config->xGraphicalDasaWindow, config->yGraphicalDasaWindow );
		break;
		case VIEW_ASHTAKAVARGA:
			size = wxSize( config->xAshtakaVargaWindow, config->yAshtakaVargaWindow );
		break;
		case VIEW_DASA:
			size = wxSize( config->xDasaWindow, config->yDasaWindow );
		break;
		case VIEW_URANIAN:
			size = wxSize( config->xUranianWindow, config->yUranianWindow );
		break;
		case VIEW_HORA:
			size = wxSize( config->xHoraWindow, config->yHoraWindow );
		break;
		case VIEW_ECLIPSE:
			size = wxSize( config->xEclipseWindow, config->yEclipseWindow );
		break;
		case VIEW_EPHEM:
			size = wxSize( config->xEphemWindow, config->yEphemWindow );
		break;
		case VIEW_PARTNER:
			size = wxSize( config->xPartnerWindow, config->yPartnerWindow );
		break;
		case VIEW_YOGA:
			size = wxSize( config->xYogaWindow, config->yYogaWindow );
		break;
		case VIEW_VARGA:
			size = wxSize( config->xVargaSummaryWindow, config->yVargaSummaryWindow );
		break;
		case VIEW_SHADBALA:
			size = wxSize( config->xShadbalaWindow, config->yShadbalaWindow );
		break;
		case VIEW_YOGA_EDITOR:
			size = wxSize( config->xYogaEditor, config->yYogaEditor );
		break;
		case VIEW_MULTIPLE:
			size = wxSize( config->xMultipleView, config->yMultipleView );
		break;
		case VIEW_EMPTY:
			size = wxSize( 100, 100 );
		break;
		default:
			printf( "Error: wrong view id %d in getSizeForViewId\n", viewId );
			assert( false );
		break;
	}
	return size;
}

/**************************************************************
***
**    getSizeForViewId
***
***************************************************************/
void setConfigSizeForViewId( const int &viewId, const wxSize &size, const bool vedic )
{
	switch ( viewId )
	{
		case VIEW_PLANETLIST:
			config->xMainWindow = size.x;
			config->yMainWindow = size.y;
		break;
		case VIEW_GRAPHIC:
			if ( vedic )
			{
				config->xVedicGraphicWindow = size.x;
				config->yVedicGraphicWindow = size.y;
			}
			else
			{
				config->xWesternGraphicWindow = size.x;
				config->yWesternGraphicWindow = size.y;
			}
		break;
		case VIEW_TEXT:
			config->xTextWindow = size.x;
			config->yTextWindow = size.y;
		break;
		case VIEW_DASA:
			config->xDasaWindow = size.x;
			config->yDasaWindow = size.y;
		break;
		case VIEW_GRAPHICALDASA:
			config->xGraphicalDasaWindow = size.x;
			config->yGraphicalDasaWindow = size.y;
		break;
		case VIEW_TRANSIT:
			config->xTransitWindow = size.x;
			config->yTransitWindow = size.y;
		break;
		case VIEW_SBC:
			config->xSbcWindow = size.x;
			config->ySbcWindow = size.y;
		break;
		case VIEW_SOLAR:
			config->xSolarWindow = size.x;
			config->ySolarWindow = size.y;
		break;
		case VIEW_URANIAN:
			config->xUranianWindow = size.x;
			config->yUranianWindow = size.y;
		break;
		case VIEW_HORA:
			config->xHoraWindow = size.x;
			config->yHoraWindow = size.y;
		break;
		case VIEW_ECLIPSE:
			config->xEclipseWindow = size.x;
			config->yEclipseWindow = size.y;
		break;
		case VIEW_PARTNER:
			config->xPartnerWindow = size.x;
			config->yPartnerWindow = size.y;
		break;
		case VIEW_EPHEM:
			config->xEphemWindow = size.x;
			config->yEphemWindow = size.y;
		break;
		case VIEW_YOGA:
			config->xYogaWindow = size.x;
			config->yYogaWindow = size.y;
		break;
		case VIEW_VARGA:
			config->xVargaSummaryWindow = size.x;
			config->yVargaSummaryWindow = size.y;
		break;
		case VIEW_SHADBALA:
			config->xShadbalaWindow = size.x;
			config->yShadbalaWindow = size.y;
		break;
		case VIEW_ASHTAKAVARGA:
			config->xAshtakaVargaWindow = size.x;
			config->yAshtakaVargaWindow = size.y;
		break;
		case VIEW_YOGA_EDITOR:
			config->xYogaEditor = size.x;
			config->yYogaEditor = size.y;
		break;
		case VIEW_MULTIPLE:
			config->xMultipleView = size.x;
			config->yMultipleView = size.y;
		break;
		case VIEW_EMPTY:
		break;
		default:
			printf( "Error: wrong view id %d in setConfigSizeForViewId\n", viewId );
			assert( false );
		break;
	}
}

/**************************************************************
***
**    tellThatRestartIsRecommended
***
***************************************************************/
void tellThatRestartIsRecommended( wxWindow *parent )
{
    static bool alreadysaidthat = false;
    if ( ! alreadysaidthat ) doMessageBox( parent, _( "Restart of the application is recommended."));
    alreadysaidthat = true;
}

/**************************************************************
***
**    tellThatPdfSupportsNoBgImages
***
***************************************************************/
void tellThatPdfSupportsNoBgImages( wxWindow *parent )
{
    static bool alreadysaidthat = false;
    if ( ! alreadysaidthat ) doMessageBox( parent, _( "Pdf Writer supports no background images."));
    alreadysaidthat = true;
}

/**************************************************************
***
**    tellThatFontIsNotOk
***
***************************************************************/
void tellThatFontIsNotOk( wxWindow *parent )
{
	static bool alreadysaidthat = false;
	wxString s;
	if ( ! alreadysaidthat )
	{
		alreadysaidthat = true;
		s.Printf( _( "Symbol font %s is not installed. Glyphs for planets and signs will not be displayed." ), DEFAULT_SYMBOL_FONT_NAME );
		doMessageBox( parent, s, wxOK | wxICON_EXCLAMATION );
	}
}

/**************************************************************
***
**    tellThatObjectsAreNotCalculated
***
***************************************************************/
void tellThatObjectsAreNotCalculated( wxWindow *parent, const int i )
{
	static bool alreadysaidthat = false;
	if ( ! alreadysaidthat )
	{
		alreadysaidthat = true;
		wxString s;
		if ( i == 1 ) s <<  _( "Some data could not be calculated." );
		else s.Printf( _( "Data for %d objects could not be calculated."), i );
		s << wxT( " " ) <<  _( "Please look if you configured Swiss Ephemeris data files in Configuration/Ephemeris correctly.");
		doMessageBox( parent, s, wxOK | wxICON_EXCLAMATION );
	}
}


