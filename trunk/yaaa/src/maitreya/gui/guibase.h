/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/guibase.h
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

#ifndef _GUIBASE_H_
#define _GUIBASE_H_

#ifdef __GNUG__
	#pragma interface "guibase.h"
#endif

#include <wx/event.h>
#include <wx/string.h>

class wxWindow;

#define DEFAULT_DIALOG_STYLE wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxCAPTION

BEGIN_DECLARE_EVENT_TYPES()
  DECLARE_EVENT_TYPE( TOOLPANEL_CHANGED, wxID_HIGHEST + 1000 )
  DECLARE_EVENT_TYPE( TOOLPANEL_CREATEDOC, wxID_HIGHEST + 1001 )
END_DECLARE_EVENT_TYPES()

// Message box functions
int doMessageBox( wxWindow *parent, const wxString message, const long style = wxOK | wxICON_INFORMATION );
void tellThatRestartIsRecommended( wxWindow* );
void tellThatPdfSupportsNoBgImages( wxWindow* );
void tellThatFontIsNotOk( wxWindow *parent );
void tellThatObjectsAreNotCalculated( wxWindow *parent, const int = 0 );

int mapCommand2ViewId( const int &command, int &vedic, int &subtype );

// Sizing stuff
wxSize getSizeForViewId( const int &viewId, const bool vedic = true );
void setConfigSizeForViewId( const int &viewId, const wxSize &size, const bool vedic = true );
void moveWindow2Center( wxWindow *w );

#endif

