/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigPanel.cpp
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
	#pragma implementation "ConfigPanel.h"
#endif

#include "ConfigPanel.h"

IMPLEMENT_CLASS( ConfigPanel, wxPanel )

/*****************************************************
**
**   ConfigPanel   ---   Constructor 
**
******************************************************/
ConfigPanel::ConfigPanel( wxWindow* parent )
 : wxPanel( parent, -1 )
{
	dirty = false;
}

/*****************************************************
**
**   ConfigPanel   ---   OnSize 
**
******************************************************/
void ConfigPanel::OnSize( wxSizeEvent &event )
{
	//int a = event.GetSize().GetWidth();
	//int b = event.GetSize().GetHeight();
	event.Skip();
}

/*****************************************************
**
**   ConfigPanel   ---   OnChoice
**
******************************************************/
void ConfigPanel::OnChoice( wxCommandEvent& )
{
	dirty = true;
}
